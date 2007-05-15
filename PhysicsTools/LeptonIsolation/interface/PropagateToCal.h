#include <algorithm>
#include <vector>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/Math.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class MagneticField;

class PropagateToCal {
public:
  PropagateToCal(const edm::ParameterSet &);
  ~PropagateToCal();
  bool propagate(const GlobalPoint& vertex, 
  	 		       GlobalVector& Cand, int charge,
			       const edm::EventSetup &setup) const;

private:
  const  MagneticField* getMagneticField(const edm::EventSetup &setup) const;
  bool   theIgnoreMaterial_;    /// whether or not propagation should ignore material
  double radius_, maxZ_, minZ_; /// Cylinder defining the inner surface of the calorimeter
};
