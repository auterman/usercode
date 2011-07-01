//system
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>

const double data = 32;
const double background = 29.6;
const double backgroundUncertainty = 7.3;
const double luminosity = 715.0;
const double luminosityUncertainty = 0.05 * luminosity;   //5%
const double scaleUncertainty = 0.2;                      //suggested by Yuri, see mail June 30th.

struct point {
  double gluino;
  double squark;
  double chi;
  double xsec;
  double lumi;
  double u_lumi;
  double kfactor;
  double u_NLO;
  double acc;
  double u_acc;
  double u_pdf;
  double u_stat;
  double signal_contamination;
  double signal_controlregion;
  double back;
  double u_back;
  double data; 
  double ngen; 
};

class points{
 public:
  point* Get(double gl, double sq, double chi){
    for (std::vector<point>::iterator it=p_.begin();it!=p_.end();++it)
      if (it->gluino==gl&&it->squark==sq&&it->chi==chi)
        return &(*it);
     return 0;	
  }
  void Add(point p){p_.push_back(p);}
  void Write(const std::string dir){
    for (std::vector<point>::iterator it=p_.begin();it!=p_.end();++it){
      std::ofstream ofile;
      std::stringstream ss;
      ss << dir<<"_"<<it->gluino<<"_"<<it->squark<<"_"<<it->chi<<".txt";
      ofile.open (ss.str().c_str());
      ofile << "gluino = "<<it->gluino<<"\n";
      ofile << "squark = "<<it-> squark<<"\n";
      ofile << "chi1 = "<<it-> chi<<"\n";
      ofile << "Xsection = "<<it-> xsec<<"\n";
      ofile << "Luminosity = "<<it-> lumi<<"\n";
      ofile << "Luminosity.uncertainty = "<<it-> u_lumi<<"\n";
      ofile << "kfactor = "<<it-> kfactor<<"\n";
      ofile << "signal.scale.uncertainty.relativ = "<<it-> u_NLO<<"\n";
      ofile << "signal.acceptance = "<<it-> acc<<"\n";
      ofile << "signal.Stat.uncertainty = "<<it->u_stat<<"\n";
      ofile << "signal.acceptance.uncertainty = "<<it-> u_acc<<"\n";
      ofile << "signal.PDF.uncertainty.realativ = "<<it-> u_pdf<<"\n";
      ofile << "background = "<<it-> back<<"\n";
      ofile << "background.uncertainty = "<<it-> u_back<<"\n";
      ofile << "data = "<<it-> data<<"\n";  
      ofile << "signal.ngen = "<<it->ngen<<"\n";  
      ofile << "signal.contamination = "<<it->signal_contamination<<"\n";  
      ofile << "signal.controlregion = "<<it->signal_controlregion<<"\n";  
      ofile.close();
    }
  } 
 private:
  std::vector<point> p_; 
} Points;

void ReadXsec(const std::string filelist)
{
   std::ifstream masses_file;
   masses_file.open(filelist.c_str());
   std::string file;
   point p;
   p.data= data;
   p.back = background;
   p.u_back = backgroundUncertainty;
   p.lumi = luminosity;
   p.u_lumi = luminosityUncertainty;
   p.u_NLO = scaleUncertainty;
   double nAcc;
   while(1){
     masses_file >> p.gluino >> p.squark >> p.chi >> p.ngen >> nAcc >> p.xsec;
     if(!masses_file.good()) break;
     Points.Add( p );
   }  
}

void ReadPDF(const std::string filelist)
{
   std::ifstream masses_file;
   masses_file.open(filelist.c_str());
   std::string file;
   point p;
   while (1) {
      masses_file >> p.gluino >> p.squark >> p.u_pdf;
      if (!masses_file.good()) break;
      point * a = 0;
      a = Points.Get( p.gluino, p.squark,  50);  if (a) a->u_pdf = 0.01*p.u_pdf;
      a = Points.Get( p.gluino, p.squark, 150);  if (a) a->u_pdf = 0.01*p.u_pdf;
      a = Points.Get( p.gluino, p.squark, 500);  if (a) a->u_pdf = 0.01*p.u_pdf;
   }
   masses_file.close();
}

void ReadkFactor(const std::string filelist)
{
   std::ifstream masses_file;
   masses_file.open(filelist.c_str());
   std::string file;
   point p;
   while (1) {
      masses_file >> p.gluino >> p.squark >> p.kfactor;
      if (!masses_file.good()) break;
      point * a = 0;
      a = Points.Get( p.gluino, p.squark, 50);  if (a) a->kfactor = p.kfactor;
      a = Points.Get( p.gluino, p.squark, 150); if (a) a->kfactor = p.kfactor;
      a = Points.Get( p.gluino, p.squark, 500); if (a) a->kfactor = p.kfactor;
   }
   masses_file.close();
}

int main(void)
{
  ReadXsec("MC_Map_Dec5.dat");
  ReadPDF("xsectionPDFErrors.dat");
  ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
  //@@TODO: Read SignalAcceptance & Calculate total signal uncertainty
  Points.Write("limits/GMSB");
}
