set script_dir [file dirname [file normalize [info script]]]
open_project [file join $script_dir "build/v2_gige_ps_hls_vivado/v2_gige_ps_hls.xpr"]
puts "TODO: generate wrapper, run synthesis, implementation, and bitstream after BD is completed."

