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

SRC=ConfigFile.cc table.cc anyoption.cc MyConfig.cc demo.cc 

%.o: %.cc
		$(C) $(RCXX) -c $<

all: demo


demo: $(SRC:.cc=.o) 
		$(LD) $(SRC:.cc=.o) $(RLXX) $(JCORR) -o $@
		@echo '-> $@ executable created.'

plain:
		./demo --dir GMSB_SqGl-Bino --mcha1 5000
		./demo --dir GMSB_SqGl-Wino --mchi1 5000 --mcha1 375

clean:
		@rm -f *.o 
		@rm -f demo
