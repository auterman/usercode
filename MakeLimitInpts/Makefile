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

SRC=ConfigFile.cc table.cc mkInputs.cc
SCAN=${scan}
message=${m}

%.o: %.cc
		$(C) $(RCXX) -c $<

all: mkInputs

clean:
	@rm -f *.o 
	@rm -f mkInputs 

demo:   ## use as: make demo scan=<directory>
	rm -rf DataCards/${SCAN}-Demo
	mkdir DataCards/${SCAN}-Demo
	cp DataCards/${SCAN}/GMSB_700_820_*.txt        			DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}_SingleChannels/GMSB_700_820_*_bin*.txt   	DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}/GMSB_1700_1120_*.txt      			DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}_SingleChannels/GMSB_1700_1120_*_bin*.txt 	DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}/GMSB_800_1620_*.txt       			DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}_SingleChannels/GMSB_800_1620_*_bin*.txt  	DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}/GMSB_1700_720_*.txt      			DataCards/${SCAN}-Demo/.
	cp DataCards/${SCAN}_SingleChannels/GMSB_1700_720_*_bin*.txt 	DataCards/${SCAN}-Demo/.

mkInputs: $(SRC:.cc=.o) 
	$(LD) $(SRC:.cc=.o) $(RLXX) -o $@
	@echo '-> executable $@ created.'

commit:         ## use as: make commit m=<message>
		git add Makefile *.h *.cc
		git commit -m "auto-commit by Makefile: ${message}"	
		git push	
