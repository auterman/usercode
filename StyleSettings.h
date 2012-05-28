// $Id: StyleSettings.h,v 1.1 2012/05/09 15:43:16 auterman Exp $

#ifndef STYLE_SETTINGS_H
#define STYLE_SETTINGS_H

#include <iostream>

#include "TString.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TGraph.h"



struct style{
  std::string LegendTitel;
  float LegendMinX, LegendMaxX, LegendMinY, LegendMaxY;
  TLatex * cms, *cmsprelim, *lumi, *excluded;
  int smooth_flag;
  int smooth_points;
  void (*coverUp)();
};

void DrawNeutrNNLSP() {
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

style* GetSqGlBinoStyle(){
  style * s = new style();
  s->LegendTitel = "#splitline{GGM bino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.26;
  s->LegendMaxX=0.5;
  s->LegendMinY=0.26;
  s->LegendMaxY=0.5;
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
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=0;
  
  return s;
}

style* GetSqGlWinoStyle(){
  style * s = new style();
  s->LegendTitel = "#splitline{GGM wino-like #tilde{#chi}^{0}}{m_{#tilde{#chi}^{0}} = 375 GeV}";
  s->LegendMinX=0.5;
  s->LegendMaxX=0.85;
  s->LegendMinY=0.5;
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
  s->smooth_flag=0;
  s->smooth_points=25;
  s->coverUp=0;
  
  return s;
}

style* GetGlChiPlotStyle(std::string neutralinotype, std::string squarkmass){
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
  s->coverUp=DrawNeutrNNLSP;

  return s;
}

style* GetSMSPlotStyle(std::string title){
  style * s = new style();
  s->LegendTitel = "SMS "+title;
  s->LegendMinX=0.46;
  s->LegendMinY=0.69;
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
  s->excluded=0;
  s->smooth_flag=2;
  s->smooth_points=15;
  s->coverUp=DrawNeutrNNLSP;

  return s;
}
  
  
namespace util {

  //!  Encapsulates different pad and histogram styles
  //!
  //!  \author   Matthias Schroeder (www.desy.de/~matsch)
  //!  \date     2010/03/09
  //!  $Id: StyleSettings.h,v 1.1 2012/05/09 15:43:16 auterman Exp $
  // -------------------------------------------------------------------------------------
  class StyleSettings {
  public:
    enum Style { Screen, Presentation, Paper };
    
    static Style style() {
      Style st = Screen;
      TString mode = gStyle->GetTitle();
      if( mode == "Presentation" ) st = Presentation;
      else if( mode == "Paper" ) st = Paper;
      return st;
    }
    static void screen() { setStyle("Screen",true); }
    static void screenNoTitle() { setStyle("Screen",false); }
    static void paper() { setStyle("Paper",true); }
    static void paperNoTitle() { setStyle("Paper",false); }
    static void presentation() { setStyle("Presentation",true); }
    static void presentationNoTitle() { setStyle("Presentation",false); }
    static void cms() { setStyle("CMS",false); }
    static int color(int i) {
      int col[5] = { 1, 2, 4, 7, 8 };
      return (i>=0 && i<5) ? col[i] : 1;
    }
    static int lineWidth() {
      int width = 1;
      TString mode = "Presentation";
      if( mode.CompareTo(gStyle->GetTitle()) == 0 ) {
	width = 2;
      }
      return width;
    }
    
  private:
    static void setStyle(const TString &mode, bool spaceForTitle) {
      // Set title of current style object
      gStyle->SetTitle(mode);

      // Zero horizontal error bars
      gStyle->SetErrorX(0);

      //  For 'colz' TH2
      gStyle->SetPalette(1);
    
      //  For the canvas
      gStyle->SetCanvasBorderMode(0);
      gStyle->SetCanvasColor(kWhite);
      gStyle->SetCanvasDefH(800); //Height of canvas
      gStyle->SetCanvasDefW(800); //Width of canvas
      gStyle->SetCanvasDefX(0);   //Position on screen
      gStyle->SetCanvasDefY(0);
    
      //  For the frame
      gStyle->SetFrameBorderMode(0);
      gStyle->SetFrameBorderSize(1);
      gStyle->SetFrameFillColor(kBlack);
      gStyle->SetFrameFillStyle(0);
      gStyle->SetFrameLineColor(kBlack);
      gStyle->SetFrameLineStyle(0);
      gStyle->SetFrameLineWidth(1);
    
      //  For the Pad
      gStyle->SetPadBorderMode(0);
      gStyle->SetPadColor(kWhite);
      gStyle->SetPadGridX(false);
      gStyle->SetPadGridY(false);
      gStyle->SetGridColor(0);
      gStyle->SetGridStyle(3);
      gStyle->SetGridWidth(1);

      //  Margins
      if( mode == "Presentation" ) {
	if( spaceForTitle ) {
	  gStyle->SetPadTopMargin(0.11);
	  gStyle->SetPadBottomMargin(0.18);
	  gStyle->SetPadLeftMargin(0.25);
	  gStyle->SetPadRightMargin(0.04);
	} else {
	  gStyle->SetPadTopMargin(0.05);
	  gStyle->SetPadBottomMargin(0.18);
	  gStyle->SetPadLeftMargin(0.19);
	  gStyle->SetPadRightMargin(0.04);
	}
      } else if( mode == "Paper" || mode == "CMS"  ) {
	if( spaceForTitle ) {
	  gStyle->SetPadTopMargin(0.06);
	  gStyle->SetPadBottomMargin(0.18);
	  gStyle->SetPadLeftMargin(0.2);
	  gStyle->SetPadRightMargin(0.04);
	} else {
	  gStyle->SetPadTopMargin(0.05);
	  gStyle->SetPadBottomMargin(0.17);
	  gStyle->SetPadLeftMargin(0.18);
	  gStyle->SetPadRightMargin(0.04);
	}
      } else {
	if( spaceForTitle ) {
	  gStyle->SetPadTopMargin(0.10);
	  gStyle->SetPadBottomMargin(0.14);
	  gStyle->SetPadLeftMargin(0.18);
	  gStyle->SetPadRightMargin(0.04);
	} else {
	  gStyle->SetPadTopMargin(0.08);
	  gStyle->SetPadBottomMargin(0.14);
	  gStyle->SetPadLeftMargin(0.18);
	  gStyle->SetPadRightMargin(0.04);
	}
      }

      //  For the histo:
      gStyle->SetHistLineColor(kBlack);
      gStyle->SetHistLineStyle(0);
      gStyle->SetHistLineWidth(1);
    
      //  For the statistics box:
      if( mode == "Screen" ) {
	gStyle->SetOptStat("eMR");
	gStyle->SetStatColor(kWhite);
	gStyle->SetStatFont(42);
	gStyle->SetStatFontSize(0.03);
	gStyle->SetStatTextColor(1);
	gStyle->SetStatFormat("6.4g");
	gStyle->SetStatBorderSize(1);
	gStyle->SetStatX(0.94);              
	gStyle->SetStatY(0.86);              
	gStyle->SetStatH(0.16);
	gStyle->SetStatW(0.22);
      } else {
	gStyle->SetOptStat(0);
      }
    
      //  For the Global title:
      gStyle->SetOptTitle(1);
      gStyle->SetTitleFont(42,"");
      gStyle->SetTitleColor(1);
      gStyle->SetTitleTextColor(1);
      gStyle->SetTitleFillColor(0);
      gStyle->SetTitleFontSize(0.1);
      gStyle->SetTitleAlign(23);
      gStyle->SetTitleX(0.6);
      gStyle->SetTitleH(0.05);
      gStyle->SetTitleBorderSize(0);

      //  For the axis
      gStyle->SetAxisColor(1,"XYZ");
      gStyle->SetTickLength(0.03,"XYZ");
      gStyle->SetNdivisions(510,"XYZ");
      if( mode == "CMS" ) {
	gStyle->SetPadTickX(0);
	gStyle->SetPadTickY(0);
      } else {
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
      }
      gStyle->SetStripDecimals(kFALSE);
    
      //  For the axis labels and titles
      gStyle->SetTitleColor(1,"XYZ");
      gStyle->SetLabelColor(1,"XYZ");
      if( mode == "Presentation" ) {
	// For the axis labels:
	gStyle->SetLabelFont(42,"XYZ");
	gStyle->SetLabelOffset(0.007,"XYZ");
	gStyle->SetLabelSize(0.045,"XYZ");
      
	// For the axis titles:
	gStyle->SetTitleFont(42,"XYZ");
	gStyle->SetTitleSize(0.06,"XYZ");
	gStyle->SetTitleXOffset(1.2);
	if( spaceForTitle ) gStyle->SetTitleYOffset(2.0);
	else                gStyle->SetTitleYOffset(1.5);
      } else if ( mode == "Paper" || mode == "CMS" ) {
	// For the axis labels:
	gStyle->SetLabelFont(42,"XYZ");
	gStyle->SetLabelOffset(0.007,"XYZ");
	gStyle->SetLabelSize(0.04,"XYZ");
      
	// For the axis titles:
	gStyle->SetTitleFont(42,"XYZ");
	gStyle->SetTitleSize(0.045,"XYZ");
	gStyle->SetTitleXOffset(1.5);
	if( spaceForTitle ) gStyle->SetTitleYOffset(2.1);
	else                gStyle->SetTitleYOffset(1.8);
      } else {
	// For the axis labels:
	gStyle->SetLabelFont(42,"XYZ");
	gStyle->SetLabelOffset(0.007,"XYZ");
	gStyle->SetLabelSize(0.035,"XYZ");
      
	// For the axis titles:
	gStyle->SetTitleFont(42,"XYZ");
	gStyle->SetTitleSize(0.04,"XYZ");
	gStyle->SetTitleXOffset(1.5);
	if( spaceForTitle ) gStyle->SetTitleYOffset(2.1);
	else                gStyle->SetTitleYOffset(1.8);
      }


      //  For the legend
      gStyle->SetLegendBorderSize(0);

      //  For the statistics box
      if( mode == "Presentation" ) {
	if( spaceForTitle ) {
	  gStyle->SetStatFontSize(0.04);
	  gStyle->SetStatX(0.92);              
	  gStyle->SetStatY(0.86);              
	  gStyle->SetStatH(0.2);
	  gStyle->SetStatW(0.3);
	} else {
	  gStyle->SetStatFontSize(0.04);
	  gStyle->SetStatX(0.92);              
	  gStyle->SetStatY(0.92);              
	  gStyle->SetStatH(0.2);
	  gStyle->SetStatW(0.3);
	}
      } else {
	if( spaceForTitle ) {
	  gStyle->SetStatFontSize(0.03);
	  gStyle->SetStatX(0.92);              
	  gStyle->SetStatY(0.86);              
	  gStyle->SetStatH(0.16);
	  gStyle->SetStatW(0.22);
	} else {
	  gStyle->SetStatFontSize(0.03);
	  gStyle->SetStatX(0.92);              
	  gStyle->SetStatY(0.92);              
	  gStyle->SetStatH(0.16);
	  gStyle->SetStatW(0.22);
	}
      }

      std::cout << "Adjusted gStyle for " << std::flush;
      if( mode == "Screen" ) std::cout << "screen viewing" << std::flush;
      else if( mode == "Paper" ) std::cout << "papers" << std::flush;
      else if( mode == "CMS" ) std::cout << "CMS PAS" << std::flush;
      else std::cout << "presentations" << std::flush;
      std::cout << " and " << std::flush;
      if( spaceForTitle ) std::cout << "histograms with title." << std::endl;
      else std::cout << "histograms without title." << std::endl;
    }
  };  
}
#endif


