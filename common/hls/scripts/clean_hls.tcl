set script_dir [file dirname [file normalize [info script]]]
set hls_root [file normalize [file join $script_dir ".."]]

foreach path [list [file join $hls_root "build"] [file join $hls_root "ip_repo"]] {
    if {[file exists $path]} {
        file delete -force $path
        puts "Deleted $path"
    }
}

