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

### Create Vivado prj
source TCL/vivado_create.tcl

### Create and Synth Static Part
source TCL/rest_create_and_synth.tcl

### Synth Reconfig Modules
source TCL/synth_rms.tcl

### Create Reconfig System and Reference design
source TCL/reconfig_system_create.tcl
source TCL/reference_configuration_create.tcl

### Create Other Configurations
source TCL/par_configurations.tcl

### Create Blanking Configuration
#source TCL/blanking_create.tcl

### Verify Configurations
#source TCL/impl_verify.tcl

### Generate Bit Files
source TCL/bit_gen.tcl
##############################################################




