# -----------------------------------------------------------------
# jtag_server_cmds.tcl
#
# 9/14/2011 D. W. Hawkins (dwh@caltech.edu)
#
# Altera JTAG socket server Tcl commands.
#
# The JTAG server provides remote hardware access/control functions
# to clients.
#
# The server accept Tcl string commands from the client, issues the
# command to the hardware, and then returns any response data
# to the client.
#
# -----------------------------------------------------------------
# Notes:
# ------
#
# 1. To use the commands via a console, use:
#
#    quartus_stp -s
#    tcl> source jtag_server_cmds.tcl
#
#    This same procedure can be used with SystemConsole
#
# 2. When this script is run under quartus_stp, it requires
#    the package: altera_jtag_to_avalon_stp.
#
#    The package contains Tcl scripts for accessing
#    JTAG-to-Avalon-ST/MM bridges from quartus_stp.
#
#    See jtag_server.tcl for how to load the package.
#
# -----------------------------------------------------------------
# References
# ----------
#
# 1. Brent Welch, "Practical Programming in Tcl and Tk",
#    3rd Ed, 2000.
#
# -----------------------------------------------------------------

# =================================================================
# Tool detection
# =================================================================
#
# The server can be run from either quartus_stp or SystemConole.
#
# Tcl usually allows you to detect the toolname using
# 'info nameofexecutable', however, under SystemConsole this
# is an empty string. In other cases, the global argv0
# holds the application name, but under the Quartus Tcl console
# there is no argv0! However, nameofexecutable does work there,
# so start with that, and if its empty, try argv0.
#
proc detect_tool {} {
	global jtag argv0

	# Get the tool name
	set toolname [info nameofexecutable]
	if {[string length $toolname] == 0} {
		if {[info exists argv0]} {
			set toolname $argv0
		}
	}

	# Strip the name to just that of the application
	set toolname [file rootname [file tail $toolname]]

	# Test for the two allowed tools
	set jtag(tool_ok) 0
	if {[string compare $toolname quartus_stp] == 0} {
		set jtag(tool) stp
		set jtag(tool_ok) 1
	} elseif {[string compare $toolname system-console] == 0} {
		set jtag(tool) sc
		set jtag(tool_ok) 1
	}
	return
}

proc is_tool_ok {} {
	global jtag
	if {![info exists jtag(tool_ok)]} {
		detect_tool
	}
	return $jtag(tool_ok)
}

proc is_system_console {} {
	global jtag
	if {![info exists jtag(tool_ok)]} {
		detect_tool
	}
	if {[string compare $jtag(tool) sc] == 0} {
		return 1
	} else {
		return 0
	}
}

proc has_fileevent {} {
	if {[string compare [info command fileevent] "fileevent"] == 0} {
		return 1
	} else {
		return 0
	}
}

# =================================================================
# Debug mode
# =================================================================
#
# In debug mode, the global array 'jtag' is used  as fake
# memory for read/write accesses
#
proc jtag_debug_mode {val} {
	global jtag
	set jtag(debug) $val
	return
}

proc is_debug_mode {} {
	global jtag
	if {$jtag(debug) == 1} {
		return 1
	} else {
		return 0
	}
}

# =================================================================
# JTAG access
# =================================================================
#
proc jtag_open {{index 0}} {
	global jtag

	if {[is_system_console]} {
		# Close any open service
		if {[info exists jtag(master)]} {
			close_service master $jtag(master)
		}

		# Get the list of masters
		set masters [get_service_paths master]
		if {[llength $masters] == 0} {
			error "Error: No JTAG-to-Avalon-MM device found!"
		}

		# Access the first master in the masters list
		set jtag(master) [lindex $masters $index]
		open_service master $jtag(master)

	} else {

		# Use the quartus_stp procedures to find the first
		# JTAG-to-Avalon-MM bridge (master)
		#
		# First check if a board is found
		# MODIFICACION: Seleccionar device 0 1
		if {[catch {altera_jtag_to_avalon_stp::jtag_open 0 1} result]} {
			error "Error: JTAG device open failed\n -> $result"
		}
		# Find the first master
		altera_jtag_to_avalon_stp::jtag_print_hub_info
		set len [altera_jtag_to_avalon_stp::jtag_number_of_nodes]
		for {set i 0} {$i < $len} {incr i} {
			if {[altera_jtag_to_avalon_stp::jtag_node_is_master $i]} {
				set jtag(master) $i
				break;
			}
		}
		puts "master: $jtag(master)"
		if {![info exists jtag(master)]} {
			error "Error: No JTAG-to-Avalon-MM device found!"
		}
	}
	return
}

proc jtag_close {} {
	global jtag

	if {[is_system_console]} {
		if {[info exists jtag(master)]} {
			close_service master $jtag(master)
			unset jtag(master)
		}
	} else {
		altera_jtag_to_avalon_stp::jtag_close
	}
	return
}

proc jtag_read {addr} {
	global jtag

	# Check the argument is a valid value by reformatting
	# the address as an 8-bit hex value
	set addr [expr {$addr & 0xFFFFFFFF}]
	if {[catch {format "0x%.8X" $addr} addr]} {
		error "Error: Invalid address\n -> '$addr'"
	}

	if {[is_debug_mode]} {

		# Check if the fake address exists, if it does not,
		# then create it and initialize it to 0.
		if {![info exists jtag(addr_$addr)]} {
			set jtag(addr_$addr) 0
		}
		puts "DEBUG: jtag_read $addr"
		return $jtag(addr_$addr)
	} else {
		if {![info exists jtag(master)]} {
			jtag_open
		}
	}

	# Read 32-bits
	#puts "S: rd $addr"
	if {[is_system_console]} {
		if {[catch {master_read_32 $jtag(master) $addr 1} result]} {
			# JTAG connection lost?
			jtag_close
			error "Error: Check the JTAG interface\n -> '$result'"
		}
	} else {
		puts "$jtag(master)"
		if {[catch {altera_jtag_to_avalon_stp::jtag_read $jtag(master) $addr} result]} {
			# JTAG connection lost?
			jtag_close
			error "Error: Check the JTAG interface\n -> '$result'"
		}
	}
	return $result
}

proc jtag_write {addr data} {
	global jtag

	# Check the arguments are valid values by reformatting
	# them as 8-bit hex values
	set addr [expr {$addr & 0xFFFFFFFF}]
	if {[catch {format "0x%.8X" $addr} addr]} {
		error "Error: Invalid address\n -> '$addr'"
	}
	set data [expr {$data & 0xFFFFFFFF}]
	if {[catch {format "0x%.8X" $data} data]} {
		error "Error: Invalid write data\n -> '$data'"
	}

	if {[is_debug_mode]} {

		# Write to the address
		set jtag(addr_$addr) $data

		puts "DEBUG: jtag_write $addr $data"
		return
	} else {
		if {![info exists jtag(master)]} {
			jtag_open
		}
	}

	# Write 32-bits
	#puts "S: wr $addr $data"
	if {[is_system_console]} {
		if {[catch {master_write_32 $jtag(master) $addr [list $data]} result]} {
			# JTAG connection lost?
			jtag_close
			error "Error: Check the JTAG interface\n -> '$result'"
		}
	} else {
		if {[catch {altera_jtag_to_avalon_stp::jtag_write $jtag(master) $addr $data} result]} {
			# JTAG connection lost?
			jtag_close
			error "Error: Check the JTAG interface\n -> '$result'"
		}
	}
	return
}

# =================================================================
# Server socket
# =================================================================
#
# For details on socket programming, see Welch Ch. 17 [1].
#

# Start the server on the specified port
proc server_listen {port} {
	global jtag
	if {[catch {socket -server server_accept $port} result]} {

#		error "Error: Server start-up failed\n -> $result"
		puts "Error: Server start-up failed\n -> $result"

		# For some reason, this error path causes quartus_stp
		# to generate an error backtrace and then hang the
		# console (Task manager is needed to kill it).
		#
		# This happens if the 'error' function or a 'return'
		# is used here. The 'exit' command still works
		# (but you do not get a full error backtrace).
		#
		# This is not ideal, but its better than a hung console.
		exit
	}
	set jtag(port)   $port
	set jtag(socket) $result
	return
}

# Server client-accept callback
proc server_accept {client addr port} {

	puts "Accept $client from $addr port $port"

	# Configure the client for line-based communication
	fconfigure $client -buffering line

	# Setup the client handler
	if  {[has_fileevent]} {
		puts "Handle the client via a fileevent callback"
		fileevent $client readable [list client_handler $client]

	} else {

		# SystemConsole (prior to Quartus 11.1sp1)
		#
		# * There is no support for fileevent, so SystemConsole
		#   has to busy loop on one client at a time.
		#
		# * If the client closes its connection cleanly via
		#   client_close, then the socket generates an EOF
		#   and the server will wait for the next client.
		#
		# * However, if the client disconnection is not cleanm,
		#   eg., the client uses exit or ctrl-C to exit, then
		#   the server does not receive an eof, and it remains
		#   blocked on the dead client connection. New clients,
		#   will not be handled!
		#
		#   In constrast, when the server is run under
		#   quartus_stp, the server log indicates that a ctrl-C
		#   from a client quartus_stp generates an
		#   'empty command' followed by 'disconnected'
		#
		puts "Handle the client via a blocked read on the socket"
		while {![eof $client]} {
			client_handler $client
		}
		puts "SERVER ($client): disconnected"
	}
	return
}

# Client handler
#
# * Note: this handler executes *any* Tcl command from the client
#   so the client can actually send arbitrary strings, eg., from
#   a client console, the following is possible:
#
#   tcl> puts $jtag(socket) "expr {5*10}"
#   tcl> gets $jtag(socket)
#   50
#
#   The client handler could be changed to restrict the client
#   command requests to a restricted list of acceptable commands.
#
proc client_handler {client} {
	if {[eof $client]} {
		puts "SERVER ($client): disconnected"
		close $client
	} elseif {[catch {gets $client cmd}]} {
		puts "SERVER ($client): error reading a line"
		close $client
	} elseif {[string length $cmd] == 0} {
		# When the client closes a connection, an empty
		# command is generated, followed by EOF
		# (which the SystemConsole loop breaks on)
		puts "SERVER ($client): empty command"
	} else {
		# Execute the command and return the response
		#puts "SERVER ($client): $cmd"
		if {[catch {eval $cmd} rsp]} {
			puts "SERVER ($client): Invalid command from the client"
		} else {
			if {[string length $rsp] > 0} {
				#puts "SERVER ($client): $rsp"
				puts $client $rsp
			}
		}
	}
	return
}
