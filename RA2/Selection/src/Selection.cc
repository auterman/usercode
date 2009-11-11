
#include "RA2/Selection/interface/Selection.h"
#include <iostream>
#include <iomanip>

Selection::Selection(const edm::ParameterSet& iConfig) :
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  filterSelection_( iConfig.getParameter<std::vector<std::string> >("filterSelection") ),
  weightName_(iConfig.getParameter<edm::InputTag>("weightName") ),
  Nevents(0)
{
  // Translate filterSelection strings into selector indices
  //filterSelectionIndices_.reserve(filterSelection_.size());
  for ( size_t i=0; i<filterSelection_.size(); ++i ) {
    filterSelectionIndices_.push_back(sequence_.selectorIndex(filterSelection_[i]));
    SelectionStats_.push_back( Selection::TStatistics() );
  }
  
  // List all selectors and selection variables
  edm::LogVerbatim("Selection") << "Selectors are:" << std::endl;
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {
      edm::LogVerbatim("Selection") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i )
        edm::LogVerbatim("Selection") << "    - " << (*it)->variableNames()[i];
      edm::LogVerbatim("Selection") << std::endl;
    }


}


//________________________________________________________________________________________
Selection::~Selection() { }


// ------------ method called on each new Event  ------------
bool
Selection::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Retrieve the decision of each selector module
  SelectorDecisions decisions = sequence_.decisions(iEvent);
 
  edm::Handle< double > event_weight;
  iEvent.getByLabel(weightName_, event_weight);
  weight_ = (event_weight.isValid() ? (*event_weight) : 1.0);
  ++Nevents;

  bool dec(true);
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ){ // only "filter selection"
    //Filter decision
    dec = dec && decisions.decision(filterSelectionIndices_[i]);
    
    //Statistics
    SelectionStats_[i].NTotal += weight_;
    if (decisions.decision(i)) SelectionStats_[i].NSelected += weight_;
    if (decisions.cumulativeDecision(i)) SelectionStats_[i].NCumulative += weight_;
    if (decisions.complementaryDecision(i)){
      SelectionStats_[i].Nminus1total += weight_;
      if (decisions.decision(i)) SelectionStats_[i].Nminus1 += weight_;
    }  
  }
  
  // Cutflow-Analysis
  /*
  std::ostringstream dbg;
  dbg << "selector decisions " << std::endl
      << "  name, 2xdecision, 2xcompl. decision, 2xcumul. decision" << std::endl;
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ) {
    std::string name = sequence_.selectorName(i);
    size_t idxFromName = sequence_.selectorIndex(name);
    dbg << " " << name
	<< " " << decisions.decision(i)
	<< " " << decisions.decision(idxFromName)
	<< " " << decisions.complementaryDecision(i)
	<< " " << decisions.complementaryDecision(idxFromName)
	<< " " << decisions.cumulativeDecision(i)
	<< " " << decisions.cumulativeDecision(idxFromName)
	<< std::endl;
  }
  dbg << " global decision = ("<<dec<<") " << decisions.globalDecision();
  std::cout << dbg.str() <<std::endl;

  std::ostringstream dbg1;
  std::vector<double> vars = sequence_.values();
  dbg1 << "All variable values" << std::endl;
  for ( size_t i=0; i<sequence_.variableNames().size(); ++i ) {
    dbg1 << sequence_.variableNames()[i] << " " << vars[i] << std::endl;
  }
  */
  return dec;

}

//________________________________________________________________________________________
// Called once per job, at start
void 
Selection::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
// Called once per job, at end

void 
Selection::endJob() 
{
  std::ostringstream dbg;
  dbg << "\nStatistical CutFlow Analysis\n"
      << "----------------------------\n";
  if (SelectionStats_.size()>0) {
    dbg << "Total events = "  << SelectionStats_[0].NTotal
        << ", average weight = " << SelectionStats_[0].NTotal/Nevents<<"\n";
  }  
  dbg << "  variable                cut (eff)             n-1 (eff)           cumulative (eff)";
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ) 
    dbg << "\n"
        << std::setw(20) << sequence_.selectorName(i)
        << "  " <<std::setw(10)<< SelectionStats_[i].NSelected << " (" << std::setw(10) << SelectionStats_[i].Eff()<<")"
        << "  " <<std::setw(10)<< SelectionStats_[i].Nminus1total << " (" <<std::setw(10) << SelectionStats_[i].EffNminus1()<<")"
        << "  " <<std::setw(10)<< SelectionStats_[i].NCumulative << " (" <<std::setw(10) << SelectionStats_[i].EffCumulative()<<")";
  //edm::LogVerbatim("Selection") << dbg.str() << std::endl;
  std::cout << dbg.str() << std::endl;
}


//define this as a plug-in
DEFINE_FWK_MODULE(Selection);
