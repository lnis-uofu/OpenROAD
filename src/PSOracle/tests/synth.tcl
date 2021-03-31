yosys -import
read_verilog -defer -sv gcd.v
read_liberty -lib sky130_fd_sc_hd__tt_025C_1v80.lib
read_verilog -defer cells_clkgate_hd.v
synth  -top gcd -flatten
opt -purge
techmap -map cells_latch_hd.v
dfflibmap -liberty sky130_fd_sc_hd__tt_025C_1v80.lib
opt
abc -D 10 \
    -script abc.script \
    -liberty sky130_fd_sc_hd__tt_025C_1v80.lib \
    -constr abc.constr
setundef -zero
splitnets
opt_clean -purge
hilomap -singleton \
        -hicell sky130_fd_sc_hd__conb_1 HI \
        -locell sky130_fd_sc_hd__buf_4 A X
insbuf -buf sky130_fd_sc_hd__buf_4 A X
check
write_verilog -noattr -noexpr -nohex -nodec mapped.v
