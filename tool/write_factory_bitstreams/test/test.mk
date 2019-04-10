TOOL=../write_factory_bitstreams


all: testLE test


test:
	$(TOOL) -z bitstream.bit out.bit
	$(TOOL) -e out.bit out2.bit
	diff out.bit zip.bit
	diff out.bit_full out2.bit
	diff out.bit_full aligned.bit
	$(RM) out*

testLE:
	$(TOOL) -le -z bitstreamLE.bit out.bit
	$(TOOL) -le -e out.bit out2.bit
	diff out.bit zipLE.bit
	diff out.bit_full out2.bit
	diff out.bit_full alignedLE.bit
	$(RM) out*


clean:
	$(RM) -rf out* *~
