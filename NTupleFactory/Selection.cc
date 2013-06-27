// $Id: Selection.cc,v 1.10 2013/06/12 17:23:46 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "Selection.h"

#include "TMath.h"

#include <complex>

const float EA_Charged_Small = 0.012;
const float EA_Neutral_Small = 0.030;
const float EA_Photon_Small = 0.148;
const float EA_Charged_Large = 0.010;
const float EA_Neutral_Large = 0.057;
const float EA_Photon_Large = 0.130;

//H/E < 0:05; sigma_ietaieta < 0.012; Iso charged had < 2.6 GeV; Iso neutral had < 3.5 GeV + 0.04pT; Iso EM < 1:3 GeV + 0:005pT
bool tightIsolation(const Event*evt, unsigned idx)
{
  float pt = (evt->Get<FloatArray>("photon_pt")).val[idx];
  
  return (evt->Get<FloatArray>("photon_hadTowOverEm").val[idx]  < 0.05  &&
          evt->Get<FloatArray>("photon_sigmaIetaIeta").val[idx] < 0.012 &&
	  evt->Get<FloatArray>("photon_chargedIso").val[idx]    < 2.6   &&
	  evt->Get<FloatArray>("photon_neutralIso").val[idx]    < 3.5 + 0.04*pt &&
	  evt->Get<FloatArray>("photon_photonIso").val[idx]     < 1.3 + 0.005*pt 
	 ); 
}

// to be studied....
bool looseIsolation(const Event*evt, unsigned idx)
{
  float pt = (evt->Get<FloatArray>("photon_pt")).val[idx];
  float chargedIso = evt->Get<FloatArray>("photon_chargedIso").val[idx];
  float sigmaietaieta = evt->Get<FloatArray>("photon_sigmaIetaIeta").val[idx];

  return (evt->Get<FloatArray>("photon_hadTowOverEm").val[idx]  < 0.05  &&
	  evt->Get<FloatArray>("photon_neutralIso").val[idx]    < 3.5 + 0.04*pt &&
	  evt->Get<FloatArray>("photon_photonIso").val[idx]     < 1.3 + 0.005*pt &&
          sigmaietaieta < 0.014 &&
	  chargedIso    < 15.   &&
	  (sigmaietaieta > 0.012 || chargedIso > 2.6)
	 ); 
}

float DeltaPhi(float a, float b) {
  float temp = std::abs(a-b);
  if (temp <= TMath::Pi())
    return temp;
  else
    return  2.*TMath::Pi() - temp;
} 

bool matchPhotonJet(const Event*evt, unsigned iPhoton, unsigned iJet)
{
  const float maxDR = 0.3; //0.3 cone
  const float maxDpTrel = 0.3; //30%
  float dphi = DeltaPhi((evt->Get<FloatArray>("photon_phi")).val[iPhoton], (evt->Get<FloatArray>("jet_phi")).val[iJet]);  
  float deta = (evt->Get<FloatArray>("photon_eta")).val[iPhoton] - (evt->Get<FloatArray>("jet_eta")).val[iJet];
  float dpt  = (evt->Get<FloatArray>("photon_pt")).val[iPhoton] - (evt->Get<FloatArray>("jet_pt")).val[iJet];
  float sumpt= (evt->Get<FloatArray>("photon_pt")).val[iPhoton] + (evt->Get<FloatArray>("jet_pt")).val[iJet];
  return dphi*dphi+deta*deta <= maxDR*maxDR && 
         std::abs(dpt/sumpt) <= maxDpTrel;
}


void FindTightPhoton(int * idxPhoton, const Event*evt)
{
  for (unsigned i=0; i<ARRAYSIZE; ++i) {
    if ( evt->Get<FloatArray>("photon_pt").val[i] < 80.) continue;
    if ( std::abs(evt->Get<FloatArray>("photon_eta").val[i]) > 1.4442) continue;
    if ( evt->Get<FloatArray>("photon_pixelseed").val[i] ) continue;
    if ( !tightIsolation(evt, i)) continue;
    *idxPhoton=i;	
    return; 
  }
}

void FindLoosePhoton(int * idxPhoton, const Event*evt)
{
  for (unsigned i=0; i<ARRAYSIZE; ++i) {
    if ( evt->Get<FloatArray>("photon_pt").val[i] < 80.) continue;
    if ( std::abs(evt->Get<FloatArray>("photon_eta").val[i]) > 1.4442) continue;
    if ( evt->Get<FloatArray>("photon_pixelseed").val[i] ) continue;
    if ( !looseIsolation(evt, i)) continue;
    *idxPhoton=i;	
    return; 
  }
}

void FindGoodJets(int * idxJet1, int * idxJet2, int idxPhoton, const Event *evt)
{
  int l_idxJet1 = -1, l_idxJet2 = -1;
  for (unsigned i=0; i<ARRAYSIZE; ++i) {
    if ( (evt->Get<FloatArray>("jet_pt")).val[i] < 30.) continue;
    if ( std::abs(evt->Get<FloatArray>("jet_eta").val[i]) > 2.6) continue;
    if ( matchPhotonJet(evt, idxPhoton, i) ) continue;
    if (l_idxJet1<0) l_idxJet1=i;
    else {
      l_idxJet2=i;	
      break; 
    }  
  }
  *idxJet1=l_idxJet1;
  *idxJet2=l_idxJet2;
}  


//photon pt>=80GeV, HT>=450GeV, njets(30GeV)>=2 && Abs(jet_eta) < 2.6, Abs(photon_eta) < 1.4442, met > 100  
bool FinalSelectionExceptMet::Pass(const Event*evt) const 
{
  //MET
  if (evt->Get<Float_t>("met") <= 100.) return false;

  //HT
  if (evt->Get<Float_t>("ht") <= 450.) return false;

  // Find a good photon
  int idxPhoton = -1;
  FindTightPhoton( &idxPhoton, evt);
  if (idxPhoton<0) return false;
  
  //Find good jets
  int idxJet1 = -1, idxJet2 = -1;
  FindGoodJets(&idxJet1, &idxJet2, idxPhoton, evt);
  if (idxJet1<0 || idxJet2<0) return false;
  
  return true;
}

//photon pt>=80GeV, HT>=450GeV, njets(30GeV)>=2 && Abs(jet_eta) < 2.6, Abs(photon_eta) < 1.4442, tight isolation
bool QcdPhoton::Pass(const Event*evt) const 
{
  //HT
  if (evt->Get<Float_t>("ht") <= 450.) return false;

  // Find a good photon
  int idxPhoton = -1;
  FindTightPhoton( &idxPhoton, evt);
  if (idxPhoton<0) return false;
  
  //Find good jets
  int idxJet1 = -1, idxJet2 = -1;
  FindGoodJets(&idxJet1, &idxJet2, idxPhoton, evt);
  if (idxJet1<0 || idxJet2<0) return false;
 
  return true;
}

//photon pt>=80GeV, HT>=450GeV, njets(30GeV)>=2 && Abs(jet_eta) < 2.6, Abs(photon_eta) < 1.4442, loose (and not tight) isolation
bool QcdFake::Pass(const Event*evt) const 
{
  //HT
  if (evt->Get<Float_t>("ht") <= 450.) return false;

  // Find a good photon
  int idxPhoton = -1;
  FindLoosePhoton( &idxPhoton, evt);
  if (idxPhoton<0) return false;
  
  //Find good jets
  int idxJet1 = -1, idxJet2 = -1;
  FindGoodJets(&idxJet1, &idxJet2, idxPhoton, evt);
  if (idxJet1<0 || idxJet2<0) return false;
  
  return true;
}
