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
    double signal  = config.read<double>("signal.LO");
    double sigUncert = config.read<double>("signal.LO.uncertainty");
    double sigSignal = config.read<double>("signal.LO.signalregion.IsoMuon");
    double sigSigTau  = config.read<double>("signal.LO.signalregion.Tau");
    double signalNLO  = config.read<double>("signal.NLO");
    double sigUncertNLO = config.read<double>("signal.NLO.uncertainty");
    double sigSignalNLO = config.read<double>("signal.NLO.signalregion.IsoMuon");
    double sigSigTauNLO = config.read<double>("signal.NLO.signalregion.Tau");
    double bkgControl = config.read<double>("background.controlregion.IsoMuon");
    double bkgSignal  = config.read<double>("background.signalregion.IsoMuon");
  
    double xsec = config.read<double>("Xsection");
    double kfactor = config.read<double>("signal.kFactor");
    
    //bkgUncert += sigSignal * sigUncert/signal;
    
    //config.add("data", ndata);
    //config.add("background", nback);
    //config.add("background.uncertainty", bkgUncert);

    char * n = new char[32];
    sprintf(n,"bkgd%d",file); TH1 * bgd  = new TH1F(n, "",1,0,1);
    sprintf(n,"bkgd_NoSig%d",file); TH1 * bgd_NoSig  = new TH1F(n, "",1,0,1);
    sprintf(n,"data%d",file); TH1 * data = new TH1F(n, "",1,0,1);
    sprintf(n,"sig%d", file); TH1 * sig  = new TH1F(n,"",1,0,1);
    data->SetBinContent(1,ndata);
    data->SetBinError(1,sqrt(ndata));
    bgd_NoSig->SetBinContent(1,nback);
    bgd_NoSig->SetBinError(1,bkgUncert);
    double bLO=(nback - sigSignal -sigSigTau<0?0:nback - sigSignal -sigSigTau);
    bgd->SetBinContent(1,bLO);
    bgd->SetBinError(1,bkgUncert);
    sig->SetBinContent(1,signal);
    sig->SetBinError(1,sigUncert);

    cls limit_sys("cls_limit.ps",
    	  sig,  // signal with total syst. and stat. errors
    	  bgd,  // with total syst. and stat. errors
    	  data,
	  bgd_NoSig); 
    limit_sys.SetStat(true); //Consider Bin-Errors (containing the full syst. uncertainties)
    limit_sys.SetNMC(50000); //number of pseudo-experiments for each CL calculation
    //limit_sys.Draw();

    sprintf(n,"bkgdNLO%d",file); TH1 * bgdNLO  = new TH1F(n, "",1,0,1);
    sprintf(n,"bkgdNoSigNLO%d",file); TH1 * bgdNoSigNLO  = new TH1F(n, "",1,0,1);
    sprintf(n,"dataNLO%d",file); TH1 * dataNLO = new TH1F(n, "",1,0,1);
    sprintf(n,"sigNLO%d", file); TH1 * sigNLO  = new TH1F(n,"",1,0,1);
    
    double bNLO=(nback - sigSignalNLO -sigSigTauNLO<0?0:nback - sigSignalNLO - sigSigTauNLO);

    dataNLO->SetBinContent(1,ndata);
    dataNLO->SetBinError(1,sqrt(ndata));
    bgdNLO->SetBinContent(1,bNLO);
    bgdNLO->SetBinError(1,bkgUncert);
    bgdNoSigNLO->SetBinContent(1,nback);
    bgdNoSigNLO->SetBinError(1,bkgUncert);
    sigNLO->SetBinContent(1,signalNLO);
    sigNLO->SetBinError(1,sigUncertNLO);

    cls limit_sys_NLO("cls_limit_NLO.ps",
    	  sigNLO,  // signal with total syst. and stat. errors
    	  bgdNLO,  // with total syst. and stat. errors
    	  dataNLO,
	  bgdNoSigNLO); 
    limit_sys_NLO.SetStat(true); //Consider Bin-Errors (containing the full syst. uncertainties)
    limit_sys_NLO.SetNMC(50000); //number of pseudo-experiments for each CL calculation
    //limit_sys.Draw();

    try {
      //limit_sys.WriteResult( &config, xsec );
      limit_sys.WriteConfidence( &config, "LO." );
      limit_sys_NLO.WriteConfidence( &config, "NLO." );
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

    std::cout<<"d:"<<ndata<<", b:"<<bNLO<<", s:"<<signalNLO<<std::endl;

  }
}


int main(int argc, char *argv[])
{
  Limit(argc,argv);
}
