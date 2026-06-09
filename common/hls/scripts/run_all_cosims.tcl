set script_dir [file dirname [file normalize [info script]]]
set hls_root [file normalize [file join $script_dir ".."]]
set HLS_FLOW "cosim"
source [file join $hls_root "kernels/stereo_pipeline/run_hls.tcl"]

