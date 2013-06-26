// $Id: Selection.h,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef SELECTION_H
#define SELECTION_H

#include "Event.h"

#include <string>

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



#endif
