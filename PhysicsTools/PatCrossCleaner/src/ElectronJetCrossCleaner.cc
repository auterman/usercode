
#include "PhysicsTools/PatCrossCleaner/interface/ElectronJetCrossCleaner.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace pat;
using namespace std;

const CrossCleanerResult ElectronJetCrossCleaner::clean( 
             const vector<pat::Electron>& Electrons,
             const vector<pat::Jet>& Jets ) const
{
  CrossCleanerResult resultmap;
  CrossCleanerValue dummy;
  vector<CrossCleanerValue> result;
  CrossCleanerResult::Filler filler(resultmap);

  //just a really stupid examplary cross cleaning:
  vector<pat::Electron>::const_iterator Electron = Electrons.begin();
  vector<pat::Jet>::const_iterator Jet = Jets.begin();
  for (; Electron!=Electrons.end(); ++Electron) {
    for (; Jet!=Jets.end(); ++Jet) {
       dummy.keep=true;
       dummy.objects.clear();
       if (deltaR(*Electron,*Jet) < config_.value ){
	  dummy.objects.push_back( &(*Jet) );
	  dummy.keep=false;
       }
       result.push_back(dummy);
    }
  }
  //filler.insert(Electrons, result.begin(), result.end());

  return resultmap;
}    
