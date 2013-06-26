// $Id: MainLoop.cc,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "MainLoop.h"
#include "Event.h"
#include "Selection.h"
#include "Analyzer.h"
#include "Factory.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <string>

//The main function 
int main(int argc, char* argv[])
{
  TCanvas c1("c1","c1",600,600);


  ///Factory approach ------------------------------------------------------------------------
  Factory thefactory;
  
  //Wjets sample
  std::vector<std::string> wjetsfiles;
  wjetsfiles.push_back("input/WJets_V01.00_tree.root");
  Sample wjet_sample(wjetsfiles,"susyTree","WjetsSelector","WJets","WJets",1);
  wjet_sample.SetFillColor(5);
  //thefactory.AddSample( &wjet_sample );
  
  //QCD sample
  std::vector<std::string> qcdfiles;
  qcdfiles.push_back("input/QCD_250_500_V01.00__7_tree.root");
  Sample qcd_sample(qcdfiles,"susyTree","QcdSelector","QCD","QCD",1);
  qcd_sample.SetFillColor(7);
  thefactory.AddSample( &qcd_sample );
  
  //Selections
  Selection * inclusive = new Selection("Inclusive");
  Selection * presel    = new PreSelection("PreSel");

  //Some Analyzer tasks ---
  //give 20 status updates, i.e. every 5%
  Analyzer* status = new PrintStatus(20); 
  thefactory.AddAnalyzer( inclusive, status ); 

  //1D plot for every sample
  TH1 * h_met = new TH1F("met",";Met [GeV]; events",100,0,500);
  Analyzer* plot_Met = new Plot1D(&c1,h_met,new FloatVariable("met"),new FloatArrayVariable("jet_pt",0));
  thefactory.AddAnalyzer( presel, plot_Met ); 

  //2D plot for every sample
  TH2 * h_met_vs_jet1pt = new TH2F("met_jet1pt",";Met [GeV]; Jet 1 p_{T}; events",100,0,500,100,0,500);
  Analyzer* plot_Met_vs_Jet1Pt = new Plot2D(&c1,h_met_vs_jet1pt,new FloatVariable("met"),new FloatArrayVariable("jet_pt",0));
  thefactory.AddAnalyzer( presel, plot_Met_vs_Jet1Pt ); 

  //1D stacked plot of met
  TH1 * h_met_templ = new TH1F("mettmpl",";Met [GeV]; events",100,0,1000);//template plot for style etc
  h_met_templ->SetStats(0);
  Analyzer* stacked_Met = new Stack(&c1,h_met_templ,new FloatVariable("met") );
  thefactory.AddAnalyzer( presel, stacked_Met ); 

  //Print some variables for all events to the shell
  //Analyzer* PrintVariables = new Print(new FloatVariable("met"),new FloatArrayVariable("jet_pt",0),new FloatArrayVariable("jet_pt",1));
  //thefactory.AddAnalyzer( 1, PrintVariables ); 

  //cutflow table
  CutFlow* cutflow = new CutFlow( inclusive );
  cutflow->AddSelection( presel );
  thefactory.AddAnalyzer( inclusive, cutflow ); 

  //process events
  thefactory.Process();


  return 0;
}
