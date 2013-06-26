C=c++
LD=c++

#O2 for optimization, g for debugging, pg for profiling
SPECIALFLAGS= -g #-g -Wall -pg#-O2
ROOTAUXCFLAGS=$(shell root-config --auxcflags)
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) #-lMath # -lMinuit
CFLAGS = $(SPECIALFLAGS) -Wall $(ROOTAUXCFLAGS)
LFLAGS = $(SPECIALFLAGS) #-lz 

RCXX=$(SPECIALFLAGS) -Wno-deprecated -Wall $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS) -lstdc++

SRC=ConfigFile.cc table.cc anyoption.cc Combine.cc mergeInputs.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: mergeInputs

clean:
	@rm -f *.o 
	@rm -f mergeInputs 


table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc

ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

Combine.o: Combine.cc Combine.h
		$(C) $(RCXX) -c Combine.cc 

anyoption.o: anyoption.cc anyoption.h
		$(C) $(RCXX) -c anyoption.cc 

mergeInputs.o: mergeInputs.cc
	$(C) $(RCXX) -c mergeInputs.cc

mergeInputs: $(SRC:.cc=.o) 
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o mergeInputs
	@echo '-> executable mergeInputs created.'

