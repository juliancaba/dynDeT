###############################################################
### Rest of System (Vivado project within TOP and other comp
###############################################################

open_project $prjName/$prjName.xpr


# Add static files (constraints included)
foreach it [fileGroup::getFileGroup $staticFiles] {
    import_files -norecurse $it
}

set synthName synth_1 


puts "Top $topName $designName"

if {$topName eq $designName} {
    # make_wrapper -files [get_files $originDir/$prjName/$prjName.srcs/sources_1/bd/$designName/$designName.bd] -top
    # import_files -norecurse $originDir/$prjName/$prjName.srcs/sources_1/bd/$designName/hdl/$designName\_wrapper.vhd 
    make_wrapper -files [get_files ./$prjName/$prjName.srcs/sources_1/bd/$designName/$designName.bd] -top
    import_files -norecurse ./$prjName/$prjName.srcs/sources_1/bd/$designName/hdl/$designName\_wrapper.vhd 
    set topName $designName\_wrapper
} else {
    set_property top $topName [current_fileset]
}

update_compile_order -fileset sources_1

reset_run $synthName
set cores [exec nproc]
launch_runs $synthName -jobs $cores
wait_on_run $synthName
open_run $synthName -name $synthName

write_checkpoint -force $synthDir/$topName.dcp
report_timing_summary -file $reportDir/static_synth_timing_summary.rpt
report_utilization -file $reportDir/static_synth_util.rpt
#file copy $originDir/$prjName/$prjName.runs/$synthName/$topName.dcp $synthDir
###############################################################


close_project

puts "INFO: Project closed"
