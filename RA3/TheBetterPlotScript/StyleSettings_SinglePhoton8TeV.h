// $Id: StyleSettings.h,v 1.6 2012/06/26 08:57:31 auterman Exp $

#ifndef STYLE_SETTINGS_SINGLEPHOTON8TEV_H
#define STYLE_SETTINGS_SINGLEPHOTON8TEV_H

#include <iostream>

#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TGraph.h"

#include "StyleSettings.h"




style* GetSqGlBinoStyle8TeV(const std::string& njets=""){ /// Sq-Gl Bino /// ---------------------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "#splitline{GGM bino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.26;
  s->LegendMaxX=0.61;
  s->LegendMinY=0.26;
  s->LegendMaxY=0.45;
  s->lumi = new TLatex(0.58, 0.901, ((std::string)"4.03fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma"+njets).c_str());
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
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=0;
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

style* GetSqGlWinoStyle8TeV(const std::string& njets=""){ /// Sq-Gl Wino /// ---------------------------------------------------------------------
  style * s = new style();
  s->LegendTitel = "#splitline{GGM wino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.47;
  s->LegendMaxX=0.82;
  s->LegendMinY=0.64;
  s->LegendMaxY=0.85;
  s->lumi = new TLatex(0.58, 0.901, ((std::string)"4.03fb^{  -1}  #sqrt{s} = 8 TeV   #geq1#gamma"+njets).c_str());
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
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=0;
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


