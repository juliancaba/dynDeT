package provide rmGroup 1.0


namespace eval ::rmGroup {
    # Set up state
    variable rmGroup [list]
    variable rmGroupAttributes [list]
}


proc ::rmGroup::create {rmName} {
    variable rmGroup
    variable rmGroupAttributes

    if {[lsearch -exact $rmGroup $rmName] >= 0} {
	puts "WARNING: $rmName reconfig module is already stored"
	return
    }    
    
    array set attributes [list {ID} $rmName \
			      {SRC_LOC} "" \
			      {INSTANCE_NAME} "" \
			      {CELL} "" \
			      {HW_RESOURCES} [list] \
			      {MODULES_ID} [list] \
			      {MODULES_FILES} [list] \
                              {RESET_AFTER_RECONFIG} OFF \
                              {SNAPPING_MODE} OFF]
    lappend rmGroupAttributes [array get attributes]
    lappend rmGroup $rmName

    return $rmName
}


proc ::rmGroup::getRMAttributes {rmID} {
    variable rmGroup
    variable rmGroupAttributes

    set it [lsearch -exact $rmGroup $rmID]
    return [lindex $rmGroupAttributes $it]
}


proc ::rmGroup::getRM_IDs {} {
    variable rmGroup

    return $rmGroup
}


proc ::rmGroup::getRMAttribute {rmID attribute} {
    variable rmGroup
    variable rmGroupAttributes

    set it [lsearch -exact $rmGroup $rmID]
    array set attributes [lindex $rmGroupAttributes $it]
    return $attributes($attribute)
}


proc ::rmGroup::getRMSrcLocation {rmID} {
    return [getRMAttribute $rmID SRC_LOC]
}


proc ::rmGroup::getRMInstanceName {rmID} {
    return [getRMAttribute $rmID INSTANCE_NAME]
}

proc ::rmGroup::getRMCell {rmID} {
    variable loc
    variable name
    set loc [getRMAttribute $rmID SRC_LOC]
    set name [getRMAttribute $rmID INSTANCE_NAME]
    
    if { $loc == "" } {
	return $name
    }
    return $loc/$name
}



proc ::rmGroup::getRMHwResources {rmID} {
    return [getRMAttribute $rmID HW_RESOURCES]
}


proc ::rmGroup::getRM_Modules {rmID} {
    return [getRMAttribute $rmID MODULES_ID]
}

proc ::rmGroup::getRMResetAfterReconfig {rmID} {
    return [getRMAttribute $rmID RESET_AFTER_RECONFIG]
}

proc ::rmGroup::getRMSnappingMode {rmID} {
    return [getRMAttribute $rmID SNAPPING_MODE]
}


proc ::rmGroup::setAttribute {rmID value attribute} {
    variable rmGroup
    variable rmGroupAttributes

    set it [lsearch -exact $rmGroup $rmID]
    array set attributesAux [lindex $rmGroupAttributes $it]
    set attributesAux($attribute) $value
    set rmGroupAttributes [lreplace $rmGroupAttributes $it $it [array get attributesAux]]
}


proc ::rmGroup::setSrcLocation {rmID value} {
    setAttribute $rmID $value SRC_LOC
}


proc ::rmGroup::setHwResources {rmID value} {
    setAttribute $rmID $value HW_RESOURCES
}


proc ::rmGroup::setInstanceName {rmID value} {
    setAttribute $rmID $value INSTANCE_NAME
}

proc ::rmGroup::setResetAfterReconfig {rmID value} {
    setAttribute $rmID $value RESET_AFTER_RECONFIG
}

proc ::rmGroup::setSnappingMode {rmID value} {
    setAttribute $rmID $value SNAPPING_MODE
}


proc ::rmGroup::addNewModule {rmID moduleName files} {
    variable rmGroup
    variable rmGroupAttributes

    if {[lsearch -exact [getRMAttribute $rmID MODULES_ID] $moduleName] >= 0} {
	puts "WARNING: $moduleName module is already stored"
	return
    }   


    set it [lsearch -exact $rmGroup $rmID]
    array set attributesAux [lindex $rmGroupAttributes $it]
    lappend attributesAux(MODULES_ID) $moduleName
    lappend attributesAux(MODULES_FILES) $files
    set rmGroupAttributes [lreplace $rmGroupAttributes $it $it [array get attributesAux]]
}


proc ::rmGroup::getGroupFileID {rmID modID} {
    variable rmGroup
    variable rmGroupAttributes

    set itRM [lsearch -exact $rmGroup $rmID]
    array set attributes [lindex $rmGroupAttributes $itRM]
    set modulesAux $attributes(MODULES_ID)
    set itMod [lsearch -exact $modulesAux $modID]
    return [lindex $attributes(MODULES_FILES) $itMod]
}



