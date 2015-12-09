// $Id: Plotting.h,v 1.1 2012/06/26 13:58:58 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef PLOTTING_H
#define PLOTTING_H

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
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TPaveText.h"
#include "TGraph.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include "PlotTools.h"
#include "GeneratorMasses.h"
//#include "StyleSettings.h"

struct style;
static bool plotPNG    = true;
static bool plotC      = true;
static bool plotROOT   = true;
static bool plotLINEAR   = true;
static TCanvas * c1 = 0;

void DrawPlot2D(PlotTools *PlotTool, TCanvas*canvas, TH2* h, const std::string& flag, const string& x, const std::string& y, const std::string& var,
                const std::string& ztitel, double zmin=-999, double zmax=-999, style*s=0 );

void DrawHist1D(PlotTools *PlotTool, TCanvas*canvas, const std::string& flag, const string& x, const std::string& y, const std::string& var,
                const std::string& titel, int n);

void DrawStandardUncertaintyPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h);

void DrawStandardPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h);

void DrawStandardLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h);

void Draw1DLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, style*s, TH1*h);

void DrawPlot1D(PlotTools *PlotTool, TCanvas*canvas, TH1* h, const std::string& flag, const string& x, const std::string& var,style*s);

void DrawStandardPlotsPerBin(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h);

TGraph * InOutPlot(PlotTools *PlotTool, TCanvas*, std::string flag, const std::string& x, const std::string& y, const std::string& R, TH2*h, unsigned idx=0, int color=0, int style=0);

void DrawExclusion(PlotTools *StdPlotTool, PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, 
                   TH1*hp, TH1*h, style*s, std::string lflavor="");

void DrawExclusion1D(PlotTools *PlotTool, TCanvas*canvas, TH1* h, const std::string& flag, const string& x, style*s);

void GetPlotTools(PlotTools*& plotTools, std::string filename, const std::string& x, const std::string& y, std::string GeneratorFile, unsigned factor);


template<typename T>
T Maximum(T v1, T v2){
  return (v1>=v2?v1:v2);
}

template<typename T>
T Minimum(T v1, T v2){
  return (v1<v2?v1:v2);
}

#endif
