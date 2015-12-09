// $Id: Event.cc,v 1.11 2012/06/26 13:58:23 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "Event.h"
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

  evt.Add( ReadVariable(config, "number",      "Point", -1 ) );    
  evt.Add( ReadVariable(config, "wino",        "wino mass" ) );
  evt.Add( ReadVariable(config, "bino",        "bino mass" ) );
  evt.Add( ReadVariable(config, "higgsino",    "higgsino mass", -1 ) );
  evt.Add( ReadVariable(config, "chargino",    "chargino mass", -1 ) );
  evt.Add( ReadVariable(config, "gluino",      "gluino mass" ) );
  evt.Add( ReadVariable(config, "squark",      "squark mass" ) );
  evt.Add( ReadVariable(config, "Xsection",    "Xsection.NLO" ) );
  evt.Add( ReadVariable(config, "Lumi",        "Lumi" ) );
  
  
  evt.Add( ReadVariable(config, "bin0_signal",      "bin0_signal") );
  evt.Add( ReadVariable(config, "bin1_signal",      "bin1_signal") );
  evt.Add( ReadVariable(config, "bin2_signal",      "bin2_signal") );
  evt.Add( ReadVariable(config, "bin3_signal",      "bin3_signal") );
  evt.Add( ReadVariable(config, "contamination","signal.contamination", 0 ) );
  evt.Add( ReadVariable(config, "R_firstguess","R_firstguess" ) );
  evt.Add( ReadVariable(config, "u_signal_theory","signal u_PDF_xsec", 0 ) );
  //evt.Add( ReadVariable(config, "signal","signal", 0 ) );
  
  evt.Add( ReadVariable(config, "ObsRasym",    "CLs observed asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRasym",    "CLs expected asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM1asym",  "CLs expected m1sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP1asym",  "CLs expected p1sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM2asym",  "CLs expected m2sigma asymptotic", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP2asym",  "CLs expected p2sigma asymptotic", -9999999 ) );

  //"Optional" variables with default values:
  evt.Add( ReadVariable(config, "ObsR",        "CLs observed",         -9999999 ) );
  evt.Add( ReadVariable(config, "ExpR",        "CLs expected",         -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM1",      "CLs expected m1sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP1",      "CLs expected p1sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRM2",      "CLs expected m2sigma", -9999999 ) );
  evt.Add( ReadVariable(config, "ExpRP2",      "CLs expected p2sigma", -9999999 ) );

  //If the signal strength R>=20 the full CLs limits are cut off - use asymptotic in this case
  if ( fabs(evt.Get("ObsR"))<0.000001 || fabs(evt.Get("ExpR"))<0.000001 ||
       fabs(evt.Get("ObsR")-20)<0.1   || fabs(evt.Get("ExpR")-20)<0.1)
   {
      evt.Set("ObsR",   evt.Get("ObsRasym"));
      evt.Set("ExpR",   evt.Get("ExpRasym"));
      evt.Set("ExpRM1", evt.Get("ExpRM1asym"));
      evt.Set("ExpRM2", evt.Get("ExpRM2asym"));
      evt.Set("ExpRP1", evt.Get("ExpRP1asym"));
      evt.Set("ExpRP2", evt.Get("ExpRP2asym"));
   }

}

void CalculateVariablesOnTheFly(Event& evt)
{
  evt.Add( Variable(evt.Get("bin0_signal")+evt.Get("bin1_signal")+evt.Get("bin2_signal")+evt.Get("bin3_signal"), new Info("signal",   "") ) );

  evt.Add( Variable(evt.Get("signal")/(evt.Get("Xsection")*evt.Get("Lumi")), new Info("Acceptance",   "") ) );
  evt.Add( Variable(evt.Get("signal")/(evt.Get("Xsection")*evt.Get("Lumi")), new Info("Acceptance_ni",   "") ) );


  evt.Add( Variable(0, new Info("ObsRtheoryM1","") ) );
  evt.Add( Variable(evt.Get("ObsR")*evt.Get("Xsection"), new Info("ObsXsecLimit","") ) );
  evt.Add( Variable(evt.Get("ExpR")*evt.Get("Xsection"), new Info("ExpXsecLimit","") ) );
  evt.Add( Variable(evt.Get("ExpRM1")*evt.Get("Xsection"), new Info("ExpXsecLimitM1","") ) );
  evt.Add( Variable(evt.Get("ExpRP1")*evt.Get("Xsection"), new Info("ExpXsecLimitP1","") ) );
  evt.Add( Variable(evt.Get("ExpRM2")*evt.Get("Xsection"), new Info("ExpXsecLimitM2","") ) );
  evt.Add( Variable(evt.Get("ExpRP2")*evt.Get("Xsection"), new Info("ExpXsecLimitP2","") ) );
  evt.Add( Variable(evt.Get("ObsRasym")*evt.Get("Xsection"), new Info("ObsXsecLimitasym","") ) );
  evt.Add( Variable(evt.Get("ExpRasym")*evt.Get("Xsection"), new Info("ExpXsecLimitasym","") ) );
  evt.Add( Variable(evt.Get("ExpRM1asym")*evt.Get("Xsection"), new Info("ExpXsecLimitM1asym","") ) );
  evt.Add( Variable(evt.Get("ExpRP1asym")*evt.Get("Xsection"), new Info("ExpXsecLimitP1asym","") ) );
  evt.Add( Variable(evt.Get("ExpRM2asym")*evt.Get("Xsection"), new Info("ExpXsecLimitM2asym","") ) );
  evt.Add( Variable(evt.Get("ExpRP2asym")*evt.Get("Xsection"), new Info("ExpXsecLimitP2asym","") ) );
  evt.Add( Variable(100.*(evt.Get("signal")-evt.Get("contamination"))/(evt.Get("Xsection")*evt.Get("Luminosity")), new Info("AcceptanceCorrected","") ) );
  evt.Add( Variable(evt.Get("signal")/(evt.Get("Xsection")*evt.Get("Luminosity")), new Info("AcceptanceCalc","") ) );
  evt.Add( Variable(100*evt.Get("Acceptance"), new Info("AcceptancePercent","") ) );
  evt.Add( Variable(100.*evt.Get("contamination")/evt.Get("signal"), new Info("ContaminationRelToSignal","") ) );

  double scl = fabs(evt.Get("u_signal_theory"))/evt.Get("signal");
  double scl_up = scl, scl_dn = scl;
  evt.Add( Variable( evt.Get("ObsR")*(1.+scl_up), new Info("ObsRTheoM1","") ) );
  evt.Add( Variable( evt.Get("ObsR")*(1.-scl_dn), new Info("ObsRTheoP1","") ) );
  evt.Add( Variable( evt.Get("ExpR")*(1.+scl_up), new Info("ExpRTheoM1","") ) );
  evt.Add( Variable( evt.Get("ExpR")*(1.-scl_dn), new Info("ExpRTheoP1","") ) );
  evt.Add( Variable( evt.Get("Xsection")*(1.+scl_up), new Info("XsectionTheoM1","") ) );
  evt.Add( Variable( evt.Get("Xsection")*(1.-scl_dn), new Info("XsectionTheoP1","") ) );

  evt.Add( Variable( evt.Get("ObsR")-evt.Get("ExpRM2"), new Info("ObsRmM2","") ) );
  evt.Add( Variable( evt.Get("ObsR")-evt.Get("ExpRP2"), new Info("ObsRmP2","") ) );
  evt.Add( Variable( (evt.Get("ExpRM2")!=0?evt.Get("ObsR")/evt.Get("ExpRM2"):0), new Info("ObsRdM2","") ) );
  evt.Add( Variable( (evt.Get("ExpRP2")!=0?evt.Get("ObsR")/evt.Get("ExpRP2"):0), new Info("ObsRdP2","") ) );

  evt.Add( Variable(evt.Get("ObsRTheoM1")*evt.Get("Xsection"), new Info("ObsXsecLimitM1","") ) );
  evt.Add( Variable(evt.Get("ObsRTheoP1")*evt.Get("Xsection"), new Info("ObsXsecLimitP1","") ) );

  evt.Add( Variable( evt.Get("ObsRasym")*(1.+scl_up), new Info("ObsRTheoM1asym","") ) );
  evt.Add( Variable( evt.Get("ObsRasym")*(1.-scl_dn), new Info("ObsRTheoP1asym","") ) );
  evt.Add( Variable(evt.Get("ObsRTheoM1asym")*evt.Get("Xsection"), new Info("ObsXsecLimitM1asym","") ) );
  evt.Add( Variable(evt.Get("ObsRTheoP1asym")*evt.Get("Xsection"), new Info("ObsXsecLimitP1asym","") ) );

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

            ///@@Quick and dirty fix of the binning problem in the '2012-06-22-09-36-GMSB_gBino_7TeV_2j/filelist.txt' scan:
            if ( filelist=="2012-06-22-09-36-GMSB_gBino_7TeV_2j/filelist.txt" && evt.Get("gluino") < 1020.)
                evt.Set("gluino", evt.Get("gluino") - 10.0);
            ///@@The scan should be regenerated!

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

Event * Event::Clone()
{
    return new Event(this);
}

void Event::Add(Variable var)
{
    vars_.push_back(var);
    if (VariableIndex_.find(var.GetInfo()->name)==VariableIndex_.end())
        VariableIndex_[var.GetInfo()->name] = vars_.size()-1;
}

void FindFirst(Events::iterator& first, Events::iterator& last, const std::string& var, const double value )
{
    for ( ; first!=last; first++) if ( first->Get(var)==value ) break;
}
void FindLast(Events::iterator first, Events::iterator& last, const std::string& var, const double value )
{
    for ( ; first<=last; first++) if ( first->Get(var)!=value ) break;
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



