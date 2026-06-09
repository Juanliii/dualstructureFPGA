set script_dir [file dirname [file normalize [info script]]]
open_project [file join $script_dir "build/v1_usb_ps_hls_vivado/v1_usb_ps_hls.xpr"]
file mkdir [file join $script_dir "../vitis/xsa"]
puts "TODO: write_hw_platform -fixed -include_bit -force -file ../vitis/xsa/v1_usb_ps_hls.xsa"

