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


# common object files:
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

Plotting.o: Plotting.cc Plotting.h 
		$(C) $(RCXX) -c Plotting.cc 

TheLimits.o: TheLimits.cc TheLimits.h
		$(C) $(RCXX) -c TheLimits.cc 

# single photon specific
StyleSettings_SinglePhoton.o: StyleSettings.h
		$(C) $(RCXX) -c StyleSettings_SinglePhoton.cc

plot.o: plot.cc plot.h   
		$(C) $(RCXX) -c plot.cc 

plot: $(SRC:.cc=.o) Event.o StyleSettings_SinglePhoton.o Plotting.o plot.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_SinglePhoton.o Event.o plot.o Plotting.o $(RLXX) $(JCORR) -o plot
		@echo '-> plot executable created.'

# di photon specific
StyleSettings_DiPhoton.o: StyleSettings.h
		$(C) $(RCXX) -c StyleSettings_DiPhoton.cc

plotDiPhoton.o: plotDiPhoton.cc plot.h
		$(C) $(RCXX) -c plotDiPhoton.cc 

plotDiPhoton: $(SRC:.cc=.o) EventDiPhoton.o StyleSettings_DiPhoton.o Plotting.o plotDiPhoton.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_DiPhoton.o EventDiPhoton.o plotDiPhoton.o Plotting.o $(RLXX) $(JCORR) -o plotDiPhoton
		@echo '-> plotDiPhoton executable created.'

clean:
		@rm -f *.o 
		@rm -f plot
