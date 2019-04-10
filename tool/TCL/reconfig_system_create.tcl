###############################################################
### Create Reconfig System
###############################################################
puts "INFO: Creating Reconfig System"

open_checkpoint $synthDir/$topName.dcp
set_param hd.visual 1


# Read Reference Modules
############################
set rmList [rmGroup::getRM_IDs]
foreach itRM $rmList {
    set instanceCELL [rmGroup::getRMCell $itRM]
    set moduleList [rmGroup::getRM_Modules $itRM]
    set moduleRef [lindex $moduleList 0]

    read_checkpoint -cell $instanceCELL $synthDir/$itRM\_$moduleRef.dcp
    set_property HD.RECONFIGURABLE 1 [get_cells $instanceCELL]
}
write_checkpoint -force $checksDir/reference.dcp


# Define areas
############################

foreach itRM $rmList {
    set instanceNAME [rmGroup::getRMInstanceName $itRM]
    set instanceCELL [rmGroup::getRMCell $itRM]
    set hwResources [rmGroup::getRMHwResources $itRM]
    set rmResetAfterReconfig [rmGroup::getRMResetAfterReconfig $itRM]
    set rmSnappingMode [rmGroup::getRMSnappingMode $itRM]

    startgroup
    create_pblock pblock_$instanceNAME
    resize_pblock pblock_$instanceNAME -add $hwResources
    add_cells_to_pblock pblock_$instanceNAME [get_cells -quiet $instanceCELL] -clear_locs
    endgroup

   set_property RESET_AFTER_RECONFIG $rmResetAfterReconfig [get_pblocks pblock_$instanceNAME]
   set_property SNAPPING_MODE $rmSnappingMode [get_pblocks pblock_$instanceNAME]
#    set_property RESET_AFTER_RECONFIG 1 [get_pblocks pblock_$instanceNAME]
#    set_property SNAPPING_MODE ON [get_pblocks pblock_$instanceNAME]
#    set_property CONTAIN_ROUTING ON [get_pblocks pblock_$instanceNAME]
}


# Report DRC
############################
set ruledeck "ruledeck_1"
create_drc_ruledeck $ruledeck
add_drc_checks -ruledeck ruledeck_1 [get_drc_checks {HDPR-43 HDPR-33 HDPR-24 HDPR-21 HDPR-20 HDPR-41 HDPR-40 HDPR-30 HDPR-68 HDPR-67 HDPR-66 HDPR-65 HDPR-64 HDPR-63 HDPR-62 HDPR-61 HDPR-60 HDPR-59 HDPR-58 HDPR-57 HDPR-55 HDPR-54 HDPR-53 HDPR-51 HDPR-50 HDPR-49 HDPR-48 HDPR-47 HDPR-46 HDPR-45 HDPR-44 HDPR-42 HDPR-38 HDPR-37 HDPR-36 HDPR-35 HDPR-34 HDPR-32 HDPR-29 HDPR-28 HDPR-27 HDPR-26 HDPR-25 HDPR-23 HDPR-22 HDPR-19 HDPR-18 HDPR-17 HDPR-16 HDPR-15 HDPR-14 HDPR-13 HDPR-12 HDPR-11 HDPR-10 HDPR-9 HDPR-8 HDPR-7 HDPR-6 HDPR-5 HDPR-4 HDPR-3 HDPR-2 HDPR-1}]
report_drc -name drc_1 -ruledecks [list $ruledeck]
delete_drc_ruledeck $ruledeck


# Export Constraints
############################
write_xdc -force $prjName/fplan.xdc


puts "INFO: Reconfig System Created"
###############################################################
