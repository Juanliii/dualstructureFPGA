set script_dir [file dirname [file normalize [info script]]]
set vitis_dir [file normalize [file join $script_dir ".."]]
setws [file join $vitis_dir "workspace"]
app build -name v2_gige_capture_control

