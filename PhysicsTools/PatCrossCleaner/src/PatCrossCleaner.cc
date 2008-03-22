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
// $Id$
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
//DataFormats
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
//User
#include "PhysicsTools/Utilities/interface/EtComparator.h"
#include "PhysicsTools/Utilities/interface/PtComparator.h"
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
  _patCaloTowers ( iConfig.getParameter<edm::InputTag>( "patCaloTowers" ) ),
  _patTracks     ( iConfig.getParameter<edm::InputTag>( "patTracks" ) ),
  _patVertices   ( iConfig.getParameter<edm::InputTag>( "patVertices" ) ),

  _EJselectionCfg(iConfig.getParameter<edm::ParameterSet>("ElectronJetCrossCleaning")),    
  _ElectronJetCC(reco::modules::make<ElectronJetCrossCleaner>(_EJselectionCfg))
{

  ///produces cross-cleaned collections of above objects
  //Alternative: produce cross-cleaning decision & MET correction per object
  produces<std::vector<Jet> >();
  produces<std::vector<MET> >();
  produces<std::vector<Muon> >();
  produces<std::vector<Electron> >();
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
   Handle<std::vector<Jet> > pJets;
   iEvent.getByLabel(_patJets,pJets);
   GreaterByEt<pat::Jet>  eTComparator_;
   std::vector<Jet> Jets = *pJets;
   std::sort(Jets.begin(), Jets.end(), eTComparator_);

   //MET
   Handle<std::vector<MET> > pMets;
   iEvent.getByLabel(_patMets,pMets);
   std::vector<MET> Mets = *pMets;

   //Electrons   
   Handle<std::vector<Electron> > pElectrons;
   iEvent.getByLabel(_patElectrons,pElectrons);
   GreaterByPt<pat::Electron>  pTElectronComparator_;
   std::vector<Electron> Electrons = *pElectrons;
   std::sort(Electrons.begin(), Electrons.end(), pTElectronComparator_);

   //Muons   
   Handle<std::vector<Muon> > pMuons;
   iEvent.getByLabel(_patMuons,pMuons);
   GreaterByPt<pat::Muon>  pTMuonComparator_;
   std::vector<Muon> Muons = *pMuons;
   std::sort(Muons.begin(), Muons.end(), pTMuonComparator_);

   //CaloTowers
   Handle<CaloTowerCollection> pTowers;
   iEvent.getByLabel(_patCaloTowers,pTowers);

   //Tracks
   Handle<reco::TrackCollection> pTracks;
   iEvent.getByLabel(_patTracks,pTracks);

   //Vertices
   Handle<reco::VertexCollection> pVertices;
   iEvent.getByLabel(_patVertices,pVertices);


   ///1. Call the cross-cleaning algorithms:
   _conflicts = _ElectronJetCC.clean( Electrons, Jets );
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
   std::vector<Electron> cleanElectrons;
   for (std::vector<Electron>::const_iterator it = Electrons.begin();
        it!=Electrons.end(); ++it )
     if (_conflicts[ *it ].keep) //no conflicts for this e
   //if (_conflicts.find( *it )==_conflicts.end()) //no conflicts for this e
       cleanElectrons.push_back( *it );
   std::auto_ptr<std::vector<Electron> > ptrElectrons(&cleanElectrons);
   iEvent.put(ptrElectrons);

/*   
   //Jets
   std::vector<Jet> cleanJets;
   for (std::vector<Jet>::const_iterator it = Jets.begin();
        it!=Jets.end(); ++it )
     if (_conflicts[ *it  ].keep)//no conflicts for this jet
       cleanJets.push_back( *it );
   std::auto_ptr<std::vector<Jet> > ptrJets(&cleanJets);
   iEvent.put(ptrJets);

   //Muons
   std::vector<Muon> cleanMuons;
   for (std::vector<Muon>::const_iterator it = Muons.begin();
        it!=Muons.end(); ++it )
     if (_conflicts[ *it  ].keep)//no conflicts for this mu
       cleanMuons.push_back( *it );
   std::auto_ptr<std::vector<Muon> > ptrMuons(&cleanMuons);
   iEvent.put(ptrMuons);
*/
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
