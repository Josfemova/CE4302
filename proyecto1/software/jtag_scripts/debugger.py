# se conecta al jtag y manipula los regs de control del cpu 

import cmd, sys
import socket
import memory_interface as mi

class Debugger(cmd.Cmd):
    intro = 'Welcome to the SISA debugger.\n'
    prompt = '(sisa-dbg) '
    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((mi.HOST, mi.PORT))
    data_loaded = False

    def do_dummy(self, arg):
        'Dummy command'
        print(arg.split())

    def do_read_mem(self, arg):
        "read provided address"
        addr = int(arg,0)
        print(f"0x{mi.read_mem(self.socket, addr):08X}");

    def do_write_mem(self, arg):
        "write_mem <addr> <data>"
        spl = arg.split()
        addr = int(spl[0],0)
        data = int(spl[1],0)
        mi.write_mem(self.socket, addr, data)

    def do_mem_dump(self, arg):
        """
        Memory dump to a txt file
        Usage: mem_dump <addr> <rows> <cols> <txtfile>
        """
        spl = arg.split()
        addr = int(spl[0],0)
        rows = int(spl[1],0)
        cols = int(spl[2],0)
        txtf = spl[3]
        words = rows * cols 
        with open(txtf, "w") as wf:
            wf.write(f"{rows}\n")
            wf.write(f"{cols}\n")
            for i in range(0, words):
                strval = mi.read_mem(self.socket, addr+ (i<<2))
                wf.write(f"{strval:08X}\n")
            

    def do_pause(self, arg):
        'Pause core execution, enables manual stepping'
        mi.write_mem(self.socket, mi.CSR_MANUAL_STEP_EN, 1)
        print("Enabled manual stepping")

    def do_step(self, arg):
        'Usage: step <step count>. Step the paused cpu a given number of cycles'
        state_str = "if: 0x{:08X}\tde: 0x{:08X}\tex: 0x{:08X}\tmem: 0x{:08X}\twb: 0x{:08x}"
        #try:
        steps = 1
        if arg != "":
            steps = int(arg)
        for i in range(0, steps):
            pstate = mi.get_pipeline_state(self.socket)
            print(f"Pipeline state for step {i}:")
            print(state_str.format(*pstate))
            print(80*"=")
            mi.write_mem(self.socket, mi.CSR_MANUAL_STEPS, 1)
        #except:
        #    print("Invalid argument")

    def do_continue(self, arg):
        mi.write_mem(self.socket, mi.CSR_MANUAL_STEP_EN, 0)
        print("Disabled manual stepping")

    def do_load_data(self, args):
        """
        Load the data needed to execute encryption algorithm
        Usage: load_data <data_txt_file> <key_file> <enc, dec>
        The key file it's a 4 line file where each files has a 32bit word in
        hex format, little endian
        """
        sp = args.split()
        datafile = sp[0]
        keyfile  = sp[1]
        op = 0 if (sp[2] == "enc") else 1
        mi.load_data(self.socket, datafile, keyfile, op)
        self.data_loaded = True

    def do_load_code(self, args):
        """
        Load the given .txt file to the CPU, immediatly jump to its execution
        Usage: load_code <code_txt_file> 
        The txt file contains a 32bit word per line in binary format, one line
        per instruction
        """
        if self.data_loaded | 1:
            txt = args
            mi.load_code(self.socket, txt)
            # para el próximo run
            self.data_loaded = False 
            print("Code has been loaded")
        else:
            print("You need to load the data before trying to execute the encryption algorithm")
    def do_exit(self, arg):
        'exit debugger'
        self.socket.close()
        exit()

if __name__ == '__main__':
    Debugger().cmdloop()