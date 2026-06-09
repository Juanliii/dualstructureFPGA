set script_dir [file dirname [file normalize [info script]]]
set vitis_dir [file normalize [file join $script_dir ".."]]
setws [file join $vitis_dir "workspace"]

set xsa_file [file join $vitis_dir "xsa/v3_gige_pl_hls.xsa"]
if {![file exists $xsa_file]} {
    puts "ERROR: Missing $xsa_file. Export XSA from Vivado first."
    exit 1
}

platform create -name v3_gige_pl_hls_platform -hw $xsa_file -proc psu_cortexa53 -os linux
platform write

