set script_dir [file dirname [file normalize [info script]]]
set vitis_dir [file normalize [file join $script_dir ".."]]
setws [file join $vitis_dir "workspace"]

app create -name v1_usb_capture_control -platform v1_usb_ps_hls_platform -domain linux_domain -template "Empty Application(C++)"
importsources -name v1_usb_capture_control -path [file join $vitis_dir "apps/v1_usb_capture_control/src"]

