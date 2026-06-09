set script_dir [file dirname [file normalize [info script]]]
set vitis_dir [file normalize [file join $script_dir ".."]]
setws [file join $vitis_dir "workspace"]

app create -name v3_pl_control -platform v3_gige_pl_hls_platform -domain linux_domain -template "Empty Application(C++)"
importsources -name v3_pl_control -path [file join $vitis_dir "apps/v3_pl_control/src"]

