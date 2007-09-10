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
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include "PhysicsTools/LeptonIsolation/interface/PropagateToCal.h"

template <typename T1, typename C2>
class CalIsolationAlgo {
public:
  typedef double value_type;
  CalIsolationAlgo( const edm::ParameterSet & );
  ~CalIsolationAlgo();
  double operator()(const T1 &, const C2 &, const edm::EventSetup &) const;

private:
  double dRMin_, dRMax_, dzMax_;
  bool   do_propagation_;
  PropagateToCal SrcAtCal;
};

template <typename T1, typename C2>
CalIsolationAlgo<T1,C2>::CalIsolationAlgo( const edm::ParameterSet & cfg ) :
  dRMin_( cfg.template getParameter<double>( "dRMin" ) ),
  dRMax_( cfg.template getParameter<double>( "dRMax" ) ),
  do_propagation_( cfg.template getParameter<bool>( "PropagateToCal" ) ),
  SrcAtCal(cfg)///class to propagate particles from the vertex to the calorimeter
{
}

template <typename T1, typename C2>
CalIsolationAlgo<T1,C2>::~CalIsolationAlgo() {
}

template <typename T1, typename C2> double CalIsolationAlgo<T1,C2>::
operator()(const T1 & cand, const C2 & elements, const edm::EventSetup &iSetup) const {
  const GlobalPoint Vertex(cand.vx(), cand.vy(), cand.vz());//@@check if this is [cm]!
  GlobalVector Cand(cand.pt(), cand.eta(), cand.phi()); 

  ///Extrapolate charged particles from their vertex to the point of entry into the
  ///calorimeter, if this is requested in the cfg file.
  if (do_propagation_ && cand.charge()!=0) 
     SrcAtCal.propagate(Vertex, Cand, cand.charge(), iSetup);

  double etSum = 0;
  for( typename C2::const_iterator elem = elements.begin(); 
       elem != elements.end(); ++elem ) {
    double dR = deltaR( elem->eta(), elem->phi(), 
                        (double)Cand.eta(), (double)Cand.phi() );
    if ( dR < dRMax_ && dR > dRMin_ ) {
      etSum += elem->et();
    }
  }
  return etSum;
}
