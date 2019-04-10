###############################################################
### Verify Implementations
###############################################################
set cfgList [cfgGroup::getCFG_IDs]

foreach itCFG $cfgList {
    lappend implConfigurations $implDir/$itCFG\_route_design.dcp
}
if {$createBlanking} {
    lappend implConfigurations $implDir/blanking_route_design.dcp
}

pr_verify -initial $implDir/reference_route_design.dcp -additional $implConfigurations
close_project
###############################################################

