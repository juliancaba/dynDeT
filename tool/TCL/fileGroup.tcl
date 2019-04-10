package provide fileGroup 1.0


namespace eval ::fileGroup {
    # Set up state
    variable fGroup
    variable fGroupAttributes
    variable id 0
}


proc ::fileGroup::getTopName {fgID} {
    variable fGroupAttributes

    for {set it 0} {$it < [llength $fGroupAttributes($fgID)]} {incr it} {
	set fileAux [lindex $fGroupAttributes($fgID) $it]
	array set attrAux $fileAux	
	if {$attrAux(TOP) eq "TRUE"} {
	    return $attrAux(NAME)
	}
	unset attrAux
    }
    return ""
}


proc ::fileGroup::create {} {
    variable fGroup
    variable fGroupAttributes
    variable id

    set fgID "fGroup[incr id]"
    set fGroup($fgID) [list]
    set fGroupAttributes($fgID) [list]
    return $fgID
}


proc ::fileGroup::add {fgID path fileName {top "FALSE"}} {
    variable fGroup
    variable fGroupAttributes
    
    set fullFile $path/$fileName
    
    if {[lsearch -exact $fGroup($fgID) $fullFile] >= 0} {
	puts "WARNING: $fullFile is already stored"	
	return 
    }

    if {$top eq "TRUE"} {
	set topFile [getTopName $fgID]
	puts "TOP:  $topFile"
	if {$topFile ne ""} {
	    puts "WARNING: $topFile is the top ($fileName will not be stored!!!)"
	    return
	}
    }

    lappend fGroup($fgID) $fullFile
    array set attributes [list {PATH} $path \
			       {NAME} [lindex [split $fileName {.}] 0] \
			       {EXT} [lindex [split $fileName {.}] 1] \
			      {TOP} [string toupper $top]]
	
    lappend fGroupAttributes($fgID) [array get attributes]
}


proc ::fileGroup::getFileGroup {fgID} {
    variable fGroup

    return $fGroup($fgID)
}


proc ::fileGroup::getFileGroupSize {fgID} {
    variable fGroup

    return [llength $fGroup($fgID)]
}


proc ::fileGroup::getFileGroupAttributes {fgID} {
    variable fGroupAttributes

    return $fGroupAttributes($fgID)
}


proc ::fileGroup::getFileAttributes {fgID fileIndex} {
    variable fGroupAttributes

    return [lindex $fGroupAttributes($fgID) $fileIndex]
}


proc ::fileGroup::getFileAttribute {fgID fileIndex attribute} {
    variable fGroupAttributes

    array set attributes [lindex $fGroupAttributes($fgID) $fileIndex]
    return $attributes($attribute)
}
