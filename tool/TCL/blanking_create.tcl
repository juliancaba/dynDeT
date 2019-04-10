###############################################################
### Create Blanking Configuration
###############################################################

if {$createBlanking} {
    puts "INFO: Creating Blanking Configuration"

    open_checkpoint $checksDir/static_route_design.dcp
        

    set rmList [rmGroup::getRM_IDs]
    foreach itRM $rmList {
	set instanceCELL [rmGroup::getRMCell $itRM]

        update_design -buffer_ports -cell $instanceCELL
    }

    place_design
    route_design

    write_checkpoint -force $implDir/blanking_route_design.dcp

    close_design
    puts "INFO: Blanking Configuration Created"
    
} else {
    puts "INFO: The Blanking Configuration will not be created"
}

###############################################################
