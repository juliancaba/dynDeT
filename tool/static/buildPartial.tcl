source design.tcl


###############################################################
### Flow
###############################################################
set topName [fileGroup::getTopName $staticFiles]
if {$topName eq ""} {
    set topName $designName
}


### Create fileSystem
source TCL/fileSystem.tcl

### Synth Reconfig Modules
source TCL/synth_rms.tcl

### Create Other Configurations
source TCL/par_configurations.tcl

### Verify Configurations
source TCL/impl_verify.tcl

### Generate Bit Files
source TCL/bit_gen.tcl
##############################################################




