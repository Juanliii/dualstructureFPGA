set script_dir [file dirname [file normalize [info script]]]
set vitis_dir [file normalize [file join $script_dir ".."]]
setws [file join $vitis_dir "workspace"]

app create -name v2_gige_capture_control -platform v2_gige_ps_hls_platform -domain linux_domain -template "Empty Application(C++)"
importsources -name v2_gige_capture_control -path [file join $vitis_dir "apps/v2_gige_capture_control/src"]

