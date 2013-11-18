# CTA, RWTH 11.11.13
# provide the name of your class in the TARGETSRC variable

########################################################
# The rest of the makefile does not need to be modified

ROOTLIBS      = -L$(ROOTSYS)/lib -lHist -lTree -lGpad
ROOTGLIBS     = -lGpad -lGui -lGX11 -lX3d

EXECUTABLE=QcdAnalysis
TARGETSRC=Files/PhotonSelector.cc Files/MySelector.cc
TOOLSSRC=Tools/ConfigFile.cc
TOOLSO = $(TOOLSSRC:.cc=_cc.o)
TARGETCINT=$(TARGETSRC:.cc=Cint.cxx)
TARGETCINTO=$(TARGETSRC:.cc=Cint.o)
TARGETO = $(TARGETSRC:.cc=_cc.o)
TARGETSO = $(TARGETSRC:.cc=_cc.so)

# Linux/gcc-2.96 version
CWD 	      = $(shell pwd)
CXX           = c++
CXXFLAGS      = -O -Wall -fPIC -I$(ROOTSYS)/include
LD            = c++
LDFLAGS       = -g
SOFLAGS       = -Wl -shared
LIBS          = $(ROOTLIBS) -lm -ldl -rdynamic 
EXELIBS       = -L$(CWD)/Files -lPhotonSelector_cc -lMySelector_cc -lProof -lProofPlayer

all: $(TARGETSO) $(EXECUTABLE)

%Cint.cxx:
	rootcint -f $@ -c -I$(ROOTSYS)/include $(@:Cint.cxx=.h)

%Cint.o: $(TARGETCINT)
	$(CXX) $(CXXFLAGS) -o $@ -c $(@:.o=.cxx)

%_cc.o: %.h %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $(@:_cc.o=.cc)

%_cc.so: %_cc.o %Cint.o $(TOOLSO)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(@:.so=.o) $(@:_cc.so=Cint.o) $(TOOLSO) $(LIBS) -o $@

$(EXECUTABLE): $(EXECUTABLE).cc $(TARGETSO) 
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $(EXELIBS) $(EXECUTABLE).cc -o $(EXECUTABLE)

clean:
	@rm -f $(EXECUTABLE) $(TARGETO) $(TARGETCINTO) $(TARGETCINT) $(TARGETSRC:.cc=Cint.h) $(TARGETSO) $(TARGETSO:.so=.d)

