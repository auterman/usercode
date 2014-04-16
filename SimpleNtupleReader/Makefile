C=c++
LD=c++
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit

CFLAGS = $(SPECIALFLAGS) -fPIC  -I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated
LFLAGS = $(SPECIALFLAGS) -lz 


RCXX=$(CFLAGS) -c -fPIC -Wno-deprecated  $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

#SRC=GJets_Photon.cc GJets_Jet.cc QCD_Photon.cc QCD_Jet.cc Interface.cc Tools.cc Reader.cc
SRC=$(wildcard *.cc)

%.o: %.cc %.h
		$(C) $(RCXX) -c $<
		

all: Reader


Reader: $(SRC:.cc=.o) 
		$(LD) $(SRC:.cc=.o)  $(RLXX) $(JCORR) -o $@
		@echo '-> $@ executable created.'

clean:
		@rm -f *.o 
		@rm -f Reader
