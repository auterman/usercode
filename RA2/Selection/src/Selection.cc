
#include "RA2/Selection/interface/Selection.h"

Selection::Selection(const edm::ParameterSet& iConfig) :
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  filterSelection_( iConfig.getParameter<std::vector<std::string> >("filterSelection") )
{
 
  // Translate filterSelection strings into selector indices
  filterSelectionIndices_.reserve(filterSelection_.size());
  for ( size_t i=0; i<filterSelection_.size(); ++i )
    filterSelectionIndices_.push_back(sequence_.selectorIndex(filterSelection_[i]));

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


  bool dec(true);
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ) // only "filter selection"
    dec = dec && decisions.decision(filterSelectionIndices_[i]);

//   // Get all variables from sequence
//   std::vector<double> values = sequence_.values();
//     edm::LogInfo("SusyRa2Variables") <<"values.size() "<<values.size();
//   for ( size_t i=0; i<values.size(); ++i ) {
//     edm::LogInfo("SusyRa2Variables") << values[i];
//   }

  
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
}


//define this as a plug-in
DEFINE_FWK_MODULE(Selection);
