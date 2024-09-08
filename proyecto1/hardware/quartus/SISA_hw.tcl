# TCL File Generated by Component Editor 18.1
# Sat Sep 07 21:38:58 CST 2024
# DO NOT MODIFY


# 
# SISA "SISA" v1.0
#  2024.09.07.21:38:58
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module SISA
# 
set_module_property DESCRIPTION ""
set_module_property NAME SISA
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME SISA
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL core_top
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file alu.sv SYSTEM_VERILOG PATH ../rtl/core/alu.sv
add_fileset_file control_unit.sv SYSTEM_VERILOG PATH ../rtl/core/control_unit.sv
add_fileset_file core_top.sv SYSTEM_VERILOG PATH ../rtl/core/core_top.sv TOP_LEVEL_FILE
add_fileset_file hazard_unit.sv SYSTEM_VERILOG PATH ../rtl/core/hazard_unit.sv
add_fileset_file imm_extend.sv SYSTEM_VERILOG PATH ../rtl/core/imm_extend.sv
add_fileset_file jump_cond_ctrl.sv SYSTEM_VERILOG PATH ../rtl/core/jump_cond_ctrl.sv
add_fileset_file load_store_unit.sv SYSTEM_VERILOG PATH ../rtl/core/load_store_unit.sv
add_fileset_file register_file.sv SYSTEM_VERILOG PATH ../rtl/core/register_file.sv
add_fileset_file stage_decode.sv SYSTEM_VERILOG PATH ../rtl/core/stage_decode.sv
add_fileset_file stage_execute.sv SYSTEM_VERILOG PATH ../rtl/core/stage_execute.sv
add_fileset_file stage_instruction_fetch.sv SYSTEM_VERILOG PATH ../rtl/core/stage_instruction_fetch.sv
add_fileset_file stage_memory.sv SYSTEM_VERILOG PATH ../rtl/core/stage_memory.sv
add_fileset_file stage_writeback.sv SYSTEM_VERILOG PATH ../rtl/core/stage_writeback.sv

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL core_top
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file alu.sv SYSTEM_VERILOG PATH ../rtl/core/alu.sv
add_fileset_file control_unit.sv SYSTEM_VERILOG PATH ../rtl/core/control_unit.sv
add_fileset_file core_top.sv SYSTEM_VERILOG PATH ../rtl/core/core_top.sv
add_fileset_file hazard_unit.sv SYSTEM_VERILOG PATH ../rtl/core/hazard_unit.sv
add_fileset_file imm_extend.sv SYSTEM_VERILOG PATH ../rtl/core/imm_extend.sv
add_fileset_file jump_cond_ctrl.sv SYSTEM_VERILOG PATH ../rtl/core/jump_cond_ctrl.sv
add_fileset_file load_store_unit.sv SYSTEM_VERILOG PATH ../rtl/core/load_store_unit.sv
add_fileset_file register_file.sv SYSTEM_VERILOG PATH ../rtl/core/register_file.sv
add_fileset_file stage_decode.sv SYSTEM_VERILOG PATH ../rtl/core/stage_decode.sv
add_fileset_file stage_execute.sv SYSTEM_VERILOG PATH ../rtl/core/stage_execute.sv
add_fileset_file stage_instruction_fetch.sv SYSTEM_VERILOG PATH ../rtl/core/stage_instruction_fetch.sv
add_fileset_file stage_memory.sv SYSTEM_VERILOG PATH ../rtl/core/stage_memory.sv
add_fileset_file stage_writeback.sv SYSTEM_VERILOG PATH ../rtl/core/stage_writeback.sv


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges NONE
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset reset Input 1


# 
# connection point instr_mem_master
# 
add_interface instr_mem_master avalon start
set_interface_property instr_mem_master addressUnits SYMBOLS
set_interface_property instr_mem_master associatedClock clock
set_interface_property instr_mem_master associatedReset reset
set_interface_property instr_mem_master bitsPerSymbol 8
set_interface_property instr_mem_master burstOnBurstBoundariesOnly false
set_interface_property instr_mem_master burstcountUnits WORDS
set_interface_property instr_mem_master doStreamReads false
set_interface_property instr_mem_master doStreamWrites false
set_interface_property instr_mem_master holdTime 0
set_interface_property instr_mem_master linewrapBursts false
set_interface_property instr_mem_master maximumPendingReadTransactions 0
set_interface_property instr_mem_master maximumPendingWriteTransactions 0
set_interface_property instr_mem_master readLatency 1
set_interface_property instr_mem_master readWaitTime 0
set_interface_property instr_mem_master setupTime 0
set_interface_property instr_mem_master timingUnits Cycles
set_interface_property instr_mem_master writeWaitTime 0
set_interface_property instr_mem_master ENABLED true
set_interface_property instr_mem_master EXPORT_OF ""
set_interface_property instr_mem_master PORT_NAME_MAP ""
set_interface_property instr_mem_master CMSIS_SVD_VARIABLES ""
set_interface_property instr_mem_master SVD_ADDRESS_GROUP ""

add_interface_port instr_mem_master instr_memory_readdata readdata Input 32
add_interface_port instr_mem_master instr_memory_addr address Output 32
add_interface_port instr_mem_master instr_memory_read_en read Output 1
add_interface_port instr_mem_master instr_memory_byteenable byteenable Output 4


# 
# connection point data_mem_master
# 
add_interface data_mem_master avalon start
set_interface_property data_mem_master addressUnits SYMBOLS
set_interface_property data_mem_master associatedClock clock
set_interface_property data_mem_master associatedReset reset
set_interface_property data_mem_master bitsPerSymbol 8
set_interface_property data_mem_master burstOnBurstBoundariesOnly false
set_interface_property data_mem_master burstcountUnits WORDS
set_interface_property data_mem_master doStreamReads false
set_interface_property data_mem_master doStreamWrites false
set_interface_property data_mem_master holdTime 0
set_interface_property data_mem_master linewrapBursts false
set_interface_property data_mem_master maximumPendingReadTransactions 0
set_interface_property data_mem_master maximumPendingWriteTransactions 0
set_interface_property data_mem_master readLatency 1
set_interface_property data_mem_master readWaitTime 0
set_interface_property data_mem_master setupTime 0
set_interface_property data_mem_master timingUnits Cycles
set_interface_property data_mem_master writeWaitTime 0
set_interface_property data_mem_master ENABLED true
set_interface_property data_mem_master EXPORT_OF ""
set_interface_property data_mem_master PORT_NAME_MAP ""
set_interface_property data_mem_master CMSIS_SVD_VARIABLES ""
set_interface_property data_mem_master SVD_ADDRESS_GROUP ""

add_interface_port data_mem_master data_memory_addr address Output 32
add_interface_port data_mem_master data_memory_byteenable byteenable Output 4
add_interface_port data_mem_master data_memory_read_en read Output 1
add_interface_port data_mem_master data_memory_readdata readdata Input 32
add_interface_port data_mem_master data_memory_write_en write Output 1
add_interface_port data_mem_master data_memory_writedata writedata Output 32


# 
# connection point instr_export
# 
add_interface instr_export conduit end
set_interface_property instr_export associatedClock ""
set_interface_property instr_export associatedReset ""
set_interface_property instr_export ENABLED true
set_interface_property instr_export EXPORT_OF ""
set_interface_property instr_export PORT_NAME_MAP ""
set_interface_property instr_export CMSIS_SVD_VARIABLES ""
set_interface_property instr_export SVD_ADDRESS_GROUP ""

add_interface_port instr_export instr_if instr_if Output 32
add_interface_port instr_export instr_de instr_de Output 32
add_interface_port instr_export instr_ex instr_ex Output 32
add_interface_port instr_export instr_mem instr_mem Output 32
add_interface_port instr_export instr_wb instr_wb Output 32

