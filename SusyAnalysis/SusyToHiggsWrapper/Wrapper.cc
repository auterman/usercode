
#include "Chameleon.h"
#include "table.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>//.h
#include <stdlib.h>//.h


void Wrap(ConfigFile * bkg, ConfigFile * sig) {
  using namespace std;
  using namespace Table;

  int Mzero   = sig->Value("mSUGRA","Mzero");
  int Mhalf   = sig->Value("mSUGRA","Mhalf");
  int TanBeta = sig->Value("mSUGRA","TanBeta");
  int Azero   = sig->Value("mSUGRA","Azero");
  int Mu      = sig->Value("mSUGRA","Mu");
  double XsecLO = sig->Value("mSUGRA","Xsection");
  string outFileName;
  {
    stringstream ss;
    ss << "mSUGRA_"<<Mzero<<"_"<<Mhalf<<"_"<<TanBeta<<"_"<<Azero<<"_"<<Mu<<".txt";
    outFileName=ss.str();   
  }
  ofstream out;
  out.open (outFileName.c_str());
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  out << "# " << getenv("USER") 
      << ": " << asctime (timeinfo) 
      << "#\n"
      << "# Mzero = "<<Mzero << "\n"
      << "# Mhalf = "<<Mhalf << "\n"
      << "# TanBeta = "<<TanBeta << "\n"
      << "# Azero = "<<Azero << "\n"
      << "# Mu = "<<Mu << "\n"
      << "# XsectionLO = "<<XsecLO << "\n"
      << endl;
 
  //higgs mass - use for tracking the mSUGRA params:
  out << "higgs mass " << TanBeta 
      << setfill ('0') << setw (4) << Mzero  << setfill ('0') << setw (4) << Mhalf
      << setfill (' ') << endl;

  string signalName = "signal";

  int n_channels    = bkg->Value("definitions","n_channels",1);
  int n_nuisance    = bkg->Value("definitions","n_nuisance");
  int n_backgrounds = bkg->Value("definitions","n_backgrounds");
  
  if ( n_channels != sig->Value("definitions","n_channels") ||
       n_nuisance != sig->Value("definitions","n_nuisance") ) {
    std::cerr<<"ERROR: Consistency check failed! "
             <<"n_channels and n_nuisance in bkg-config does not match signal config."<<std::endl;
    exit(2);
  } 
  
  out << "imax " << std::setw(2) << n_channels    << "  number of channels" << std::endl;
  out << "jmax " << std::setw(2) << n_backgrounds << "  number of backgrounds" << std::endl;
  out << "kmax " << std::setw(2) << n_nuisance    << "  number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
  out << "------------" << std::endl;  

  //observed events in all channels
  TTable observed("# observed events");\
  observed.SetStyle(Empty);
  observed.SetDelimiter("  ");
  observed.AddColumn<string>(""); for (int bin=1; bin<=n_channels; ++bin) observed.AddColumn<int>("");
  observed << "bin";         for (int bin=1; bin<=n_channels; ++bin) observed << bin;
  observed << "observation"; 
  for (int bin=1; bin<=n_channels; ++bin) {
    stringstream ss;
    ss << "n" << bin;
    observed << (int)bkg->Value("observation",ss.str());
  }  
  out << observed << "------------\n" << std::endl;  

  
  //expected events in all channels for signal and all backgrounds
  TTable exp("# expected events");
  exp.SetStyle(Empty);
  exp.SetDelimiter("  ");
  exp.AddColumn<string>(""); 
  exp.SetMinumumWidth(20,0);//make first column at least 20 chars
    for (int bin=1; bin<=n_channels; ++bin) 
      for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        exp.AddColumn<string>("");
  exp << "bin"; 
    for (int bin=1; bin<=n_channels; ++bin)
      for (int sample=1; sample<=n_backgrounds+1; ++sample) {
         stringstream ss;
         ss << bin;
         exp << ss.str();
      }
  exp << "process";
  for (int bin=1; bin<=n_channels; ++bin) {
    exp << signalName;
    for (int sample=1; sample<=n_backgrounds; ++sample) {
      stringstream ss1;
      ss1 << "bkg_" << sample;
      stringstream ss2;
      ss2 << "background_yield_" << sample;
      stringstream ss3;
      ss3 << "b" << bin;
      exp << (string)bkg->Value(ss2.str(),"name",ss1.str());
    }  
  }  
  exp << "process"; 
  for (int bin=1; bin<=n_channels; ++bin) 
      for (int sample=1; sample<=n_backgrounds+1; ++sample) {
         stringstream ss;
         ss << (sample-1);
         exp << ss.str();
      }	
  exp << "rate"; 
  for (int bin=1; bin<=n_channels; ++bin) {
    stringstream ss1;
    ss1 << "s" << bin;
    exp << (string)sig->Value("signal_yield",ss1.str());
    for (int sample=1; sample<=n_backgrounds; ++sample) {
      stringstream ss2;
      ss2 << "background_yield_" << sample;
      stringstream ss3;
      ss3 << "b" << bin;
      exp << (string)bkg->Value(ss2.str().c_str(),ss3.str());
    }
  }  
  out << exp << "------------" << std::endl;  

  TTable sys("");
  sys.SetStyle(Empty);
  sys.SetDelimiter("  ");
  sys.AddColumn<string>(""); 
  sys.SetMinumumWidth(20,0);//make first column at least 20 chars
    for (int bin=1; bin<=n_channels; ++bin) 
      for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        sys.AddColumn<string>("");
  for (int n=1; n<=n_nuisance; ++n){
    stringstream ss1;
    ss1 << "nuisance_" << n;
    sys << (string)bkg->Value(ss1.str().c_str(),"name",ss1.str());
    for (int bin=1; bin<=n_channels; ++bin) {
      stringstream ss2;
      ss2 << "sigma_s_" << bin;
      double d = sig->Value(ss1.str().c_str(),ss2.str()) +1;
      stringstream ss4;
      ss4<<d;
      string s=(ss4.str()=="1"?"-":ss4.str());
      sys << s;
      for (int sample=1; sample<=n_backgrounds; ++sample) {
        stringstream ss3;
        ss3 << "sigma_b" << sample << "_" << bin;
	double d = bkg->Value(ss1.str().c_str(),ss3.str()) +1;
        stringstream ss4;
	ss4<<d;
	string s=(ss4.str()=="1"?"-":ss4.str());
        sys << s;
      }
    }  
  }
  out << sys << "------------" << std::endl;  
  
  
  cout << "...writing output file "<< outFileName <<endl;
  out.close();
   
}

int main(int argc, char* argv[]){

  if (argc<=1){ 
    std::cerr<<"ERROR: Expected configs!"<<std::endl;
    exit(1);
  } else if (argc==2){ 
    std::cerr<<"ERROR: Expected background AND >=1 signal config!"<<std::endl;
    exit(1);
  } 

  std::string bkgFileName=argv[1];
  std::cout << "opening background and data config file: "<<bkgFileName<<std::endl;
  ConfigFile bkgConfig( bkgFileName );

  for (int file=2; file<argc; ++file){ 
    std::string fileName=argv[file];
    std::cout << "opening scan signal "<<file-1<<" from "<<argc-2<<": "<<fileName<<std::endl;
    ConfigFile sigConfig( fileName );

    Wrap(&bkgConfig, &sigConfig);
  }

  return 0;
}  
