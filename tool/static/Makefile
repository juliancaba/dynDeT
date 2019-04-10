GEN_FOLDERS = Bitstreams Checkpoints Implementations Reports Synth sdk hd_visual


.PHONY: clean


all: build


build: build.tcl design.tcl
	vivado -mode batch -source $^

buildPartial: buildPartial.tcl design.tcl
	vivado -mode batch -source $^

runSDKExportTCL:
	vivado -mode batch -source TCL/sdk_export.tcl

runSDKLaunchTCL:
	vivado -mode batch -source TCL/sdk_launch.tcl

program: program.tcl
	vivado -mode batch -source $^	


clean:
	$(RM) -rf *~ $(GEN_FOLDERS) vivado* *.html *.log *.jou


