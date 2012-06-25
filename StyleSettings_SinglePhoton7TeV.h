// $Id: StyleSettings.h,v 1.4 2012/06/25 07:11:41 auterman Exp $

#ifndef STYLE_SETTINGS_SINGLEPHOTON_7TEV_H
#define STYLE_SETTINGS_SINGLEPHOTON_7TEV_H

#include <iostream>

#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TGraph.h"

#include "StyleSettings.h"


style* GetSqGlBinoStyle(){ /// Sq-Gl Bino /// ---------------------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "#splitline{GGM bino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.26;
  s->LegendMaxX=0.61;
  s->LegendMinY=0.26;
  s->LegendMaxY=0.45;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded = new TLatex(0.3, 0.7, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_points=25;
  return s;
}

style* GetSqGlWinoStyle(){ /// Sq-Gl Wino /// ---------------------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "#splitline{GGM wino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.47;
  s->LegendMaxX=0.82;
  s->LegendMinY=0.64;
  s->LegendMaxY=0.85;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.2, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded = new TLatex(0.3, 0.3, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(25);
  s->smooth_points=25;
  
  return s;
}


/// ----------- Gl - Bino ------------ /// ----------------------------------------------------------------------------------

void Draw_GlBino_CoverUp() {
	TGraph*cover = new TGraph(0);
	cover->SetPoint(0, 200,  300);
	cover->SetPoint(1, 1200, 1210);
	cover->SetPoint(2, 1200, 1190);
	cover->SetPoint(3,    0,    0);
	cover->SetFillColor(kWhite);
	//cover->SetLineColor(kBlack);
	cover->Draw("f");

	TGraph*gluinoNLSP = new TGraph(0);
	gluinoNLSP->SetPoint(0, 50, 50);
	gluinoNLSP->SetPoint(1, 3000, 3000);
	gluinoNLSP->SetPoint(2, 3000, 50);
	gluinoNLSP->SetPoint(3, 50, 50);
	gluinoNLSP->SetFillColor(kGray);
	gluinoNLSP->Draw("f");

	TLatex tex;
	tex.SetTextSize(0.03);
	tex.SetTextFont(62);
	tex.SetNDC(true);
	tex.DrawLatex(0.6, 0.25, "#tilde{g} NLSP");
	gPad->RedrawAxis();
}


style* Get_GlBino_PlotStyle(){ 
  style * s = new style();
  s->LegendTitel = "#splitline{GGM bino-like #tilde{#chi}^{0}}{m_{#tilde{q}} = 2500 GeV}";
  s->LegendMinX=0.26;
  s->LegendMinY=0.68;
  s->LegendMaxX=0.69;
  s->LegendMaxY=0.85;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  
  s->lumiTemperaturePlot = new TLatex(0.48, 0.906, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumiTemperaturePlot->SetNDC(true);
  s->lumiTemperaturePlot->SetTextColor(12);
  s->lumiTemperaturePlot->SetTextFont(43);
  s->lumiTemperaturePlot->SetTextSize(20);
  s->cmsTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS}");
  s->cmsTemperaturePlot->SetNDC(true);
  s->cmsTemperaturePlot->SetTextColor(12);
  s->cmsTemperaturePlot->SetTextFont(43);
  s->cmsTemperaturePlot->SetTextSize(20);
  s->cmsprelimTemperaturePlot = new TLatex(0.21, 0.906, "#bf{CMS preliminary}");
  s->cmsprelimTemperaturePlot->SetNDC(true);
  s->cmsprelimTemperaturePlot->SetTextColor(12);
  s->cmsprelimTemperaturePlot->SetTextFont(43);
  s->cmsprelimTemperaturePlot->SetTextSize(20);

  s->excluded = new TLatex(0.3, 0.4, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->smooth_points=25;
  s->coverUp=Draw_GlBino_CoverUp;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;
  s->MinXsecZ=0.001;
  s->MaxXsecZ=0.5;

  return s;
}

/// ----------- Gl - Wino ------------ /// ----------------------------------------------------------------------------------

style* GetGlWinoPlotStyle(std::string neutralinotype, std::string squarkmass){ 
  style * s = new style();
  s->LegendTitel = "#splitline{GGM " + neutralinotype + "-like #tilde{#chi}^{0}}{m_{#tilde{q}} = "+squarkmass+" GeV}";
  s->LegendMinX=0.46;
  s->LegendMinY=0.64;
  s->LegendMaxX=0.84;
  s->LegendMaxY=0.84;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded = new TLatex(0.3, 0.4, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->smooth_points=15;
  s->coverUp=DrawNeutrNNLSPandDiagonalCut;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;

  return s;
}

style* GetWinoBinoPlotStyle(){ /// Cha - Chi /// -----------------------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "GGM    m_{#tilde{q}} =  m_{#tilde{g}} = 5 TeV";
  s->LegendMinX=0.4;
  s->LegendMinY=0.68;
  s->LegendMaxX=0.89;
  s->LegendMaxY=0.88;
  s->lumi = new TLatex(0.6, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded = new TLatex(0.6, 0.4, "excluded");
  s->excluded->SetNDC(true);
  s->excluded->SetTextColor(12);
  s->excluded->SetTextFont(43);
  s->excluded->SetTextSize(20);
  s->smooth_flag=2;
  s->smooth_points=15;
  s->coverUp=DrawWinoBinoDiagonalCut;
  s->iCLsObsExcl=1;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=1;
  s->iCLsObsTheop1=1;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  return s;
}

style* GetSMST1ggStyle(){ /// SMS T1 gg /// -------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "SMS #gamma#gamma";
  s->LegendMinX=0.31;
  s->LegendMinY=0.69;
  s->LegendMaxX=0.69;
  s->LegendMaxY=0.84;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded=0;
  s->smooth_flag=2;
  s->smooth_points=30;
  s->coverUp=DrawNeutrNNLSP;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  return s;
}
  
style* GetSMST1lgStyle(){ /// SMS T1 lg /// -------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "SMS #gamma + X";
  s->LegendMinX=0.31;
  s->LegendMinY=0.69;
  s->LegendMaxX=0.69;
  s->LegendMaxY=0.84;
  s->lumi = new TLatex(0.58, 0.901, "4.6fb^{  -1}  #sqrt{s} = 7 TeV   #geq1#gamma, #geq2 jets");
  s->lumi->SetNDC(true);
  s->lumi->SetTextColor(12);
  s->lumi->SetTextFont(43);
  s->lumi->SetTextSize(20);
  s->cms = new TLatex(0.21, 0.901, "#bf{CMS}");
  s->cms->SetNDC(true);
  s->cms->SetTextColor(12);
  s->cms->SetTextFont(43);
  s->cms->SetTextSize(20);
  s->cmsprelim = new TLatex(0.21, 0.901, "#bf{CMS preliminary}");
  s->cmsprelim->SetNDC(true);
  s->cmsprelim->SetTextColor(12);
  s->cmsprelim->SetTextFont(43);
  s->cmsprelim->SetTextSize(20);
  s->excluded=0;
  s->smooth_flag=2;
  s->smooth_points=50;
  s->coverUp=DrawNeutrNNLSP;
  s->iCLsObsExcl=0;  
  s->iCLsExpExcl=0;  
  s->iCLsExpExclm1=0;
  s->iCLsExpExclp1=0;
  s->iCLsObsTheom1=0;
  s->iCLsObsTheop1=0;
  s->iCLsExpTheom1=0;
  s->iCLsExpTheop1=0;  

  return s;
}

#endif


