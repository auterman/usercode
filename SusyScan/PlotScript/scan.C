#include <algorithm>
#include <map>
#include <stdlib.h>
#include <iostream>
#include "Riostream.h"
#include <cmath>

#include <TApplication.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TArrow.h>
#include <TGaxis.h>
#include <TPad.h>
#include <TColor.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TMarker.h>


#include "scan.h"


TROOT root("GUI", "GUI test environement");

using namespace std;


double total_xsec(const SUSY_XSECS point)
{ return point.total(); }


bool charged_LSP(const SUSY_POINT point)
{ return ( (fabs(point.MZ1) - point.MTAU1)>0.0 ); }

bool gluino(const SUSY_POINT point, const double mass)
{ return ( fabs(point.MGL - mass)<5.0 ); }

bool ul(const SUSY_POINT point, const double mass)
{ return ( fabs(point.MUL - mass)<5.0 ); }

bool neutralino_1(const SUSY_POINT point, const double mass)
{ return ( fabs(fabs(point.MZ1) - mass)<1.0 ); }

bool sq_equal_gluino(const SUSY_POINT point, const double treshold)
{ return ( fabs(point.MUL - point.MGL)<treshold ); }


bool xs_sq_equal_gluino(const SUSY_XSECS point, const double treshold)
{ if (point.sgsg()<1E-5&&point.sqsq()<1E-5) return false; else return (fabs(point.sgsg() - point.sqsq())<treshold*point.sqsq()/100. ); }


TScan::TScan()
{
  plot_id = 0;
  bins_x = 200;
  min_x  = 0.0;
  max_x  = 2000.0;
  bins_y = 200;
  min_x  = 0.0;
  max_y  = 2000.0;
}

TScan::~TScan()
{
}

template <class T> MyTGraph * TScan::IsoMassLine(std::vector<T> pnts, bool(*func)(T,double), double mass )
{
  MyTGraph * result = new MyTGraph(1);
  int i=0;
  for (typename std::vector<T>::const_iterator it=pnts.begin();
       it!=pnts.end(); ++it) {
     if ( func( *it, mass) )
       result->SetPoint(i++, it->MZERO, it->MHALF);
  } 
  return result;
}

template <class T> TH2F * TScan::Area(std::vector<T> pnts, double(*func)(T))
{
  char * name = new char[255];
  sprintf(name,"Area_plot%d",++plot_id);
  TH2F * result = new TH2F(name,"",bins_x,min_x,max_x,bins_y,min_x,max_y);
  for (typename std::vector<T>::const_iterator it=pnts.begin();
       it!=pnts.end(); ++it) {
     result->Fill(it->MZERO, it->MHALF, func(*it) );
  } 
  delete name;
  return result;
}

template <class T> TH2F * TScan::Area(std::vector<T> pnts, bool(*func)(T))
{
  char * name = new char[255];
  sprintf(name,"Area_plot%d",++plot_id);
  TH2F * result = new TH2F(name,"",bins_x,min_x,max_x,bins_y,min_x,max_y);
  for (typename std::vector<T>::const_iterator it=pnts.begin();
       it!=pnts.end(); ++it) {
     if ( func( *it ) ) 
       result->Fill(it->MZERO, it->MHALF, 1.);
  } 
  delete name;
  return result;
}

TH2F * TScan::Area51()
{
  char * name = new char[255];
  sprintf(name,"Area51_plot%d",++plot_id);
  TH2F * result = new TH2F(name,"",bins_x,min_x,max_x,bins_y,min_x,max_y);
  for (int x=1; x<=bins_x; ++x){
    for (int y=1; y<=bins_y; ++y){
      result->SetBinContent(x,y,1.);
    }  
  }
  for (std::vector<SUSY_POINT>::const_iterator it=points.begin();
       it!=points.end(); ++it) {
    result->SetBinContent((int)(bins_x*it->MZERO/(max_x-min_x)),
                          (int)(bins_y*it->MHALF/(max_y-min_y)),0.);
  }     
  delete name;
  return result;
}

MyTGraph * TScan::GetContour(TH2F*plot, int flag)
{
   char * name = new char[255];
   sprintf(name,"canv%d",++plot_id);
   TCanvas * canv = new TCanvas(name,"c1",600,600);
   canv->cd();
   plot->SetContour(2);
   plot->SetContourLevel(0,0);
   plot->SetContourLevel(1,0.5);
   plot->SetFillColor(1);
   plot->Draw("CONT List");
   canv->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   int ncontours = contours->GetSize();
   if (ncontours==0) return 0;
   //cout << "N contours = " << ncontours << endl;
   TList *list = (TList*)contours->At(0);
   MyTGraph *gr1 = (MyTGraph*)list->First();
   delete name;
   delete canv;
   delete list;
   
   return gr1;
}

void TScan::ReadGeneratedMasses(std::string file)
{
   ifstream masses_file;
   masses_file.open(file.c_str());
   SUSY_POINT p;
   while (1) {
      masses_file >> p.MZERO      
                  >> p.MHALF	  
                  >> p.TANB	  
                  >> p.SGNMU	  
                  >> p.AZERO	  
                  >> p.MTOP	  
                  >> p.muQ	  
                  >> p.Q	  
                  >> p.M1	  
                  >> p.M2
                  >> p.M3	  
                  >> p.MGL	  
                  >> p.MUL	  
                  >> p.MB1	  
                  >> p.MSN	  
                  >> p.MNTAU	  
                  >> p.MZ1	  
                  >> p.MW1	  
                  >> p.MHL	  
                  >> p.MUR	  
                  >> p.MB2	  
                  >> p.MEL	  
                  >> p.MTAU1	  
                  >> p.MZ2	  
                  >> p.MW2	  
                  >> p.MHH	  
                  >> p.MDL	  
                  >> p.MT1	  
                  >> p.MER	  
                  >> p.MTAU2	  
                  >> p.MZ3	  
                  >> p.MHA	  
                  >> p.MDR	  
                  >> p.MT2	  
                  >> p.MZ4	  
                  >> p.MHp;

      if (!masses_file.good()) break;
      if (fabs(p.SGNMU)!=1.) {
         cerr << "check lines near m0=" << p.MZERO << ", m1/2=" << p.MHALF << endl;
         break;
      }	
      points.push_back(p);
   }
}

void TScan::ReadGeneratedXsects(std::string file)
{
   ifstream xsects_file;
   xsects_file.open(file.c_str());
   SUSY_XSECS x;
   while (1) {
      xsects_file >> x.MZERO      
                  >> x.MHALF	  
                  >> x.TANB	  
                  >> x.SGNMU	  
                  >> x.AZERO	  
                  >> x.MTOP	  
                  >> x.XS0
                  >> x.XS201
                  >> x.XS202
                  >> x.XS204
                  >> x.XS205
                  >> x.XS207
                  >> x.XS208
                  >> x.XS209
                  >> x.XS210
                  >> x.XS211
                  >> x.XS212
                  >> x.XS213
                  >> x.XS214
                  >> x.XS216
                  >> x.XS217
                  >> x.XS218
                  >> x.XS219
                  >> x.XS220
                  >> x.XS221
                  >> x.XS222
                  >> x.XS223
                  >> x.XS224
                  >> x.XS225
                  >> x.XS226
                  >> x.XS227
                  >> x.XS228
                  >> x.XS229
                  >> x.XS230
                  >> x.XS231
                  >> x.XS232
                  >> x.XS233
                  >> x.XS234
                  >> x.XS235
                  >> x.XS236
                  >> x.XS237
                  >> x.XS238
                  >> x.XS239
                  >> x.XS240
                  >> x.XS241
                  >> x.XS242
                  >> x.XS243
                  >> x.XS244
                  >> x.XS246
                  >> x.XS247
                  >> x.XS248
                  >> x.XS249
                  >> x.XS250
                  >> x.XS251
                  >> x.XS252
                  >> x.XS253
                  >> x.XS254
                  >> x.XS256
                  >> x.XS258
                  >> x.XS259
                  >> x.XS261
                  >> x.XS262
                  >> x.XS263
                  >> x.XS264
                  >> x.XS265
                  >> x.XS271
                  >> x.XS272
                  >> x.XS273
                  >> x.XS274
                  >> x.XS275
                  >> x.XS276
                  >> x.XS277
                  >> x.XS278
                  >> x.XS279
                  >> x.XS280
                  >> x.XS281
                  >> x.XS282
                  >> x.XS283
                  >> x.XS284
                  >> x.XS285
                  >> x.XS286
                  >> x.XS287
                  >> x.XS288
                  >> x.XS289
                  >> x.XS290
                  >> x.XS291
                  >> x.XS292
                  >> x.XS293
                  >> x.XS294
                  >> x.XS295
                  >> x.XS296
                  >> x.XS297
                  >> x.XS298
                  >> x.XS299
                  >> x.XS300
                  >> x.XS301;
      if (!xsects_file.good()) break;
      if (fabs(x.SGNMU)!=1.) {
         cerr << "check lines near m0=" << x.MZERO << ", m1/2=" << x.MHALF << endl;
         break;
      }	
      xsects.push_back(x);
   }
}

int TScan::DoStuff()
{
   ///Read in the generated masses
   ReadGeneratedMasses( "mass_scan.dat" );
   cout << "Read in " << points.size() << " mass points." <<endl;

   ///Read in the generated x-sections
   ReadGeneratedXsects( "xsec_scan.dat" );
   cout << "Read in " << xsects.size() << " x-section points." <<endl;

   ///Where are no mass-points generated?
   TH2F   * no_solution         = Area51(  );
   MyTGraph * contour_no_solution = GetContour( no_solution );
   contour_no_solution->SetLineColor(1);
   contour_no_solution->SetLineWidth(4);

   ///Where is the LSP charged?
   TH2F   * ch_LSP = Area<SUSY_POINT>( points, charged_LSP );
   MyTGraph * contour_charged_LSP = GetContour( ch_LSP );
   contour_charged_LSP->SetLineColor(6);
   contour_charged_LSP->SetLineWidth(4);

   ///Main Canvas
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   c1->SetFillStyle   ( 4000 );
   c1->SetLeftMargin  ( 0.15 );
   c1->SetRightMargin ( 0.15 );
   c1->SetBottomMargin( 0.10 );
   c1->cd();
   //c1->SetTopMargin   ( 0.05 );
   //TPostScript ps("plots.ps",-112);

   ///The base hist of the mSUGRA-plane with axis labels
   //TH2F * plane = new TH2F("plane","The mSUGRA parameter space;m_{0} [GeV];m_{1/2} [GeV]", bins_x,min_x,max_x,bins_y,min_x,max_y);
   SetBins(80,80);//xsec-binning is 25 GeV
   TH2F * plane = Area<SUSY_XSECS>( xsects, total_xsec );
   SetBins(400,400);//mass-binning is 5 GeV
   plane->SetTitle("The mSUGRA parameter space;m_{0} [GeV];m_{1/2} [GeV];#sigma_{SUSY incl.} [pb]");
   plane->GetYaxis()->SetTitleOffset(  1.8 );
   plane->GetZaxis()->SetTitleOffset(  1.1 );
   plane->GetZaxis()->SetRangeUser(0.00001, 1000.);
   gPad->SetLogz(1);
   plane->Draw("COLZ");

   ///Now, add what ever should be displayed else
   //---------------------------------------------------------------------------//
   TLatex t;
   t.SetTextColor(14);
   t.SetTextAlign(12);
   t.SetTextSize(0.02);
   
   ///gluino
   gStyle->SetLineColor(15);//gray
   MyTGraph * m_gluino5  = IsoMassLine( points, gluino,  500. ); m_gluino5->Draw();
   MyTGraph * m_gluino10 = IsoMassLine( points, gluino, 1000. ); m_gluino10->Draw();
   MyTGraph * m_gluino20 = IsoMassLine( points, gluino, 2000. ); m_gluino20->Draw();
   MyTGraph * m_gluino30 = IsoMassLine( points, gluino, 3000. ); m_gluino30->Draw();
   MyTGraph * m_gluino40 = IsoMassLine( points, gluino, 4000. ); m_gluino40->Draw();
   t.DrawLatex(360.,450.,"#tilde{g}=1000 GeV");
   t.DrawLatex(600.,930.,"#tilde{g}=2000 GeV");
   t.DrawLatex(870.,1420.,"#tilde{g}=3000 GeV");
   t.DrawLatex(1150.,1930.,"#tilde{g}=4000 GeV");
   
   ///neutralino
   //MyTGraph * m_neutralino1 = IsoMassLine( neutralino_1, 500. ); m_neutralino1->Draw();
   
   ///left-handed up-like squarks
   MyTGraph * m_ul5  = IsoMassLine( points, ul,  500. ); m_ul5->Draw();
   MyTGraph * m_ul10 = IsoMassLine( points, ul, 1000. ); m_ul10->Draw();
   MyTGraph * m_ul15 = IsoMassLine( points, ul, 1500. ); m_ul15->Draw();
   MyTGraph * m_ul20 = IsoMassLine( points, ul, 2000. ); m_ul20->Draw();
   MyTGraph * m_ul25 = IsoMassLine( points, ul, 2500. ); m_ul25->Draw();
   MyTGraph * m_ul30 = IsoMassLine( points, ul, 3000. ); m_ul30->Draw();
   MyTGraph * m_ul35 = IsoMassLine( points, ul, 3500. ); m_ul35->Draw();
   MyTGraph * m_ul40 = IsoMassLine( points, ul, 4000. ); m_ul40->Draw();
   t.DrawLatex(800.,250.,"#tilde{u}_{L}=1000 GeV");
   t.DrawLatex(1670.,470.,"#tilde{u}_{L}=2000 GeV");
   t.DrawLatex(1670.,1250.,"#tilde{u}_{L}=3000 GeV");
   t.DrawLatex(1670.,1930.,"#tilde{u}_{L}=4000 GeV");

   ///Contours
   contour_no_solution->Add(max_x+5,430);
   contour_no_solution->Add(max_x+5,min_y-5);
   contour_no_solution->Add(min_x,min_y-5);
   contour_no_solution->SetFillColor( 1 );
   contour_no_solution->SetFillStyle( 3454 );
   contour_no_solution->Draw("lf");
   contour_no_solution->Draw();

   contour_charged_LSP->Add(min_x-5, 726.25);
   contour_charged_LSP->Add(min_x-5, max_y+5);
   contour_charged_LSP->SetFillColor( 6 );
   contour_charged_LSP->SetFillStyle( 3454 );
   contour_charged_LSP->SetLineColor( 6 );
   contour_charged_LSP->SetLineWidth( 4 );
   contour_charged_LSP->Draw("lf");
   contour_charged_LSP->Draw();
   t.SetTextSize(0.03);t.SetTextColor( 6 );
   t.DrawLatex(200.,1800.,"charged LSP (#tilde{#tau})");

   // m(gluino)==m(squark)
   gStyle->SetLineColor(1);//black
   MyTGraph * m_ulgl  = IsoMassLine( points, sq_equal_gluino,  0.2 ); 
   m_ulgl->SetLineWidth(3); m_ulgl->Draw();
   t.SetTextSize(0.03);t.SetTextColor( 1 );
   t.DrawLatex( 750.,1200.,"m(#tilde{u}_{L}) < m(#tilde{g})");
   t.DrawLatex(1350.,1150.,"m(#tilde{u}_{L}) > m(#tilde{g})");


   // xs(gluino)==xs(squark)
   MyTGraph * xs_ulgl  = IsoMassLine( xsects, xs_sq_equal_gluino,  5.0 ); 
   xs_ulgl->SetLineWidth(3); xs_ulgl->Draw();
   t.SetTextSize(0.02);t.SetTextColor( 1 );
   t.DrawLatex( 890.,510.,"#sigma(#tilde{q}#tilde{q}) > #sigmam(#tilde{g}#tilde{g})");
   t.DrawLatex(1280.,450.,"#sigma(#tilde{q}#tilde{q}) < #sigmam(#tilde{g}#tilde{g})");

   TMarker m;
   m.SetMarkerStyle(29);
   m.SetMarkerColor(2);
   m.SetMarkerSize(1.5);
   m.DrawMarker(1000,1000);  
   m.DrawMarker(1000,200);   
   m.DrawMarker(1000,400);   
   m.DrawMarker(1000,600);   
   m.DrawMarker(1000,800);   
   m.DrawMarker(1200,1000);  
   m.DrawMarker(1200,1400);  
   m.DrawMarker(1200,1800);  
   m.DrawMarker(1200,200);   
   m.DrawMarker(1200,600);   
   m.DrawMarker(1600,1000);  
   m.DrawMarker(1600,1400);  
   m.DrawMarker(1600,1800);  
   m.DrawMarker(1600,200);   
   m.DrawMarker(1600,600);   
   m.DrawMarker(2000,1000);  
   m.DrawMarker(2000,1400);  
   m.DrawMarker(2000,1800);  
   m.DrawMarker(2000,200);   
   m.DrawMarker(2000,600);   
   m.DrawMarker(200,200);    
   m.DrawMarker(400,200);    
   m.DrawMarker(400,400);    
   m.DrawMarker(400,600);    
   m.DrawMarker(600,1000);   
   m.DrawMarker(600,200);    
   m.DrawMarker(600,400);    
   m.DrawMarker(600,600);    
   m.DrawMarker(600,800);    
   m.DrawMarker(800,1000);   
   m.DrawMarker(800,1400);   
   m.DrawMarker(800,200);    
   m.DrawMarker(800,400);    
   m.DrawMarker(800,600);    
   m.DrawMarker(800,800);    
   //DONE
   c1->SaveAs("dirt.eps");
   gApplication->Run();    //interactive

   //ps.NewPage();

}

int scan(void)
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

   
   TScan * scan = new TScan();
   scan->DoStuff();
   delete scan;
}

int main(int argc, char** argv)
{
  TApplication theApp("App", &argc, argv);
  return scan();
}
