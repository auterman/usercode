// $Id: Factory.cc,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/


#include "TChain.h"

#include "Factory.h"
#include "Analyzer.h"
#include "Selection.h"
#include "Event.h"


Event* Factory::NextEvent()
{
  Event *evt = new Event;
  int R;
  evt->SetRObject( cl_.Construct() );  
  evt->GetRObject().Invoke("Init", R, Args_ );
  tchain_->GetEntry(processed_);

  return evt;
}

void Factory::AddAnalyzer(Selection* sel, Analyzer* p){
  if (std::find(analyses_[sel].begin(),analyses_[sel].end(),p)==analyses_[sel].end())
    analyses_[sel].push_back(p);
}

void Factory::Process()
{
  //init analyzers
  for (std::map<Selection*,std::vector<Analyzer*> >::iterator it=analyses_.begin(); it!=analyses_.end(); ++it)
    for (std::vector<Analyzer*>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
      (*i)->InitJob();

  //read samples
  for (std::vector<Sample*>::iterator sample=samples_.begin(); sample!=samples_.end(); ++sample) {
    tchain_ = new TChain( (*sample)->GetTree().c_str() );
    for (std::vector<std::string>::const_iterator file=(*sample)->GetFiles()->begin(); file!=(*sample)->GetFiles()->end(); ++file)
	tchain_->Add( file->c_str() );

    cl_ = ROOT::Reflex::Type::ByName( (*sample)->GetSelector() );
    Args_.clear();
    Args_.push_back( (void*)(tchain_) );
    nentries_ = tchain_->GetEntries();
    processed_=0;
    (*sample)->SetNevents( nentries_ );

    //init analyzers for relevant selection and all samples
    for (std::map<Selection*,std::vector<Analyzer*> >::iterator it=analyses_.begin(); it!=analyses_.end(); ++it)
      for (std::vector<Analyzer*>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
        (*i)->InitSample(it->first, *sample);

    //process events through all analyzers
    for (; processed_ < nentries_; processed_++){
      Event * evt = NextEvent();
      evt->SetSample( *sample);
      for (std::map<Selection*,std::vector<Analyzer*> >::iterator it=analyses_.begin(); it!=analyses_.end(); ++it)
        if (it->first->Pass(evt))
	  for (std::vector<Analyzer*>::iterator i=it->second.begin(); i!=it->second.end(); ++i){
	    (*i)->Produce(evt);
	    (*i)->Process(evt);
	  }  
      
      delete evt;
    }
    
    //post-processing analyzers for sample
    for (std::map<Selection*,std::vector<Analyzer*> >::iterator it=analyses_.begin(); it!=analyses_.end(); ++it)
      for (std::vector<Analyzer*>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
        (*i)->EndSample();
      
     delete tchain_;
  }
  
  //post-processing analyzers for sample
  for (std::map<Selection*,std::vector<Analyzer*> >::iterator it=analyses_.begin(); it!=analyses_.end(); ++it)
    for (std::vector<Analyzer*>::iterator i=it->second.begin(); i!=it->second.end(); ++i)
      (*i)->EndJob();
 
}

