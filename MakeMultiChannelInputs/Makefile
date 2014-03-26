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

%.o: %.cc
		$(C) $(RCXX) -c $<

all: mkInputs

clean:
	@rm -f *.o 
	@rm -f mkInputs 

demo:
	cp DataCards/GMSB_SqGl-Bino/GMSB_700_820_375_5000.txt        			DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Bino_SingleChannels/GMSB_700_820_375_5000_bin*.txt   	DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Bino/GMSB_1700_1120_375_5000.txt      			DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Bino_SingleChannels/GMSB_1700_1120_375_5000_bin*.txt 	DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Bino/GMSB_800_1620_375_5000.txt       			DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Bino_SingleChannels/GMSB_800_1620_375_5000_bin*.txt  	DataCards/SqGl-Bino-Demo/.
	cp DataCards/GMSB_SqGl-Wino/GMSB_700_820_5000_375.txt        			DataCards/SqGl-Wino-Demo/.
	cp DataCards/GMSB_SqGl-Wino_SingleChannels/GMSB_700_820_5000_375_bin*.txt   	DataCards/SqGl-Wino-Demo/.
	cp DataCards/GMSB_SqGl-Wino/GMSB_1500_820_5000_375.txt       			DataCards/SqGl-Wino-Demo/.
	cp DataCards/GMSB_SqGl-Wino_SingleChannels/GMSB_1500_820_5000_375_bin*.txt  	DataCards/SqGl-Wino-Demo/.
	cp DataCards/GMSB_SqGl-Wino/GMSB_900_1320_5000_375.txt       			DataCards/SqGl-Wino-Demo/.
	cp DataCards/GMSB_SqGl-Wino_SingleChannels/GMSB_900_1320_5000_375_bin*.txt  	DataCards/SqGl-Wino-Demo/.

mkInputs: $(SRC:.cc=.o) 
	$(LD) $(SRC:.cc=.o) $(RLXX) -lMathMore  -o $@
	@echo '-> executable $@ created.'

