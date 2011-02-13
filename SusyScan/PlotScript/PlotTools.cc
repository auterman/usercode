#include "PlotTools.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>
#include <algorithm>
#include <iostream>

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
  //std::cout<< _scan->size() <<",  &scan="<<_scan<<std::endl;
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
    h->SetBinContent( h->GetXaxis()->FindBin(x(*it)), 
                      h->GetYaxis()->FindBin(y(*it)), f(*it) );
  } 
}

template<class T>
void PlotTools<T>::Graph( TGraph*g, double(*x)(const T*), double(*y)(const T*),double ymin)
{
  unsigned i = g->GetN();
  std::sort(_scan->begin(),_scan->end(),sort_by(x));
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
    if (y(*it)>=ymin) g->SetPoint(i++, x(*it), y(*it));
    //std::cout << i << ": x="<<x(*it)<< ", y="<<y(*it)<< std::endl;
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
   std::sort(result.begin(),result.end(),sort_TGraph());
   return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, int ncont, int flag)
{
   return (TGraph*)GetContours(h, ncont).at(flag)->Clone();
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont, int flag,
		      int color, int style){
  TH2*hist = (TH2*)h->Clone();
  Area(hist, x, y, func);
  TGraph * graph = GetContour(hist, ncont, flag);
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  return graph;
}
 
template<class T>
void PlotTools<T>::Print(double(*f)(const T*), double(*x)(const T*), double(*y)(const T*), TGraph*g, double p)
{
   for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     for (int j=0; j<g->GetN(); ++j) {
       double gx, gy;
       g->GetPoint(j, gx, gy);
       if ( (x(*it)-gx)*(x(*it)-gx) +  (y(*it)-gy)*(y(*it)-gy) < p*p)
         std::cout << x(*it) << ", " << y(*it) << " :: " << f(*it) << std::endl;
     }
   } 

}



template<class T>
bool PlotTools<T>::sort_TGraph::operator()(const TGraph*g1, const TGraph*g2)
{ 
   return g1->GetN() > g2->GetN();
}

TGraph * MakeBand(TGraph *g1, TGraph *g2, bool b){
  TGraph * res = new TGraph(g1->GetN()+g2->GetN());
  int p=0;
  for (int i=0; i<g1->GetN(); ++i) {
    double x, y;
    g1->GetPoint(i, x, y);
    res->SetPoint(p++, x, y);
  }
  for (int i=g2->GetN()-1; i>=0; --i) {
    double x, y;
    g2->GetPoint(i, x, y);
    res->SetPoint(p++, x, y);
  }
  res->SetLineColor( g1->GetLineColor() );
  res->SetFillColor( g2->GetLineColor() );
  return res;
}


template class PlotTools<SusyScan>;
template class PlotTools<GeneratorMasses>;
