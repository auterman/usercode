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
};

#endif
