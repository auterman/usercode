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

tables:
		#old used for 8 TeV, 4fb-1
		./demo --dir Bino --mchi1  375 --mcha1 5000		> ResultTable_Bino_Sq700_Gl820.txt
		./demo --dir Bino --mchi1  375 --mcha1 5000 --style tex --caption > ResultTable_Bino_Sq700_Gl820.tex
		./demo --dir Wino --mchi1 5000 --mcha1  375		> ResultTable_Wino_Sq700_Gl820.txt
		./demo --dir Wino --mchi1 5000 --mcha1  375 --style tex --caption > ResultTable_Wino_Sq700_Gl820.tex
		#new 8TeV 20fb-1, as defined in mail to Valentina of 21.3.14
		./demo --dir Bino --mchi1 375 --mcha1 5000 --msquark 1700 --mgluino 1120	     > ResultTable_Bino_Sq1700_Gl1120.txt
		./demo --dir Bino --mchi1 375 --mcha1 5000 --msquark 1700 --mgluino 1120 --style tex --caption > ResultTable_Bino_Sq1700_Gl1120.tex
		./demo --dir Bino --mchi1 375 --mcha1 5000 --msquark  800 --mgluino 1620	     > ResultTable_Bino_Sq800_Gl1620.txt
		./demo --dir Bino --mchi1 375 --mcha1 5000 --msquark  800 --mgluino 1620 --style tex --caption > ResultTable_Bino_Sq800_Gl1620.tex
		./demo --dir Wino --mchi1 5000 --mcha1 375 --msquark 1500 --mgluino  820	     > ResultTable_Wino_Sq1500_Gl820.txt
		./demo --dir Wino --mchi1 5000 --mcha1 375 --msquark 1500 --mgluino  820 --style tex --caption > ResultTable_Wino_Sq1500_Gl820.tex
		./demo --dir Wino --mchi1 5000 --mcha1 375 --msquark  900 --mgluino 1320	     > ResultTable_Wino_Sq900_Gl1320.txt
		./demo --dir Wino --mchi1 5000 --mcha1 375 --msquark  900 --mgluino 1320 --style tex --caption > ResultTable_Wino_Sq900_Gl1320.tex

cp:		
		cp *.tex SUS-14-004/.
		cp *.tex AN-13-207/.

clean:
		@rm -f *.o 
		@rm -f demo
