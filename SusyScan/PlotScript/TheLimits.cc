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
   std::cout << "Read " << _scan.size() << " points from argument list." <<std::endl;
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
   std::cout << "Read " << _scan.size() << " points from file " << filelist <<std::endl;
   masses_file.close();
}

void TheLimits::match()
{
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    bool match = false;
    for (std::vector<GeneratorMasses*>::const_iterator gt=_masses.begin();gt!=_masses.end();++gt){
      if ((*it)->Mzero==(*gt)->Mzero && 
          (*it)->Mhalf==(*gt)->Mhalf && 
	  (*it)->Azero==(*gt)->Azero &&
	  (*it)->TanBeta==(*gt)->TanBeta &&
	  (*it)->Mu==(*gt)->Mu ) {
        (*it)->M1 = (*gt)->M1;
        (*it)->M2 = (*gt)->M2;
        (*it)->M3 = (*gt)->M3;
        (*it)->MGL = (*gt)->MGL;
        (*it)->MUL = (*gt)->MUL;
        (*it)->MB1 = (*gt)->MB1;
        (*it)->MSN = (*gt)->MSN;
        (*it)->MNTAU = (*gt)->MNTAU;
        (*it)->MZ1 = (*gt)->MZ1;
        (*it)->MW1 = (*gt)->MW1;
        (*it)->MHL = (*gt)->MHL;
        (*it)->MUR = (*gt)->MUR;
        (*it)->MB2 = (*gt)->MB2;
        (*it)->MEL = (*gt)->MEL;
        (*it)->MTAU1 = (*gt)->MTAU1;
        (*it)->MZ2 = (*gt)->MZ2;
        (*it)->MW2 = (*gt)->MW2;
        (*it)->MHH = (*gt)->MHH;
        (*it)->MDL = (*gt)->MDL;
        (*it)->MT1 = (*gt)->MT1;
        (*it)->MER = (*gt)->MER;
        (*it)->MTAU2 = (*gt)->MTAU2;
        (*it)->MZ3 = (*gt)->MZ3;
        (*it)->MHA = (*gt)->MHA;
        (*it)->MDR = (*gt)->MDR;
        (*it)->MT2 = (*gt)->MT2;
        (*it)->MZ4 = (*gt)->MZ4;
        (*it)->MHp = (*gt)->MHp;
	match = true;
	//std::cout <<"m0="<<(*it)->Mzero<<", m1/2="<<(*it)->Mhalf<< ", sq="<<(*it)->MUL << ", gl="<<(*it)->MGL <<std::endl; 
      }	  
    }
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
      //std::cout <<"m0="<< p->Mzero<<", m1/2="<< p->Mhalf<< ", sq="<<p->MUL << ", gl="<<p->MGL <<std::endl; 
      _masses.push_back( p );
   }
   std::cout << "Read " << _masses.size() << " points from file " << file <<std::endl;

}


void TheLimits::OverwriteLimits(std::string flag)
{
  std::cout<<"WARNING: OVERWRITING LIMITS!!!!"<<std::endl;
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    if (flag=="ABCD_MHT") {
      (*it)->ExpNsigLimit = 17.85;
      (*it)->PLExpNsigLimit = 12.558;
      (*it)->FCExpNsigLimit = 15.75;
      (*it)->MCMCExpNsigLimit = 15.8085;
      (*it)->ObsNsigLimit = 10.0637;
      (*it)->PLObsNsigLimit = 4.22124;
      (*it)->FCObsNsigLimit  = 0.75;
      (*it)->MCMCObsNsigLimit  = 8.75403;
    }
    else if (flag=="ABCD_HT") {
      (*it)->ExpNsigLimit = 24.0438;
      (*it)->PLExpNsigLimit = 17.8537;
      (*it)->FCExpNsigLimit = 22.05;
      (*it)->MCMCExpNsigLimit = 20.781;
      (*it)->ObsNsigLimit    = 18.24;
      (*it)->PLObsNsigLimit = 9.70375;
      (*it)->FCObsNsigLimit  = 11.85;
      (*it)->MCMCObsNsigLimit  = 14.34;
    }
    
    
      (*it)->ExpXsecLimit    = (*it)->ExpNsigLimit * (*it)->Xsection / (*it)->signal;
      (*it)->PLExpXsecLimit  = (*it)->PLExpNsigLimit  * (*it)->Xsection / (*it)->signal;
      (*it)->FCExpXsecLimit  = (*it)->FCExpNsigLimit  * (*it)->Xsection / (*it)->signal;
      (*it)->MCMCExpXsecLimit= (*it)->MCMCExpNsigLimit  * (*it)->Xsection / (*it)->signal;
      (*it)->ObsXsecLimit    = (*it)->ObsNsigLimit  * (*it)->Xsection / (*it)->signal;
      (*it)->PLObsXsecLimit  = (*it)->PLObsNsigLimit  * (*it)->Xsection / (*it)->signal;
      (*it)->FCObsXsecLimit  = (*it)->FCObsNsigLimit   * (*it)->Xsection / (*it)->signal;
      (*it)->MCMCObsXsecLimit= (*it)->MCMCObsNsigLimit  * (*it)->Xsection / (*it)->signal;
    
  
  } 


}

void TheLimits::ExpandGrid(int s)
{
  std::vector<SusyScan*> new_grid;
  for (std::vector<SusyScan*>::iterator it=_scan.begin(); it!=_scan.end(); ++it){
    double x=9999, y=9999;
    std::vector<SusyScan*>::iterator next_x=_scan.end(), next_y=_scan.end();
    for (std::vector<SusyScan*>::iterator nx=_scan.begin(); nx!=_scan.end(); ++nx)
      if ((*it)->Mzero<(*nx)->Mzero && fabs((*it)->Mzero-(*nx)->Mzero)<x) {
        x=fabs((*it)->Mzero-(*nx)->Mzero);
	next_x=nx;
      }
    for (std::vector<SusyScan*>::iterator ny=_scan.begin(); ny!=_scan.end(); ++ny)
      if ( (*it)->Mzero==(*ny)->Mzero && (*it)->Mhalf<(*ny)->Mhalf && fabs((*it)->Mhalf-(*ny)->Mhalf)<y) {
        y=fabs((*it)->Mhalf-(*ny)->Mhalf);
	next_y=ny;
      }
    if (next_y!=_scan.end()) {  
	SusyScan*  n = new SusyScan((**it + **next_y) * 0.5);
	new_grid.push_back( n );
	//std::cout 
	//<< "l:"<<(*it)->Mzero<<","<<(*it)->Mhalf
	//<< " <>"<<    n->Mzero<<","<<n->Mhalf
	//<< " <> r:"<<(*next_y)->Mzero<<","<<(*next_y)->Mhalf
	//<<std::endl;
    }  
      
  } 
  _scan.insert(_scan.end(), new_grid.begin(), new_grid.end());
}
