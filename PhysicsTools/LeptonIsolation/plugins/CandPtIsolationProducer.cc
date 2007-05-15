/* \class CandPtIsolationProducer
 *
 * computes and stores isolation using PtAlgo for Candidates
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/LeptonIsolation/interface/IsolationProducer.h"
#include "PhysicsTools/LeptonIsolation/interface/PtIsolationAlgo.h"

typedef IsolationProducer<reco::CandidateCollection, reco::CandidateCollection,
			  PtIsolationAlgo<reco::Candidate,reco::CandidateCollection> > 
                          CandPtIsolationProducer;

DEFINE_FWK_MODULE( CandPtIsolationProducer );
