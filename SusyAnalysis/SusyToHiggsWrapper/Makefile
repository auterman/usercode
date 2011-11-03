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

SRC=table.cc Wrapper.cc Chameleon.cc 

%.o: %.cc
		$(C) $(RCXX) -c $<

all: Wrapper

clean:
	@rm -f *.o 
	@rm -f Wrapper 


Chameleon.o: Chameleon.cc Chameleon.h 
	$(C) $(RCXX) -c Chameleon.cc

table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc

Wrapper.o: Wrapper.cc
	$(C) $(RCXX) -c Wrapper.cc

Wrapper: $(SRC:.cc=.o) table.h Chameleon.h
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o Wrapper
	@echo '-> executable Wrapper created.'

