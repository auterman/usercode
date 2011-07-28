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

SRC=ConfigFile.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: Limit

clean:
	@rm -f *.o 
	@rm -f Limit 


ConfigFile.o: ConfigFile.h ConfigFile.cc
	$(C) $(RCXX) -c ConfigFile.cc


Limit.o: Limit.cc ConfigFile.h
	$(C) $(RCXX) -c Limit.cc

Limit: $(SRC:.cc=.o) Limit.o
	$(LD) $(SRC:.cc=.o) Limit.o $(RLXX) -lMathMore -lRooStats -lRooFitCore -lRooFit	-lMinuit -lFoam  -o Limit
	@echo '-> executable Limit created.'

