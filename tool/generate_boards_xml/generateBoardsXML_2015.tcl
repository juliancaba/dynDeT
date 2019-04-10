### HDADR - Javier Sevilla Ballesteros

# Generate an XML file with the properties of the specified board
proc generateBoardXML {part board} {
    # Create and open a temporal project / design
    create_project -in_memory -part $part
    set_property board_part $board [current_project]
    read_vhdl ./dummy.vhd
    synth_design -mode out_of_context -flatten_hierarchy rebuilt -top dummy -part $part
    
    # Generate XML
    set outputFile $board
    append outputFile ".xml"
    set outputFileID [open $outputFile "w"]
    
    set listOfSites [get_sites]
    set listOfSites_Size [llength $listOfSites]
    
    # Write the header of the XML file
    puts -nonewline $outputFileID "<?xml version='1.0' encoding='UTF-8'?>\n\n"
    puts -nonewline $outputFileID "<!-- HDADR - $board XML definition -->\n\n"
    
    # Write the number of rows and columns of the board
    set rows 0
    set columns 0
    foreach site $listOfSites {
        set site_RPM_X [get_property RPM_X $site]
        set site_RPM_Y [get_property RPM_Y $site]
    
        if {$rows < $site_RPM_Y} {
            set rows $site_RPM_Y
        }
    
        if {$columns < $site_RPM_X} {
            set columns $site_RPM_X
        }
    }

    set rows [expr $rows + 1]
    set columns [expr $columns + 1]

    puts -nonewline $outputFileID "<Board name=\"$board\" rows=\"$rows\" columns=\"$columns\">\n"

    # Write the needed information for each site of the board
    foreach site $listOfSites {
        set site_NAME [get_property NAME $site]
        set site_TYPE [get_property SITE_TYPE $site]
        set site_CLOCKREGION [get_property CLOCK_REGION $site]
        set site_RPM_X [get_property RPM_X $site]
        set site_RPM_Y [get_property RPM_Y $site]
    
        if {![string equal $site_TYPE "TIEOFF"]} {
            puts -nonewline $outputFileID "\t<Site name=\"$site_NAME\" type=\"$site_TYPE\" clkreg=\"$site_CLOCKREGION\" rpmx=\"$site_RPM_X\" rpmy=\"$site_RPM_Y\"/>\n"
        }
    }

    puts -nonewline $outputFileID "</Board>\n"
    
    #Close the output file
    close $outputFileID
    
    #Close the temporal design and project
    close_design
    close_project
}

# ZedBoard
generateBoardXML "xc7z020clg484-1" "em.avnet.com:zed:part0:1.2"
# Kintex-7 KC705 Evaluation Platform (XC7K325T-2FFG900C)
generateBoardXML "xc7k325tffg900-2" "xilinx.com:kc705:part0:1.2"
