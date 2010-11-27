#include "TheLimits.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "IsoMassLine.h"

#include "TH2.h"
#include "TH2F.h"

void TheLimits::plot(TH2*h, double(*x)(const SusyScan*), double(*y)(const SusyScan*), double(*f)(const SusyScan*) )
{
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin();it!=_scan.end();++it )
    //h->Fill( x(*it), y(*it), f(*it) );
    h->SetBinContent( h->GetXaxis()->FindBin(x(*it)), 
                      h->GetYaxis()->FindBin(y(*it)), f(*it) );
}

void TheLimits::match(const std::vector<GeneratorMasses> genm)
{
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    bool match = false;
    for (std::vector<GeneratorMasses>::const_iterator gt=genm.begin();gt!=genm.end();++gt){
      if ((*it)->Mzero==gt->MZERO && 
          (*it)->Mhalf==gt->MHALF && 
	  (*it)->Azero==gt->AZERO &&
	  (*it)->TanBeta==gt->TANB &&
	  (*it)->Mu==gt->SGNMU ) {
        (*it)->M1 = gt->M1;
        (*it)->M2 = gt->M2;
        (*it)->M3 = gt->M3;
        (*it)->MGL = gt->MGL;
        (*it)->MUL = gt->MUL;
        (*it)->MB1 = gt->MB1;
        (*it)->MSN = gt->MSN;
        (*it)->MNTAU = gt->MNTAU;
        (*it)->MZ1 = gt->MZ1;
        (*it)->MW1 = gt->MW1;
        (*it)->MHL = gt->MHL;
        (*it)->MUR = gt->MUR;
        (*it)->MB2 = gt->MB2;
        (*it)->MEL = gt->MEL;
        (*it)->MTAU1 = gt->MTAU1;
        (*it)->MZ2 = gt->MZ2;
        (*it)->MW2 = gt->MW2;
        (*it)->MHH = gt->MHH;
        (*it)->MDL = gt->MDL;
        (*it)->MT1 = gt->MT1;
        (*it)->MER = gt->MER;
        (*it)->MTAU2 = gt->MTAU2;
        (*it)->MZ3 = gt->MZ3;
        (*it)->MHA = gt->MHA;
        (*it)->MDR = gt->MDR;
        (*it)->MT2 = gt->MT2;
        (*it)->MZ4 = gt->MZ4;
        (*it)->MHp = gt->MHp;
	match = true;
      }	  
    }
    //if (!match) std::cout << "No match for M0="<<(*it)->Mzero
    //                      << ", M12="<<(*it)->Mhalf<<std::endl;
  }
}
