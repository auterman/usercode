#include "plot.h"
#include "SusyScan.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"

#include <string>

double Mzero(const SusyScan* p){ return p->Mzero; }
double Mhalf(const SusyScan* p){ return p->Mhalf; }
double Xsection(const SusyScan* p){ return p->Xsection; }
  
void TheLimits::plot(TH2*h, double(*x)(const SusyScan*), double(*y)(const SusyScan*), double(*f)(const SusyScan*) )
{
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin();it!=_scan.end();++it )
    h->Fill( x(*it), y(*it), f(*it) );
}

int plot(int argc, char** argv)
{
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
   //gStyle->SetFillColor(0);
   //gStyle->SetOptStat(kFALSE);
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
 
   //gROOT->SetStyle("MyStyle");
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   c1->SetFillStyle   ( 4000 );
   c1->SetLeftMargin  ( 0.15 );
   c1->SetRightMargin ( 0.15 );
   c1->SetBottomMargin( 0.10 );
   c1->cd();
   
   TheLimits * doIt = new TheLimits();
   for (int i = 1; i<argc; ++i)
   {
     doIt->add( new SusyScan(argv[i]) );
   }
   //std::cout << "Read "<<scan->size()<<" points."<<std::endl;
   //TRint *theApp = new TRint("ROOT example", &argc, argv);
   //theApp->Run();
   TApplication * theApp = new TApplication("App", &argc, argv);
   
   TH2F*h = new TH2F("xsec","mSUGRA,m_0 [GeV]; m_{1/2} [GeV]",
                     100,0,2000,100,0,2000);

   doIt->plot(h, Mzero, Mhalf, Xsection);
   h->Draw("colz");
   //c1->Draw();
   c1->SaveAs("plot.pdf");
   std::cout<<"Drawn!!"<<std::endl;
   
   gApplication->Run();
}


int main(int argc, char** argv)
{
 // TRint *theApp = new TRint("ROOT example", &argc, argv);
  
  return plot(argc, argv);
}
