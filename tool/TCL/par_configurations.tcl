###############################################################
### Create Other Configurations
###############################################################
puts "INFO: Creating Other Configurations"

set cfgList [cfgGroup::getCFG_IDs]

foreach itCFG $cfgList {
    if {[file exists $implDir/$itCFG\_route_design.dcp] == 1 } {
        puts "INFO: $itCFG configuration was already created."
	continue
    }
    puts "INFO: Creating $itCFG Configuration"
    set rmList [cfgGroup::getConfigurationRMs $itCFG]
    set moduleList [cfgGroup::getConfigurationModules $itCFG]
    set modeList [cfgGroup::getConfigurationModes $itCFG]

    open_checkpoint $checksDir/static_route_design.dcp

    # New
    foreach itXDC [fileGroup::getFileGroup $constraintFiles] {
	read_xdc $itXDC
    }
    read_xdc $prjName/fplan.xdc

    for {set it 0} {$it < [llength $rmList]} {incr it} {
	set rmAux [lindex $rmList $it]
	set moduleAux [lindex $moduleList $it]
	set modeAux [lindex $modeList $it]
	
	set instanceCELL [rmGroup::getRMCell $rmAux]

	read_checkpoint -cell $instanceCELL $synthDir/$rmAux\_$moduleAux.dcp -strict	
    }

    opt_design
    place_design
    #phys_opt_design
    route_design

    write_checkpoint -force $implDir/$itCFG\_route_design.dcp
    
    for {set it 0} {$it < [llength $rmList]} {incr it} {
	set rmAux [lindex $rmList $it]
	set moduleAux [lindex $moduleList $it]
	set modeAux [lindex $modeList $it]
	
	set instanceCELL [rmGroup::getRMCell $rmAux]

	write_checkpoint -force -cell $instanceCELL $checksDir/$moduleAux\_route_design.dcp	
    }


    # New
    #    close_design
    close_project
    puts "INFO: $itCFG Configuration Created"
}

puts "INFO: Other Configurations were created"
###############################################################
