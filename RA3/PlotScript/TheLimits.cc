#include "TheLimits.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <fstream>
#include <iostream>
#include <cmath>

void TheLimits::Fill(int argc, char** argv)
{
   for (int i = 1; i<argc; ++i)
   {
     add( new SusyScan(argv[i]) );
   }
}

void TheLimits::Fill(const std::string filelist)
{
   std::ifstream masses_file;
   masses_file.open(filelist.c_str());
   std::string file;
   while (1) {
      GeneratorMasses * p = new GeneratorMasses;
      masses_file >> file;
      if (!masses_file.good()) break;
      add( new SusyScan(file));
   }
   std::cout << filelist << ": "<< _scan.size() <<std::endl;
   //check
   masses_file.close();
}

void TheLimits::match()
{
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    bool match = false;


    //if (!match) std::cout << "No match for M0="<<(*it)->Mzero
    //                      << ", M12="<<(*it)->Mhalf<<std::endl;
  }
}

void TheLimits::FillGeneratorMasses(std::string file)
{
   std::ifstream masses_file;
   masses_file.open(file.c_str());
   while (1) {
      GeneratorMasses * p = new GeneratorMasses;
      masses_file >> p->Mzero
                  >> p->Mhalf
                  >> p->TanBeta
                  >> p->Mu  
                  >> p->Azero
                  >> p->Mtop  
                  >> p->muQ	  
                  >> p->Q	  
                  >> p->M1	  
                  >> p->M2
                  >> p->M3	  
                  >> p->MGL	  
                  >> p->MUL	  
                  >> p->MB1	  
                  >> p->MSN	  
                  >> p->MNTAU	  
                  >> p->MZ1	  
                  >> p->MW1	  
                  >> p->MHL	  
                  >> p->MUR	  
                  >> p->MB2	  
                  >> p->MEL	  
                  >> p->MTAU1	  
                  >> p->MZ2	  
                  >> p->MW2	  
                  >> p->MHH	  
                  >> p->MDL	  
                  >> p->MT1	  
                  >> p->MER	  
                  >> p->MTAU2	  
                  >> p->MZ3	  
                  >> p->MHA	  
                  >> p->MDR	  
                  >> p->MT2	  
                  >> p->MZ4	  
                  >> p->MHp;

      if (!masses_file.good()) break;
      //std::cout << p->Mzero<<", "<<p->Mhalf<<", "<<p->TanBeta<<std::endl;
      if (fabs(p->Mu)!=1.) {
         std::cerr << "check lines near m0=" << p->Mzero << ", m1/2=" << p->Mhalf << std::endl;
         break;
      }	
      _masses.push_back( p );
   }
}


void TheLimits::OverwriteLimits(std::string flag)
{
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    
  
  } 


}


//void TheLimits::ExpandGrid<SusyScan>(double(*x)(const SusyScan*), double(*y)(const SusyScan*) );

