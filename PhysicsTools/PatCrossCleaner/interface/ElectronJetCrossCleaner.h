#ifndef ElectronJetCrossCleaner_h
#define ElectronJetCrossCleaner_h

/**
    \class pat::ElectronJetCrossCleaner ElectronJetCrossCleaner.h "PhysicsTools/PatUtils/ElectronJetCrossCleaner.h"
    \brief cross cleans objets

    \version $Id: ElectronJetCrossCleaner.h,v 1.0 2008/03/22 11:23:38 auterman Exp $
**/

#include "PhysicsTools/PatCrossCleaner/interface/CrossCleanerResult.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

namespace pat {

  /// Structure defining the electron - jet cross cleaning
  struct ElectronJetCleaning {
    double value; ///Some exemplary cut value
  };


  class ElectronJetCrossCleaner {
  public:
    ElectronJetCrossCleaner( const ElectronJetCleaning& cfg ) : config_( cfg ) {}
    ~ElectronJetCrossCleaner() {}

    const CrossCleanerResult clean(
           const std::vector<pat::Electron>& Electrons,
           const std::vector<pat::Jet>& Jets
         ) const;
    

  private:
    ElectronJetCleaning config_;

  }; // class
} // namespace

#endif
