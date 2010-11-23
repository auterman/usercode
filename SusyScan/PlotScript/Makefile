C=g++
LD=g++
F77=g77
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit

CFLAGS = $(SPECIALFLAGS) -I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated -m32 -Wa,--32
LFLAGS = $(SPECIALFLAGS) -L../../lib/$(SRT_SUBDIR)/ -lz -m32 


RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC=SusyScan.cc ConfigFile.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: scan plot


ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

SusyScan.o: SusyScan.cc SusyScan.h
		$(C) $(RCXX) -c SusyScan.cc 

scan.o: scan.cc scan.h
		$(C) $(RCXX) -c scan.cc 

scan: $(SRC:.cc=.o) scan.o 
		$(LD) $(SRC:.cc=.o) scan.o $(RLXX) $(JCORR) -o scan
		@echo '-> scan executable created.'

plot.o: plot.cc plot.h
		$(C) $(RCXX) -c plot.cc 

plot: $(SRC:.cc=.o) plot.o 
		$(LD) $(SRC:.cc=.o) plot.o $(RLXX) $(JCORR) -o plot
		@echo '-> plot executable created.'

clean:
		@rm -f *.o 
		@rm -f scan plot
