###############################################################
### Generate Bit Files
###############################################################
proc genBitFile {implementation implDir bitsDir compress} {
    if {[file exists $bitsDir/$implementation.bit] == 1 } {
        puts "INFO: $implementation bit file was already created."
        
    } else {
	open_checkpoint $implDir/$implementation\_route_design.dcp
	set_property SEVERITY {Warning} [get_drc_checks LUTLP-1] 
	#set_property bitstream.general.perFrameCRC yes [current_design]
	#set_property bitstream.general.compress true [current_design]
	if {$compress} {
		set_property bitstream.general.compress true [current_design]
	}
	write_bitstream -force -bin -file $bitsDir/$implementation.bit
	close_project
    }
}

# Reference
###################
puts "INFO: Generating Bit Files"
genBitFile reference $implDir $bitsDir $compressReference
puts "INFO: Reference Bit File Created"

# Configurations
###################
set cfgList [cfgGroup::getCFG_IDs]
foreach itCFG $cfgList {
    genBitFile $itCFG $implDir $bitsDir [cfgGroup::getCfgBitstreamCompress $itCFG]
    puts "INFO: $itCFG Bit File Created"
}

# Blanking
###################
if {$createBlanking} {
    genBitFile blanking $implDir $bitsDir $compressBlanking
    puts "INFO: Blanking Bit File Created"
}
###############################################################

