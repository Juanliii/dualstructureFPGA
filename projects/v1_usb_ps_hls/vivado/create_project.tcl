set script_dir [file dirname [file normalize [info script]]]
set proj_dir [file normalize [file join $script_dir "build/v1_usb_ps_hls_vivado"]]
set part_name "xczu5ev-sfvc784-2-i"
set ip_repo [file normalize [file join $script_dir "../../../common/hls/ip_repo"]]

create_project -force v1_usb_ps_hls $proj_dir -part $part_name
set_property target_language Verilog [current_project]

if {[file exists $ip_repo]} {
    set_property ip_repo_paths $ip_repo [current_project]
    update_ip_catalog
}

source [file join $script_dir "bd/create_bd.tcl"]
save_project_as v1_usb_ps_hls $proj_dir -force

