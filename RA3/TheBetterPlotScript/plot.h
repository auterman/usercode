#ifndef PLOT_H
#define PLOT_H

#include "PlotTools.h"
#include "GeneratorMasses.h"

#include <string>
#include <vector>


void GetPlotTools(PlotTools*& plotTools, std::string filename, std::string GeneratorFile="", unsigned factor=0);
void AddEvents(PlotTools*& plotTools, std::string filename, std::string GeneratorFile="");

class LimitGraphs{
 public:
  LimitGraphs(TH2*h, const std::string& scan, std::string gen, int factor, const std::string& x, const std::string y,
              const std::string& name, int nsmooth, int color, int idx_obs, int idx_exp, int idx_exp_m1, int idx_exp_p1):
	      name_(name){

    Events * events = new Events();
    ReadEvents(*events, scan);
  
    //plotting helper functions
    PlotTool = new PlotTools(events);

    //Require an observed CLs limit:
    //plotTools->Remove("ObsR", Compare::less, 0.0);
    PlotTool->FillEmptyPointsByInterpolation("gluino", "squark");


    //Make grid in Mzero, Mhalf finer by factors of 2 by linear interpolation
    for (int i=2; i<=factor; i*=2)
      PlotTool->ExpandGrid("gluino", "squark");
    // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
    // of 2 in x and y for each "ExpandGrid
  
    //Add generator information of particles masses if a file is given
    if (gen!="") {
      std::vector<GeneratorMasses> GenMasses;
      ReadGeneratorMasses(GenMasses, gen);
      Match( GenMasses, *events);
    }  

    g_obs   = PlotTool->GetContour(h, x, y, "ObsR",   3, idx_obs,    color, 1);
    g_exp   = PlotTool->GetContour(h, x, y, "ExpR",   3, idx_exp,    color, 1);
    g_expm1 = PlotTool->GetContour(h, x, y, "ExpRM1", 3, idx_exp_m1, color, 3);
    g_expp1 = PlotTool->GetContour(h, x, y, "ExpRP1", 3, idx_exp_p1, color, 3);
    Smooth(g_obs,   nsmooth);
    Smooth(g_exp,   nsmooth);
    Smooth(g_expm1, nsmooth);
    Smooth(g_expp1, nsmooth);
    g_obs_asym   = PlotTool->GetContour(h, x, y, "ObsRasym",   3, idx_obs,    color, 1);
    g_exp_asym   = PlotTool->GetContour(h, x, y, "ExpRasym",   3, idx_exp,    color, 1);
    g_expm1_asym = PlotTool->GetContour(h, x, y, "ExpRasymM1", 3, idx_exp_m1, color, 3);
    g_expp1_asym = PlotTool->GetContour(h, x, y, "ExpRasymP1", 3, idx_exp_p1, color, 3);
    Smooth(g_obs_asym,   nsmooth);
    Smooth(g_exp_asym,   nsmooth);
    Smooth(g_expm1_asym, nsmooth);
    Smooth(g_expp1_asym, nsmooth);
  }

  TGraph * Obs(){return g_obs;}
  TGraph * ObsAsym(){return g_obs_asym;}
  TGraph * ExpAsym(int s=0){
    switch(s){
     case -1 : return g_expm1_asym;
     case +1 : return g_expp1_asym;
     default : return g_exp_asym;
     return 0;
    }
  }
  TGraph * Exp(int s=0){
    switch(s){
     case -1 : return g_expm1;
     case +1 : return g_expp1;
     default : return g_exp;
     return 0;
    }
  }
  TH2* GetHist(){return h;}
  PlotTools * GetPlot(){return PlotTool;}
  std::string Name(){return name_;}
  ~LimitGraphs(){delete PlotTool;};

 private:
  TGraph * g_obs, *g_exp, *g_expm1, *g_expp1;
  TGraph * g_obs_asym, *g_exp_asym, *g_expm1_asym, *g_expp1_asym;
  TH2 * h;
  std::string name_;
  PlotTools * PlotTool;
};

#endif
