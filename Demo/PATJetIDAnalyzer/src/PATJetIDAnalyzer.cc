// -*- C++ -*-
//
// Package:    PATJetIDAnalyzer
// Class:      PATJetIDAnalyzer
// 
/**\class PATJetIDAnalyzer PATJetIDAnalyzer.cc Demo/PATJetIDAnalyzer/src/PATJetIDAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Mon Feb 25 11:33:02 CET 2008
// $Id$
//
//

//system includes
#include <fstream>
#include <iostream>
//framework includes
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
//root includes
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
//User include files
#include "DataFormats/Math/interface/deltaR.h"
//AOD
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
//PAT
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "Demo/PATJetIDAnalyzer/interface/NameScheme.h"
#include "Demo/PATJetIDAnalyzer/interface/PATJetIDAnalyzer.h"



//
// constructors and destructor
//
PATJetIDAnalyzer::PATJetIDAnalyzer(const edm::ParameterSet& iConfig) :
  _recJet  ( iConfig.getParameter<edm::InputTag>( "recJet" ) ),
  _genJet  ( iConfig.getParameter<edm::InputTag>( "genJet" ) ),
  _recMet  ( iConfig.getParameter<edm::InputTag>( "recMet" ) ),
  _genMet  ( iConfig.getParameter<edm::InputTag>( "genMet" ) ),
  _hist    ( iConfig.getParameter<std::string>( "hist" ) ),
  _jetminpt( iConfig.getParameter<double>( "MinJetPt" ) ),
  _jetmaxeta(iConfig.getParameter<double>( "MaxJetEta" ) )
{
   //now do what ever initialization is needed

}


PATJetIDAnalyzer::~PATJetIDAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PATJetIDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  
  //CaloJets
  edm::Handle<reco::CaloJetCollection> CJets;
  iEvent.getByLabel( _recJet, CJets );
  reco::CaloJetCollection CaloJets = *CJets;
  std::sort( CaloJets.begin(), CaloJets.end(), PtGreater());
  
  //GenJets
  edm::Handle<reco::GenJetCollection> GJets; 
  iEvent.getByLabel( _genJet, GJets );
  reco::GenJetCollection GenJets = *GJets;
  std::sort( GenJets.begin(), GenJets.end(), PtGreater());

  // matching maps for GenJets and CaloJets and vice versa
  // fills some histograms inside (dR vs. dE, rec. eff., gen. vs. matched ...)
  makeMatchingMaps(GJets,CJets);

  //CaloJets
  unsigned multiplicity = 0;
  double met=0.0, ht=0.0;
  for (reco::CaloJetCollection::const_iterator it=CaloJets.begin();
       it!=CaloJets.end(); ++it) {
    if (it->pt() > _jetminpt && fabs( it->eta() )<_jetmaxeta){
      if (multiplicity<_njets ) {
	_pt_jet[     multiplicity]->Fill( it->pt()  );
	_eta_jet[    multiplicity]->Fill( it->eta() );
	_phi_jet[    multiplicity]->Fill( it->phi() );
	_emfrac_jet[ multiplicity]->Fill( it->emEnergyFraction() );
	_hadfrac_jet[multiplicity]->Fill( it->energyFractionHadronic() );
	_n60_jet[    multiplicity]->Fill( it->n60() );
	_n90_jet[    multiplicity]->Fill( it->n90() );
	_area_jet[   multiplicity]->Fill( it->towersArea() );
      }
      ++multiplicity;
      ht += it->pt();
    }  
  }  
  _jetmult->Fill( multiplicity );
  _ht->Fill( ht );

  //MET
  edm::Handle<reco::CaloMETCollection> recMet;
  iEvent.getByLabel(_recMet,recMet);

  for ( reco::CaloMETCollection::const_iterator it=recMet->begin(); 
        it!=recMet->end(); ++it) {
    met = it->pt();
    _met->Fill( met );
    _metx->Fill(met*sin(it->phi()));
    _mety->Fill(met*cos(it->phi()));
    break;
  }
  _metmult->Fill( recMet->size() );
  _htmet->Fill( ht+met );
  _metsig->Fill( met / (0.97*sqrt(ht)) );
  if (multiplicity>1 ) {
    _nv->Fill( (met*met-(CaloJets[0].pt()-CaloJets[1].pt())*(CaloJets[0].pt()-CaloJets[1].pt())) / (CaloJets[1].pt()*CaloJets[1].pt())  );
    _nv2->Fill( (met*met-(CaloJets[0].pt()-CaloJets[1].pt())*(CaloJets[0].pt()-CaloJets[1].pt())) / (CaloJets[0].pt()*CaloJets[0].pt()) );
    _dijet->Fill(sqrt(pow(CaloJets[0].energy()+CaloJets[1].energy(),2)
		      -(pow(CaloJets[0].px()+CaloJets[1].px(),2)
			+pow(CaloJets[0].py()+CaloJets[1].py(),2)
			+pow(CaloJets[0].pz()+CaloJets[1].pz(),2) ) ) );
  }  

  //GenJets
  multiplicity = 0;
  met=0.0; 
  ht=0.0;
  for (reco::GenJetCollection::const_iterator it=GenJets.begin();
       it!=GenJets.end(); ++it) {
    if (it->pt() > _jetminpt && fabs( it->eta() )<_jetmaxeta) {
      if (multiplicity<_ngenjets ) {
	_pt_genjet[       multiplicity]->Fill( it->pt()  );
	_eta_genjet[      multiplicity]->Fill( it->eta() );
	_phi_genjet[      multiplicity]->Fill( it->phi() );
	_emfrac_genjet[   multiplicity]->Fill( it->emEnergy() / it->energy() );
	_hadfrac_genjet[  multiplicity]->Fill( it->hadEnergy() / it->energy() );
	_invisible_genjet[multiplicity]->Fill( it->invisibleEnergy() / it->energy() );
	_aux_genjet[      multiplicity]->Fill( it->auxiliaryEnergy() / it->energy() );
      }
      ++multiplicity;
      ht += it->pt();
    }  
  }  
  _genjetmult->Fill( multiplicity );
  
  //GenMET
  edm::Handle<reco::GenMETCollection> genMet;
  iEvent.getByLabel(_genMet,genMet);

  for ( reco::GenMETCollection::const_iterator it=genMet->begin(); 
        it!=genMet->end(); ++it) {
    met = it->pt();
    _genmet->Fill( met );
    _genmetx->Fill(met*sin(it->phi()));
    _genmety->Fill(met*cos(it->phi()));
    break;
  }
  _genmetmult->Fill( recMet->size() );
  _genht->Fill( ht );
  _genhtmet->Fill( ht+met );
  if (multiplicity>1 ) {
    _gendijet->Fill(sqrt(pow(GenJets[0].energy()+GenJets[1].energy(),2)
			 -(pow(GenJets[0].px()+GenJets[1].px(),2)
			   +pow(GenJets[0].py()+GenJets[1].py(),2)
			   +pow(GenJets[0].pz()+GenJets[1].pz(),2) ) ) );
  }  

}


// ------------ method called once each job just before starting event loop  ------------
void 
PATJetIDAnalyzer::beginJob(const edm::EventSetup&)
{
  if( _hist.empty() )
    return;

  edm::Service<TFileService> fs;
  if( !fs ){
    throw edm::Exception( edm::errors::Configuration,
                          "TFile Service is not registered in cfg file" );
  }

  NameScheme hsusy("hsusy");
  ofstream hist(_hist.c_str(), std::ios::out);
  //CaloJets
  for (unsigned i=0; i<_njets; ++i) {
    _pt_jet[i]      = fs->make<TH1F>(hsusy.name(hist, "ptj",  i ), hsusy.name("PtJet",  i), 100,  0. , 1000.);
    _eta_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "etaj", i ), hsusy.name("EtaJet", i), 100, -5. ,    5.);
    _phi_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "phij", i ), hsusy.name("PhiJet", i),  70, -3.5,    3.5);
    _emfrac_jet[i]  = fs->make<TH1F>(hsusy.name(hist, "emfj", i ), hsusy.name("EmfJet", i),  50,  0. ,    2.);
    _hadfrac_jet[i] = fs->make<TH1F>(hsusy.name(hist, "hadfj",i ), hsusy.name("HadfJet",i),  50,  0. ,    2.);
    _n60_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "n60j", i ), hsusy.name("n60Jet", i),  50,  0. ,   50.);
    _n90_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "n90j", i ), hsusy.name("n90Jet", i),  50,  0. ,   50.);
    _area_jet[i]    = fs->make<TH1F>(hsusy.name(hist, "areaj",i ), hsusy.name("AreaJet",i),  50,  0. ,    5.);
  }
  _jetmult   = fs->make<TH1F>(hsusy.name(hist, "multj" ), hsusy.name("JetMultiplicity"), 21,  -0.5, 20.5);
  _ht        = fs->make<TH1F>(hsusy.name(hist, "ht"    ), hsusy.name("Ht"),     100,    0.0, 2000.);
  _htmet     = fs->make<TH1F>(hsusy.name(hist, "htmet" ), hsusy.name("HtMet"),  100,    0.0, 2000.);
  _dijet     = fs->make<TH1F>(hsusy.name(hist, "dijet" ), hsusy.name("DiJet"),  100,    0.0, 2000.);
  //GenJets
  for (unsigned i=0; i<_ngenjets; ++i) {
    _pt_genjet[i]      = fs->make<TH1F>(hsusy.name(hist, "ptgj",  i ), hsusy.name("PtGenJet",  i), 100,  0. , 1000.);
    _eta_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "etagj", i ), hsusy.name("EtaGenJet", i), 100, -5. ,    5.);
    _phi_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "phigj", i ), hsusy.name("PhiGenJet", i),  70, -3.5,    3.5);
    _emfrac_genjet[i]  = fs->make<TH1F>(hsusy.name(hist, "emfgj", i ), hsusy.name("EmfGenJet", i),  50,  0. ,    5.);
    _hadfrac_genjet[i] = fs->make<TH1F>(hsusy.name(hist, "hadfgj",i ), hsusy.name("HadfGenJet",i),  50,  0. ,    5.);
    _invisible_genjet[i]=fs->make<TH1F>(hsusy.name(hist, "invisgj",i), hsusy.name("InvisiblefGenJet", i), 50, 0.,5.);
    _aux_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "auxgj", i ), hsusy.name("AuxfGenJet", i), 50,  0. ,    5.);
  }
  _genjetmult   = fs->make<TH1F>(hsusy.name(hist, "multgj" ), hsusy.name("GenJetMultiplicity"), 21,  -0.5, 20.5);
  _genht        = fs->make<TH1F>(hsusy.name(hist, "htgj"   ), hsusy.name("GenHt"),     100,    0.0, 2000.);
  _genhtmet     = fs->make<TH1F>(hsusy.name(hist, "htmetgj"), hsusy.name("GenHtMet"),  100,    0.0, 2000.);
  _gendijet     = fs->make<TH1F>(hsusy.name(hist, "dijetgj"), hsusy.name("GenDiJet"),  100,    0.0, 2000.);
  //MET
  _met       = fs->make<TH1F>(hsusy.name(hist, "met"   ), hsusy.name("Met"),    100,     0.0, 1000.);
  _metmult   = fs->make<TH1F>(hsusy.name(hist, "metmult"),hsusy.name("MetMult"), 11,    -0.5, 10.5);
  _metx      = fs->make<TH1F>(hsusy.name(hist, "metx"  ), hsusy.name("MetX"),   100, -1000.0, 1000.);
  _mety      = fs->make<TH1F>(hsusy.name(hist, "mety"  ), hsusy.name("MetY"),   100, -1000.0, 1000.);
  _metsig    = fs->make<TH1F>(hsusy.name(hist, "metsig"), hsusy.name("MetSig"), 100,     0.0, 100.);
  //GenMET
  _genmet       = fs->make<TH1F>(hsusy.name(hist, "metgj"   ), hsusy.name("GenMet"),    100,     0.0, 1000.);
  _genmetmult   = fs->make<TH1F>(hsusy.name(hist, "metmultgj"),hsusy.name("GenMetMult"), 11,    -0.5, 10.5);
  _genmetx      = fs->make<TH1F>(hsusy.name(hist, "metxgj"  ), hsusy.name("GenMetX"),   100, -1000.0, 1000.);
  _genmety      = fs->make<TH1F>(hsusy.name(hist, "metygj"  ), hsusy.name("GenMetY"),   100, -1000.0, 1000.);
  //other
  _nv        = fs->make<TH1F>(hsusy.name(hist, "nv"    ), hsusy.name("NV"),     100,   -5.0,  5.);
  _nv2       = fs->make<TH1F>(hsusy.name(hist, "nv2"   ), hsusy.name("NV2"),    100,   -5.0,  5.);
  //matching
  _GenOnCalo_match = fs->make<TH2F>(hsusy.name(hist,"gencalo"), hsusy.name("GenOnCalo"), 60, 0., 3., 120, -1, 5.);
  _CaloOnGen_match = fs->make<TH2F>(hsusy.name(hist,"calogen"), hsusy.name("CaloOnGen"), 60, 0., 3., 120, -1, 5.);
  _GenVsMatched_pt  = fs->make<TH2F>(hsusy.name(hist,"genmatched_pt"), hsusy.name("GenVsMatched_Pt")  , 100,  0. , 2000. , 100,  0. , 2000.);
  _GenVsMatched_eta = fs->make<TH2F>(hsusy.name(hist,"genmatched_eta"), hsusy.name("GenVsMatched_Eta"), 100, -5. ,    5. , 100, -5. ,    5.);
  _GenVsMatched_phi = fs->make<TH2F>(hsusy.name(hist,"genmatched_phi"), hsusy.name("GenVsMatched_Phi"),  70, -3.5,    3.5,  70, -3.5,    3.5);
  _RecoEff_pt  = fs->make<TH1F>(hsusy.name(hist, "recoeffpt") , hsusy.name("RecoEff_Pt") , 100,  0. , 2000. );
  _RecoEff_eta = fs->make<TH1F>(hsusy.name(hist, "recoeffeta"), hsusy.name("RecoEff_Eta"), 100, -5. ,    5. );
  _RecoEff_phi = fs->make<TH1F>(hsusy.name(hist, "recoeffphi"), hsusy.name("RecoEff_Phi"),  70, -3.5,    3.5);
  //helper histograms
  _RecoTot_pt = new TH1F("recototpt" , "RecoTot_Pt" , 100,  0. , 2000. );
  _RecoTot_eta= new TH1F("recototeta", "RecoTot_Eta", 100, -5. ,    5. );
  _RecoTot_phi= new TH1F("recototphi", "RecoTot_Phi",  70, -3.5,    3.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATJetIDAnalyzer::endJob() {
  _RecoTot_pt->Sumw2();
  _RecoEff_pt->Sumw2();
  _RecoEff_pt->Divide(_RecoTot_pt);
  _RecoTot_eta->Sumw2();
  _RecoEff_eta->Sumw2();
  _RecoEff_eta->Divide(_RecoTot_eta);
  _RecoTot_phi->Sumw2();
  _RecoEff_phi->Sumw2();
  _RecoEff_phi->Divide(_RecoTot_phi);
}

// ------------ redo matching 
void 
PATJetIDAnalyzer::makeMatchingMaps(edm::Handle<reco::GenJetCollection> GenJets, edm::Handle<reco::CaloJetCollection> CaloJets)
{
  MatchingMapGen.clear();
  MatchingMapJet.clear();

  // Loop over generator Jets
  for(reco::GenJetCollection::const_iterator genjet = GenJets->begin(); genjet!=GenJets->end(); ++genjet) {
    const reco::CaloJet* jetbest=0;
    const reco::CaloJet* jetmatched=0;
    double minDelR= 0.3;
    double minDelE=-1/4.;
    double maxDelE= 3.0;
    double mindE=999.;
    double mindR=999.;
    double delR;
    double delE;
    double E1=(*genjet).energy();
    bool matched = false;
    // Loop over Calo jets
    for(reco::CaloJetCollection::const_iterator calojet = CaloJets->begin(); calojet!=CaloJets->end(); ++calojet){  
      double E2=(*calojet).energy();
      delR=deltaR(*calojet,*genjet);
      delE=(E2-E1)/E1;
      // This is the matching condition
      if(delR<minDelR && delE>minDelE && delE<maxDelE){
	matched=true;
	minDelR=delR;
	jetmatched=&(*calojet);
      }
      if(delR<mindR){
	mindR=delR;
	mindE=delE;
	jetbest=&(*calojet);
      }
    }
    if (matched) {
      MatchingMapGen.insert(make_pair(&(*genjet),&(*jetmatched)));
      _GenVsMatched_pt->Fill((*genjet).pt(),(*jetmatched).pt());
      _GenVsMatched_eta->Fill((*genjet).eta(),(*jetmatched).eta());
      _GenVsMatched_phi->Fill((*genjet).phi(),(*jetmatched).phi());
      _RecoEff_pt->Fill((*genjet).pt());
      _RecoEff_eta->Fill((*genjet).eta());
      _RecoEff_phi->Fill((*genjet).phi());
    }
    _RecoTot_pt->Fill((*genjet).pt());
    _RecoTot_eta->Fill((*genjet).eta());
    _RecoTot_phi->Fill((*genjet).phi());
    _GenOnCalo_match->Fill(mindR,mindE);
  }

  // Loop over Calo Jets
  for(reco::CaloJetCollection::const_iterator calojet = CaloJets->begin(); calojet!=CaloJets->end(); ++calojet) {
    const reco::GenJet* jetbest=0;
    const reco::GenJet* jetmatched=0;
    double minDelR= 0.3;
    double minDelE=-1/4.;
    double maxDelE= 3.0;
    double mindE=999.;
    double mindR=999.;
    double delR;
    double delE;
    double E1=(*calojet).energy();
    bool matched = false;
    // Loop over jets
    for(reco::GenJetCollection::const_iterator genjet = GenJets->begin(); genjet!=GenJets->end(); ++genjet){  
      double E2=(*genjet).energy();
      delR=deltaR(*genjet,*calojet);
      delE=(E2-E1)/E1;
      // This is the matching condition
      if(delR<minDelR && delE>minDelE && delE<maxDelE){
	matched=true;
	minDelR=delR;
	jetmatched=&(*genjet);
      }
      if(delR<mindR){
	mindR=delR;
	mindE=delE;
	jetbest=&(*genjet);
      }
    }
    if (matched) {
      MatchingMapJet.insert(make_pair(&(*calojet),&(*jetmatched)));
    }
    _CaloOnGen_match->Fill(mindR,mindE);
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(PATJetIDAnalyzer);
