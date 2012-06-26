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

SRC=Variable.cc ConfigFile.cc GeneratorMasses.cc PlotTools.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: plot plotDiPhoton


ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

Event.o: Event.cc Event.h
		$(C) $(RCXX) -c Event.cc 

EventDiPhoton.o: EventDiPhoton.cc Event.h
		$(C) $(RCXX) -c EventDiPhoton.cc 

Variable.o: Variable.cc Variable.h
		$(C) $(RCXX) -c Variable.cc 

GeneratorMasses.o: GeneratorMasses.cc GeneratorMasses.h
		$(C) $(RCXX) -c GeneratorMasses.cc 

PlotTools.o: PlotTools.cc PlotTools.h 
		$(C) $(RCXX) -c PlotTools.cc 

TheLimits.o: TheLimits.cc TheLimits.h
		$(C) $(RCXX) -c TheLimits.cc 

plot.o: plot.cc plot.h StyleSettings.h StyleSettings_SinglePhoton7TeV.h StyleSettings_DiPhoton.h 
		$(C) $(RCXX) -c plot.cc 

plot: $(SRC:.cc=.o) Event.o plot.o 
		$(LD) $(SRC:.cc=.o) Event.o plot.o $(RLXX) $(JCORR) -o plot
		@echo '-> plot executable created.'

plotDiPhoton.o: plotDiPhoton.cc plot.h StyleSettings.h StyleSettings_DiPhoton.h 
		$(C) $(RCXX) -c plotDiPhoton.cc 

plotDiPhoton: $(SRC:.cc=.o) EventDiPhoton.o plotDiPhoton.o 
		$(LD) $(SRC:.cc=.o) EventDiPhoton.o plotDiPhoton.o $(RLXX) $(JCORR) -o plotDiPhoton
		@echo '-> plotDiPhoton executable created.'

clean:
		@rm -f *.o 
		@rm -f plot
