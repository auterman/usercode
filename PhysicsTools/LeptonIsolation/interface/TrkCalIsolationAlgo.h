#include <algorithm>
#include <vector>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/Math.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include <TObjArray.h>

template <typename T1, typename C2>
class TrkCalIsolationAlgo {
public:
  typedef double value_type;
  TrkCalIsolationAlgo( const edm::ParameterSet & );
  ~TrkCalIsolationAlgo();
  double operator()(const T1 &, const C2 &, const edm::EventSetup &) const;

private:
  double dRMin_, dRMax_, dzMax_;
};

template <typename T1, typename C2>
TrkCalIsolationAlgo<T1,C2>::TrkCalIsolationAlgo( const edm::ParameterSet & cfg ) :
  dRMin_( cfg.template getParameter<double>( "dRMin" ) ),
  dRMax_( cfg.template getParameter<double>( "dRMax" ) )
{
}

template <typename T1, typename C2>
TrkCalIsolationAlgo<T1,C2>::~TrkCalIsolationAlgo() {
}

///This source (track) already has defined outer eta and phi. 
///This is the track's end point in the tracker, this should be close
///the tracks entry into the calorimeter.
///A specialized template operator () for tracks in the CalIsolationAlgo class is not
///feasable, since the () operator cannot be overloaded.
template <typename T1, typename C2> double TrkCalIsolationAlgo<T1,C2>::
operator()(const T1 & cand, const C2 & elements, const edm::EventSetup &iSetup) const {
  double etSum = 0;
  for( typename C2::const_iterator elem = elements.begin(); 
       elem != elements.end(); ++elem ) {
    double dR = deltaR( elem->eta(), elem->phi(), 
                        cand.outerEta(), cand.outerPhi() );
    if ( dR < dRMax_ && dR > dRMin_ ) {
      etSum += elem->et();
    }
  }
  return etSum;
}
