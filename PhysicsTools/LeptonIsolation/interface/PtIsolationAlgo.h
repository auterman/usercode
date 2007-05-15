#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <algorithm>
#include "PhysicsTools/Utilities/interface/Math.h"

template <typename T1, typename C2>
class PtIsolationAlgo {
public:
  typedef double value_type;
  PtIsolationAlgo( const edm::ParameterSet & );
  ~PtIsolationAlgo();
  double operator()(const T1 &, const C2 &, const edm::EventSetup &) const;

private:
  double dRMin_, dRMax_, dzMax_;
};

template <typename T1, typename C2>
PtIsolationAlgo<T1,C2>::PtIsolationAlgo( const edm::ParameterSet & cfg ) :
  dRMin_( cfg.template getParameter<double>( "dRMin" ) ),
  dRMax_( cfg.template getParameter<double>( "dRMax" ) ),
  dzMax_( cfg.template getParameter<double>( "dzMax" ) ) {
}

template <typename T1, typename C2>
PtIsolationAlgo<T1,C2>::~PtIsolationAlgo() {
}

template <typename T1, typename C2>
double PtIsolationAlgo<T1,C2>::operator()(const T1 & cand, const C2 & elements, const edm::EventSetup &) const {
  double ptSum = 0;
  for( typename C2::const_iterator elem = elements.begin(); elem != elements.end(); ++ elem ) {
    double dz = fabs( elem->vz() - cand.vz() );
    //double dR = deltaR( elem->p4(), cand.p4() );
    double dR = deltaR( elem->eta(), elem->phi(), cand.eta(), cand.phi() );
    if ( dz < dzMax_ &&  dR < dRMax_ && dR > dRMin_ ) {
      ptSum += elem->pt();
    }
  }

  return ptSum;
}
