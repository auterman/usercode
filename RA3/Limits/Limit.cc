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
    std::cerr<<"Error: Expected at least one input config file '"<<argv[0]<<" <limit config files(s)>'!"<<std::endl;
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

    double limit = roostats_cl95(ilum, slum, eff, seff, bck, sbck, n );
    LimitResult expected_limit = roostats_clm(ilum, slum, eff, seff, bck, sbck, ntoys);

    config.add("limit.observed", limit);
    config.add("limit.expected", expected_limit.GetExpectedLimit() );
    config.add("limit.expected.m1sigma", expected_limit.GetOneSigmaLowRange() );
    config.add("limit.expected.p1sigma", expected_limit.GetOneSigmaHighRange() );
    config.add("limit.expected.1sigmaCoverage", expected_limit.GetOneSigmaCoverage() );
    config.add("limit.expected.m2sigma", expected_limit.GetTwoSigmaLowRange() );
    config.add("limit.expected.p2sigma", expected_limit.GetTwoSigmaHighRange() );
    config.add("limit.expected.2sigmaCoverage", expected_limit.GetTwoSigmaCoverage() );

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
