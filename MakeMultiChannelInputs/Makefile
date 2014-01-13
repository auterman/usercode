C=g++
LD=g++

#O2 for optimization, g for debugging, pg for profiling
SPECIALFLAGS= -g #-g -Wall -pg#-O2
ROOTAUXCFLAGS=$(shell root-config --auxcflags)
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) #-lMath # -lMinuit
CFLAGS = $(SPECIALFLAGS) -Wall $(ROOTAUXCFLAGS)
LFLAGS = $(SPECIALFLAGS) #-lz 

RCXX=$(SPECIALFLAGS) -Wno-deprecated -Wall $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS) 

SRC=ConfigFile.cc table.cc mkInputs.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: mkInputs

clean:
	@rm -f *.o 
	@rm -f mkInputs 


table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc

ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

mkInputs.o: mkInputs.cc
	$(C) $(RCXX) -c mkInputs.cc

mkInputs: $(SRC:.cc=.o) table.h 
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o mkInputs
	@echo '-> executable mkInputs created.'

