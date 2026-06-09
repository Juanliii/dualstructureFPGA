set script_dir [file dirname [file normalize [info script]]]
set hls_root [file normalize [file join $script_dir "../.."]]
source [file join $hls_root "scripts/hls_env.tcl"]

set build_root [file join $hls_root "build"]
set project_name "stereo_pipeline_hls"
set ip_dir [file join $hls_root "ip_repo/stereo_pipeline"]
file mkdir $build_root
file mkdir $ip_dir

set old_cwd [pwd]
cd $build_root

open_project -reset $project_name
set_top stereo_pipeline
add_files [file join $script_dir "stereo_pipeline.cpp"] -cflags "-I[file join $hls_root common]"
add_files -tb [file join $script_dir "tb_stereo_pipeline.cpp"] -cflags "-I[file join $hls_root common]"

open_solution -reset "solution1"
set_part $FPGA_PART
create_clock -period $HLS_CLOCK_PERIOD_NS -name default

if {$HLS_FLOW == "csim" || $HLS_FLOW == "all"} {
    csim_design
}

if {$HLS_FLOW == "csynth" || $HLS_FLOW == "cosim" || $HLS_FLOW == "export" || $HLS_FLOW == "all"} {
    csynth_design
}

if {$HLS_FLOW == "cosim" || $HLS_FLOW == "all"} {
    cosim_design
}

if {$HLS_FLOW == "export" || $HLS_FLOW == "all"} {
    export_design -format ip_catalog -output [file join $ip_dir "stereo_pipeline.zip"]
}

close_project
cd $old_cwd

exit
