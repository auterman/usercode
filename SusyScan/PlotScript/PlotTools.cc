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
TGraph * PlotTools<T>::GetContour(TH2*plot, int flag)
{
   plot->SetContour(2);
   plot->SetContourLevel(0,0);
   plot->SetContourLevel(1,0.5);
   plot->SetFillColor(1);
   plot->Draw("CONT List");
   gPad->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   int ncontours      = contours->GetSize();
   //std::cout << "N contours = " << ncontours << std::endl;
   TList *list	      = (TList*)contours->At(0);
   //list->GetSize();
   TGraph *gr1 = (TGraph*)list->First();
   return gr1;
}

template class PlotTools<SusyScan>;
template class PlotTools<GeneratorMasses>;
