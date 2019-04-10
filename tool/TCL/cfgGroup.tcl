package provide cfgGroup 1.0


namespace eval ::cfgGroup {
    # Set up state
    variable cfgGroup [list]
    variable cfgGroupAttributes [list]
}


proc ::cfgGroup::create {cfgName} {
    variable cfgGroup
    variable cfgGroupAttributes

    if {[lsearch -exact $cfgGroup $cfgName] >= 0} {
	puts "WARNING: $cfgName configuration is already stored"
	return
    }    
    
    array set attributes [list {ID} $cfgName \
			      {RM} [list] \
			      {MODULE} [list] \
			      {MODE} [list] \
                              {BITSTREAM_GENERAL_COMPRESS} off]
    lappend cfgGroupAttributes [array get attributes]
    lappend cfgGroup $cfgName

    return $cfgName
}


proc ::cfgGroup::getCFG_IDs {} {
    variable cfgGroup

    return $cfgGroup
}


proc ::cfgGroup::addNewModule {cfgID rmID modID mode} {
    variable cfgGroup
    variable cfgGroupAttributes

    set it [lsearch -exact $cfgGroup $cfgID]
    array set attributesAux [lindex $cfgGroupAttributes $it]
    lappend attributesAux(RM) $rmID
    lappend attributesAux(MODULE) $modID
    lappend attributesAux(MODE) $mode
    set cfgGroupAttributes [lreplace $cfgGroupAttributes $it $it [array get attributesAux]]
}


proc ::cfgGroup::getConfigurationAttribute {cfgID attribute} {
    variable cfgGroup
    variable cfgGroupAttributes

    set it [lsearch -exact $cfgGroup $cfgID]
    array set attributesAux [lindex $cfgGroupAttributes $it]
    return $attributesAux($attribute)
}


proc ::cfgGroup::getConfigurationRMs {cfgID} {
    return [getConfigurationAttribute $cfgID RM]
}


proc ::cfgGroup::getConfigurationModules {cfgID} {
    return [getConfigurationAttribute $cfgID MODULE]
}


proc ::cfgGroup::getConfigurationModes {cfgID} {
    return [getConfigurationAttribute $cfgID MODE]
}

proc ::cfgGroup::getCfgBitstreamCompress {cfgID} {
    return [getConfigurationAttribute $cfgID BITSTREAM_GENERAL_COMPRESS]
}

proc ::cfgGroup::setBitstreamCompress {cfgID value} {
    variable cfgGroup
    variable cfgGroupAttributes

    set it [lsearch -exact $cfgGroup $cfgID]
    array set attributesAux [lindex $cfgGroupAttributes $it]
    set attributesAux(BITSTREAM_GENERAL_COMPRESS) $value
    set cfgGroupAttributes [lreplace $cfgGroupAttributes $it $it [array get attributesAux]]
}

