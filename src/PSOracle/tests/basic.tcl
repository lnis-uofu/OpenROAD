yosys -import

read_verilog c17.v
hierarchy -check -auto-top
procs
flatten
fsm
techmap
opt -purge
dfflibmap -liberty asap_simple.lib
abc -D 100000 -liberty asap_simple.lib -script abc.script -showtmp stat
write_verilog -noattr -noexpr -nohex -nodec c17_mapped.v
