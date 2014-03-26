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


mkInputs: $(SRC:.cc=.o) 
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o $@
	@echo '-> executable $@ created.'

