###############################################################
### Define Part, Package, Speedgrade 
###############################################################
set reportDir Reports
set synthDir Synth
set implDir Implementations
set checksDir Checkpoints
set bitsDir Bitstreams
set sdkDir sdk
###############################################################



###############################################################
### Create fileSystem
###############################################################
if {[file exists $reportDir] == 0} {
    file mkdir $reportDir
}

if {[file exists $synthDir] == 0} {
    file mkdir $synthDir
}

if {[file exists $implDir] == 0} {
    file mkdir $implDir
}

if {[file exists $checksDir] == 0} {
    file mkdir $checksDir
}

if {[file exists $bitsDir] == 0} {
    file mkdir $bitsDir
}

if {[file exists $sdkDir] == 0} {
    file mkdir $sdkDir
}
###############################################################
