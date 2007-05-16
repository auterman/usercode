/* \class HHisolatedMuons
 *
 * Produces isolated Muons, i.e. remove not isolated ones
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HHAnalysis/HHIsolationProducer/interface/HHIsolationProducer.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"

typedef std::vector<double> isolation_result;
typedef HHIsolationProducer<reco::MuonCollection, isolation_result>
        HHisolatedMuons;

DEFINE_FWK_MODULE( HHisolatedMuons );
