#include "PlotTools.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>
#include <algorithm>

#include "TGraph.h"
#include "TH2.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"

template<class T>
TGraph * PlotTools<T>::Line( double(*x)(const T*), double(*y)(const T*), 
                             double(*func)(const T*), const double mass, const double diff )
{
  TGraph * result = new TGraph(1);
  std::sort(_scan->begin(),_scan->end(),sort_by(x));
  int i=0;
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     if ( fabs(func( *it)-mass)<diff )
       result->SetPoint(i++, x(*it), y(*it));
  } 
  return result;
}


template<class T>
void PlotTools<T>::Area( TH2*h, double(*x)(const T*), double(*y)(const T*), 
                         double(*f)(const T*) )
{
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
    h->SetBinContent( h->GetXaxis()->FindBin(x(*it)), 
                      h->GetYaxis()->FindBin(y(*it)), f(*it) );
  } 
}

template<class T>
std::vector<TGraph*> PlotTools<T>::GetContours(TH2*h, int ncont)
{
   if (!h) return std::vector<TGraph*>();
   TH2 * plot = (TH2*)h->Clone();
   plot->SetContour(ncont);
   
   plot->SetFillColor(1);
   plot->Draw("CONT Z List");
   gPad->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   int ncontours      = contours->GetSize();
   //std::cout << "N contours = " << ncontours << std::endl;
   std::vector<TGraph*> result;
   for (int i=0;i<ncontours;++i){
     TList *list = (TList*)contours->At(i);
     TGraph* curv = (TGraph*)list->First();
     if (curv) result.push_back( curv );
     for(int j = 0; j < list->GetSize(); j++){
         curv = (TGraph*)list->After(curv); // Get Next graph
         if (curv) result.push_back( curv );
     }
   }  
   delete plot;
   return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, int ncont, int flag)
{
   return (TGraph*)GetContours(h, ncont).at(flag)->Clone();
}

/*
template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, int ncont, int flag)
{
   if (!h) return 0;
   TH2 * plot = (TH2*)h->Clone();
   plot->SetContour(ncont);
   plot->SetFillColor(1);
   plot->Draw("CONT Z List");
   gPad->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   int ncontours       = contours->GetSize();
   TList *list	       = 0;
   TGraph *gr1         = 0;
   if (flag<ncontours) list = (TList*)contours->At(flag);
   if (list) gr1 = (TGraph*)list->First()->Clone();
   if (gr1)  gr1->SetLineWidth(2);
   if (plot) delete plot;
   //don't delete contours or list: These belongs to gROOT !!!
   return gr1;
}
*/

template class PlotTools<SusyScan>;
template class PlotTools<GeneratorMasses>;
