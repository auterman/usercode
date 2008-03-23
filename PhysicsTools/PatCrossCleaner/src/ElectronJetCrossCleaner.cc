
#include "PhysicsTools/PatCrossCleaner/interface/ElectronJetCrossCleaner.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace pat;
using namespace std;


void ElectronJetCrossCleaner::clean( 
             const edm::Handle<edm::View<Electron> >& Electrons,
             const edm::Handle<edm::View<Jet> >& Jets,
	     CrossCleanerMap & assMap) const
{
  // just a really stupid examplary cross cleaning:
  // (remove jet if closer in dR to an electron than config_.value)
  for (unsigned int iElectron=0; iElectron!=Electrons->size(); ++iElectron) {
    edm::RefToBase<reco::Candidate> electronRef( Electrons->refAt(iElectron) );
    for (unsigned int iJet=0; iJet!=Jets->size(); ++iJet) {
       if (deltaR((*Electrons)[iElectron], (*Jets)[iJet] ) < config_.value ){
	  edm::RefToBase<reco::Candidate> jetRef( Jets->refAt(iJet) );
	  assMap[jetRef].objects.push_back( electronRef );
	  assMap[jetRef].keep=false;
       }
    }
  }
}    
