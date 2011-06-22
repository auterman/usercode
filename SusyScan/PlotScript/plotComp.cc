#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "PlotTools.h"
#include "TheLimits.h"
#include "GlobalFunctions.h"
#include "StyleSettings.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"

#include "TChain.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMarker.h"

#include <string>
#include <cmath>
#include <stdio.h>

int plot(int argc, char** argv)
{
   //interactive root session
   //TApplication theApp("App", 0, 0);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }   

   /*
   //Style stuff
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
   gStyle->SetCanvasColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);

   gStyle->SetTitleFillColor(0);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleX(0.10);
   gStyle->SetTitleY(0.98);
   gStyle->SetTitleW(0.8);
   gStyle->SetTitleH(0.06);

   gStyle->SetErrorX(0);
   gStyle->SetStatColor(0);
   gStyle->SetStatBorderSize(0);
   gStyle->SetStatX(0);
   gStyle->SetStatY(0);
   gStyle->SetStatW(0);
   gStyle->SetStatH(0);

   gStyle->SetTitleFont(22);
   gStyle->SetLabelFont(22,"X");
   gStyle->SetLabelFont(22,"Y");
   gStyle->SetLabelFont(22,"Z");
   gStyle->SetLabelSize(0.03,"X");
   gStyle->SetLabelSize(0.03,"Y");
   gStyle->SetLabelSize(0.03,"Z");
   */
   util::StyleSettings::paper();
   gStyle->SetPadBottomMargin(0.18);
 
   //gROOT->SetStyle("MyStyle");
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   //c1->SetFillStyle   ( 4000 );
   //c1->SetLeftMargin  ( 0.15 );
   //c1->SetRightMargin ( 0.15 );
   //c1->SetBottomMargin( 0.10 );
   c1->cd();
   
   //Get limits and generator masses ---------------------------------------------------
   TheLimits * genpoints3MHT = new TheLimits();
   genpoints3MHT->Fill("limits_moriond_MHT/filelist_tb3.txt"); 

   TheLimits * genpoints3HT = new TheLimits();
   genpoints3HT->Fill("limits_moriond_HT/filelist_tb3.txt"); 
                       
   TheLimits * genpoints10MHT = new TheLimits();
   genpoints10MHT->Fill("limits_moriond_MHT/filelist_tb10.txt"); 

   TheLimits * genpoints10HT = new TheLimits();
   genpoints10HT->Fill("limits_moriond_HT/filelist_tb10.txt"); 

   TheLimits * genpoints50MHT = new TheLimits();
   genpoints50MHT->Fill("limits_moriond_MHT/filelist_tb50.txt"); 

   TheLimits * genpoints50HT = new TheLimits();
   genpoints50HT->Fill("limits_moriond_HT/filelist_tb50.txt"); 

   //genpoints->FillGeneratorMasses("GenScan_tb50.dat");
   //genpoints->match();
   //genpointsHT->FillGeneratorMasses("GenScan_tb50.dat");
   //genpointsHT->match();

   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools3MHT  = new PlotTools<SusyScan>(genpoints3MHT->GetScan());
   PlotTools<SusyScan> * plotTools3HT   = new PlotTools<SusyScan>(genpoints3HT->GetScan());
   PlotTools<SusyScan> * plotTools10MHT = new PlotTools<SusyScan>(genpoints10MHT->GetScan());
   PlotTools<SusyScan> * plotTools10HT  = new PlotTools<SusyScan>(genpoints10HT->GetScan());
   PlotTools<SusyScan> * plotTools50MHT = new PlotTools<SusyScan>(genpoints50MHT->GetScan());
   PlotTools<SusyScan> * plotTools50HT  = new PlotTools<SusyScan>(genpoints50HT->GetScan());



   //the histograms
   c1->SetLogz(1);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);



   c1->SetRightMargin ( 0.1 );
   // TestContours in M0 - M1/2
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     40,200,600,26,140,400);
   TH2F*texpexcl=(TH2F*)texcl->Clone();
   plotTools3HT->Area(texpexcl, Mzero, Mhalf, NLOExpExclCLp1sigma);
   std::vector<TGraph*> contours = plotTools3HT->GetContours(texpexcl,3); 
   texpexcl->Draw("colz");
   int col=kBlue-10;
   for (std::vector<TGraph*>::iterator cont=contours.begin(); cont!=contours.end(); ++cont){
     if (! *cont) continue;
     double x, y;
     (*cont)->GetPoint(0, x, y);
     (*cont)->SetLineColor(col);
     (*cont)->Draw("l");
     TLatex l; l.SetTextSize(0.04); l.SetTextColor(col++);
     char val[20];
     sprintf(val,"%d",(int)(cont-contours.begin()));
     l.DrawLatex(x,y,val); 
     //if (cont-contours.begin()>3) break;
   }
   c1->SaveAs("results/ExclusionTestContours_m0_m12_Comp.pdf");


   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,650,26,200,400);
   TH2F*hs3  = new TH2F("hs3","",50,0,509.9,35,100,450);
   TH2F*hs10 = new TH2F("hs10","",100,0,1009.9,35,100,450);
   TH2F*hs50 = new TH2F("hs50","",40,200,600,26,140,400);

   TGraph * gCLsExp50HT    = plotTools50HT ->GetContour(hs50,Mzero,Mhalf,NLOExpExclCL, 3,0, 4,2); 
   TGraph * gCLsExp50MHT   = plotTools50MHT->GetContour(hs50,Mzero,Mhalf,NLOExpExclCL, 3,0, 4,2); 
   TGraph * gCLsObs50HT    = plotTools50HT ->GetContour(hs50,Mzero,Mhalf,NLOObsExclCL, 3,0, 4,1); 
   TGraph * gCLsObs50MHT   = plotTools50MHT->GetContour(hs50,Mzero,Mhalf,NLOObsExclCL, 3,0, 4,1); 
   TGraph * gCLsExp10HT    = plotTools10HT ->GetContour(hs10,Mzero,Mhalf,NLOExpExclCL, 3,0, 1,2); 
   TGraph * gCLsExp10MHT   = plotTools10MHT->GetContour(hs10,Mzero,Mhalf,NLOExpExclCL, 3,0, 1,2); 
   TGraph * gCLsObs10HT    = plotTools10HT ->GetContour(hs10,Mzero,Mhalf,NLOObsExclCL, 3,0, 1,1); 
   TGraph * gCLsObs10MHT   = plotTools10MHT->GetContour(hs10,Mzero,Mhalf,NLOObsExclCL, 3,0, 1,1); 
   TGraph * gCLsExp3HT     = plotTools3HT  ->GetContour(hs3, Mzero,Mhalf,NLOExpExclCL, 3,3, 2,2); 
   TGraph * gCLsExp3MHT    = plotTools3MHT ->GetContour(hs3, Mzero,Mhalf,NLOExpExclCL, 3,3, 2,2); 
   TGraph * gCLsObs3HT     = plotTools3HT  ->GetContour(hs3, Mzero,Mhalf,NLOObsExclCL, 3,3, 2,1); 
   TGraph * gCLsObs3MHT    = plotTools3MHT ->GetContour(hs3, Mzero,Mhalf,NLOObsExclCL, 3,3, 2,1); 

   TGraph * gCheck1 = (TGraph*)gCLsObs10HT->Clone();
   TGraph * gCheck2 = (TGraph*)gCLsObs10MHT->Clone();
   Smooth( gCLsObs3HT,   27); gCLsObs3HT->SetLineWidth( 3 );
   Smooth( gCLsObs3MHT,  27);
   Smooth( gCLsExp3HT,   27 ); gCLsExp3HT->SetLineWidth( 3 );
   Smooth( gCLsExp3MHT,  27 ); 
   Smooth( gCLsObs10HT,  27);  gCLsObs10HT->SetLineWidth( 3 );
   Smooth( gCLsObs10MHT, 27);
   Smooth( gCLsExp10HT,  27 ); gCLsExp10HT->SetLineWidth( 3 );
   Smooth( gCLsExp10MHT, 27 ); 
   Smooth( gCLsObs50HT,  27);  gCLsObs50HT->SetLineWidth( 3 );
   Smooth( gCLsObs50MHT, 27);
   Smooth( gCLsExp50HT,  27 ); gCLsExp50HT->SetLineWidth( 3 );
   Smooth( gCLsExp50MHT, 27 ); 

   TGraph * CLsObs3  = plotTools3HT ->ChooseBest(gCLsObs3HT,gCLsObs3MHT, gCLsObs3HT,gCLsObs3MHT);
   TGraph * CLsExp3  = plotTools3HT ->ChooseBest(gCLsExp3HT,gCLsExp3MHT, gCLsExp3HT,gCLsExp3MHT);
   TGraph * CLsObs10 = plotTools10HT->ChooseBest(gCLsObs10HT,gCLsObs10MHT, gCLsObs10HT,gCLsObs10MHT);
   TGraph * CLsExp10 = plotTools10HT->ChooseBest(gCLsExp10HT,gCLsExp10MHT, gCLsExp10HT,gCLsExp10MHT);
   TGraph * CLsObs50 = plotTools50HT->ChooseBest(gCLsObs50HT,gCLsObs50MHT, gCLsObs50HT,gCLsObs50MHT);
   TGraph * CLsExp50 = plotTools50HT->ChooseBest(gCLsExp50HT,gCLsExp50MHT, gCLsExp50HT,gCLsExp50MHT);

   hexcl->GetYaxis()->SetTitleOffset(1.3);
   hexcl->GetXaxis()->SetTitleOffset(0.92);
   hexcl->Draw("colz");
   
   CLsObs3->Draw("l");
   CLsExp3->Draw("l");
   CLsObs10->Draw("l");
   CLsExp10->Draw("l");
   CLsObs50->Draw("l");
   CLsExp50->Draw("l");
   TLatex ms; ms.SetTextSize(0.03); ms.SetTextFont(42);//ms.SetTextColor(12);
   ms.DrawLatex(100,389,"CMS preliminary"); 


   TLegend * leg = new TLegend(0.55,0.65,0.90,0.92);
   leg->SetBorderSize(0);leg->SetFillColor(0);leg->SetFillStyle(4000);leg->SetTextFont(42);
   leg->SetHeader("L_{int} = 36/pb, #sqrt{s} = 7 TeV");
   leg->AddEntry(CLsObs3,  "Observed, tan#beta=3","l");
   leg->AddEntry(CLsExp3,  "Expected, tan#beta=3","l");
   leg->AddEntry(CLsObs10, "Observed, tan#beta=10","l");
   leg->AddEntry(CLsExp10, "Expected, tan#beta=10","l");
   leg->AddEntry(CLsObs50, "Observed, tan#beta=50","l");
   leg->AddEntry(CLsExp50, "Expected, tan#beta=50","l");
   leg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results/Exclusion_m0_m12_Comp.pdf");


   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
