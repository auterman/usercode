#include "event.h"
#include "ConfigFile.h"
#include "GeneratorMasses.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>


void ReadEvent(Event& evt, ConfigFile& config)
{
  //If no default value is specified here, and a data-card does not contain the requested variable, 
  //the event is skipped, after an error message [void ReadEvents(Events& evts, const std::string& filelist)].
  //                           <Variable Name>, <Name in Cfg File>
  evt.Add( ReadVariable(config, "gluino",      "gluino" ) );
  evt.Add( ReadVariable(config, "squark",      "squark" ) );
  evt.Add( ReadVariable(config, "chi1",        "chi1" ) );
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

  evt.Add( ReadVariable(config, "u_signal_scale", "signal.scale.uncertainty", 0 ) );
  evt.Add( ReadVariable(config, "u_signal_pdf",   "signal.PDF.uncertainty", 0 ) );

  if (1)
  for (int ch=0; ch<nchannels; ++ch) {
    std::stringstream ss;
    ss<<ch;
    std::string flag = ss.str(); 
    evt.Add( ReadVariable(config, "signal_"+flag,                  "signal_"+flag, 0 ) );

    evt.Add( ReadVariable(config, "signal_"+flag+"_contamination", "signal_contamination_"+flag, -1) ); 	       
    
    //normalize with signal:
    double signal = evt.Get("signal_"+flag);
    evt.Set( "signal_"+flag+"_contamination", 100.*evt.Get("signal_"+flag+"_contamination")/signal );
  }

}

void CalculateVariablesOnTheFly(Event& evt)
{
  evt.Add( Variable(0, new Info("ObsRtheoryM1","") ) );
  evt.Add( Variable(evt.Get("ObsR")*evt.Get("Xsection"), new Info("ObsXsecLimit","") ) );
  evt.Add( Variable(evt.Get("ExpR")*evt.Get("Xsection"), new Info("ExpXsecLimit","") ) );

  
  double scl = sqrt(pow(evt.Get("u_signal_scale"),2)+pow(evt.Get("u_signal_pdf"),2));
  evt.Add( Variable( evt.Get("ObsR")*(1.+scl), new Info("ObsRTheoM1","") ) );
  evt.Add( Variable( evt.Get("ObsR")*(1.-scl), new Info("ObsRTheoP1","") ) );
  evt.Add( Variable( evt.Get("ExpR")*(1.+scl), new Info("ExpRTheoM1","") ) );
  evt.Add( Variable( evt.Get("ExpR")*(1.-scl), new Info("ExpRTheoP1","") ) );

  evt.Add( Variable( evt.Get("ObsR")-evt.Get("ExpRM2"), new Info("ObsRmM2","") ) );
  evt.Add( Variable( evt.Get("ObsR")-evt.Get("ExpRP2"), new Info("ObsRmP2","") ) );
  evt.Add( Variable( (evt.Get("ExpRM2")!=0?evt.Get("ObsR")/evt.Get("ExpRM2"):0), new Info("ObsRdM2","") ) );
  evt.Add( Variable( (evt.Get("ExpRP2")!=0?evt.Get("ObsR")/evt.Get("ExpRP2"):0), new Info("ObsRdP2","") ) );

}

void AddGeneratorVariables(Event& evt, GeneratorMasses& p)
{
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



