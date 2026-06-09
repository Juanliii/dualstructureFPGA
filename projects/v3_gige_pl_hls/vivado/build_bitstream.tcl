set script_dir [file dirname [file normalize [info script]]]
open_project [file join $script_dir "build/v3_gige_pl_hls_vivado/v3_gige_pl_hls.xpr"]
puts "TODO: generate wrapper, run synthesis, implementation, and bitstream after RTL/BD is completed."

