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
      //if(SusyScan::checkPointOk(file))
      add( new SusyScan(file));
      if (!masses_file.good()) break;
   }
   std::cout << filelist << ": "<< _scan.size() <<std::endl;
   //check
   masses_file.close();
}

void TheLimits::FillBestExpectedLimit(const std::string filelist,const std::string filelist2)
{

   std::ifstream masses_file,masses_file2;
  // std::cout<<"read file 1"<<filelist<<std::endl;
   masses_file.open(filelist.c_str());
  // std::cout<<"read file 2"<<filelist2<<std::endl;
   masses_file2.open(filelist2.c_str());
  // std::cout<<"file 1 good"<<masses_file.good()<<std::endl;
  // std::cout<<"file 2 good"<<masses_file2.good()<<std::endl;
   std::string file,file2;
   while (1) {

      masses_file >> file;
      masses_file2 >> file2;
      //  std::cout<<"add susy scan"<<std::endl;
            add( new SusyScan(file,file2));
      if (!masses_file.good()) {
    //	  std::cout<<"--file 1 good"<<masses_file.good()<<std::endl;
    	  break;}
      if (!masses_file2.good()) {
    //	  std::cout<<"--file 2 good"<<masses_file2.good()<<std::endl;
    	  break;}

    //  std::cout<<"==done"<<std::endl;
   }
   std::cout << filelist << ": "<< filelist2 << ": "<< _scan.size() <<std::endl;
   //check
   masses_file.close();
   masses_file2.close();
  // std::cout<<"fill best expected limit - done"<<std::endl;
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

void TheLimits::FillEmptyPointsByInterpolationInMsqMgl()
{
  //first find out where to expect points
  std::cout<< "start: TheLimits::FillEmptyPointsByInterpolationInMsqMgl()" <<std::endl;
  double gridy=9999, miny=9999, maxy=0, gridx=9999, minx=9999, maxx=0;
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin(); it!=_scan.end(); ++it){
    if ((*it)->Msquark<minx) minx=(*it)->Msquark;
    if ((*it)->Msquark>maxx) maxx=(*it)->Msquark;
    if ((*it)->Mgluino<miny) miny=(*it)->Mgluino;
    if ((*it)->Mgluino>maxy) maxy=(*it)->Mgluino;
    for (std::vector<SusyScan*>::const_iterator zt=it+1; zt!=_scan.end(); ++zt){
      if ( fabs((*it)->Msquark - (*zt)->Msquark) < gridx && 
           (*it)->Mgluino==(*zt)->Mgluino) gridx = fabs((*it)->Msquark - (*zt)->Msquark);
      if ( fabs((*it)->Mgluino - (*zt)->Mgluino && 
           (*it)->Msquark==(*zt)->Msquark) < gridy ) gridy = fabs((*it)->Mgluino - (*zt)->Mgluino);
    }
  } 
  //Now, interpolate
  gridx=80;
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin(); it!=_scan.end(); ++it){
     //find next neighbor in x and y for it:
    std::vector<SusyScan*>::const_iterator nextx=_scan.end(), nexty=_scan.end();
    double dx=9999, dy=9999; 
    for (std::vector<SusyScan*>::const_iterator zt=_scan.begin(); zt!=_scan.end(); ++zt){
      if (it==zt) continue;
      if ( fabs((*it)->Msquark - (*zt)->Msquark) < dx && 
           (*it)->Msquark < (*zt)->Msquark && (*it)->Mgluino==(*zt)->Mgluino) {
        dx = fabs((*it)->Msquark - (*zt)->Msquark);
	nextx = zt;
      }	
      if ( fabs((*it)->Mgluino - (*zt)->Mgluino) < dy && 
           (*it)->Mgluino < (*zt)->Mgluino && (*it)->Msquark==(*zt)->Msquark) {
        dy = fabs((*it)->Mgluino - (*zt)->Mgluino);
	nexty = zt;
      }
      //if (dx==gridx && dy==gridy) break;	
      if (dx==gridx) break;	
    }
    //interpolate in x:
    if (dx!=gridx && nextx!=_scan.end()){
        //std::cout << "m0 = " <<(*it)->Msquark  << ", m12="<< (*it)->Mgluino << std::endl;
       double dist = (*nextx)->Msquark - (*it)->Msquark;
       for (double x=gridx; x<dist; x+=gridx ){
         //std::cout << "adding m0 = " <<x << ", m12="<< (*it)->Mgluino << std::endl;
	 _scan.push_back( new SusyScan( ( (**it * (x/dist)) + (**nextx * (1.-x/dist)) )));
       }	 
    }	

  }
  std::cout<< "done: TheLimits::FillEmptyPointsByInterpolationInMsqMgl()" <<std::endl;
}


//void TheLimits::ExpandGrid<SusyScan>(double(*x)(const SusyScan*), double(*y)(const SusyScan*) );

