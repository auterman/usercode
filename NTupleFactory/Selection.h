// $Id: Selection.h,v 1.11 2013/06/12 17:23:46 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef SELECTION_H
#define SELECTION_H

#include "Event.h"

#include <string>
#include <cmath>
#include <algorithm>

void FindTightPhoton(int * idxPhoton, const Event*evt);
void FindLoosePhoton(int * idxPhoton, const Event*evt);
void FindGoodJets(int * idxJet1, int * idxJet2, int idxPhoton, const Event *evt);

// inclusive base class ----------------------------
class Selection {
 public:
  Selection(const std::string& name):name_(name){}
  virtual ~Selection(){}
  virtual bool Pass(const Event*evt) const  {return true;}
  std::string Name() const {return name_;}
 protected:
  std::string name_; 
};


//implementations ----------------------------------

//photon pt>=75GeV, HT>=400GeV, njets(30GeV)>=2
class PreSelection : public Selection {
 public:
  PreSelection(const std::string& name):Selection(name){}
  virtual bool Pass(const Event*evt) const {
    if ((evt->Get<FloatArray>("photon_pt")).val[0]<  75.) return false;
    if ( evt->Get<Float_t>   ("ht")               < 400.) return false;
    if ((evt->Get<FloatArray>("jet_pt")).val[1]   <  30.) return false;
    return true;
  }
};


//photon pt>=80GeV, HT>=450GeV, njets(30GeV)>=2 && Abs(jet_eta) < 2.6, Abs(photon_eta) < 1.4442
class FinalSelectionExceptMet : public Selection {
 public:
  FinalSelectionExceptMet(const std::string& name):Selection(name){}
  virtual bool Pass(const Event*evt) const;
};


//photon pt>=80GeV, HT>=450GeV, njets(30GeV)>=2 && Abs(jet_eta) < 2.6, Abs(photon_eta) < 1.4442, met > 100  
class FinalSelection : public FinalSelectionExceptMet {
 public:
  FinalSelection(const std::string& name):FinalSelectionExceptMet(name){}
  virtual bool Pass(const Event*evt) const {
    if (evt->Get<Float_t>("met") <= 100.) return false;
    return FinalSelectionExceptMet::Pass( evt );
  }
};


// data-driven QCD -----------------------------------
const float MetCutQcd = 100.;
///base class <-> not used directly-------------------
class QcdFake : public Selection {
 public:
  QcdFake(const std::string& name):Selection(name){}
  virtual bool Pass(const Event*evt) const;
};

///base class <-> not used directly-------------------
class QcdPhoton : public Selection {
 public:
  QcdPhoton(const std::string& name):Selection(name){}
  virtual bool Pass(const Event*evt) const;
};
//----------------------------------------------------

//met < 100 GeV, fake_g (loose isolation)
class QcdWeightFake : public QcdFake {
 public:
  QcdWeightFake(const std::string& name):QcdFake(name){}
  virtual bool Pass(const Event*evt) const {
    if ( evt->Get<Float_t>("met") > MetCutQcd) return false;
    return QcdFake::Pass( evt );;
  }
};

//met < 100 GeV, photon (tight iso)
class QcdWeightPhoton : public QcdPhoton {
 public:
  QcdWeightPhoton(const std::string& name):QcdPhoton(name){}
  virtual bool Pass(const Event*evt) const {
    if ( evt->Get<Float_t>("met") > MetCutQcd) return false;
    return QcdPhoton::Pass( evt );;
  }
};

//met > 100 GeV, fake_g (loose isolation)
class QcdControlFake : public QcdFake {
 public:
  QcdControlFake(const std::string& name):QcdFake(name){}
  virtual bool Pass(const Event*evt) const {
    if ( evt->Get<Float_t>("met") <= MetCutQcd) return false;
    return QcdFake::Pass( evt );;
  }
};

//met > 100 GeV, photon (tight iso)
class QcdControlPhoton : public QcdPhoton {
 public:
  QcdControlPhoton(const std::string& name):QcdPhoton(name){}
  virtual bool Pass(const Event*evt) const {
    if ( evt->Get<Float_t>("met") <= MetCutQcd) return false;
    return QcdPhoton::Pass( evt );;
  }
};
//--------------------------------------------------------

#endif
