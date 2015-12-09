C=c++
LD=c++
F77=g77
#O2 for optimization, g for debugging
SPECIALFLAGS=-g # -O2 -g
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs) 
#-lMinuit

CFLAGS = $(SPECIALFLAGS) 
#-I. -I./include -I$(SRT_PUBLIC_CONTEXT)/include -I$(ROOTSYS)/include -Wno-deprecated
LFLAGS = $(SPECIALFLAGS) 
#-lz 


RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

##Common source files
SRC=table.cc Overview.cc Variable.cc OldExclusionContours.cc ConfigFile.cc GeneratorMasses.cc PlotTools.cc Plotting.cc

%.o: %.cc
		$(C) $(RCXX) -c $<

all: plotSinglePhoton8TeV20fb plotEWKphotonParked
#plotSinglePhoton7TeV plotSinglePhoton8TeV plot_Combination



# single photon 8 TeV, 20 fb-1 specific ----------------------------------------------------------------------------------------
plotEWKphotonParked: $(SRC:.cc=.o) StyleSettings_EWKphotonParked.o Event_EWKphotonParked.o plot_EWKphotonParked.o 
		$(LD) $(SRC:.cc=.o) Event_EWKphotonParked.o StyleSettings_EWKphotonParked.o plot_EWKphotonParked.o $(RLXX) $(JCORR) -o plotEWKphotonParked
		@echo '-> plotEWKphotonParked executable created.'


# single photon 7 TeV specific ----------------------------------------------------------------------------------------
plotSinglePhoton7TeV: $(SRC:.cc=.o) StyleSettings_SinglePhoton_7TeV.o plotSinglePhoton7TeV.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_SinglePhoton_7TeV.o plotSinglePhoton7TeV.o $(RLXX) $(JCORR) -o plotSinglePhoton7TeV
		@echo '-> plotSinglePhoton7TeV executable created.'

# single photon 8 TeV specific ----------------------------------------------------------------------------------------
plotSinglePhoton8TeV: $(SRC:.cc=.o) StyleSettings_SinglePhoton_8TeV.o plotSinglePhoton8TeV.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_SinglePhoton_8TeV.o plotSinglePhoton8TeV.o $(RLXX) $(JCORR) -o plotSinglePhoton8TeV
		@echo '-> plotSinglePhoton8TeV executable created.'

# single photon 8 TeV, 20 fb-1 specific ----------------------------------------------------------------------------------------
plotSinglePhoton8TeV20fb: $(SRC:.cc=.o) StyleSettings_SinglePhoton_8TeV20fb.o Event_SinglePhoton8TeV20fb.o plotSinglePhoton8TeV20fb.o 
		$(LD) $(SRC:.cc=.o) Event_SinglePhoton8TeV20fb.o StyleSettings_SinglePhoton_8TeV20fb.o plotSinglePhoton8TeV20fb.o $(RLXX) $(JCORR) -o plotSinglePhoton8TeV20fb
		@echo '-> plotSinglePhoton8TeV20fb executable created.'

# Combination specific ----------------------------------------------------------------------------------------
plot_Combination: $(SRC:.cc=.o) Event.o StyleSettings_Combination.o  plot_Combination.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_Combination.o plot_Combination.o $(RLXX) $(JCORR) -o plot_Combination
		@echo '-> plot_Combination executable created.'

# di photon specific           ----------------------------------------------------------------------------------------
plotDiPhoton: $(SRC:.cc=.o) EventDiPhoton.o StyleSettings_DiPhoton.o Plotting.o plotDiPhoton.o 
		$(LD) $(SRC:.cc=.o) StyleSettings_DiPhoton.o EventDiPhoton.o plotDiPhoton.o Plotting.o $(RLXX) $(JCORR) -o plotDiPhoton
		@echo '-> plotDiPhoton executable created.'

nedit:
		sublime -n plotSinglePhoton8TeV20fb.cc StyleSettings_SinglePhoton_8TeV20fb.cc Event_SinglePhoton8TeV20fb.cc Plotting.h Plotting.cc PlotTools.h PlotTools.cc Makefile &

neditEWK:
		sublime -n plot_EWKphotonParked.cc StyleSettings_EWKphotonParked.cc Event_EWKphotonParked.cc Plotting.h Plotting.cc PlotTools.h PlotTools.cc Makefile &

commit:
		git add Makefile *.h *.cc
		git commit -m "auto-commit by makefile"
		git push

fetch:
		scp nafhh-cms01.desy.de:Releases/CMSSW_6_1_1/src/Jobs/2014*ino.tar.gz .; ls 2014*ino.tar.gz
		

clean:
		@rm -f *.o 
		@rm -f plotSinglePhoton7TeV plotSinglePhoton8TeV
