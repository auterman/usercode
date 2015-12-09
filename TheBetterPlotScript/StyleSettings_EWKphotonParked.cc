// $Id: StyleSettings_SinglePhoton_8TeV.cc,v 1.1 2012/06/29 20:27:21 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#include <iostream>

#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TPave.h"
#include "TLine.h"

#include "StyleSettings.h"
#include "OldExclusionContours.h"
#include "Overview.h"

void SetDefault(style * s=0)
{
  if (!s) return;
  if (s->leg) {
    s->leg->SetBorderSize(0);
    s->leg->SetLineColor(0);
    s->leg->SetFillColor(10);
    s->leg->SetFillStyle(1001);
    s->leg->SetTextFont(42);
    s->leg->SetTextSize(0.03);
  }
  if (s->excluded) {
    s->excluded->SetNDC(true);
    s->excluded->SetTextColor(1);
    s->excluded->SetTextFont(43);
    s->excluded->SetTextSize(25);
  }
  s->lumiTemperaturePlot = new TLatex(0.455, 0.912, "7.4 fb^{  -1}   (8 TeV)   #geq1 #gamma + MET");
  s->lumiTemperaturePlot->SetNDC(true);
  s->lumiTemperaturePlot->SetTextColor(1);
  s->lumiTemperaturePlot->SetTextFont(43);
  s->lumiTemperaturePlot->SetTextSize(25);
  s->cmsTemperaturePlot = new TLatex(0.19, 0.912, "#bf{CMS}");
  s->cmsTemperaturePlot->SetNDC(true);
  s->cmsTemperaturePlot->SetTextColor(1);
  s->cmsTemperaturePlot->SetTextFont(43);
  s->cmsTemperaturePlot->SetTextSize(35);

  s->lumiExclusionPlot = new TLatex(0.51, 0.902, "7.4 fb^{  -1}   (8 TeV)   #geq1 #gamma + MET");
  s->lumiExclusionPlot->SetNDC(true);
  s->lumiExclusionPlot->SetTextColor(1);
  s->lumiExclusionPlot->SetTextFont(43);
  s->lumiExclusionPlot->SetTextSize(25);
  s->cmsExclusionPlot = new TLatex(0.23, 0.84, "#bf{CMS}");
  s->cmsExclusionPlot->SetNDC(true);
  s->cmsExclusionPlot->SetTextColor(1);
  s->cmsExclusionPlot->SetTextFont(43);
  s->cmsExclusionPlot->SetTextSize(35);

  s->lumi      = new TLatex(0.55, 0.912, "7.4 fb^{  -1}   (8 TeV)   #geq1 #gamma + MET");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(1);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(25);
  s->cms       = new TLatex(0.2, 0.84, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(1);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(35);


}

style* SqGlBino_Style(){ /// Sq-Gl Bino /// ---------------------------------------------------------------------
  return 0;
}

style* SqGlWino_Style(){ /// Sq-Gl Wino /// ---------------------------------------------------------------------
  return 0;
}

style* SMST5gg_Style(){ /// Sq-Chi1  /// ---------------------------------------------------------------------
  return 0;
}

style* SMST5wg_Style(){ /// Sq-Chi1  /// ---------------------------------------------------------------------
  return 0;
}

style* GlBino_Style(){ 
  return 0;
}

style* GlWino_Style(){ 
  return 0;
}

/// Cha - Chi /// -----------------------------------------------------------------------
void DrawWinoBinoDiagonalCut(style * s=0, TLegend*l=0) {
    TGraph*cover = new TGraph(0);
    cover->SetPoint(0,   0,   0);
    cover->SetPoint(1, 1200, 1200);
    cover->SetPoint(2, 1200, 0);
    cover->SetPoint(3,   0,   0);
    cover->SetFillColor(kGray);

    TLatex tex;
    tex.SetTextSize(0.025);
    tex.SetTextFont(62);
    tex.SetNDC(true);

    TGraph * exp_7TeV_WinoBino = Diphoton_Exp_7TeV_WinoBino();
    //exp_7TeV_WinoBino->Draw("same");
    //if (l) l->AddEntry(exp_7TeV_WinoBino, "Exp. Classic 2-#gamma","l");

    //cover->Draw("f");
    //tex.DrawLatex(0.6, 0.25, "bino m_{#tilde{#chi}^{0}} > wino m_{#tilde{#chi}}");

    if (s->gCLsObsExcl) s->gCLsObsExcl->SetLineColor( kBlack );
    if (s->gCLsObsTheom1) s->gCLsObsTheom1->SetLineColor( kBlack );
    if (s->gCLsObsTheop1) s->gCLsObsTheop1->SetLineColor( kBlack );

    gPad->RedrawAxis();

}

style* WinoBino_Style(){ 
  style * s = new style();
  SetDefault(s);
  s->LimitFlavor = "asym";

  s->leg = new TLegend(0.47,0.17,0.79,0.33,"GGM    m_{#tilde{q}} =  m_{#tilde{g}} = 5 TeV");//new TLegend(0.21,0.68,0.6,0.88,"GGM    m_{#tilde{q}} =  m_{#tilde{g}} = 5 TeV");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.03);

  s->excluded = new TLatex(0.6, 0.4, "Excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->doSmooth=0;
  s->smooth_points=15;
  s->PostExclusionPlotting=DrawWinoBinoDiagonalCut;
  s->iCLsObsExcl=1;  
  s->iCLsExpExcl=1;  
  s->iCLsExpExclm1=1;
  s->iCLsExpExclp1=1;
  s->iCLsObsTheom1=1;
  s->iCLsObsTheop1=1;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  s->MinXsecZ=0.00091;
  s->MaxXsecZ=10;
  s->SetMoreLogLabels = false;

  return s;
}


void DrawFancyLegendSymbols(style * s=0, TLegend*leg=0) {

    double x=0.5175, y=0.76, w=0.067, h=0.04;
    TPave * b2 = new TPave(x-w/2.,y-h/2., x+w/2.,y+h/2., 0, "NDC,NB");
    b2->SetFillColor(kYellow);
    b2->Draw("same");
    TPave * b1 = new TPave(x-w/2.,y-h/3.5, x+w/2.,y+h/3.5, 0, "NDC,NB");
    b1->SetFillColor(kGreen);
    b1->Draw("same");
    TLine * l = new TLine();
    l->SetLineColor(kRed);
    l->SetLineStyle(kDashed);
    l->SetLineWidth(2);
    l->DrawLineNDC(x-w/2.,y,x+w/2.,y);

    double ty=0.808; 
    TLine * t = new TLine();
    t->SetLineColor(kBlack);
    t->SetLineStyle(kDotted);
    t->SetLineWidth(1);
    //t->DrawLineNDC(x-w/2.,ty-h/3.5,x+w/2.,ty-h/3.5);
    //t->DrawLineNDC(x-w/2.,ty+h/3.5,x+w/2.,ty+h/3.5);

    ty=0.714; 
    TLine * tx = new TLine();
    tx->SetLineColor(kBlue);
    tx->SetLineStyle(kSolid);
    tx->SetLineWidth(1);
    tx->DrawLineNDC(x-w/2.,ty-h/3.5,x+w/2.,ty-h/3.5);
    tx->DrawLineNDC(x-w/2.,ty+h/3.5,x+w/2.,ty+h/3.5);
}

style* WinoBino1D_Style(){ 
  style * s = new style();
  SetDefault(s);
  s->LimitFlavor = "asym";

  s->leg = new TLegend(0.47,0.69,0.85,0.88,"GGM    m_{Wino} =  m_{Bino} + 10 GeV");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.035);

  s->excluded = new TLatex(0.6, 0.4, "Excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->doSmooth=0;
  s->smooth_points=15;
  s->PostExclusionPlotting=DrawFancyLegendSymbols;
  s->iCLsObsExcl=1;  
  s->iCLsExpExcl=1;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=1;
  s->iCLsObsTheop1=1;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  s->MinXsecZ=0.001;
  s->MaxXsecZ=10;
  s->SetMoreLogLabels = false;

  return s;
}

void DrawFancyLegendSymbols_TChi(style * s=0, TLegend*leg=0) {

    double x=0.5418, y=0.76, w=0.058, h=0.04;
    TPave * b2 = new TPave(x-w/2.,y-h/2., x+w/2.,y+h/2., 0, "NDC,NB");
    b2->SetFillColor(kYellow);
    b2->Draw("same");
    TPave * b1 = new TPave(x-w/2.,y-h/3.5, x+w/2.,y+h/3.5, 0, "NDC,NB");
    b1->SetFillColor(kGreen);
    b1->Draw("same");
    TLine * l = new TLine();
    l->SetLineColor(kRed);
    l->SetLineStyle(kDashed);
    l->SetLineWidth(2);
    l->DrawLineNDC(x-w/2.,y,x+w/2.,y);

    double ty=0.808; 
    TLine * t = new TLine();
    t->SetLineColor(kBlack);
    t->SetLineStyle(kDotted);
    t->SetLineWidth(1);
    //t->DrawLineNDC(x-w/2.,ty-h/3.5,x+w/2.,ty-h/3.5);
    //t->DrawLineNDC(x-w/2.,ty+h/3.5,x+w/2.,ty+h/3.5);

    ty=0.714; 
    TLine * tx = new TLine();
    tx->SetLineColor(kBlue);
    tx->SetLineStyle(kSolid);
    tx->SetLineWidth(1);
    tx->DrawLineNDC(x-w/2.,ty-h/3.5,x+w/2.,ty-h/3.5);
    tx->DrawLineNDC(x-w/2.,ty+h/3.5,x+w/2.,ty+h/3.5);
}

style* SMS_TChiNg_Style(){ 
  style * s = new style();
  SetDefault(s);
  s->LimitFlavor = "asym";

  s->leg = new TLegend(0.5,0.69,0.83,0.88,"TChiNg");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.035);

  s->excluded = new TLatex(0.6, 0.4, "Excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->doSmooth=0;
  s->smooth_points=15;
  s->PostExclusionPlotting=DrawFancyLegendSymbols_TChi;
  s->iCLsObsExcl=1;  
  s->iCLsExpExcl=1;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=1;
  s->iCLsObsTheop1=1;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  s->MinXsecZ=0.001;
  s->MaxXsecZ=10;
  s->SetMoreLogLabels = false;

  return s;
}

style* SMS_TChiWg_Style(){ 
  style * s = new style();
  SetDefault(s);
  s->LimitFlavor = "asym";

  s->leg = new TLegend(0.5,0.69,0.83,0.88,"TChiWg");
  s->leg->SetBorderSize(0);
  s->leg->SetLineColor(0);
  s->leg->SetFillColor(10);
  s->leg->SetFillStyle(1001);
  s->leg->SetTextFont(42);
  s->leg->SetTextSize(0.035);

  s->excluded = new TLatex(0.6, 0.4, "Excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->doSmooth=0;
  s->smooth_points=15;
  s->PostExclusionPlotting=DrawFancyLegendSymbols_TChi;
  s->iCLsObsExcl=1;  
  s->iCLsExpExcl=1;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=1;
  s->iCLsObsTheop1=1;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

//  s->MinXsecZ=0.001;
//  s->MaxXsecZ=10;
  s->SetMoreLogLabels = false;

  return s;
}




style* SMST1gg_Style(){ 
  return 0;
}

  
style* SMST1lg_Style(){ 
  return 0;
}

