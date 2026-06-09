set FPGA_PART "xczu5ev-sfvc784-2-i"
set HLS_CLOCK_PERIOD_NS 6.667

if {![info exists HLS_FLOW]} {
    set HLS_FLOW "all"
}

puts "Using Vitis HLS flow: $HLS_FLOW"
puts "Using FPGA part: $FPGA_PART"

