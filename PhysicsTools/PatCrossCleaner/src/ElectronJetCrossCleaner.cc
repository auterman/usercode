
#include "PhysicsTools/PatCrossCleaner/interface/ElectronJetCrossCleaner.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

using namespace pat;
using namespace std;


void ElectronJetCrossCleaner::clean( 
             const edm::View<Electron>& Electrons,
             const edm::View<Jet>& Jets,
	     CrossCleanerMap & assMap) const
{
  //check the ratio of shared energy between electron & jet to the jet energy,
  //if the deltaR between electron and jet is smaller than a min value
  for (unsigned int iElectron=0; iElectron!=Electrons.size(); ++iElectron) {
    edm::RefToBase<reco::Candidate> electronRef( Electrons.refAt(iElectron) );
    for (unsigned int iJet=0; iJet!=Jets.size(); ++iJet) {
       if ( deltaR(Electrons[iElectron], Jets[iJet] ) > config_.deltaR_min )
         continue;
       if ( SharedEnergy_(Electrons[iElectron], Jets[iJet]) > 
             config_.sharedByjetEratio_max ) {
	  edm::RefToBase<reco::Candidate> jetRef( Jets.refAt(iJet) );
	  assMap[jetRef].objects.push_back( electronRef );
	  assMap[jetRef].keep=false;
       }
    }
  }
}    


///this function is copied from the SusyAnalyzer: SusyRecoTools::IsEMObjectInJet
//@@ -> template it so that it can also be used for photons
double ElectronJetCrossCleaner::SharedEnergy_( 
             const pat::Electron& emobject,
             const pat::Jet& jet) const
{
  double result = 0.0;

  //first: gather the electrons tower-id's and EM transv. energies
  //@@ gsfElectron->caloTower() returns only 1(!) tower -> this has to be fixed!
  std::map<std::pair<int,int>,double> etowers;
  const CaloTower ct = *(emobject.originalObjectRef()->caloTower());
  etowers[ std::make_pair(ct.id().iphi(),ct.id().ieta()) ] = ct.emEt();
  //const CaloTowerCollection ets = ...
  //for ( CaloTowerCollection::const_iterator it=ets.begin();
  //      it!=ets.end(); ++it ) {
  //  etowers[ std::make_pair(it->id().iphi(),it->id().ieta()) ] = it->emEt();
  //}

  //then: check against jet and return overlap energy
  std::vector<CaloTowerRef> jt = jet.originalObjectRef()->getConstituents();
  for (std::vector <CaloTowerRef>::const_iterator tow = jt.begin();
       tow!=jt.end(); ++tow) {
     std::pair<int,int> idx = std::make_pair((*tow)->id().iphi(),(*tow)->id().ieta());
     if (etowers.find(idx) != etowers.end())
        result += etowers[idx];
  }	

  return result;
}

/* the original function in SusyAnalyzer
bool SusyRecoTools::IsEMObjectInJet(int ichk, int iJet, math::XYZVector* sharedMomentum)
{
 // Checks whether an electron or photon is included in the jet energy
 // and if true, it returns the momentum vector shared by the two
 
 
 if (ichk < 0 || iJet < 0){return false;}
 
 if (DEBUGLVL >= 2){
   cout << " object index = " << ichk
         << " type = " << RecoData[ichk]->particleType()
         << " compared with jet, index = " << iJet << endl;
 }
 const SuperCluster* superCluster = NULL;
 int ptype = RecoData[ichk]->particleType()/10;
 if (ptype == 1){
   // Get the electron candidate
   const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
   if (elecand == NULL) {
     if (DEBUGLVL >= 2){
       cout << " IsEMObjectInJet: No electron candidate for this electron, index = " 
            << ichk << endl;
     }
     return false;
   }
   if (DEBUGLVL >= 2){
     cout << "  Electron energy = " << elecand->energy() << endl;
   }
   superCluster = &(*(elecand->superCluster() ));
 } else if (ptype == 4){
   // Get the photon candidate
   const Photon* photcand = RecoData[ichk]->photonCandidate();
   if (photcand == NULL) {
     if (DEBUGLVL >= 2){
       cout << " IsEMObjectInJet: No photon candidate for this photon, index = " 
            << ichk << endl;
     }
     return false;
   }
   if (DEBUGLVL >= 2){
     cout << "  Photon energy = " << photcand->energy() << endl;
   }
   superCluster = &(*(photcand->superCluster() ));
 }
 
 // Define a window in eta,phi for the SuperCluster
 float phimin=0., phimax=0., etamin=0., etamax=0.;
 bool window = EMCaloTowerWindow(ichk, phimin, phimax, etamin, etamax); 
 if (!window){ return false;}
 
 // Get the jet candidate and the CaloJet
 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(RecoData[iJet]->jetCandidate());
 if (jetcand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " IsEMObjectInJet: No jet candidate for this electron, index = " 
          << iJet << endl;
   }
   return false;
 }
  
  // Collect the CaloTowers inside this window, save their detId in a vector
 vector<CaloTowerDetId> eleDetId;
 vector<float> eleTowerEnergy;
 vector<float> eleTowerEta;
 vector<float> eleTowerPhi;
 float eleEnergySum = 0.;
 CaloTowerCollection::const_iterator calo;
 for (calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
   float tow_eta = calo->eta();
   float tow_phi = calo->phi();
   if (IsInPhiWindow (tow_phi, phimin, phimax) 
       && (tow_eta-etamin)*(tow_eta-etamax) <= 0.){
//     cout << "  Tower eta = " << tow_eta << " phi = " << tow_phi
//          << " dphimin = " << dphimin << " dphimax = " << dphimax << endl;
     eleDetId.push_back(calo->id());
     eleTowerEnergy.push_back(calo->emEnergy());
     eleTowerEta.push_back(calo->eta());
     eleTowerPhi.push_back(calo->phi());
     eleEnergySum += calo->emEnergy();
//     cout << "  Electron tower energy = " << calo->emEnergy()
//          << ", Electron energy sum = " << eleEnergySum << endl;
    }
 }
 if (DEBUGLVL > 2){
   cout << "  Electron caloTower energy sum = " << eleEnergySum << endl;
 }
  
 // Collect the CaloTowers detIds for the jet 
  // the following line does not work for 130
// vector<CaloTowerDetId> jetDetId = jetcand->getTowerIndices();
 vector<CaloTowerRef> jetCaloRefs = jetcand->getConstituents();
 if (DEBUGLVL >= 2){
   cout << "  Jet energy = " << jetcand->energy() << endl;
 }
  
 // Loop over the detIds vector of the electron
 float sharedEnergy = 0.;
 float sharedPx = 0.;
 float sharedPy = 0.;
 float sharedPz = 0.;
 for (unsigned int i = 0; i < eleDetId.size(); i++){
   // find whether this detId is in the jet detId list
   for (unsigned int j = 0; j < jetCaloRefs.size(); j++){
   // if yes, add its energy to the sum
     if (eleDetId[i] == jetCaloRefs[j]->id() ){
       sharedEnergy += eleTowerEnergy[i];
       float eleTowerTheta = 2. * atan(exp(-eleTowerEta[i]));
       if (eleTowerTheta < 0.) {eleTowerTheta += 3.141592654;}
       float sintheta = sin(eleTowerTheta);
       sharedPx += eleTowerEnergy[i]*sintheta*cos(eleTowerPhi[i]);
       sharedPy += eleTowerEnergy[i]*sintheta*sin(eleTowerPhi[i]);
       sharedPz += eleTowerEnergy[i]*cos(eleTowerTheta);
//       cout << "  Shared tower energy = " << eleTowerEnergy[i]
//            << ", shared energy sum = " << sharedEnergy << endl;
     }
   }
 }
 if (DEBUGLVL >= 2){
   cout << "  CaloTower shared energy sum = " << sharedEnergy << endl;
 }
 
 eleDetId.clear();
 eleTowerEnergy.clear();
 eleTowerEta.clear();
 eleTowerPhi.clear();
 jetCaloRefs.clear();

 sharedMomentum->SetXYZ(sharedPx,sharedPy,sharedPz);
 
 if (sharedEnergy > 0.) {return true;}
 else {return false;}

}
*/
