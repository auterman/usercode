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

SRC=TLimit.cc TLimitDataSource.cc TConfidenceLevel.cc solve.cc table.cc cls.cc ConfigFile.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: Limit rooStat

clean:
	@rm -f *.o 
	@rm -f *.ps
	@rm -f Limit doEfficiencies drawTT scan1D rooStat


TConfidenceLevel.o: TConfidenceLevel.cc
	$(C) $(RCXX) -c TConfidenceLevel.cc

TLimitDataSource.o: TLimitDataSource.cc
	$(C) $(RCXX) -c TLimitDataSource.cc

TLimit.o: TLimit.cc
	$(C) $(RCXX) -c TLimit.cc

solve.o: solve.cc solve.h 
	$(C) $(RCXX) -c solve.cc

ConfigFile.o: ConfigFile.h ConfigFile.cc
	$(C) $(RCXX) -c ConfigFile.cc

cls.o: cls.cc cls.h solve.h 
	$(C) $(RCXX) -c cls.cc

table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc


Limit.o: Limit.cc ConfigFile.h
	$(C) $(RCXX) -c Limit.cc

Limit: $(SRC:.cc=.o) Limit.o
	$(LD) $(SRC:.cc=.o) Limit.o $(RLXX) -o Limit
	@echo '-> executable Limit created.'



rooStat.o: rooStat.cc ConfigFile.h
	$(C) $(RCXX)  -c rooStat.cc

rooStat: $(SRC:.cc=.o) rooStat.o
	$(LD) $(SRC:.cc=.o) rooStat.o $(RLXX) -lRooStats -lRooFitCore -lRooFit -lMinuit -lFoam -o rooStat
	@echo '-> executable rooStat created.'


