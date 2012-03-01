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

SRC=ConfigFile.cc table.cc anyoption.cc demo.cc 

%.o: %.cc
		$(C) $(RCXX) -c $<

all: demo


table.o: table.cc table.h column.h
	$(C) $(RCXX) -c table.cc

ConfigFile.o: ConfigFile.cc ConfigFile.h
		$(C) $(RCXX) -c ConfigFile.cc 

anyoption.o: anyoption.cc anyoption.h
		$(C) $(RCXX) -c anyoption.cc 

demo.o: demo.cc demo.h
		$(C) $(RCXX) -c demo.cc 

demo: $(SRC:.cc=.o) 
		$(LD) $(SRC:.cc=.o) $(RLXX) $(JCORR) -o demo
		@echo '-> demo executable created.'

clean:
		@rm -f *.o 
		@rm -f demo
