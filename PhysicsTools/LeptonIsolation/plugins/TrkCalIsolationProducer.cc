/* \class TrkCalIsolationProducer
 *
 * computes and stores calorimeter isolation using CalIsolationAlgo for Tracks
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/LeptonIsolation/interface/IsolationProducer.h"
#include "PhysicsTools/LeptonIsolation/interface/CalIsolationAlgo.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

typedef IsolationProducer<reco::TrackCollection, CaloTowerCollection,
			  CalIsolationAlgo<reco::Track, CaloTowerCollection> > 
                          TrkCalIsolationProducer;

DEFINE_FWK_MODULE( TrkCalIsolationProducer );
