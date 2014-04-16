#include "Interface.h"


//template<>
//Bool_t  Interface<GJets_Photon>::MySelector::Process(Long64_t entry)
//{
//  std::cout << eventNumber << ". event" << std::endl;
//  return true;
//}


double CalcTransMet(double met, double phi_met, double phi_photon){
  return met * sin( DeltaPhi(phi_met,phi_photon) );
}

double CalcParalMet(double met, double phi_met, double phi_photon){
  return met * cos( DeltaPhi(phi_met,phi_photon) );
}

double transverse_met(GJets_Photon*t){ return CalcTransMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double transverse_met(GJets_Jet*t){    return CalcTransMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double transverse_met(QCD_Photon*t){   return CalcTransMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double transverse_met(QCD_Jet*t){      return CalcTransMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}

double parallel_met(GJets_Photon*t){ return CalcParalMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double parallel_met(GJets_Jet*t){    return CalcParalMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double parallel_met(QCD_Photon*t){   return CalcParalMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}
double parallel_met(QCD_Jet*t){      return CalcParalMet(t->met,t->metPhi-kPI,t->photons_phi[0]);}


bool tight_isolated(double pt, double ptstar, double phi, double eta, int seed, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso)
{
  double pT = (ptstar>0.?ptstar:pt);
  return (pT>110. &&
          fabs(eta)<1.442 &&
          seed==0 &&
          HoE <0.05 &&
	  sigmaIetaOeta < 0.012 &&
	  chaIso < 2.6 &&
	  neuIso < 3.5 + 0.04*pT &&
	  phoIso < 1.3 + 0.005*pT
	 );
}		    

bool loose_isolated(double pt, double ptstar, double phi, double eta, int seed, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso)
{
  double pT = (ptstar>0.?ptstar:pt);
  return (!(tight_isolated(pt,ptstar,phi,eta,seed,HoE,sigmaIetaOeta,chaIso,neuIso,phoIso))&&
          pT>110. &&
          fabs(eta)<1.442 &&
          seed==0 &&
          HoE <0.05 &&
	  sigmaIetaOeta < 0.012 &&
	  (chaIso < 5.2 || (neuIso < 3.5 + 0.04*pT && phoIso < 1.3 + 0.005*pT)) &&
	  (neuIso < 7 + 0.06*pT || (chaIso < 2.6 && phoIso < 1.3 + 0.005*pT)) &&
	  (phoIso < 2.6 + 0.0075*pT || (chaIso < 2.6 &&	neuIso < 3.5 + 0.04*pT))
	 );
}		    



Double_t DeltaPhi(Double_t x, Double_t y) {
   // (static function) returns phi angle in the interval [-PI,PI)
   x -= y;
   while (x >= kPI) x -= kTWOPI;
   while (x < -kPI) x += kTWOPI;
   return x;
}

