//User
#include "ConfigFile.h"
#include "roostats_cl95.C"

//system
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <stdlib.h>


//ROOT
#include "TH1.h"
#include "TCanvas.h"
#include "TRint.h"
#include "RooRealVar.h"
#include "RooUniform.h"
#include "RooDataSet.h"
#include "RooConstVar.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "RooPoisson.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/HybridCalculatorOriginal.h"
#include "RooStats/HypoTestInverterResult.h"
#include "RooStats/HypoTestInverter.h"

using namespace std;
using namespace RooStats;


void Limit(int argc, char *argv[])
{
  if (argc<2){
    std::cerr<<"Error: Expected at least one input config file '"<<argv[1]<<" <limit config files(s)>'!"<<std::endl;
    exit(1);
  }

  for (int file=1; file<argc; ++file){
    
    //read a susy signal scan point as specified by the argument(s)
    std::cout << "opening: "<<argv[file]<<std::endl;
    ConfigFile config(argv[file]);
    int    n       = config.read<int>("data");
    double bck     = config.read<double>("background");
    double sbck    = config.read<double>("background.uncertainty");
    double eff     = config.read<double>("signal.acceptance");
    double seff    = config.read<double>("signal.acceptance.uncertainty");
    double sig_cont= config.read<double>("signal.contamination");
    double ilum    = config.read<double>("Luminosity");
    double slum    = config.read<double>("Luminosity.uncertainty");
    int ntoys = 10000;
    
    int nuisanceModel = 0;

    double bckSignalContNotSubtracted=bck;
    double bckSignalContSubtracted=bck-(sig_cont*bck);
    if(bckSignalContSubtracted<0)bckSignalContSubtracted=0;

    std::cout<<"Background:"<<bck<<std::endl;
    std::cout<<"Background no Signal Cont Subtraction:"<<bckSignalContNotSubtracted<<std::endl;
    std::cout<<"Background  Signal Cont Subtraction:"<<bckSignalContSubtracted<<std::endl;

    std::cout<<"======================================================================Limit with subtraction of Signal Contamination"<<bck<<std::endl;
        double limitSigCont = roostats_cl95(ilum, slum, eff, seff, bckSignalContSubtracted, sbck, n, false, nuisanceModel,bckSignalContNotSubtracted );
        LimitResult expected_limitSigCont = roostats_clm(ilum, slum, eff, seff, bckSignalContSubtracted, sbck, ntoys, nuisanceModel,bckSignalContNotSubtracted);


    std::cout<<"======================================================================Limit w/o subtraction of Signal Contamination"<<bck<<std::endl;
    double limit = roostats_cl95(ilum, slum, eff, seff, bck, sbck, n, false, nuisanceModel,-1 );
    LimitResult expected_limit = roostats_clm(ilum, slum, eff, seff, bck, sbck, ntoys, nuisanceModel,-1);


    config.add("limit.observed", limit);
    config.add("limit.expected", expected_limit.GetExpectedLimit() );
    config.add("limit.expected.m1sigma", expected_limit.GetOneSigmaLowRange() );
    config.add("limit.expected.p1sigma", expected_limit.GetOneSigmaHighRange() );
    config.add("limit.expected.1sigmaCoverage", expected_limit.GetOneSigmaCoverage() );
    config.add("limit.expected.m2sigma", expected_limit.GetTwoSigmaLowRange() );
    config.add("limit.expected.p2sigma", expected_limit.GetTwoSigmaHighRange() );
    config.add("limit.expected.2sigmaCoverage", expected_limit.GetTwoSigmaCoverage() );

    	config.add("limitSigCont.observed", limitSigCont);
        config.add("limitSigCont.expected", expected_limitSigCont.GetExpectedLimit() );
        config.add("limitSigCont.expected.m1sigma", expected_limitSigCont.GetOneSigmaLowRange() );
        config.add("limitSigCont.expected.p1sigma", expected_limitSigCont.GetOneSigmaHighRange() );
        config.add("limitSigCont.expected.1sigmaCoverage", expected_limitSigCont.GetOneSigmaCoverage() );
        config.add("limitSigCont.expected.m2sigma", expected_limitSigCont.GetTwoSigmaLowRange() );
        config.add("limitSigCont.expected.p2sigma", expected_limitSigCont.GetTwoSigmaHighRange() );
        config.add("limitSigCont.expected.2sigmaCoverage", expected_limitSigCont.GetTwoSigmaCoverage() );

    //write stuff:  
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    ofstream ofile;
    ofile.open (argv[file]);
    ofile << config.getComment() << getenv("USER") 
          << ": " << asctime (timeinfo) 
	  << config.getComment() << "\n"
	  << config;
    ofile.close();
  }
}


int main(int argc, char *argv[])
{
  try{ 
      Limit(argc,argv);
  }
  catch(exception& e){
      cout << "Exception catched: " << e.what();
  }
}
