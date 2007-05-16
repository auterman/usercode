/* \class HHIsolationMuon
 *
 * Filters not isolated Muons
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HHAnalysis/HHIsolationFilter/interface/HHIsolationFilter.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"

typedef std::vector<double> isolation_result;
typedef HHIsolationFilter<reco::MuonCollection, isolation_result> HHIsolationMuonFilter;

DEFINE_FWK_MODULE( HHIsolationMuonFilter );

