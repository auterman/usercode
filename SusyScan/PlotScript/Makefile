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

SRC=SusyScan.cc ConfigFile.cc GeneratorMasses.cc PlotTools.cc PrintTools.cc TheLimits.cc 

%.o: %.cc
		$(C) $(RCXX) -c $<

all: plot3tb plot3tb_extended plot10tb plot10tb_extended plot50tb plotComp


ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

SusyScan.o: SusyScan.cc SusyScan.h
		$(C) $(RCXX) -c SusyScan.cc 

GeneratorMasses.o: GeneratorMasses.cc GeneratorMasses.h
		$(C) $(RCXX) -c GeneratorMasses.cc 

PlotTools.o: PlotTools.cc PlotTools.h GlobalFunctions.h
		$(C) $(RCXX) -c PlotTools.cc 

PrintTools.o: PrintTools.cc PrintTools.h GlobalFunctions.h
		$(C) $(RCXX) -c PrintTools.cc 

TheLimits.o: TheLimits.cc TheLimits.h
		$(C) $(RCXX) -c TheLimits.cc 

plot3tb.o: plot3tb.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plot3tb.cc 

plot3tb: $(SRC:.cc=.o) plot3tb.o 
		$(LD) $(SRC:.cc=.o) plot3tb.o $(RLXX) $(JCORR) -o plot3tb
		@echo '-> plot3tb executable created.'

plot3tb_extended.o: plot3tb_extended.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plot3tb_extended.cc 

plot3tb_extended: $(SRC:.cc=.o) plot3tb_extended.o 
		$(LD) $(SRC:.cc=.o) plot3tb_extended.o $(RLXX) $(JCORR) -o plot3tb_extended
		@echo '-> plot3tb_extended executable created.'

plot10tb.o: plot10tb.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plot10tb.cc 

plot10tb: $(SRC:.cc=.o) plot10tb.o 
		$(LD) $(SRC:.cc=.o) plot10tb.o $(RLXX) $(JCORR) -o plot10tb
		@echo '-> plot10tb executable created.'

plot10tb_extended.o: plot10tb_extended.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plot10tb_extended.cc 

plot10tb_extended: $(SRC:.cc=.o) plot10tb_extended.o 
		$(LD) $(SRC:.cc=.o) plot10tb_extended.o $(RLXX) $(JCORR) -o plot10tb_extended
		@echo '-> plot10tb_extended executable created.'

plot50tb.o: plot50tb.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plot50tb.cc 

plot50tb: $(SRC:.cc=.o) plot50tb.o 
		$(LD) $(SRC:.cc=.o) plot50tb.o $(RLXX) $(JCORR) -o plot50tb
		@echo '-> plot50tb executable created.'

plotComp.o: plotComp.cc GlobalFunctions.h StyleSettings.h
		$(C) $(RCXX) -c plotComp.cc 

plotComp: $(SRC:.cc=.o) plotComp.o 
		$(LD) $(SRC:.cc=.o) plotComp.o $(RLXX) $(JCORR) -o plotComp
		@echo '-> plotComp executable created.'

clean:
		@rm -f *.o 
		@rm -f plot10tb plot10tb_extended plot3tb plot3tb_extended plot50tb plotComp
		
