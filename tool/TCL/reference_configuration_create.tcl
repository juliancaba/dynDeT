###############################################################
### Reference Configuration
###############################################################

# Read Constraints
############################
foreach itXDC [fileGroup::getFileGroup $constraintFiles] {
    read_xdc $itXDC
}
read_xdc $prjName/fplan.xdc


# PAR tasks
############################
opt_design
place_design
#phys_opt_design
route_design

write_checkpoint -force $implDir/reference_route_design.dcp
report_utilization -file $reportDir/reference_utilization.rpt
report_timing_summary -file $reportDir/reference_timming_summary.rpt


# Write Checkpoints
############################
foreach itRM $rmList {
    set instanceNAME [rmGroup::getRMInstanceName $itRM]
    set instanceCELL [rmGroup::getRMCell $itRM]

    write_checkpoint -force -cell $instanceCELL $checksDir/$itRM\_$instanceNAME\_route_design.dcp
}

write_debug_probes $implDir/debug_nets.ltx

# Clear RMs
############################
foreach itRM $rmList {
    set instanceCELL [rmGroup::getRMCell $itRM]

    update_design -cell $instanceCELL -black_box
}


# Lock design
############################
lock_design -level routing

write_checkpoint -force $checksDir/static_route_design.dcp

puts "INFO: Reference Design created"

# New
#close_design
###############################################################
