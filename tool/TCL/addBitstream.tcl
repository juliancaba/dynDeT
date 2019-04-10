###############################################################
### Add reference bitstream
###############################################################
source TCL/fileSystem.tcl
source design.tcl

exec cp $bitsDir/reference.bit $sdkDir/
set rootDir [pwd]
cd $sdkDir/
exec zip $designName\_wrapper.hdf reference.bit
cd $rootDir


puts "INFO: Added Reference bitstream"
###############################################################

