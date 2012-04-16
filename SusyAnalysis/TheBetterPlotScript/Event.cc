#include "event.h"
#include "ConfigFile.h"
#include "GeneratorMasses.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


void ReadEvent(Event& evt, ConfigFile& config)
{
  //If no default value is specified here, and a data-card does not contain the requested variable, 
  //the event is skipped, after an error message [void ReadEvents(Events& evts, const std::string& filelist)].
  //                           <Variable Name>, <Name in Cfg File>
  evt.Add( ReadVariable(config, "Mzero",       "Mzero" ) );
  evt.Add( ReadVariable(config, "Mhalf",       "Mhalf" ) );
  evt.Add( ReadVariable(config, "TanBeta",     "TanBeta" ) );
  evt.Add( ReadVariable(config, "Xsection",    "Xsection.NLO" ) );
  evt.Add( ReadVariable(config, "Luminosity",  "Luminosity" ) );

  evt.Add( ReadVariable(config, "ObsRasym",    "CLs observed asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasym",    "CLs expected asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasymM1",  "CLs expected m1sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasymP1",  "CLs expected p1sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasymM2",  "CLs expected m2sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasymP2",  "CLs expected p2sigma asymptotic", -9999999 ) );
  //"Optional" variables with default values:
  evt.Add( ReadVariable(config, "ObsR",        "CLs observed", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpR",        "CLs expected", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM1",      "CLs expected m1sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP1",      "CLs expected p1sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM2",      "CLs expected m2sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP2",      "CLs expected p2sigma", -9999999 ) );

  if (1)
  for (int ch=0; ch<nchannels; ++ch) {
    std::stringstream ss;
    ss<<ch;
    std::string flag = ss.str(); 
    evt.Add( ReadVariable(config, "signal_"+flag,                  "signal_"+flag, -1 ) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_contamination", "signal_contamination_"+flag, -1) ); 	       
    evt.Add( ReadVariable(config, "signal_"+flag+"_stat_UP",	   "signal_"+flag+"_signal stat_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_stat_DN",	   "signal_"+flag+"_signal stat_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_syst_UP",	   "signal_"+flag+"_signal syst_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_syst_DN",	   "signal_"+flag+"_signal syst_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_AccPDF_UP",	   "signal_"+flag+"_signal syst acceptance PDF_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_AccPDF_DN",	   "signal_"+flag+"_signal syst acceptance PDF_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_ScaleUP",	   "signal_"+flag+"_signal syst kfactor_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_Scale_DN",	   "signal_"+flag+"_signal syst kfactor_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_XsecPDF_UP",	   "signal_"+flag+"_signal syst xSec PDF_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_XsecPDF_DN",	   "signal_"+flag+"_signal syst xSec PDF_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_trigger_UP",    "signal_"+flag+"_signal_trigger_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_trigger_DN",    "signal_"+flag+"_signal_trigger_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_JES_UP",        "signal_"+flag+"_JES_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_JES_DN",        "signal_"+flag+"_JES_DN", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_JER_UP",        "signal_"+flag+"_JER_UP", -1) );
    evt.Add( ReadVariable(config, "signal_"+flag+"_JER_DN",        "signal_"+flag+"_JER_DN", -1) );
    
    //normalize with signal:
    double signal = evt.Get("signal_"+flag);
    evt.Set( "signal_"+flag+"_contamination", 100.*evt.Get("signal_"+flag+"_contamination")/signal );
    evt.Set( "signal_"+flag+"_stat_UP",     100.*evt.Get("signal_"+flag+"_stat_UP")/signal );     
    evt.Set( "signal_"+flag+"_stat_DN",     100.*evt.Get("signal_"+flag+"_stat_DN")/signal );     
    evt.Set( "signal_"+flag+"_syst_UP",     100.*evt.Get("signal_"+flag+"_syst_UP")/signal );     
    evt.Set( "signal_"+flag+"_syst_DN",     100.*evt.Get("signal_"+flag+"_syst_DN")/signal );     
    evt.Set( "signal_"+flag+"_AccPDF_UP",   100.*evt.Get("signal_"+flag+"_AccPDF_UP")/signal );   
    evt.Set( "signal_"+flag+"_AccPDF_DN",   100.*evt.Get("signal_"+flag+"_AccPDF_DN")/signal );   
    evt.Set( "signal_"+flag+"_ScaleUP",     100.*evt.Get("signal_"+flag+"_ScaleUP")/signal );     
    evt.Set( "signal_"+flag+"_Scale_DN",    100.*evt.Get("signal_"+flag+"_Scale_DN")/signal );    
    evt.Set( "signal_"+flag+"_XsecPDF_UP",  100.*evt.Get("signal_"+flag+"_XsecPDF_UP")/signal );  
    evt.Set( "signal_"+flag+"_XsecPDF_DN",  100.*evt.Get("signal_"+flag+"_XsecPDF_DN")/signal );  
    evt.Set( "signal_"+flag+"_trigger_UP",  100.*evt.Get("signal_"+flag+"_trigger_UP")/signal );  
    evt.Set( "signal_"+flag+"_trigger_DN",  100.*evt.Get("signal_"+flag+"_trigger_DN")/signal );   
    evt.Set( "signal_"+flag+"_JES_UP",      100.*evt.Get("signal_"+flag+"_JES_UP")/signal );  
    evt.Set( "signal_"+flag+"_JES_DN",      100.*evt.Get("signal_"+flag+"_JES_DN")/signal );   
    evt.Set( "signal_"+flag+"_JER_UP",      100.*evt.Get("signal_"+flag+"_JER_UP")/signal );  
    evt.Set( "signal_"+flag+"_JER_DN",      100.*evt.Get("signal_"+flag+"_JER_DN")/signal );   
  }

}

void CalculateVariablesOnTheFly(Event& evt)
{
  evt.Add( Variable(0, new Info("ObsRtheoryM1","") ) );
  evt.Add( Variable(evt.Get("ObsR")*evt.Get("Xsection"), new Info("ObsXsecLimit","") ) );
  evt.Add( Variable(evt.Get("ExpR")*evt.Get("Xsection"), new Info("ExpXsecLimit","") ) );
  if (1)
  for (int ch=0; ch<nchannels; ++ch) {
     std::stringstream ss;
     ss<<ch;
     std::string flag = ss.str(); 
     double N = (evt.Get("Xsection")*evt.Get("Luminosity"));
     evt.Add( Variable( (N!=0?100. * evt.Get("signal_"+flag)/N:0), new Info("signal_"+flag+"_acceptance","") ) );

  }
}

void AddGeneratorVariables(Event& evt, GeneratorMasses& p)
{
  evt.Add( Variable(p.MZ1, new Info("MChi1","") ) );
  //evt.Add( Variable(p.MW1, new Info("MCha1","") ) );
  evt.Add( Variable(p.MGL, new Info("MGluino","") ) );
  evt.Add( Variable(4.0/(1./p.MUL + 1./p.MUR + 1./p.MDL + 1./p.MDR ), new Info("MSquark","") ) );
}

void ReadEvents(Events& evts, const std::string& filelist)
{
   std::cout << "...reading " << filelist<< std::flush;
   std::ifstream masses_file;
   masses_file.open(filelist.c_str());
   std::string file;
   while (1) {
      masses_file >> file;
      if (!masses_file.good()) break;
      ConfigFile config(file);
      Event evt;
      try {
        ReadEvent(evt, config);
        CalculateVariablesOnTheFly(evt);
      }
      catch(...) {
        std::cerr<<"Catched exception: skipping bad event!"<<std::endl;
	continue;
      }	
      evts.push_back(evt);
   }
   std::cout << ": "<< evts.size() << std::endl;
   //check
   masses_file.close();
}

//private lookup-table of class Event
std::map<const std::string, unsigned> Event::VariableIndex_;


void Event::Add(Variable var)
{
   vars_.push_back(var);
   if (VariableIndex_.find(var.GetInfo()->name)==VariableIndex_.end())
     VariableIndex_[var.GetInfo()->name] = vars_.size()-1;
}

void FindFirst(Events::iterator& first, Events::iterator& last, const std::string& var, const double value )
{
  for ( ;first!=last; first++) if ( first->Get(var)==value ) break;
}
void FindLast(Events::iterator first, Events::iterator& last, const std::string& var, const double value )
{
  for ( ;first<=last; first++) if ( first->Get(var)!=value ) break;
  last = first;
}

void GetEvents(Events::iterator& a, Events::iterator& b, const std::string& var, const double value )
{
  FindFirst(a, b, var, value);
  FindLast( a, b, var, value);
}

const Event Event::operator*(const double f) const {
	Event res(*this);
	for (std::vector<Variable>::iterator it=res.vars_.begin(); it != res.vars_.end(); ++it)
		it->SetValue( it->GetValue() * f);
	return res;
}

const Event Event::operator+(const Event& f) const {
	Event res(*this);
	std::vector<Variable>::iterator it = res.vars_.begin();
	std::vector<Variable>::const_iterator fi = f.vars_.begin();
	for (; it != res.vars_.end(); ++it, ++fi)
		it->SetValue( it->GetValue() + fi->GetValue() );
	return res;
}



