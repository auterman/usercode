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


bool tight_isolated(double pt, double ptstar, double phi, double eta, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso)
{
  double pT = (ptstar>0.?ptstar:pt);
  return (pT>110. &&
          fabs(eta)<1.442 &&
          HoE <0.05 &&
	  sigmaIetaOeta < 0.012 &&
	  chaIso < 2.6 &&
	  neuIso < 3.5 + 0.04*pT &&
	  phoIso < 1.3 + 0.005*pT
	 );
}		    

bool loose_isolated(double pt, double ptstar, double phi, double eta, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso)
{
  double pT = (ptstar>0.?ptstar:pt);
  return (!(tight_isolated(pt,ptstar,phi,eta,HoE,sigmaIetaOeta,chaIso,neuIso,phoIso))&&
          pT>110. &&
          fabs(eta)<1.442 &&
          HoE <0.05 &&
	  sigmaIetaOeta < 0.012 &&
	  (chaIso < 5.2 || (neuIso < 3.5 + 0.04*pT && phoIso < 1.3 + 0.005*pT)) &&
	  (neuIso < 7 + 0.06*pT || (chaIso < 2.6 && phoIso < 1.3 + 0.005*pT)) &&
	  (phoIso < 2.6 + 0.0075*pT || (chaIso < 2.6 &&	neuIso < 3.5 + 0.04*pT))
	 );
}		    

int JetMult(float g_pt, float g_eta, float g_phi, float *jets_pt, float *jets_eta, float *jets_phi, int njets)
{
  int n=0;
  for (int i=0; i<njets; ++i){
    if (jets_pt[i]<30. || fabs(jets_eta[i])>3.0 || deltaR(g_eta,g_phi,jets_eta[i],jets_phi[i])<0.5 ) continue;
    ++n;
  }
  return n;  
}

int LooseMult(int n_g,float *g_pt, float *g_ptJet, float *g_phi, float *g_eta,float *g_hadTowOverEm,float *g_sigmaIetaIeta,float *g_chargedIso,float *g_neutralIso,float *g_photonIso,int *g_pixelseed)
{
  int n=0;
  for (int i=0; i<n_g; ++i) {
    if (loose_isolated(g_pt[i],g_ptJet[i],g_phi[i],g_eta[i],g_hadTowOverEm[i],g_sigmaIetaIeta[i],
                       g_chargedIso[i],g_neutralIso[i],g_photonIso[i]) &&
       g_pixelseed==0)
     ++n;  
  }
  return n;
}

int TightMult(int n_g,float *g_pt, float *g_ptJet, float *g_phi, float *g_eta,float *g_hadTowOverEm,float *g_sigmaIetaIeta,float *g_chargedIso,float *g_neutralIso,float *g_photonIso,int *g_pixelseed)
{
  int n=0;
  for (int i=0; i<n_g; ++i) {
    if (tight_isolated(g_pt[i],g_ptJet[i],g_phi[i],g_eta[i],g_hadTowOverEm[i],g_sigmaIetaIeta[i],
                       g_chargedIso[i],g_neutralIso[i],g_photonIso[i]) &&
       g_pixelseed==0)
     ++n;  
  }
  return n;
}

bool LeptonVeto(int n_e, float * e_pt, float * e_eta, int n_m, float *m_pt, float *m_eta)
{
  for (int i=0;i<n_e;++i) if (e_pt[i]>20.&&fabs(e_eta[i])<2.5) return false;
  for (int i=0;i<n_m;++i) if (m_pt[i]>20.&&fabs(m_eta[i])<2.5) return false;
  return true;
}

ROOT::Math::PtEtaPhiEVector Recoil(float g_pt, float g_eta, float g_phi, float *jets_pt, float* jets_eta, float *jets_phi, int njets )
{
  ROOT::Math::PtEtaPhiEVector recoil;
  for (int i=0; i<njets; ++i){
    if (jets_pt[i]<30. || fabs(jets_eta[i])>3.0 || deltaR(g_eta,g_phi,jets_eta[i],jets_phi[i])<0.5 ) continue;
    double E=jets_pt[i] * cos( 2.*atan( exp(-jets_eta[i]) ));
    ROOT::Math::PtEtaPhiEVector jet(jets_pt[i],jets_eta[i],jets_phi[i],E);
    recoil += jet;
  }
  return recoil;
}


double Mht(float g_pt, float g_eta, float g_phi, float *jets_pt, float* jets_eta, float *jets_phi, int njets )
{
  ROOT::Math::PtEtaPhiEVector mht(g_pt,g_eta,g_phi, g_pt*cos(2.*atan(exp(-g_eta))));
  for (int i=0; i<njets; ++i){
//std::cout << i<<" / "<<njets
//          <<" jet pt="<<jets_pt[i]<<", eta="<<jets_eta[i]<<", phi="<<jets_phi[i]<<std::endl;
    if (jets_pt[i]<30. || fabs(jets_eta[i])>3.0 || deltaR(g_eta,g_phi,jets_eta[i],jets_phi[i])<0.5 ) continue;
    double E=jets_pt[i] * cos( 2.*atan( exp(-jets_eta[i]) ));
    ROOT::Math::PtEtaPhiEVector jet(jets_pt[i],jets_eta[i],jets_phi[i],E);
    mht += jet;
  }
  return mht.Pt();
}

double Recoil_ht(float g_pt, float g_eta, float g_phi, float *jets_pt, float* jets_eta, float *jets_phi, int njets )
{
  double ht=0;
  for (int i=0; i<njets; ++i){
    if (jets_pt[i]<30. || fabs(jets_eta[i])>3.0 || deltaR(g_eta,g_phi,jets_eta[i],jets_phi[i])<0.5 ) continue;
    double E=jets_pt[i] * cos( 2.*atan( exp(-jets_eta[i]) ));
    ht += jets_pt[i];
  }
  return ht;
}

double CorectedMet(float met, float met_phi, float g_pt, float g_eta, float g_phi, float j_pt, float j_eta, float j_phi)
{
  ROOT::Math::PtEtaPhiEVector Met(met,0,met_phi,0);
  ROOT::Math::PtEtaPhiEVector gamma(j_pt,g_eta,g_phi, j_pt*cos(2.*atan(exp(-g_eta))));
  ROOT::Math::PtEtaPhiEVector jet(j_pt,j_eta,j_phi, j_pt*cos(2.*atan(exp(-j_eta))));
  Met = Met + gamma - jet;
  return Met.Pt();
}


double Recoil_pt(ROOT::Math::PtEtaPhiEVector* recoil)
{
  return recoil->Pt();
}

double Recoil_phi(ROOT::Math::PtEtaPhiEVector* recoil)
{
  return recoil->Phi();
}

Double_t DeltaPhi(Double_t x, Double_t y) {
   // (static function) returns phi angle in the interval [-PI,PI)
   x -= y;
   while (x >= kPI) x -= kTWOPI;
   while (x < -kPI) x += kTWOPI;
   return x;
}

