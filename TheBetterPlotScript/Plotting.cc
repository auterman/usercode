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
#include <algorithm>

void DrawPlot1D(PlotTools *PlotTool, TCanvas*canvas, TH1* h, const std::string& flag, const string& x, const std::string& var,style*s)
{
   TH1F *plot1D = (TH1F*)h->Clone();
   PlotTool->Line(plot1D, x, var);
   plot1D->GetYaxis()->SetTitleOffset(1.5);
   plot1D->GetYaxis()->SetTitle(GetInfo(var)->GetLabel().c_str());
   plot1D->GetXaxis()->SetNdivisions(505);
   plot1D->SetLineColor( kRed );
   plot1D->SetLineWidth( 2 );

   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);
   if(plotROOT&&stat(("results/"+flag+"/ROOT").c_str(),&st)==-1) mkdir(("results/"+flag+"/ROOT").c_str(), 0700);
   if(plotLINEAR&&stat(("results/"+flag+"/linear").c_str(),&st)==-1) mkdir(("results/"+flag+"/linear").c_str(), 0700);

   //write plots
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   string namePlot =  flag + "_"+x+"_"+var;
   //
   plot1D->Draw("h"); 
   if (s&&s->cmsTemperaturePlot)  s->cmsTemperaturePlot->Draw();
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   canvas->SaveAs(("results/"+flag+"/log/"+namePlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs(("results/"+flag+"/PNG/"+namePlot + ".png").c_str());
   if (plotC)   canvas->SaveAs(("results/"+flag+"/C/"+namePlot + ".C").c_str());
   if (plotLINEAR) {
     bool old = canvas->GetLogy();
     canvas->SetLogy( !old );
     canvas->SaveAs(("results/"+flag+"/linear/"+namePlot + ".pdf").c_str());
     canvas->SetLogy( old );
   }
   if (plotROOT) {  
      TFile f(("results/"+flag+"/ROOT/"+namePlot + ".root").c_str(), "recreate" );
      f.cd();
      plot1D->Write();
      f.Close();
   }   
}
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
   if(plotROOT&&stat(("results/"+flag+"/ROOT").c_str(),&st)==-1) mkdir(("results/"+flag+"/ROOT").c_str(), 0700);

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
   if (plotROOT) {  
      TFile f(("results/"+flag+"/ROOT/"+namePlot + ".root").c_str(), "recreate" );
      f.cd();
      plot2D->Write();
      f.Close();
   }   
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
   if(plotROOT&&stat(("results/"+flag+"/ROOT").c_str(),&st)==-1) mkdir(("results/"+flag+"/ROOT").c_str(), 0700);

   std::string namePlot = flag +"_"+x+"_"+y+"_"+var+"_1D";
   c1->SaveAs(("results/"+flag+"/log/"+namePlot + ".pdf").c_str());
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+namePlot + ".png").c_str());
   if (plotC) c1->SaveAs(("results/"+flag+"/C/"+namePlot + ".C").c_str());
   if (plotROOT) {  
      TFile f(("results/"+flag+"/ROOT/"+namePlot + ".root").c_str(), "recreate" );
      f.cd();
      h1D->Write();
      f.Close();
   }   
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
   c1->cd();
   c1->SetLogz(1);

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"signal",        "Signal Event Yield", -999, -999, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"Acceptance",        "Acceptance", s->MinAccZ, s->MaxAccZ, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptancePercent", "Acceptance [%]", s->MinAccZ*100, s->MaxAccZ*100, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptanceCorrected",    "Acceptance corr. f. sig. cont. [%]", s->MinAccZ*100, s->MaxAccZ*100, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"ContaminationRelToSignal", "Bkg prediction from signal / Signal yield [%]", -999, -999, s );

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

void Draw1DLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, style*s, TH1*h)
{
    c1->UseCurrentStyle();
    c1->SetLogy(1);

    pt->SortBy(x);

    DrawPlot1D(pt,c1,h,flag,x,"Xsection", s);
    DrawPlot1D(pt,c1,h,flag,x,"ObsXsecLimit", s);
    DrawPlot1D(pt,c1,h,flag,x,"ExpXsecLimit", s);
    DrawPlot1D(pt,c1,h,flag,x,"ObsXsecLimitasym", s);
    DrawPlot1D(pt,c1,h,flag,x,"ExpXsecLimitasym", s);

    DrawExclusion1D(pt,c1,h,flag,x,s);

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


///Johannes /////////////////////////////////////////////////////////////////////////////////////
    DrawPlot2D(pt,c1,h,flag,x,y,"Xsection",          "NLO cross section (pb)");
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "95% CL cross section upper limit (pb)", s->MinXsecZ, s->MaxXsecZ, s );
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit (pb)", s->MinXsecZ, s->MaxXsecZ, s);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events");
    //DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events");
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimitasym",  "Observed asympt. cross section limit (pb)", s->MinXsecZ, s->MaxXsecZ, s);
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimitasym",  "Expected asympt. cross section limit (pb)", s->MinXsecZ, s->MaxXsecZ, s);
///Johannes /////////////////////////////////////////////////////////////////////////////////////
/*
///Knut /////////////////////////////////////////////////////////////////////////////////////
    DrawPlot2D(pt,c1,h,flag,x,y,"Xsection",          "NLO cross section [pb]");
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "95% CL cross section upper limit [pb]", s->MinXsecZ, s->MaxXsecZ, s );
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events");
    //DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events");
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimitasym",  "Observed asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimitasym",  "Expected asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
///Knut /////////////////////////////////////////////////////////////////////////////////////
*/
    //Linear z-scale
    c1->SetLogz(1);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", 0.5, 2.);
    //DrawPlot2D(pt,c1,h,flag,x,y,"ExpR",              "Expected R", 0.5, 2.);
    DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", 0.2, 5, s);
    DrawPlot2D(pt,c1,h,flag,x,y,"ExpR",              "Expected R", 0.2, 5, s);
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
   if (plotROOT) {
      TFile f(("results/"+flag+"/ROOT/"+nameXsPlot + ".root").c_str(), "recreate" );
      f.cd();
      h->Write();
      for (std::vector<TGraph*>::iterator cont = contours.begin(); cont != contours.end(); ++cont) {
        if (!*cont) continue;
        if (cont-contours.begin()>13) break;
        (*cont)->Write();
      }  
      f.Close();
   }
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
   s->gCLsObsExcl	  = InOutPlot(PlotTool,c1,flag,x,y,"ObsR"+lflavor,(TH2F*)h->Clone(), s->iCLsObsExcl, kBlue, 1);
   TGraph * gCLsObsExcl = s->gCLsObsExcl;

   hplot->GetZaxis()->SetTitle("Expected in/out");
   s->gCLsExpExcl   = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor,(TH2F*)h->Clone(), s->iCLsExpExcl, kOrange + 9, 9);
   TGraph * gCLsExpExcl = s->gCLsExpExcl;

   hplot->GetZaxis()->SetTitle("Expected -1 #sigma_{experimental} in/out");
//   s->gCLsExpExclm1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor+"M1",(TH2F*)h->Clone(), s->iCLsExpExclm1, kOrange - 3, 3);
   s->gCLsExpExclm1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpRM1"+lflavor,(TH2F*)h->Clone(), s->iCLsExpExclm1, kOrange - 3, 3);
   TGraph * gCLsExpExclm1 = s->gCLsExpExclm1;

   hplot->GetZaxis()->SetTitle("Expected +1 #sigma_{experimental} in/out");
//   s->gCLsExpExclp1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpR"+lflavor+"P1",(TH2F*)h->Clone(), s->iCLsExpExclp1, kOrange - 3, 3);
     s->gCLsExpExclp1 = InOutPlot(PlotTool,c1,flag,x,y,"ExpRP1"+lflavor,(TH2F*)h->Clone(), s->iCLsExpExclp1, kOrange - 3, 3);
   TGraph * gCLsExpExclp1 = s->gCLsExpExclp1;

   hplot->GetZaxis()->SetTitle("Observed -1 #sigma_{theory} in/out");
   s->gCLsObsTheom1 = InOutPlot(PlotTool,c1,flag,x,y,"ObsRTheoM1"+lflavor,(TH2F*)h->Clone(), s->iCLsObsTheom1, kBlue, 3);
   TGraph * gCLsObsTheom1 = s->gCLsObsTheom1;

   hplot->GetZaxis()->SetTitle("Observed +1 #sigma_{theory} in/out");
   s->gCLsObsTheop1 = InOutPlot(PlotTool,c1,flag,x,y,"ObsRTheoP1"+lflavor,(TH2F*)h->Clone(), s->iCLsObsTheop1, kBlue, 3);
   TGraph * gCLsObsTheop1 = s->gCLsObsTheop1;


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
   //gStyle->SetTitleOffset(1.0, "x");
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
   if (s->doSmooth) {
   Smooth(gCLsObsExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclm1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclp1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheom1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheop1,   s->smooth_points, s->smooth_flag);
/*
   if (s->second_smooth) {
     Smooth(gCLsObsExcl,     s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExcl,     s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExclm1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsExpExclp1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsObsTheom1,   s->second_smooth, s->smooth_flag);
     Smooth(gCLsObsTheop1,   s->second_smooth, s->smooth_flag);
   }
*/
   }

   hplot->GetYaxis()->SetTitleOffset(1.9);
   //hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   hplot->Draw("h");
   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclm1);
   if (gCLs1Sigma) {
      gCLs1Sigma->SetFillStyle(1001);
      gCLs1Sigma->SetFillColor(kOrange - 3);
      gCLs1Sigma->Draw("f");
   }
   //if () gObsUnsmoothed->Draw("l");
   //if (gCLsExpTheom1) gCLsExpTheom1->Draw("l");
   //gCLsExpTheop1->Draw("l");
   if (gCLsObsExcl) gCLsObsExcl->Draw("l");
   if (gCLsExpExcl) gCLsExpExcl->Draw("l");
   if (gCLsObsTheom1) gCLsObsTheom1->Draw("l");
   if (gCLsObsTheop1) gCLsObsTheop1->Draw("l");

   //Legends
   TLegend* leg = s->leg;
   TH1F * legdummy = 0;
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   if (gCLsObsTheom1&&gCLsObsTheop1) leg->AddEntry(gCLsObsTheop1, "Observed #pm1#sigma theory", "l");
   TH1F* legExp = (gCLs1Sigma ? (TH1F*)gCLs1Sigma->Clone() : 0);
   if (legExp){ 
     legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
     legExp->SetLineColor(gCLsExpExcl->GetLineColor());
     legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
     leg->AddEntry(legExp, "Expected #pm1#sigma exp.", "lf");
     //leg->AddEntry(gCLsExpTheop1, "Exp. limit #pm1#sigma signal theory", "l");
   }
   if (s->PostExclusionPlotting) s->PostExclusionPlotting(s,leg);
   leg->Draw();
   
   s->lumiExclusionPlot->Draw();
   s->cmsExclusionPlot->Draw();

   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);
   if(plotROOT&&stat(("results/"+flag+"/ROOT").c_str(),&st)==-1) mkdir(("results/"+flag+"/ROOT").c_str(), 0700);


   gPad->RedrawAxis();
   string nameExcl = flag + "_"+x+"_"+y+"_Exclusion_"+lflavor;
   c1->SaveAs(("results/"+flag+"/log/"+nameExcl + ".pdf").c_str());
   if (gCLsObsExcl) {
      gCLsObsExcl->SetName("Observed_limit");
      gCLsObsExcl->SetTitle("Observed_limit");}
   if (gCLsObsTheom1 && gCLsObsTheop1){ 
      gCLsObsTheom1->SetName("Observed_limit_dn");
      gCLsObsTheom1->SetTitle("Observed_limit_dn");
      gCLsObsTheop1->SetName("Observed_limit_up");
      gCLsObsTheop1->SetTitle("Observed_limit_up");}
   if (gCLsExpExcl){
      gCLsExpExcl->SetName("Expected_limit");
      gCLsExpExcl->SetTitle("Expected_limit");}
   if (gCLsExpExclm1&&gCLsExpExclp1){ 
      gCLsExpExclm1->SetName("Expected_limit_dn");
      gCLsExpExclm1->SetTitle("Expected_limit_dn");
      gCLsExpExclp1->SetName("Expected_limit_up");
      gCLsExpExclp1->SetTitle("Expected_limit_up");}
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+nameExcl + ".png").c_str());
   if (plotC  ) {
     h->GetZaxis()->SetTitle("");
     h->Draw("h");
     gCLsObsExcl->Draw("l");
     gCLsExpExcl->Draw("l");
     c1->SaveAs(("results/"+flag+"/C/"+nameExcl + ".C").c_str());
   }
   if (plotROOT) {
      TFile f(("results/"+flag+"/ROOT/"+nameExcl + ".root").c_str(), "recreate" );
      f.cd();
      if (gCLsExpExclm1) gCLsExpExclm1->Write();
      if (gCLsExpExclp1) gCLsExpExclp1->Write();
      if (gCLsObsExcl) gCLsObsExcl->Write();
      if (gCLsExpExcl) gCLsExpExcl->Write();
      if (gCLsObsTheom1) gCLsObsTheom1->Write();
      if (gCLsObsTheop1) gCLsObsTheop1->Write();
      f.Close();
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
   plot2D->GetZaxis()->SetTitle("95% CL cross section upper limit #sigma_{obs} (pb)");
   StdPlotTool->Area(plot2D, x, y, "ObsXsecLimit");
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   if (s->MinXsecZ!=-999) plot2D->SetMinimum(s->MinXsecZ);
   if (s->MaxXsecZ!=-999) plot2D->SetMaximum(s->MaxXsecZ);
   if (s->MinXsecZ==-999&&s->MaxXsecZ==-999) SetZRange(plot2D);
   if (s&&s->SetMoreLogLabels) {
     plot2D->GetZaxis()->SetMoreLogLabels();
     plot2D->GetZaxis()->SetLabelSize(0.035);
     plot2D->GetZaxis()->SetTitleOffset(1.5);
     c1->SetRightMargin(0.18);  
     c1->SetLeftMargin(0.15);  
   }  
   plot2D->Draw("colz"); 
   if (gCLsObsExcl){
    gCLsObsExcl->SetLineWidth(3);
    gCLsObsExcl->SetLineColor(kBlue);
    gCLsObsExcl->Draw("l");
   }
   if (gCLsObsTheom1){
    gCLsObsTheom1->SetLineColor(kBlue);
    gCLsObsTheom1->SetLineStyle(3);
    gCLsObsTheom1->SetLineWidth(2);
    gCLsObsTheom1->Draw("l");
   }
   if (gCLsObsTheop1){
    gCLsObsTheop1->SetLineColor(kBlue);
    gCLsObsTheop1->SetLineStyle(3);
    gCLsObsTheop1->SetLineWidth(2);
    gCLsObsTheop1->Draw("l");
   }
   if (gCLsExpExclm1){
    gCLsExpExclm1->SetLineWidth(2);
    gCLsExpExclm1->SetLineColor(kOrange + 9);
    gCLsExpExclm1->Draw("l");
   }
   if (gCLsExpExclp1){
    gCLsExpExclp1->SetLineWidth(2);
    gCLsExpExclp1->SetLineColor(kOrange + 9);
    gCLsExpExclp1->Draw("l");
   }
   if (gCLsExpExcl){
    gCLsExpExcl->SetLineWidth(3);
    gCLsExpExcl->SetLineStyle(9);
    gCLsExpExcl->SetLineColor(kOrange + 9);
    gCLsExpExcl->Draw("l");
   }
   //if (s->excluded) s->excluded->Draw();
   TLegend* legSMS = s->leg;
   std::string header = legSMS->GetHeader();
   legSMS->Clear();
   legSMS->SetHeader(header.c_str());
   legSMS->AddEntry(gCLsObsExcl,   "Observed", "l");
   if (gCLsObsTheom1&&gCLsObsTheop1)legSMS->AddEntry(gCLsObsTheop1, "Observed #pm1#sigma theory", "l");
   legSMS->AddEntry(gCLsExpExcl,   "Expected", "l");
   legSMS->AddEntry(gCLsExpExclm1, "Expected #pm1#sigma exp.", "l");
   if (s->PostExclusionPlotting)  s->PostExclusionPlotting(s,legSMS);   
   legSMS->Draw();
   s->lumiTemperaturePlot->Draw();
   s->cmsTemperaturePlot->Draw();
   gPad->RedrawAxis();
   nameExcl = flag + "_"+x+"_"+y+"_Exclusion_witXsecLimit";
   c1->SaveAs(("results/"+flag+"/log/"+nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs(("results/"+flag+"/PNG/"+nameExcl + ".PNG").c_str());
   //if (plotC) c1->SaveAs(("results/"+flag+"/C/"+nameExcl + ".C").c_str());
   if (plotROOT) {
      TFile f(("results/"+flag+"/ROOT/"+nameExcl + ".root").c_str(), "recreate" );
      f.cd();
      plot2D->SetName("nxsec");
      ///plot2D->SetTitle("txsec");
      plot2D->Write();
      if (gCLsExpExclm1) gCLsExpExclm1->Write();
      if (gCLsExpExclp1) gCLsExpExclp1->Write();
      if (gCLsObsExcl) gCLsObsExcl->Write();
      if (gCLsExpExcl) gCLsExpExcl->Write();
      if (gCLsObsTheom1) gCLsObsTheom1->Write();
      if (gCLsObsTheop1) gCLsObsTheop1->Write();
      f.Close();
    }
  }
}


void DrawExclusion1D(PlotTools *PlotTool, TCanvas*canvas, TH1* h, const std::string& flag, const string& x, style*s)
{

    std::cout << "   DrawExclusion1D plotting "<<x<<" from "<<h->GetXaxis()->GetXmin()<<" to "
              <<h->GetXaxis()->GetXmax() << " in " << h->GetXaxis()->GetNbins()  << " bins" << std::endl;


   TGraph * Xsec = PlotTool->Line(x, "Xsection");
   Xsec->SetLineColor( kBlue );
   Xsec->SetLineWidth( 1 );
   Xsec->SetLineStyle( 7 );
   s->gCLsObsExcl = PlotTool->Line(x, "ObsXsecLimit"+s->LimitFlavor);
   TGraph * Obs = s->gCLsObsExcl;
   Obs->SetLineWidth( 2 );
   s->gCLsExpExcl = PlotTool->Line(x, "ExpXsecLimit"+s->LimitFlavor);
   TGraph * Exp = s->gCLsExpExcl;
   Exp->SetLineWidth( 2 );
   Exp->SetLineColor( 2 );
   Exp->SetLineStyle( 2 );
   //s->gCLsObsTheom1 = PlotTool->Line(x, "ObsXsecLimitM1"+s->LimitFlavor);
   //s->gCLsObsTheop1 = PlotTool->Line(x, "ObsXsecLimitP1"+s->LimitFlavor);
   s->gCLsObsTheom1 = PlotTool->Line(x, "XsectionTheoM1");
   s->gCLsObsTheop1 = PlotTool->Line(x, "XsectionTheoP1");
   TGraph * ObsM1 = s->gCLsObsTheom1;
   TGraph * ObsP1 = s->gCLsObsTheop1;
   //ObsP1->SetLineStyle( 9 );
   //ObsM1->SetLineStyle( 9 );
   ObsM1->SetLineColor( kBlue );
   ObsP1->SetLineColor( kBlue );
   s->gCLsExpExclm1 = PlotTool->Line(x, "ExpXsecLimitM1"+s->LimitFlavor);
   s->gCLsExpExclp1 = PlotTool->Line(x, "ExpXsecLimitP1"+s->LimitFlavor);
   TGraph * ExpM1 = s->gCLsExpExclm1;
   TGraph * ExpP1 = s->gCLsExpExclp1;
   ExpM1->SetLineColor( kGreen );
   ExpP1->SetLineColor( kGreen );
   TGraph * Exp1 = MakeBand(ExpM1, ExpP1); 
   s->gCLsExpExclm2 = PlotTool->Line(x, "ExpXsecLimitM2"+s->LimitFlavor);
   s->gCLsExpExclp2 = PlotTool->Line(x, "ExpXsecLimitP2"+s->LimitFlavor);
   TGraph * ExpM2 = s->gCLsExpExclm2;
   TGraph * ExpP2 = s->gCLsExpExclp2;
   ExpM2->SetLineColor( kYellow );
   ExpP2->SetLineColor( kYellow );
   TGraph * Exp2 = MakeBand(ExpM2, ExpP2); 

   canvas->cd();
   canvas->Clear();
   TH1F *plot1D = (TH1F*)h->Clone();
   plot1D->GetYaxis()->SetTitleOffset(1.5);
   plot1D->GetYaxis()->SetTitle("Signal cross section / limit (pb)");
   plot1D->GetXaxis()->SetNdivisions(505);
   plot1D->Draw("h");
   //plot1D->SetMaximum(100*Maximum<double>( Xsec->GetHistogram()->GetMaximum(), ExpP2->GetHistogram()->GetMaximum() ) );
   plot1D->SetMaximum( 2 );
   plot1D->SetMinimum(    Minimum<double>( Xsec->GetHistogram()->GetMinimum(), ExpM2->GetHistogram()->GetMinimum() ) );


   s->leg->AddEntry(Obs,"Observed CL_{s} limit","l");
   s->leg->AddEntry(Exp,"Expected CL_{s} limit #pm1#sigma#pm2#sigma","l");
   s->leg->AddEntry(Xsec,"Signal cross section #pm1#sigma","l");
   

   //check & make dir
   struct stat st={0};
   if(stat("results",&st)==-1)     mkdir("results", 0700);
   if(stat(("results/"+flag).c_str(),&st)==-1)     mkdir(("results/"+flag).c_str(), 0700);
   if(stat(("results/"+flag+"/log").c_str(),&st)==-1) mkdir(("results/"+flag+"/log").c_str(), 0700);
   if(plotC&&stat(("results/"+flag+"/C").c_str(),&st)==-1) mkdir(("results/"+flag+"/C").c_str(), 0700);
   if(plotPNG&&stat(("results/"+flag+"/PNG").c_str(),&st)==-1) mkdir(("results/"+flag+"/PNG").c_str(), 0700);
   if(plotLINEAR&&stat(("results/"+flag+"/linear").c_str(),&st)==-1) mkdir(("results/"+flag+"/linear").c_str(), 0700);

   //write plots
   string namePlot =  flag + "_"+x+"_Exclusion1D"+s->LimitFlavor;
   //
   Exp2->Draw("fl,same");
   Exp1->Draw("fl,same");
   Xsec->Draw("l,same"); 
   Exp->Draw("l,same"); 
   Obs->Draw("l,same"); 
   ObsM1->Draw("l"); 
   ObsP1->Draw("l"); 
   s->leg->Draw();
   if (s&&s->cms)  s->cms->Draw();
   if (s&&s->lumi) s->lumi->Draw();
   if (s&&s->PostExclusionPlotting) s->PostExclusionPlotting(s, s->leg);
   gPad->RedrawAxis();
   canvas->SaveAs(("results/"+flag+"/log/"+namePlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs(("results/"+flag+"/PNG/"+namePlot + ".png").c_str());
   if (plotC)   canvas->SaveAs(("results/"+flag+"/C/"+namePlot + ".C").c_str());
   if (plotLINEAR) {
     bool old = canvas->GetLogy();
     canvas->SetLogy( !old );
     plot1D->Draw("h");
     plot1D->SetMaximum(1.0*Maximum<double>( Xsec->GetHistogram()->GetMaximum(), ExpM1->GetHistogram()->GetMaximum() ) );
     plot1D->SetMinimum(0 );
     Exp2->Draw("fl,same");
     Exp1->Draw("fl,same");
     Xsec->Draw("l,same"); 
     Exp->Draw("l,same"); 
     Obs->Draw("l,same"); 
     //ObsM1->Draw("l"); 
     //ObsP1->Draw("l"); 
     s->leg->Draw();
     if (s&&s->cms)  s->cms->Draw();
     if (s&&s->lumi) s->lumi->Draw();
     if (s&&s->PostExclusionPlotting) s->PostExclusionPlotting(s, s->leg);
     gPad->RedrawAxis();

     canvas->SaveAs(("results/"+flag+"/linear/"+namePlot + ".pdf").c_str());
     canvas->SetLogy( old );
   }

       std::cout << "   DrawExclusion1D plotting "<<x<<" from "<<plot1D->GetXaxis()->GetXmin()<<" to "
              <<plot1D->GetXaxis()->GetXmax() << " in " << plot1D->GetXaxis()->GetNbins()  << " bins" << std::endl;

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

