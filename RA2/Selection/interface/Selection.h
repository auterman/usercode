#ifndef Selection_H
#define Selection_H

// System include files
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// Framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


// SUSY include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "SusyAnalysis/EventSelector/interface/SelectorSequence.h"


//
// Class declaration
//

class Selection : public edm::EDFilter {
public:
  explicit Selection(const edm::ParameterSet&);
  ~Selection();

private:
  // *** CMSSW interface
  /// Called once per job, at start
  virtual void beginJob(const edm::EventSetup&);
  /// Called for each event: returns the global decision
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  /// Called once per job, at end
  virtual void endJob();

  // *** Data memebers
  SelectorSequence sequence_;                  ///< Interface to selectors
  std::vector<std::string> filterSelection_;   ///< Container for filter selection (i.e., hard cuts)
  std::vector<size_t> filterSelectionIndices_; ///< Selector indices for filter selection
  edm::InputTag weightName_;
  double weight_;                              ///event weight (if available, 1 otherwise)
  unsigned Nevents;
  
  //Statistics
  struct TStatistics{
    //@@ FixMe: Make NTotal static!
    TStatistics():NTotal(0),NSelected(0),Nminus1total(0),Nminus1(0),NCumulative(0){};
    double NTotal,NSelected, Nminus1total, Nminus1, NCumulative;
    double Eff(){ return (NTotal==0 ? 0.0 : NSelected/(double)NTotal); };
    double EffNminus1(){ return (Nminus1total==0 ? 0.0 : Nminus1/(double)Nminus1total); };
    double EffCumulative(){ return (NTotal==0 ? 0.0 : NCumulative/(double)NTotal); };
  };
  std::vector<TStatistics> SelectionStats_;
};

#endif
