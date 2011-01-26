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
bool PlotTools<T>::sort_TGraph::operator()(const TGraph*g1, const TGraph*g2)
{ 
   return g1->GetN() > g2->GetN();
}


template class PlotTools<SusyScan>;
template class PlotTools<GeneratorMasses>;
