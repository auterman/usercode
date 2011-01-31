//User
#include "cls.h"
#include "ConfigFile.h"
#include "table.h"

//system
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>

//ROOT
#include "TH1.h"

using namespace std;


void Limit(int argc, char *argv[])
{
  if (argc<1){ 
    std::cerr<<"Error: Expected '"<<argv[0]<<" <limit config>'!"<<std::endl;
    exit(1);
  }
  
  for (int file=1; file<argc; ++file){   
    std::cout << "opening: "<<argv[file]<<std::endl;
    ConfigFile config(argv[file]);

    int    ndata = config.read<int>("data");
    double nback  = config.read<double>("background");
    double bkgUncert = config.read<double>("background.uncertainty");
    double signal  = config.read<double>("signal");
    double sigUncert = config.read<double>("signal.uncertainty");

    double sigControl = config.read<double>("signal.controlregion.IsoMuon");
    double sigSignal  = config.read<double>("signal.signalregion.IsoMuon");
    double bkgControl = config.read<double>("background.controlregion.IsoMuon");
    double bkgSignal  = config.read<double>("background.signalregion.IsoMuon");
    
    nback -= sigSignal;
    //bkgUncert += sigSignal * sigUncert/signal;
    
    //config.add("data", ndata);
    //config.add("background", nback);
    //config.add("background.uncertainty", bkgUncert);

    char * n = new char[32];
    sprintf(n,"bkgd%d",file); TH1 * bgd  = new TH1F(n, "",1,0,1);
    sprintf(n,"data%d",file); TH1 * data = new TH1F(n, "",1,0,1);
    sprintf(n,"sig%d", file); TH1 * sig  = new TH1F(n,"",1,0,1);
    data->SetBinContent(1,ndata);
    data->SetBinError(1,sqrt(ndata));
    bgd->SetBinContent(1,nback);
    bgd->SetBinError(1,bkgUncert);
    sig->SetBinContent(1,signal);
    sig->SetBinError(1,sigUncert);

    cls limit_sys("cls_limit.ps",
    	  sig,  // signal with total syst. and stat. errors
    	  bgd,  // with total syst. and stat. errors
    	  data); 
    limit_sys.SetStat(true); //Consider Bin-Errors (containing the full syst. uncertainties)
    limit_sys.SetNMC(50000); //number of pseudo-experiments for each CL calculation
    //limit_sys.Draw();

    try {
      //limit_sys.WriteResult( &config );
      limit_sys.WriteConfidence( &config );
    }
    catch(exception& e){
      cout << "Exception catched: " << e.what();
    }

    //write stuff:  
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    ofstream ofile;
    ofile.open (argv[file]);
    if (ofile.good())
      std::cout << "writing '"<<argv[file]<<"'"<<std::endl;
    else if ( (ofile.rdstate() & ifstream::failbit ) != 0 )
      std::cerr << "ERROR opening '"<<argv[file]<<"'! Does the directory exist?"<<std::endl;
    
    ofile << config.getComment() << asctime (timeinfo) 
	  << config.getComment()<< "\n"
	  << config;
  }
}


int main(int argc, char *argv[])
{
  Limit(argc,argv);
}
