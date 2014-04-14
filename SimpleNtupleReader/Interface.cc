#include "Interface.h"


//template<>
//Bool_t  Interface<GJets_Photon>::MySelector::Process(Long64_t entry)
//{
//  std::cout << eventNumber << ". event" << std::endl;
//  return true;
//}


double met(GJets_Photon*t){return t->met;}
double met(GJets_Jet*t){return t->met;}
double met(QCD_Photon*t){return t->met;}
double met(QCD_Jet*t){return t->met;}
