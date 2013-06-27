C=c++
LD=c++

SYSTEM=$(shell python -c "from platform import platform; print platform()")
ifeq (${SYSTEM},Darwin-12.3.0-x86_64-i386-64bit) 
	GCCXMLPATH=/usr/local/bin/
else 
	GCCXMLPATH=/cvmfs/cms.cern.ch/slc5_amd64_gcc462/external/gccxml/20110825/bin
endif

SRC=Factory.cc Analyzer.cc Event.cc Selection.cc table.cc
SEL=$(wildcard include/*Selector.cc)

#O2 for optimization, g for debugging, pg for profiling
SPECIALFLAGS=  #-g -Wall -pg#-O2
ROOTAUXCFLAGS=$(shell root-config --auxcflags)
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lReflex -lReflexDict -lCintex -lCint
CFLAGS = $(SPECIALFLAGS) -fPIC -Wall $(ROOTAUXCFLAGS)
LFLAGS = $(SPECIALFLAGS)  #-lz 

RCXX=$(SPECIALFLAGS)  -fPIC -Wno-deprecated -Wall $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

DICT=$(SEL:.cc=Dict.cc)
RFLX=$(DICT:Dict.cc=_rflx.cpp)
LIBS=$(SEL:.cc=.so)


all: main QcdClosure CutStudies

clean:
	@rm -f *.o *.so *.pdf include/*Dict.cc include/*_rflx.cpp include/*.so
	@rm -f main 
	@rm -rf *.dSYM

	
reflex: $(RFLX) $(DICT)

include/%_rflx.cpp: $(SEL)
	@export GCCXML_COMPILER=gcc
	@genreflex $(SEL:.cc=.h) -o include --gccxmlpath=${GCCXMLPATH} --gccxmlopt="-I${ROOTSYS}/include --gccxml-compiler gcc"

include/%Dict.cc: $(RFLX)
	@ln -s -f $(subst Dict.cc,_rflx.cpp,$@) $@



lib%.so:  %.cc %Dict.cc
	$(LD) $^  $(RCXX) $(RLXX) -shared -o $@
	@echo '-> $@ created.'

main: $(SRC) MainLoop.cc MainLoop.h $(SRC:.cc=.h) $(subst include/,include/lib,$(LIBS))
	$(LD) $(SRC) MainLoop.cc $(subst include/,include/lib,$(LIBS)) $(RCXX) $(RLXX) -L. -o $@
	@echo '-> executable $@ created.'

QcdClosure: $(SRC) QcdClosure.cc QcdClosure.h $(SRC:.cc=.h) $(subst include/,include/lib,$(LIBS))
	$(LD) $(SRC) QcdClosure.cc $(subst include/,include/lib,$(LIBS)) $(RCXX) $(RLXX) -L. -o $@
	@echo '-> executable $@ created.'

CutStudies: $(SRC) CutStudies.cc CutStudies.h $(SRC:.cc=.h) $(subst include/,include/lib,$(LIBS))
	$(LD) $(SRC) CutStudies.cc $(subst include/,include/lib,$(LIBS)) $(RCXX) $(RLXX) -L. -o $@
	@echo '-> executable $@ created.'
