// $Id: Plotting.cc,v 1.2 2012/06/29 20:27:21 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     TheBetterPlotScript, a tool to plot final results, limits and exclusion contours, based on 'PlotScript'

     Christian Autermann, Hamburg University/LPC, February 2012
 *** ------------------------------------------------------------------------------------------------------- ***/


#include "Plotting.h"

#include "StyleSettings.h"
#include "OldExclusionContours.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void DrawPlot2D(PlotTools *PlotTool, TCanvas*canvas, TH2* h, const std::string& flag, const string& x, const std::string& y, const std::string& var,
                const std::string& ztitel, double zmin, double zmax, style*s)
{
   TH2F *plot2D = (TH2F*)h->Clone();
   plot2D->GetZaxis()->SetTitle(ztitel.c_str());
   PlotTool->Area(plot2D, x, y, var);
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   if (zmin!=-999) plot2D->SetMinimum(zmin);
   if (zmax!=-999) plot2D->SetMaximum(zmax);
   if (zmin==-999&&zmax==-999) SetZRange(plot2D);
   if (s&&s->SetMoreLogLabels) {
     plot2D->GetZaxis()->SetMoreLogLabels();
     plot2D->GetZaxis()->SetLabelSize(0.035);
     plot2D->GetZaxis()->SetTitleOffset(2.1);
     c1->SetRightMargin(0.22);  
     c1->SetLeftMargin(0.15);  
   }  

   plot2D->GetXaxis()->SetNdivisions(505);
   plot2D->Draw("colz"); 
   double tx=400;
   double ty=1.045*(plot2D->GetYaxis()->GetXmax()-plot2D->GetYaxis()->GetXmin())+plot2D->GetYaxis()->GetXmin();  

   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);

   //write plots
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   string namePlot =  flag + "_"+x+"_"+y+"_"+var;
   //
   plot2D->Draw("colz"); 
   if (s&&s->cmsTemperaturePlot)  s->cmsTemperaturePlot->Draw();
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   canvas->SaveAs(("results/"+flag+"/log/"+namePlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs(("results/"+flag+"/PNG/"+namePlot + ".png").c_str());
   if (plotC)   canvas->SaveAs(("results/"+flag+"/C/"+namePlot + ".C").c_str());
}

void DrawHist1D(PlotTools *PlotTool, TCanvas*canvas, const std::string& flag, const string& x, const std::string& y, const std::string& var,
                const std::string& titel, int n)
{
   std::vector<double> vec;
   PlotTool->VectorOf(vec, var);
   if (vec.size()<=0) return;
   std::sort(vec.begin(), vec.end());
   double median  = vec[vec.size()/2];
   double quant16 = vec[vec.size()/6.25];
   double quant84 = vec[vec.size()/1.2];
   double xmin    = vec.front();
   double xmax    = vec.back();
   std::string name = flag+"_"+x+"_"+y+"_"+var+"_1D";
   TH1F *h1D = new TH1F(name.c_str(),";;number of cMSSM points", n, xmin-sqrt(fabs(xmin)), xmax+sqrt(fabs(xmax)));
   h1D->GetXaxis()->SetTitle(titel.c_str());
   PlotTool->Hist(h1D, var);
   TLegend * leg = new TLegend(0.19, 0.7, 0.4, 0.88);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetTextSize(0.025);
   { std::stringstream ss; ss << "16% quantile: " << setprecision(3) << quant16;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   { std::stringstream ss; ss << "median : " << setprecision(3) << median;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   { std::stringstream ss; ss << "84% quantile: " << setprecision(3) << quant84;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   h1D->Draw("h");
   leg->Draw("same");
   
   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);

   std::string namePlot = flag +"_"+x+"_"+y+"_"+var+"_1D";
   c1->SaveAs(("results/"+flag+"/log/"+namePlot + ".pdf").c_str());
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+namePlot + ".png").c_str());
   if (plotC) c1->SaveAs(("results/"+flag+"/C/"+namePlot + ".C").c_str());
}


void DrawStandardUncertaintyPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
    c1->SetRightMargin(0.18);
    //Log z-scale
    c1->SetLogz(1);
    DrawPlot2D(pt,c1,h,flag,x,y,"Luminosity",        "Luminosity [pb^{-1}]", 0, 6000.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalStatUnc",     "Rel. Signal Statistical uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalSysUnc",      "Rel. Signal Systematic uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalPDFAccUnc",   "Rel. Signal PDF Accept. uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalPDFXsecUnc",  "Rel. Signal PDF cross-section uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalScaleUnc",    "Rel. Signal Scale uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalJesUpUnc",    "Rel. Signal JES (up) uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalJesDnUnc",    "Rel. Signal JES (dn) uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalJerUpUnc",    "Rel. Signal JER (up) uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalJerDnUnc",    "Rel. Signal JER (dn) uncertainty", 0.01, 100.);
    DrawPlot2D(pt,c1,h,flag,x,y,"SignalTotalUnc",    "Rel. Signal Total uncertainty", 0.01, 100.);
}

void DrawStandardPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
   //gStyle->SetPadRightMargin(0.2);
   //gStyle->SetPadLeftMargin(0.2);
   //gStyle->SetTitleOffset(1.3, "xyz");
   //gStyle->SetTitleOffset(1.9, "y");
   //gStyle->SetNdivisions(505);
   gStyle->SetTitleFont(43, "xyz");
   gStyle->SetTitleSize(32, "xyz");
   c1->UseCurrentStyle();

   //Require an observed CLs limit:
   //pt->Remove("Acceptance", Compare::less,    s->MinAccZ);
   //pt->Remove("Acceptance", Compare::greater, s->MaxAccZ);
   //Fill the holes by 2D interpolation in gl-sq
   //pt->FillEmptyPointsByInterpolation(x, y);
   
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"Xsection",          "NLO cross section [pb]");

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"signal",        "Signal Event Yield", -999, -999, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"Acceptance",        "Acceptance", s->MinAccZ, s->MaxAccZ, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptancePercent", "Acceptance [%]", s->MinAccZ*100, s->MaxAccZ*100, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptanceCorrected",    "Acceptance corr. f. sig. cont. [%]", s->MinAccZ*100, s->MaxAccZ*100, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"ContaminationRelToSignal", "Signal contamination / Signal yield [%]" );

   //1D Histograms
   //DrawHist1D(pt,c1,flag,x,y,"SignalStatUnc",	  "Rel. Signal Statistical uncertainty", 20);
   //DrawHist1D(pt,c1,flag,x,y,"SignalSysUnc",	  "Rel. Signal Systematic uncertainty", 20);
   //DrawHist1D(pt,c1,flag,x,y,"SignalPDFAccUnc",   "Rel. Signal PDF Accept. uncertainty", 20);
   //DrawHist1D(pt,c1,flag,x,y,"SignalPDFXsecUnc",  "Rel. Signal PDF cross-section uncertainty", 20);
   //DrawHist1D(pt,c1,flag,x,y,"SignalScaleUnc",    "Rel. Signal Scale uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"u_rel_signal_exp",    "Rel. Signal exp. uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"u_rel_signal_theo",   "Rel. Signal theo. uncertainty", 20);

   //DrawHist1D(pt,c1,flag,x,y,"ObsRmM2", "ObsR - ExpR-2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRmP2", "ObsR - ExpR+2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRdM2", "ObsR / ExpR-2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRdP2", "ObsR / ExpR+2sigma", 20);
}

void DrawStandardLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
    c1->UseCurrentStyle();

    //Require an observed CLs limit:
//   pt->Remove("ObsXsecLimit", Compare::less, s->MinXsecZ);
    //Fill possible holes by 2D interpolation in gl-sq
//   pt->FillEmptyPointsByInterpolation(x, y);


    //Log z-scale
    c1->SetLogz(1);
    //DrawPlot2D(pt,c1,h,flag+"_FixedBinning",x,y,"ObsXsecLimit",      "Observed cross section limit [pb]",0.001,0.02);
    DrawPlot2D(pt,c1,h,flag,x,y,"R_firstguess",      "R (first guess)", s->MinXsecZ, s->MaxXsecZ, s );
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "95% CL cross section upper limit [pb]", s->MinXsecZ, s->MaxXsecZ, s );
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events");
    //DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events");
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimitasym",  "Observed asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimitasym",  "Expected asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);

    //Linear z-scale
    c1->SetLogz(1);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", 0.5, 2.);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ExpR",              "Expected R", 0.5, 2.);
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", -999, -999, s);
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpR",              "Expected R", -999, -999, s);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsDivExp",         "Observed / Expected", 0.0, 2.);
}


void DrawStandardPlotsPerBin(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
    for (int bin=0; bin<nchannels; ++bin) {
        std::stringstream binlabel, binl;
        binlabel << ", bin "<<bin;
        binl     << bin;
        const std::string bl = binlabel.str();
        const std::string b  = binl.str();
        c1->SetLogz(1);

        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b,          	"Signal event yield"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_acceptance", 	"Acceptance [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_contamination", "Signal contamination [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_stat_UP",	"Signal Stat. UP [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_stat_DN",	"Signal Stat. DN [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_syst_UP",	"Signal Syst. UP [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_syst_DN",	"Signal Syst. DN [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_theory_UP",	"Signal Theory UP [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_theory_DN",	"Signal Theory DN [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_UP",	"Signal trigger UP [%]"+bl, 0.01, 100.);
        DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_DN", 	"Signal trigger DN [%]"+bl, 0.01, 100.);

        //1D Histograms
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_contamination", "Signal contamination [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_UP",       "Signal Stat. UP [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_DN",       "Signal Stat. DN [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_UP",       "Signal Syst. UP [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_DN",       "Signal Syst. DN [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_theory_UP",     "Signal Acc. PDF UP [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_theory_DN",     "Signal Acc. PDF DN [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_UP",    "Signal trigger UP [%]"+bl, 20);
        DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_DN",    "Signal trigger DN [%]"+bl, 20);
    }

}

TGraph * InOutPlot(PlotTools *PlotTool, TCanvas*canvas, std::string flag, const std::string& x, const std::string& y, const std::string& R, TH2*h, unsigned idx, int color, int style)
{
   canvas->cd();
   
   PlotTool->InOutFromR(h, x, y, R, 3);
   FillEmptyPoints(h,0.5);
   h->GetZaxis()->SetTitleOffset(1.5);
   h->SetMinimum(-0);h->SetMaximum(1.);
   std::vector<TGraph*> contours = GetContours(h, 3);
   /// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
   h->Draw("colz");
   int col = kBlue - 10;
   for (std::vector<TGraph*>::iterator cont = contours.begin(); cont != contours.end(); ++cont) {
   	   if (!*cont) continue;
   	   double x, y;
   	   (*cont)->GetPoint(0, x, y);
   	   (*cont)->SetLineColor(col);
   	   (*cont)->Draw("l");
   	   TLatex l;
   	   l.SetTextSize(0.04);
   	   l.SetTextColor(col++);
   	   char val[20];
   	   sprintf(val, "%d", (int) (cont - contours.begin()));
   	   l.DrawLatex(x, y, val);
   	   if (cont-contours.begin()>13) break;
   }

   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);

   //drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = flag + "_"+x+"_"+y+"_"+"InOut"+R;
   canvas->SaveAs(("results/"+flag+"/log/"+nameXsPlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs(("results/"+flag+"/PNG/"+nameXsPlot + ".png").c_str());
   if (plotC)   canvas->SaveAs(("results/"+flag+"/C/"+nameXsPlot + ".C").c_str());
   delete h;
   TGraph * res = (contours.size()>idx?(TGraph*)contours[idx]->Clone():0);
   if (res) {res->SetLineColor(color);
             res->SetLineStyle(style);}
   return res;
}

void DrawExclusion(PlotTools *StdPlotTool, PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, 
                   TH1*hp, TH1*h, style*s, std::string lflavor)
{
   //Require an observed CLs limit:
   //PlotTool->Remove("ObsR", Compare::less, 0.0);
   //PlotTool->FillEmptyPointsByInterpolation1D(x, y);
 
   TH2F *hs = (TH2F*)h->Clone();
   TH2F *hplot = (TH2F*)hp->Clone();
   hs->GetZaxis()->SetTitle("");

   if (s->PreExclusionPlotting) s->PreExclusionPlotting(s, PlotTool );

   //In/Out Plot
   hplot->GetZaxis()->SetTitle("Observed in/out");
   TGraph * gCLsObsExcl	  = InOutPlot(PlotTool,c1,flag,x,y,"ObsR"+lflavor,(TH2F*)h->Clone(), s->iCLsObsExcl, kBlue, 1);

   hplot->GetZaxis()->SetTitle("Expected in/out");
   TGraph * gCLsExpExcl   = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor,(TH2F*)h->Clone(), s->iCLsExpExcl, kOrange + 9, 9);

   hplot->GetZaxis()->SetTitle("Expected -1 #sigma_{experimental} in/out");
   TGraph * gCLsExpExclm1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor+"M1",(TH2F*)h->Clone(), s->iCLsExpExclm1, kOrange - 3, 3);

   hplot->GetZaxis()->SetTitle("Expected +1 #sigma_{experimental} in/out");
   TGraph * gCLsExpExclp1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor+"P1",(TH2F*)h->Clone(), s->iCLsExpExclp1, kOrange - 3, 3);

   hplot->GetZaxis()->SetTitle("Observed -1 #sigma_{theory} in/out");
   TGraph * gCLsObsTheom1 = InOutPlot(PlotTool,c1,flag,x,y,"ObsR"+lflavor+"TheoM1",(TH2F*)h->Clone(), s->iCLsObsTheom1, kBlue, 3);

   hplot->GetZaxis()->SetTitle("Observed +1 #sigma_{theory} in/out");
   TGraph * gCLsObsTheop1 = InOutPlot(PlotTool,c1,flag,x,y,"ObsR"+lflavor+"TheoP1",(TH2F*)h->Clone(), s->iCLsObsTheop1, kBlue, 3);


   {
   //Exclusion Contours
   //gStyle->SetPadRightMargin(0.08);
   //gStyle->SetPadLeftMargin(0.2);
   //gStyle->SetTitleOffset(1.3, "xyz");
   //gStyle->SetTitleOffset(1.9, "y");
   //gStyle->SetNdivisions(505);
   //gStyle->SetTitleFont(43, "xyz");
   //gStyle->SetTitleSize(32, "xyz");
   
   //Compromise agreed on with Dongwook:
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   gStyle->SetPadRightMargin(0.08);
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
   c1->UseCurrentStyle();
   c1->SetLogz(0);
   c1->SetTopMargin(0.11);

   
   
   gCLsObsExcl->SetLineWidth(3);
   gCLsExpExcl->SetLineWidth(3);
   

   TGraph* gObsUnsmoothed = (TGraph*)gCLsObsExcl->Clone();
   Smooth(gCLsObsExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclm1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclp1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheom1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheop1,   s->smooth_points, s->smooth_flag);
   if (s->second_smooth) {
     Smooth(gCLsObsExcl,     s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExcl,     s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExclm1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExclp1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsObsTheom1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsObsTheop1,   s->second_smooth, s->smooth_flag);
   }

   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(1001);
   gCLs1Sigma->SetFillColor(kOrange - 3);
   hplot->GetYaxis()->SetTitleOffset(1.9);
   hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   hplot->Draw("h");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   //gObsUnsmoothed->Draw("l");
   gCLsExpExcl->Draw("l");
//   gCLsExpTheom1->Draw("l");
        gCLsObsTheom1->Draw("l");
//   gCLsExpTheop1->Draw("l");
   gCLsObsTheop1->Draw("l");

   //Legends
   TLegend* leg = s->leg;
   TH1F * legdummy = 0;
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   leg->AddEntry(gCLsObsTheop1, "Observed #pm1#sigma theory", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm1#sigma exp.", "lf");
   //leg->AddEntry(gCLsExpTheop1, "Exp. limit #pm1#sigma signal theory", "l");

   if (s->PostExclusionPlotting) s->PostExclusionPlotting(s,leg);
   leg->Draw();
   s->lumi->Draw();
   s->cms->Draw();

   //check & make dir
   struct stat st={0};
    if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);


   gPad->RedrawAxis();
   string nameExcl = flag + "_"+x+"_"+y+"_Exclusion_"+lflavor;
   c1->SaveAs(("results/"+flag+"/log/"+nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+nameExcl + ".png").c_str());
   if (plotROOT) c1->SaveAs(("results/"+flag+"/C/"+nameExcl + "canvas.C").c_str());
   if (plotC  ) {
     h->GetZaxis()->SetTitle("");
     h->Draw("h");
     gCLsObsExcl->SetName("Observed limit");
     gCLsObsExcl->SetTitle("Observed limit");
     gCLsObsExcl->Draw("l");
     gCLsExpExcl->SetName("Expected limit");
     gCLsExpExcl->SetTitle("Expected limit");
     gCLsExpExcl->Draw("l");
     c1->SaveAs(("results/"+flag+"/C/"+nameExcl + ".C").c_str());
   }


   ///------------------------------------------------------------------------------------
   /// Exclusion plot SMS style with x-sect limit
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
   c1->UseCurrentStyle();
   c1->SetLogz(1);
   

   TH2 *plot2D = StdPlotTool->GetHist(x,y);
   plot2D->GetZaxis()->SetTitle("95% CL cross section upper limit [pb]");
   StdPlotTool->Area(plot2D, x, y, "ObsXsecLimit");
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   if (s->MinXsecZ!=-999) plot2D->SetMinimum(s->MinXsecZ);
   if (s->MaxXsecZ!=-999) plot2D->SetMaximum(s->MaxXsecZ);
   if (s->MinXsecZ==-999&&s->MaxXsecZ==-999) SetZRange(plot2D);
   if (s&&s->SetMoreLogLabels) {
     plot2D->GetZaxis()->SetMoreLogLabels();
     plot2D->GetZaxis()->SetLabelSize(0.035);
     plot2D->GetZaxis()->SetTitleOffset(2.1);
     c1->SetRightMargin(0.22);  
     c1->SetLeftMargin(0.15);  
   }  
   plot2D->Draw("colz"); 
   gCLsObsExcl->SetLineWidth(3);
   gCLsObsExcl->SetLineColor(kBlue);
   gCLsObsTheom1->SetLineColor(kBlue);
   gCLsObsTheop1->SetLineColor(kBlue);
   gCLsObsTheom1->SetLineStyle(3);
   gCLsObsTheop1->SetLineStyle(3);
   gCLsObsTheom1->SetLineWidth(2);
   gCLsObsTheop1->SetLineWidth(2);
   gCLsExpExclm1->SetLineWidth(2);
   gCLsExpExclp1->SetLineWidth(2);
   gCLsExpExcl->SetLineWidth(3);
   gCLsExpExcl->SetLineStyle(9);
   gCLsExpExcl->SetLineColor(kOrange + 9);
   gCLsExpExclm1->SetLineColor(kOrange + 9);
   gCLsExpExclp1->SetLineColor(kOrange + 9);
   gCLsExpExclm1->Draw("l");
   gCLsExpExclp1->Draw("l");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   gCLsObsTheom1->Draw("l");
   gCLsObsTheop1->Draw("l");
   //if (s->excluded) s->excluded->Draw();
   TLegend* legSMS = s->leg;
   std::string header = legSMS->GetHeader();
   legSMS->Clear();
   legSMS->SetHeader(header.c_str());
   legSMS->AddEntry(gCLsObsExcl,   "Observed", "l");
   legSMS->AddEntry(gCLsObsTheop1, "Observed #pm1#sigma theory", "l");
   legSMS->AddEntry(gCLsExpExcl,   "Expected", "l");
   legSMS->AddEntry(gCLsExpExclm1, "Expected #pm1#sigma exp.", "l");
   //if (s->PostExclusionPlotting)  s->PostExclusionPlotting(s,legSMS);   
   legSMS->Draw();
   s->lumiTemperaturePlot->Draw();
   s->cmsTemperaturePlot->Draw();
   gPad->RedrawAxis();
   nameExcl = flag + "_"+x+"_"+y+"_Exclusion_witXsecLimit";
   c1->SaveAs(("results/"+flag+"/log/"+nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+nameExcl + ".PNG").c_str());
   //if (plotC) c1->SaveAs(("results/"+flag+"/C/"+nameExcl + ".C").c_str());
   }
   
}





void GetPlotTools(PlotTools*& plotTools, std::string filename, const std::string& x, const std::string& y, std::string GeneratorFile, unsigned factor)
{
    if (!c1) c1 = new TCanvas("c_squ", "c_squ", 900, 800);
    c1->cd();

    Events * events = new Events();
    ReadEvents(*events, filename);

    //plotting helper functions
    plotTools = new PlotTools(events);

    //Make grid in Mzero, Mhalf finer by factors of 2 by linear interpolation
    for (int i=2; i<=factor; i*=2)
        plotTools->ExpandGrid(x, y);
    // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
    // of 2 in x and y for each "ExpandGrid

    //Add generator information of particles masses if a file is given
    if (GeneratorFile!="") {
        std::vector<GeneratorMasses> GenMasses;
        ReadGeneratorMasses(GenMasses, GeneratorFile);
        Match( GenMasses, *events);
    }
}

