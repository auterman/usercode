#ifndef ElectronJetCrossCleaner_h
#define ElectronJetCrossCleaner_h

/**
    \class pat::ElectronJetCrossCleaner ElectronJetCrossCleaner.h "PhysicsTools/PatUtils/ElectronJetCrossCleaner.h"
    \brief cross cleans objets

    \version $Id: ElectronJetCrossCleaner.h,v 1.2 2008/03/23 18:51:03 auterman Exp $
**/

#include "PhysicsTools/PatCrossCleaner/interface/CrossCleanerResult.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include <DataFormats/Common/interface/Handle.h>

namespace pat {

  /// Structure defining the electron - jet cross cleaning
  struct ElectronJetCleaning {
    double deltaR_min; ///min. distance in dR between electron & jet
    double sharedByjetEratio_max; ///max. shared to jet energy ratio
  };


  class ElectronJetCrossCleaner {
  public:
    ElectronJetCrossCleaner( const ElectronJetCleaning& cfg ) : config_( cfg ) {}
    ~ElectronJetCrossCleaner() {}

    void clean(
           //const edm::Handle< edm::View< pat::Electron> >& Electrons,
           //const edm::Handle< edm::View< pat::Jet> >& Jets,
           const edm::View< pat::Electron>& Electrons,
           const edm::View< pat::Jet>& Jets,
	   CrossCleanerMap & assMap
         ) const;

  private:
    ///calculates shared energy between electron & jet: based on SusyAnalyzer
    double SharedEnergy_( const pat::Electron& electron,
                          const pat::Jet& jet) const;

    ElectronJetCleaning config_;

  }; // class
} // namespace

#endif
