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
    double NLOXsection = config.read<double>("NLOXsection",1.0);
    double kfactor  = config.read<double>("kfactor",1.0);

    int ntoys = 2000;
    int nuisanceModel = 1; //0 = Gauss, 1=lognormal
    std::string SM = "cls"; // StatisticalModel = "bayesian", "cls", ...

    double effSC = eff - (sig_cont);
    		//Christian Autermann 5. August- ???:
    		//*bck) / (ilum * NLOXsection);
    if(effSC<0)effSC=0;
    double seffSC = seff * effSC / eff;

    std::cout<<"Signal acceptance:"<<eff<<" +- "<< seff<<std::endl;
    std::cout<<"Signal acc - cont:"<<effSC<<" +- "<<seffSC<<std::endl;
    std::cout<<"======================================================================Limit with subtraction of Signal Contamination"<<bck<<std::endl;

    //limits with signal contamination removed
    LimitResult limitSigCont=roostats_limit(ilum, slum, effSC, seffSC, bck, sbck, n, false,
    		                 nuisanceModel, SM,"plot_cl95_sigcont.pdf",12345);

    //LimitResult expected_limitSigCont = roostats_clm(ilum, slum, effSC, seffSC, bck, sbck, ntoys, nuisanceModel, SM);

    //limits (doing nothing about signal contamination)
    std::cout<<"======================================================================Limit w/o subtraction of Signal Contamination"<<bck<<std::endl;
    LimitResult limit= roostats_limit(ilum, slum, eff, seff, bck, sbck, n, false,
           nuisanceModel,SM,"plot_cl95.pdf",12345);



    //LimitResult expected_limit = roostats_clm(ilum, slum, eff, seff, bck, sbck, ntoys, nuisanceModel,SM);


    config.add("limit."+SM+".observed", limit.GetObservedLimit());
    config.add("limit."+SM+".observed.error", limit.GetObservedLimitError());
    config.add("limit."+SM+".expected", limit.GetExpectedLimit() );
    config.add("limit."+SM+".expected.m1sigma", limit.GetOneSigmaLowRange() );
    config.add("limit."+SM+".expected.p1sigma", limit.GetOneSigmaHighRange() );
    config.add("limit."+SM+".expected.1sigmaCoverage", limit.GetOneSigmaCoverage() );
    config.add("limit."+SM+".expected.m2sigma", limit.GetTwoSigmaLowRange() );
    config.add("limit."+SM+".expected.p2sigma", limit.GetTwoSigmaHighRange() );
    config.add("limit."+SM+".expected.2sigmaCoverage", limit.GetTwoSigmaCoverage() );

    config.add("limitSC."+SM+".observed", limitSigCont.GetObservedLimit());
    config.add("limitSC."+SM+".observed.error", limitSigCont.GetObservedLimitError());
    config.add("limitSC."+SM+".expected", limitSigCont.GetExpectedLimit() );
    config.add("limitSC."+SM+".expected.m1sigma", limitSigCont.GetOneSigmaLowRange() );
    config.add("limitSC."+SM+".expected.p1sigma", limitSigCont.GetOneSigmaHighRange() );
    config.add("limitSC."+SM+".expected.1sigmaCoverage", limitSigCont.GetOneSigmaCoverage() );
    config.add("limitSC."+SM+".expected.m2sigma", limitSigCont.GetTwoSigmaLowRange() );
    config.add("limitSC."+SM+".expected.p2sigma", limitSigCont.GetTwoSigmaHighRange() );
    config.add("limitSC."+SM+".expected.2sigmaCoverage", limitSigCont.GetTwoSigmaCoverage() );

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
