/* \class MuonCalIsolationProducer
 *
 * computes and stores calorimeter isolation using CalIsolationAlgo for Muons
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/LeptonIsolation/interface/IsolationProducer.h"
#include "PhysicsTools/LeptonIsolation/interface/CalIsolationAlgo.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"


typedef IsolationProducer<reco::MuonCollection, CaloTowerCollection,
			  CalIsolationAlgo<reco::Muon, CaloTowerCollection> > 
                          MuonCalIsolationProducer;

DEFINE_FWK_MODULE( MuonCalIsolationProducer );
