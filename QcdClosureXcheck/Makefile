C=g++
LD=g++

#O2 for optimization, g for debugging, pg for profiling
SPECIALFLAGS= -g  #-g -Wall -pg#-O2
ROOTAUXCFLAGS=$(shell root-config --auxcflags)
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) #-lMath # -lMinuit
CFLAGS = $(SPECIALFLAGS) -Wall $(ROOTAUXCFLAGS)
LFLAGS = $(SPECIALFLAGS) #-lz 

RCXX=$(SPECIALFLAGS) -Wno-deprecated -Wall $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS) 

SRC=PhotonSelector.cc QcdAnalysis.cc MySelector.cc 


%.o: %.cc
	$(C) $(RCXX) -c $< -o $@

all: QcdAnalysis

clean:
	@rm -f *.o QcdAnalysis

QcdAnalysis:  $(SRC:.cc=.o)
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o QcdAnalysis
	@echo '-> executable QcdAnalysis created.'

