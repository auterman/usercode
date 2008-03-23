// -*- C++ -*-
//
// Package:    PatCrossCleaner
// Class:      PatCrossCleaner
// 
/**\class PatCrossCleaner PatCrossCleaner.cc PhysicsTools/PatCrossCleaner/src/PatCrossCleaner.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian AUTERMANN
//         Created:  Sat Mar 22 12:58:04 CET 2008
// $Id: PatCrossCleaner.cc,v 1.1.1.1 2008/03/22 19:07:55 auterman Exp $
//
//


//system
#include <vector>
#include <memory>
//PAT
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
//DataFormats
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
//User
#include "PhysicsTools/PatCrossCleaner/interface/PatCrossCleaner.h"


using namespace pat;


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PatCrossCleaner::PatCrossCleaner(const edm::ParameterSet& iConfig) :
  _patJets       ( iConfig.getParameter<edm::InputTag>( "patJets" ) ),
  _patMets       ( iConfig.getParameter<edm::InputTag>( "patMets" ) ),
  _patMuons      ( iConfig.getParameter<edm::InputTag>( "patMuons" ) ),
  _patElectrons  ( iConfig.getParameter<edm::InputTag>( "patElectrons" ) ),
  _patPhotons    ( iConfig.getParameter<edm::InputTag>( "patPhotons" ) ),
  _patTaus       ( iConfig.getParameter<edm::InputTag>( "patTaus" ) ),
  _patCaloTowers ( iConfig.getParameter<edm::InputTag>( "patCaloTowers" ) ),
  _patTracks     ( iConfig.getParameter<edm::InputTag>( "patTracks" ) ),
  _patVertices   ( iConfig.getParameter<edm::InputTag>( "patVertices" ) ),

  _EJselectionCfg(iConfig.getParameter<edm::ParameterSet>("ElectronJetCrossCleaning")),    
  _ElectronJetCC(reco::modules::make<ElectronJetCrossCleaner>(_EJselectionCfg))
{

  ///produces cross-cleaned collections of above objects
  //Alternative: produce cross-cleaning decision & MET correction per object
  produces<std::vector<pat::Jet> >();
  produces<std::vector<pat::MET> >();
  produces<std::vector<pat::Muon> >();
  produces<std::vector<pat::Electron> >();
  produces<std::vector<pat::Photon> >();
  produces<std::vector<pat::Tau> >();
  //produces<std::vector<Track> >(); //there is nothing like this in PAT (yet?)
  //produces<std::vector<Tower> >(); //there is nothing like this in PAT (yet?)
}


PatCrossCleaner::~PatCrossCleaner()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PatCrossCleaner::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   //Jets   
   Handle<edm::View<pat::Jet> > pJets;
   iEvent.getByLabel(_patJets,pJets);

   //MET
   Handle<edm::View<pat::MET> > pMets;
   iEvent.getByLabel(_patMets,pMets);
   edm::View<pat::MET> Mets = *pMets;

   //Muons   
   Handle<edm::View<pat::Muon> > pMuons;
   iEvent.getByLabel(_patMuons,pMuons);

   //Electrons   
   Handle<edm::View<pat::Electron> > pElectrons;
   iEvent.getByLabel(_patElectrons,pElectrons);

   //Photons   
   Handle<edm::View<pat::Photon> > pPhotons;
   iEvent.getByLabel(_patPhotons,pPhotons);

   //Taus   
   Handle<edm::View<pat::Tau> > pTaus;
   iEvent.getByLabel(_patTaus,pTaus);
/*
   //CaloTowers
   Handle<CaloTowerCollection> pTowers;
   iEvent.getByLabel(_patCaloTowers,pTowers);

   //Tracks
   Handle<reco::TrackCollection> pTracks;
   iEvent.getByLabel(_patTracks,pTracks);

   //Vertices
   Handle<reco::VertexCollection> pVertices;
   iEvent.getByLabel(_patVertices,pVertices);
*/

   ///The association map containing the objects that are to be dropped (key),
   ///and the objects that caused them to be dropped (value).
   CrossCleanerMap  assMap;
   
   ///1. Call the cross-cleaning algorithms:
   _ElectronJetCC.clean( pElectrons, pJets, assMap );
   //_conflicts += _MuonJetCC.clean( Muons, Jets );
   //...

	
   //@@ todo: "+" and "+=" operators of the ValueMap has to be checked,
   //         if existing keys are handled correctly!
   //         Probably an exception is thrown, if a key already exists!
   


   ///2. Interference handling: 
   ///All object for which entries in the map "_conflics" exist should be
   ///dropped. However dropping one object might solve conflicts of another 
   ///object, which therefore doesn't need to be dropped. This should be 
   ///handled, here.
   
   //@@ todo...
   
   
   ///3. Re-calulate MET 

   //@@ todo...



   ///4. Produce clean collections

   //Electrons
   std::auto_ptr<std::vector<pat::Electron> > cleanElectrons( new std::vector<pat::Electron>);
   for (unsigned int i = 0; i < pElectrons->size(); ++i ){
     edm::RefToBase<reco::Candidate> electronRef( pElectrons->refAt(i) );
     if (assMap.find(electronRef) != assMap.end())
	continue;
     cleanElectrons->push_back( (*pElectrons)[ i ] );
   }
   iEvent.put(cleanElectrons);

   //Jets
   std::auto_ptr<std::vector<pat::Jet> > cleanJets( new std::vector<pat::Jet>);
   for (unsigned int i = 0; i < pJets->size(); ++i ){
     edm::RefToBase<reco::Candidate> JetRef( pJets->refAt(i) );
     if (assMap.find(JetRef) != assMap.end()) {
        // --DEBUG
       cout << "Reject jet with phi="<<(*pJets)[ i ].phi()
	    << " and eta="<<(*pJets)[ i ].eta()
	    << " because of the following objects: \n";
       for (std::vector<edm::RefToBase<reco::Candidate> >::const_iterator it=
	     assMap[JetRef].objects.begin(); it!=assMap[JetRef].objects.end(); ++it )
	     cout << "  object with phi="<<(*it)->phi()
        	  << " and eta="<<(*it)->eta()<<endl;
        // --DEBUG
	continue;
     }	 
     cleanJets->push_back( (*pJets)[ i ] );
   }
   iEvent.put(cleanJets);


   //Muons
   std::auto_ptr<std::vector<pat::Muon> > cleanMuons( new std::vector<pat::Muon>);
   for (unsigned int i = 0; i < pMuons->size(); ++i ){
     edm::RefToBase<reco::Candidate> MuonRef( pMuons->refAt(i) );
     if (assMap.find(MuonRef) != assMap.end())
	continue;
     cleanMuons->push_back( (*pMuons)[ i ] );
   }
   iEvent.put(cleanMuons);

   //Photons
   std::auto_ptr<std::vector<pat::Photon> > cleanPhotons( new std::vector<pat::Photon>);
   for (unsigned int i = 0; i < pPhotons->size(); ++i ){
     edm::RefToBase<reco::Candidate> PhotonRef( pPhotons->refAt(i) );
     if (assMap.find(PhotonRef) != assMap.end())
	continue;
     cleanPhotons->push_back( (*pPhotons)[ i ] );
   }
   iEvent.put(cleanPhotons);

   //Taus
   std::auto_ptr<std::vector<pat::Tau> > cleanTaus( new std::vector<pat::Tau>);
   for (unsigned int i = 0; i < pTaus->size(); ++i ){
     edm::RefToBase<reco::Candidate> TauRef( pTaus->refAt(i) );
     if (assMap.find(TauRef) != assMap.end())
	continue;
     cleanTaus->push_back( (*pTaus)[ i ] );
   }
   iEvent.put(cleanTaus);

}

// ------------ method called once each job just before starting event loop  ------------
void 
PatCrossCleaner::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PatCrossCleaner::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PatCrossCleaner);
