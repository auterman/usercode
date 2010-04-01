C=g++
LD=g++

#O2 for optimization, g for debugging, pg for profiling
SPECIALFLAGS= -g #-g -Wall -pg#-O2
ROOTAUXCFLAGS=$(shell root-config --auxcflags)
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit 
#-I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include 
CFLAGS = $(SPECIALFLAGS) -Wall $(ROOTAUXCFLAGS)
#-L../../lib/$(SRT_SUBDIR)/
LFLAGS = $(SPECIALFLAGS)  

RCXX=$(SPECIALFLAGS) -Wno-deprecated -Wall $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS) #-lboost_thread -lpthread 


SRC=TLimit.cc TLimitDataSource.cc TConfidenceLevel.cc cls.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: cls

clean:
	@rm -f *.o 
	@rm -f cls


TConfidenceLevel.o: TConfidenceLevel.cc
	$(C) $(RCXX) -c TConfidenceLevel.cc

TLimitDataSource.o: TLimitDataSource.cc
	$(C) $(RCXX) -c TLimitDataSource.cc

TLimit.o: TLimit.cc
	$(C) $(RCXX) -c TLimit.cc

cls.o: cls.cc
	$(C) $(RCXX) -c cls.cc

cls: $(SRC:.cc=.o)
		$(LD) $(SRC:.cc=.o) $(RLXX) -o cls
		@echo '-> cls executable created.'

#cls: $(SRC:.cc=.o) cls.cc
#		$(LD) $(RCXX) cls.cc $(RLXX) -o cls
#		@echo '-> cls executable created.'

