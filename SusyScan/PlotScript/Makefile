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

SRC=SusyScan.cc ConfigFile.cc GeneratorMasses.cc PlotTools.cc TheLimits.cc 

%.o: %.cc
		$(C) $(RCXX) -c $<

all: plot plot3tb plot50tb


ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

SusyScan.o: SusyScan.cc SusyScan.h
		$(C) $(RCXX) -c SusyScan.cc 

GeneratorMasses.o: GeneratorMasses.cc GeneratorMasses.h
		$(C) $(RCXX) -c GeneratorMasses.cc 

PlotTools.o: PlotTools.cc PlotTools.h GlobalFunctions.h
		$(C) $(RCXX) -c PlotTools.cc 

TheLimits.o: TheLimits.cc TheLimits.h
		$(C) $(RCXX) -c TheLimits.cc 

plot.o: plot.cc plot.h GlobalFunctions.h
		$(C) $(RCXX) -c plot.cc 

plot: $(SRC:.cc=.o) plot.o 
		$(LD) $(SRC:.cc=.o) plot.o $(RLXX) $(JCORR) -o plot
		@echo '-> plot executable created.'

plot3tb.o: plot3tb.cc GlobalFunctions.h
		$(C) $(RCXX) -c plot3tb.cc 

plot3tb: $(SRC:.cc=.o) plot3tb.o 
		$(LD) $(SRC:.cc=.o) plot3tb.o $(RLXX) $(JCORR) -o plot3tb
		@echo '-> plot3tb executable created.'

plot50tb.o: plot50tb.cc GlobalFunctions.h
		$(C) $(RCXX) -c plot50tb.cc 

plot50tb: $(SRC:.cc=.o) plot50tb.o 
		$(LD) $(SRC:.cc=.o) plot50tb.o $(RLXX) $(JCORR) -o plot50tb
		@echo '-> plot50tb executable created.'

clean:
		@rm -f *.o 
		@rm -f scan plot plot3tb plot50tb
