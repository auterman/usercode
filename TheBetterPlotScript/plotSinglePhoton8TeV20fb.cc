// $Id: plotSinglePhoton8TeV.cc,v 1.13 2012/06/26 13:58:23 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "plot.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
//#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TPaveText.h"
#include "TGraph.h"
#include "TError.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>

#include "StyleSettings.h"


void AddEvents(PlotTools*& plotTools, std::string filename, std::string GeneratorFile)
{
    Events * additionalEvents = new Events;
    ReadEvents(*additionalEvents, filename);
    plotTools->addEvents( additionalEvents );
    if (GeneratorFile!="") {
        std::vector<GeneratorMasses> GenMasses;
        ReadGeneratorMasses(GenMasses, GeneratorFile);
        Match( GenMasses, *additionalEvents);
    }
    delete additionalEvents;
}

void DoPlotsFor(const std::string& x, const std::string& y, const std::string& flag, const std::string& file, style*s, int factor=0, TH2*plot_range=0, TH2*plot_excl=0)  {
    PlotTools * PlotTool;
    GetPlotTools(PlotTool, file, x, y, "", 0);
    if (x=="squark"    ||y=="squark")     GetInfo("squark")->SetLabel("M_{squark} [GeV]");
    if (x=="gluino"    ||y=="gluino")     GetInfo("gluino")->SetLabel("M_{gluino} [GeV]");
    if (x=="chi1"      ||y=="chi1")       GetInfo("chi1"  )->SetLabel("M_{bino} [GeV]");
    if (x=="neutralino"||y=="neutralino") GetInfo("neutralino"  )->SetLabel("M_{neutralino} [GeV]");
    if (x=="cha1"      ||y=="cha1")       GetInfo("cha1"  )->SetLabel("M_{wino} [GeV]");
    TH2 * new_plot_range = (plot_range?plot_range:PlotTool->GetHist(x,y));
    TH2 * new_plot_excl  = (plot_excl ?plot_excl :new_plot_range);

    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gStyle->SetPadRightMargin(0.2);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.5, "y");
    gStyle->SetTitleOffset(1.2, "z");
    gStyle->SetNdivisions(505);
    gStyle->SetTitleFont(43, "xyz");
    gStyle->SetTitleSize(32, "xyz");
    //gStyle->SetLabelSize(0.03, "XYZ");


    //Require an observed CLs limit:
    PlotTool->Remove("ObsR", Compare::less, 0.00001);
    PlotTool->Remove("Xsection", Compare::less, 0.0);

    //Show what points are available before filling the holes, by plotting the acceptance
    gStyle->SetTitleFont(43, "xyz");
    gStyle->SetTitleSize(32, "xyz");
    TCanvas* c4    = new TCanvas("c4", "c4", 900, 800);
    DrawPlot2D(PlotTool,c4,new_plot_range,flag,x,y,"Acceptance_ni", "Acceptance (no interpolation)", s->MinAccZ, s->MaxAccZ, s );

    //Fill the holes by 2D interpolation in gl-sq
    PlotTool->FillEmptyPointsByInterpolation(x, y);


    PlotTools * StdPlotTool = PlotTool->Clone();
    DrawStandardPlots(StdPlotTool, flag, x,y, s, new_plot_range);
    //DrawStandardPlotsPerBin(PlotTool, "GMSB", x,y, &new_plot_range);
    DrawStandardLimitPlots(StdPlotTool, flag, x,y, s, new_plot_range);

    for (int i=2; i<=factor; i*=2)
        PlotTool->ExpandGrid(x, y);

    new_plot_range = (plot_range?plot_range:PlotTool->GetHist(x,y));
    new_plot_excl  = (plot_excl?plot_excl:new_plot_range);
    /*
        if (plot_excl == plot_range) {
          delete plot_range;
          plot_range = PlotTool->GetHist(x,y);
          plot_excl = plot_range;
        } else {
          delete plot_range;
          plot_range = PlotTool->GetHist(x,y);
        }
    */
    DrawExclusion(StdPlotTool, PlotTool,flag,x,y,new_plot_range,new_plot_excl,s); //removes points, which have no limits and fills the gaps by interpolation
    //DrawExclusion(PlotTool,flag,x,y,new_plot_range,new_plot_excl,s,"asym"); //removes points, which have no limits and fills the gaps by interpolation
}

void PlotAll(std::vector<LimitGraphs*>& lg, const std::string& flag, const std::string& limit, TH2*h=0)
{
    if (!h) h = (TH2F*)lg.front()->GetHist()->Clone();
    h->Draw();
    TLegend* leg = new TLegend(0.55,0.52,0.88,0.83,NULL,"brNDC");
    leg->SetFillColor(0);
    leg->SetShadowColor(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.025);
    leg->SetBorderSize(1);
    leg->SetHeader("CMS, L_{int} = 4.7 fb^{-1}, #sqrt{s} = 7 TeV");
    for (std::vector<LimitGraphs*>::iterator it=lg.begin(); it!=lg.end(); ++it) {
        TGraph * g = (*it)->Limit(limit);
        g->Draw("l");
        leg->AddEntry(g,(*it)->Name().c_str(),"l");
    }
    leg->Draw();
    gPad->RedrawAxis();
    std::cout<<c1<<"  "<<flag<<"  "<<limit<<std::endl;
    c1->SaveAs(("results/"+flag +"_"+limit+".pdf").c_str());
    if (plotPNG) c1->SaveAs(("results/"+flag+"_"+limit+".png").c_str());
}


void MultipleChannels(const std::string& x, const std::string& y, const std::string& flag, const std::string& dir, TH2*plot_range=0)
{
    //read all channels
    std::vector<LimitGraphs*> lg;
    lg.push_back(new LimitGraphs(dir+"/filelist.txt",    "", 1, x, y, "combined", 15, 1, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch0.txt","", 1, x, y, "Bin 0",    15, 2, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch1.txt","", 1, x, y, "Bin 1",    15, 3, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch2.txt","", 1, x, y, "Bin 2",    15, 4, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch3.txt","", 1, x, y, "Bin 3",    15, 5, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch4.txt","", 1, x, y, "Bin 4",    15, 6, 0, 0, 0, 0, plot_range) );
    lg.push_back(new LimitGraphs(dir+"/filelist_ch5.txt","", 1, x, y, "Bin 5",    15, 7, 0, 0, 0, 0, plot_range) );

    //define plot range and labels
    GetInfo("squark")->SetLabel("M_{squark} [GeV]");
    GetInfo("gluino")->SetLabel("M_{gluino} [GeV]");
    GetInfo("chi1"  )->SetLabel("M_{bino} [GeV]");
    GetInfo("neutralino"  )->SetLabel("M_{neutralino} [GeV]");
    GetInfo("cha1"  )->SetLabel("M_{wino} [GeV]");
    if (!plot_range) plot_range = (TH2F*)lg.front()->GetHist()->Clone();
    plot_range->GetXaxis()->SetTitle(GetInfo(x)->GetLabel().c_str());
    plot_range->GetYaxis()->SetTitle(GetInfo(y)->GetLabel().c_str());

    //plotting
    c1->cd();
    PlotAll(lg,flag,x+"_"+y+"_allObsAsym",plot_range);
    PlotAll(lg,flag,x+"_"+y+"_allExpAsym",plot_range);
    c1->SetRightMargin(0.2);

    for (std::vector<LimitGraphs*>::iterator it=lg.begin(); it!=lg.end(); ++it) {
        std::string l=flag+"_"+(*it)->Name();
        std::replace(l.begin(),l.end(),' ','_');
        c1->SetLogz(0);
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"AcceptanceCorrected", "Acceptance (corrected for signal cont.)");
        c1->SetLogz(1);
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ObsXsecLimitasym", "Observed asympt. cross section limit [pb]");
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ExpXsecLimitasym", "Expected asympt. cross section limit [pb]");
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ObsRasym", "Asymptotic Observed R ");
        c1->cd();
        //In/Out plot
        {
            TH2F *hplot = (TH2F*)(*it)->GetHist()->Clone();
            hplot->GetZaxis()->SetTitle("Observed in/out");
            InOutPlot((*it)->GetPlot(),c1,l,x,y,"ObsRasym",hplot);
        }
        {
            TH2F *hplot = (TH2F*)(*it)->GetHist()->Clone();
            hplot->GetZaxis()->SetTitle("Expected in/out");
            InOutPlot((*it)->GetPlot(),c1,l,x,y,"ExpRasym",hplot);
        }
    }
}


int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.18);

  plotPNG    = false;
  plotC	     = false;
  plotROOT   = false;
  
  TCanvas* c_square    = new TCanvas("c_squ2", "c_squ2", 900, 800);
  c1 = c_square;
  c1->cd();

  Overview = new OverviewTable();

  //post July bug-fixes II + Zll->Zincl
  if (1){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-08-06-12-34-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-08-06-12-39-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5wg","2014-08-06-13-34-SMS_T5wg/filelist.txt",SMST5wg_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5gg","2014-08-06-13-34-SMS_T5gg/filelist.txt",SMST5gg_Style(),4);
  }

  //post July bug-fixes II
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-08-04-19-30-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-08-04-19-29-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5wg","2014-08-05-13-41-SMS_T5wg/filelist.txt",SMST5wg_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5gg","2014-08-04-19-30-SMS_T5gg/filelist.txt",SMST5gg_Style(),4);
  }

  //post July bug-fixes
  if (0){
  //DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-07-22-14-53-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  //DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-07-22-14-53-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5wg","2014-07-22-14-55-SMS_T5wg/filelist.txt",SMST5wg_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5gg","2014-07-22-14-55-SMS_T5gg/filelist.txt",SMST5gg_Style(),4);
  }

  //post ARC green light
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-07-16-17-02-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-07-16-17-02-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  }

  //SMS T5 scans
  if (0){
  DoPlotsFor("gluino","chi1","SMS_T5wg","2014-07-15-18-45-SMS_T5wg/filelist.txt",SMST5wg_Style(),4);
  DoPlotsFor("gluino","chi1","SMS_T5gg","2014-07-15-18-45-SMS_T5gg/filelist.txt",SMST5gg_Style(),4);
  }

  //post preapproval
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-05-27-18-34-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-05-27-18-33-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  }

  //post preapproval Single-Channels
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin5","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin5.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin4","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin4.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin3","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin3.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin2","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin2.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin1","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin1.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino_bin0","2014-06-04-10-21-GMSB_SqGl_met-Bino_SingleChannels/filelist_bin0.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin5","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin5.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin4","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin4.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin3","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin3.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin2","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin2.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin1","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin1.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino_bin0","2014-06-04-10-29-GMSB_SqGl_met-Wino_SingleChannels/filelist_bin0.txt",SqGlWino_Style(),4);
  }


  //Preapproval
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-05-10-16-46-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-05-10-16-46-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  }
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_metLO_Bino","2014-05-10-16-46-GMSB_SqGl_met-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metLO_Wino","2014-05-10-16-46-GMSB_SqGl_met-Wino/filelist.txt",SqGlWino_Style(),4);
  }
  
  //2.5.2014
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Bino","2014-05-10-16-47-GMSB_SqGl_metLO-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_met_Wino","2014-05-10-16-47-GMSB_SqGl_metLO-Wino/filelist.txt",SqGlWino_Style(),4);
  }
  
  if (0){
  DoPlotsFor("squark","gluino","GMSB_8TeV_metnew_Bino","2014-05-04-13-29-GMSB_SqGl_metnew-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metnew_Wino","2014-05-04-13-29-GMSB_SqGl_metnew-Wino/filelist.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metfibo_Bino","2014-05-04-13-30-GMSB_SqGl_metfibo-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metfibo_Wino","2014-05-04-13-30-GMSB_SqGl_metfibo-Wino/filelist.txt",SqGlWino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metoptim_Bino","2014-05-04-13-20-GMSB_SqGl_metoptim-Bino/filelist.txt",SqGlBino_Style(),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_metoptim_Wino","2014-05-04-13-28-GMSB_SqGl_metoptim-Wino/filelist.txt",SqGlWino_Style(),4);
  }
    
  std::ofstream ofile;
  ofile.open("results/OverviewTable.txt");
  ofile << *Overview->Get() << std::endl;
  ofile.close();
  
  return 0;
}

int main(int argc, char** argv) {
    gErrorIgnoreLevel = 5000;
    return plot(argc, argv);
}
