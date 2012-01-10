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

SRC=table.cc Event.cc QCDrs.cc Smear.cc JetResolution.cc NtupleSelector.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: QCDrs

clean:
	@rm -f *.o 
	@rm -f QCDrs 


table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc

Event.o: Event.h Event.cc
	$(C) $(RCXX) -c Event.cc

Selection.o: Selection.h Selection.cc
	$(C) $(RCXX) -c Selection.cc

NtupleSelector.o: NtupleSelector.h NtupleSelector.cc
	$(C) $(RCXX) -c NtupleSelector.cc

Smear.o: Smear.h Smear.cc
	$(C) $(RCXX) -c Smear.cc

JetResolution.o: JetResolution.h JetResolution.cc
	$(C) $(RCXX) -c JetResolution.cc

QCDrs.o: QCDrs.cc
	$(C) $(RCXX) -c QCDrs.cc

QCDrs: $(SRC:.cc=.o) table.h 
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o QCDrs
	@echo '-> executable QCDrs created.'

