###############################################################
### Create and Synth Reconfig Modules
###############################################################
set rmList [rmGroup::getRM_IDs]

foreach itRM $rmList {
    set moduleList [rmGroup::getRM_Modules $itRM]
    foreach itMod $moduleList {	
        if {[file exists $synthDir/$itRM\_$itMod.dcp] == 1} {
            puts "INFO: Synthesis task for $itMod (Reconfigurable Partition: $itRM) was already executed."
	    continue
	}
	set moduleFilesID [rmGroup::getGroupFileID $itRM $itMod]
	foreach itFile [fileGroup::getFileGroup $moduleFilesID] {
	    set fileSplitted  [split $itFile {.}]
	    set typeFile [lindex $fileSplitted [llength $fileSplitted]-1]
	    switch $typeFile {
		"vhd" {
		    read_vhdl $itFile
		}
		"vhdl" {
		    read_vhdl $itFile
		}
		"v" {
		    read_verilog $itFile
		}
		"xci" {
		    read_ip $itFile
		}
		"dcp" {
		    read_checkpoint $itFile
		}
	    }
	}
	puts "INFO: Init synthesis task for $itMod (Reconfigure Module: $itRM)"
	synth_design -mode out_of_context -flatten_hierarchy rebuilt -top $itRM -part $part
	write_checkpoint -force $synthDir/$itRM\_$itMod.dcp
	report_utilization -file $reportDir/$itRM\_$itMod\_utilization.rpt
	report_timing_summary -file $reportDir/$itRM\_$itMod\_timing_summary.rpt
	close_project  
	puts "INFO: Finish synthesis task for $itMod (Reconfigure Module: $itRM)"  
    }
}
puts "INFO: Reconfig modules were synthesized."
###############################################################

