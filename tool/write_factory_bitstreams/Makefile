MODULE = write_factory_bitstreams

CC = g++

INCLUDE_PATH  = -I .

CC_FLAGS +=-g -std=c++11 -ansi -Wall -fPIC

OBJECTS = utils.o ZipData.o CleanHeader.o $(MODULE).o 

all: $(MODULE)

%.o: %.cc
	$(CC) $(CC_FLAGS) $(INCLUDE_PATH) -c -o $@ $^

$(MODULE): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(INCLUDE_PATH) -o $(MODULE) $^


clean:
	rm -f *.o $(MODULE) *~
