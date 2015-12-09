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
POINT=${point}

%.o: %.cc
		$(C) $(RCXX) -c $<

all: demo


demo: $(SRC:.cc=.o) 
		$(LD) $(SRC:.cc=.o) $(RLXX) $(JCORR) -o $@
		@echo '-> $@ executable created.'

		
table:  ## use as: make table point=<directory>
		./demo --dir ${POINT} --nr 181 --wino 490 --bino 480                       > ResultTable_${POINT}_Wino490_Bino480.txt
		./demo --dir ${POINT} --nr 181 --wino 490 --bino 480 --style tex --caption > ResultTable_${POINT}_Wino490_Bino480.tex
		./demo --dir ${POINT} --nr 317 --wino 640 --bino 630                       > ResultTable_${POINT}_Wino640_Bino630.txt
		./demo --dir ${POINT} --nr 317 --wino 640 --bino 630 --style tex --caption > ResultTable_${POINT}_Wino640_Bino630.tex
		#./demo --dir ${POINT} --nr 21 --wino 740 --bino 730                       > ResultTable_${POINT}_Wino740_Bino730.txt
		#./demo --dir ${POINT} --nr 21 --wino 740 --bino 730 --style tex --caption > ResultTable_${POINT}_Wino740_Bino730.tex
		cp ResultTable_${POINT}_Wino490_Bino480.tex ~/cernbox/SVN/notes/SUS-14-016/trunk/tables/.
		cp ResultTable_${POINT}_Wino640_Bino630.tex ~/cernbox/SVN/notes/SUS-14-016/trunk/tables/.
		
help:
		@echo "make table point=<directory>"		

cp:		
		cp *.tex SUS-14-004/.
		cp *.tex AN-13-207/.

clean:
		@rm -f *.o 
		@rm -f demo

edit:
		sublime -n Makefile demo.h demo.cc&
		
fetch:
		scp nafhh-cms01.desy.de:Releases/CMSSW_6_1_1/src/Jobs/2014*ino-Demo.tar.gz .; ls 2014*ino-Demo.tar.gz
