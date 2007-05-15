/* \class MuonPtIsolationProducer
 *
 * computes and stores track pt isolation using PtAlgo for Muons
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/LeptonIsolation/interface/IsolationProducer.h"
#include "PhysicsTools/LeptonIsolation/interface/PtIsolationAlgo.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"

typedef IsolationProducer<reco::MuonCollection, reco::TrackCollection,
			  PtIsolationAlgo<reco::Muon,reco::TrackCollection> > 
                          MuonPtIsolationProducer;

DEFINE_FWK_MODULE( MuonPtIsolationProducer );
