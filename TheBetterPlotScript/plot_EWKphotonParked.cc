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
    GetInfo("bino"  )->SetLabel("M_{bino} [GeV]");
    GetInfo("wino"  )->SetLabel("M_{wino} [GeV]");
    GetInfo("squark")->SetLabel("M_{squark} [GeV]");
    GetInfo("gluino")->SetLabel("M_{gluino} [GeV]");
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

    std::cout << "   plotting '"<< flag<<"'"<< std::endl;
    PlotTools * StdPlotTool = PlotTool->Clone();
    DrawStandardPlots(PlotTool->Clone(), flag, x,y, s, new_plot_range);
    //DrawStandardPlotsPerBin(PlotTool, "GMSB", x,y, &new_plot_range);
    DrawStandardLimitPlots(PlotTool->Clone(), flag, x,y, s, new_plot_range);

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
    DrawExclusion(StdPlotTool,PlotTool,flag,x,y,new_plot_range,new_plot_excl,s); //removes points, which have no limits and fills the gaps by interpolation
    //DrawExclusion(StdPlotTool,PlotTool,flag,x,y,new_plot_range,new_plot_excl,s,"asym"); //removes points, which have no limits and fills the gaps by interpolation
}

void Do1DPlotsFor(const std::string& x, const std::string& flag, const std::string& file, style*s, TH1*plot_range=0)  {
    PlotTools * PlotTool;
    GetPlotTools(PlotTool, file, x, "", "", 0);
    //Require an observed CLs limit:
    PlotTool->Remove("ObsR", Compare::less, 0.00001);
    PlotTool->Remove("Xsection", Compare::less, 0.0);

    GetInfo("bino"  )->SetLabel("M_{bino} [GeV]");
    GetInfo("wino"  )->SetLabel("M_{wino} [GeV]");
    GetInfo("Xsection")->SetLabel("signal cross section #sigma_{NLO} [pb]");
    GetInfo("ObsXsecLimit")->SetLabel("observed cross section limit [pb]");
    GetInfo("ExpXsecLimit")->SetLabel("expected cross section limit [pb]");
    TH1 * new_range = (plot_range?plot_range:PlotTool->GetHist1D(x));

    gStyle->SetPadRightMargin(0.1);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetOptStat(0);
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.5, "y");
    gStyle->SetNdivisions(505);
    gStyle->SetTitleFont(43, "xyz");
    gStyle->SetTitleSize(32, "xyz");
    //gStyle->SetLabelSize(0.03, "XYZ");

    std::cout << "   plotting 1D '"<< flag<<"'"<< std::endl;
    Draw1DLimitPlots(PlotTool->Clone(), flag, x, s, new_range);
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
    GetInfo("bino"  )->SetLabel("M_{bino} [GeV]");
    GetInfo("wino"  )->SetLabel("M_{wino} [GeV]");
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
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ObsXsecLimitasym", "Observed asympt. cross section limit [fb]");
        DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ExpXsecLimitasym", "Expected asympt. cross section limit [fb]");
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

  //GGM Wino 
  if (1) {
  DoPlotsFor("squark","gluino","GGM_Wino","2014-12-17-16-03-GGM_Wino/filelist.txt",SqGlWino_Style(),4);
  }

  if (0) { //Wino-Bino full scan
  DoPlotsFor("bino","wino","WinoBinoFullScan",   "2014-09-24-12-46-WinoBino_Scan2D/filelist.txt",WinoBino_Style(),1);

  Do1DPlotsFor("wino","WinoBino_anti_diag_390","2014-09-24-12-46-WinoBino_Scan2D/anti_diag_390.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_anti_diag_490","2014-09-24-12-46-WinoBino_Scan2D/anti_diag_490.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_bino_380","2014-09-24-12-46-WinoBino_Scan2D/bino_380.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_bino_480","2014-09-24-12-46-WinoBino_Scan2D/bino_480.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_bino_580","2014-09-24-12-46-WinoBino_Scan2D/bino_580.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_dia_10","2014-09-24-12-46-WinoBino_Scan2D/diag_10.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_dia_60","2014-09-24-12-46-WinoBino_Scan2D/diag_60.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBino_dia_110","2014-09-24-12-46-WinoBino_Scan2D/dia_110.txt",WinoBino_Style()); 
  Do1DPlotsFor("bino","WinoBino_wino_390","2014-09-24-12-46-WinoBino_Scan2D/wino_390.txt",WinoBino_Style()); 
  Do1DPlotsFor("bino","WinoBino_wino_490","2014-09-24-12-46-WinoBino_Scan2D/wino_490.txt",WinoBino_Style()); 
  Do1DPlotsFor("bino","WinoBino_wino_590","2014-09-24-12-46-WinoBino_Scan2D/wino_590.txt",WinoBino_Style()); 

  Do1DPlotsFor("wino","WinoBinoSigma_10","2014-08-15-14-39-WinoBino_NewSigma/filelist_1D.txt",WinoBino_Style()); 
  }

  //Johannes first limits
  if (0){
  DoPlotsFor("bino","wino","WinoBinoMT",   "2014-08-10-03-09-WinoBino_MT/filelist.txt",WinoBino_Style(),1);
 
  Do1DPlotsFor("wino","WinoBinoMT_10",   "2014-08-10-03-09-WinoBino_MT/filelist_1D.txt",WinoBino_Style());
 
  DoPlotsFor("bino","wino","WinoBinoSigma","2014-08-15-14-39-WinoBino_NewSigma/filelist.txt",WinoBino_Style(),1);
  Do1DPlotsFor("wino","WinoBinoSigma_10","2014-08-15-14-39-WinoBino_NewSigma/filelist_1D.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBinoSigma_35","2014-08-15-14-39-WinoBino_NewSigma/filelist_35_1D.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBinoSigma_60","2014-08-15-14-39-WinoBino_NewSigma/filelist_60_1D.txt",WinoBino_Style()); 

  DoPlotsFor("bino","wino","WinoBinoMT","2014-09-14-14-14-WinoBino_MT/filelist.txt",WinoBino_Style(),1);
  Do1DPlotsFor("wino","WinoBinoMT_10","2014-09-14-14-14-WinoBino_MT/filelist_1D.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBinoMT_35","2014-09-14-14-14-WinoBino_MT/filelist_35_1D.txt",WinoBino_Style()); 
  Do1DPlotsFor("wino","WinoBinoMT_60","2014-09-14-14-14-WinoBino_MT/filelist_60_1D.txt",WinoBino_Style()); 
  }

  if (0){
  DoPlotsFor("bino","wino","WinoBino","2014-07-29-14-44-WinoBino/filelist.txt",WinoBino_Style(),1);
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
