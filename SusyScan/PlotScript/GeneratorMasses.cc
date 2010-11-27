
#include "GeneratorMasses.h"

#include <fstream>
#include <iostream>
#include <cmath>

std::vector<GeneratorMasses> FillGeneratorMasses(std::string file)
{
   std::ifstream masses_file;
   masses_file.open(file.c_str());
   std::vector<GeneratorMasses> points;
   while (1) {
      GeneratorMasses p;
      masses_file >> p.Mzero
                  >> p.Mhalf
                  >> p.TanBeta
                  >> p.Mu  
                  >> p.Azero
                  >> p.Mtop  
                  >> p.muQ	  
                  >> p.Q	  
                  >> p.M1	  
                  >> p.M2
                  >> p.M3	  
                  >> p.MGL	  
                  >> p.MUL	  
                  >> p.MB1	  
                  >> p.MSN	  
                  >> p.MNTAU	  
                  >> p.MZ1	  
                  >> p.MW1	  
                  >> p.MHL	  
                  >> p.MUR	  
                  >> p.MB2	  
                  >> p.MEL	  
                  >> p.MTAU1	  
                  >> p.MZ2	  
                  >> p.MW2	  
                  >> p.MHH	  
                  >> p.MDL	  
                  >> p.MT1	  
                  >> p.MER	  
                  >> p.MTAU2	  
                  >> p.MZ3	  
                  >> p.MHA	  
                  >> p.MDR	  
                  >> p.MT2	  
                  >> p.MZ4	  
                  >> p.MHp;

      if (!masses_file.good()) break;
      if (fabs(p.Mu)!=1.) {
         std::cerr << "check lines near m0=" << p.Mzero << ", m1/2=" << p.Mhalf	 << std::endl;
         break;
      }	
      points.push_back( p );
   }
   return points;
}
