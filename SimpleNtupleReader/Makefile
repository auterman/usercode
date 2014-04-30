C=c++
LD=c++
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) -lMinuit

CFLAGS = $(SPECIALFLAGS) -fPIC  -I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated
LFLAGS = $(SPECIALFLAGS) -lz 


RCXX=$(CFLAGS) -c -fPIC -Wno-deprecated  $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

#SRC=GJets_Photon.cc GJets_Jet.cc QCD_Photon.cc QCD_Jet.cc Interface.cc Tools.cc Reader.cc
SRC=$(wildcard *.cc)

%.o: %.cc %.h
		$(C) $(RCXX) -c $<
		

all: Reader


Reader: $(SRC:.cc=.o) 
		$(LD) $(SRC:.cc=.o)  $(RLXX) $(JCORR) -o $@
		@echo '-> $@ executable created.'

clean:
		@rm -f *.o 
		@rm -f Reader

cp:
		@rm -rf -f plots/V03.16
		scp -r "portal.physik.rwth-aachen.de:~/GIT/usercode/SimpleNtupleReader/plots/V03.13" plots/.

commit:
		git add Makefile *.h *.cc
		git commit -m "auto-commit by Makefile"	
		git push	

sync:
		rsync  -rtvc --exclude '*.o Reader *.root'  -e ssh  ~/GIT/usercode/SimpleNtupleReader  portal.physik.rwth-aachen.de:~/GIT/usercode/
		rsync  -rtvc --exclude '*.o Reader *.root'  -e ssh  portal.physik.rwth-aachen.de:~/GIT/usercode/SimpleNtupleReader/plots ~/GIT/usercode/SimpleNtupleReader/ 

nedit:
		nedit Makefile GJets_Jet.h Reader.h Reader.cc Interface.h Interface.cc Tools.h Tools.cc &
