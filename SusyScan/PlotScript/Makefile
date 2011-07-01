C=g++
LD=g++
F77=g77
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit

CFLAGS = $(SPECIALFLAGS) -I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated -m32 -Wa,--32
LFLAGS = $(SPECIALFLAGS) -L../../lib/$(SRT_SUBDIR)/ -lz -m32 -lg2c


RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC=scan.C

%.o: %.C
		$(C) $(RCXX) -c $<

all: runjunk


scan.o: scan.C scan.h
		$(C) $(RCXX) -c scan.C 

runjunk: $(SRC:.C=.o) 
		$(LD) $(SRC:.C=.o) $(RLXX) $(JCORR) -o crap
		@echo '-> Calibration executable created.'

clean:
		@rm -rf ti_files
		@rm -f *~
		@rm -f *.o 
		@rm -f *#
		@rm -f .nfs*
		@rm -f *.bkp
		@rm -f crap
		@rm -f *.ps
		@rm -f *.eps
		@rm -f *.cfi
		@rm -f fort.*
		@rm -f .#*

