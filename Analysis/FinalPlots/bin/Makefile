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

SRC=allHadronic.cpp handleHists.cc ConfigFile.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: allHadronic

clean:
	@rm -f *.o 


ConfigFile.o: ConfigFile.h ConfigFile.cc
	$(C) $(RCXX) -c ConfigFile.cc

handleHists.o: handleHists.h handleHists.cc
	$(C) $(RCXX) -c handleHists.cc

allHadronic.o: handleHists.h ConfigFile.h
	$(C) $(RCXX) -c allHadronic.cc

allHadronic: allHadronic.o handleHists.o ConfigFile.o
	$(LD) allHadronic.o handleHists.o ConfigFile.o $(RLXX) -o allHadronic
	@echo '-> executable allHadronic created.'

