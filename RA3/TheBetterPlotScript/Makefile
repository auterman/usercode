C=g++
LD=g++
F77=g77
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit

CFLAGS = $(SPECIALFLAGS) -I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated
LFLAGS = $(SPECIALFLAGS) -lz 


RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC=event.cc variable.cc ConfigFile.cc GeneratorMasses.cc PlotTools.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: plot


ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

event.o: event.cc event.h
		$(C) $(RCXX) -c event.cc 

variable.o: variable.cc variable.h
		$(C) $(RCXX) -c variable.cc 

GeneratorMasses.o: GeneratorMasses.cc GeneratorMasses.h
		$(C) $(RCXX) -c GeneratorMasses.cc 

PlotTools.o: PlotTools.cc PlotTools.h 
		$(C) $(RCXX) -c PlotTools.cc 

TheLimits.o: TheLimits.cc TheLimits.h
		$(C) $(RCXX) -c TheLimits.cc 

plot.o: plot.cc plot.h StyleSettings.h 
		$(C) $(RCXX) -c plot.cc 

plot: $(SRC:.cc=.o) plot.o 
		$(LD) $(SRC:.cc=.o) plot.o $(RLXX) $(JCORR) -o plot
		@echo '-> plot executable created.'

clean:
		@rm -f *.o 
		@rm -f plot
