###############################################################
### Export SDK
###############################################################
puts "INFO: Export SDK"

source TCL/fileSystem.tcl
source design.tcl

open_project $prjName/$prjName.xpr

generate_target all [get_files $originDir/$prjName/$prjName.srcs/sources_1/bd/$designName/$designName.bd]

# Export
exec cp $bitsDir/reference.bit $sdkDir/
write_hwdef -force -file $sdkDir/$designName\_wrapper.hdf
write_sysdef -force -hwdef $sdkDir/$designName\_wrapper.hdf -bitfile $sdkDir/reference.bit -file $sdkDir/$designName\_wrapper_sys.hdf

close_project

puts "INFO: Project closed"
###############################################################
