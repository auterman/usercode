#include "IsoMassLine.h"
#include "TGraph.h"
#include "SusyScan.h"

#include <cmath>
#include <algorithm>


class sort_by{
 public:
  sort_by(double(*x)(const SusyScan*)):_f(x){}
  bool operator()(const SusyScan*a, const SusyScan*b){
    return _f(a)<_f(b); }
 private:
  double(*_f)(const SusyScan*);
};


TGraph * IsoMassLine::get( double(*x)(const SusyScan*), double(*y)(const SusyScan*), 
                           double(*func)(const SusyScan*), double mass, double diff )
{
  TGraph * result = new TGraph(1);
  std::sort(_scan->begin(),_scan->end(),sort_by(x));
  int i=0;
  for (std::vector<SusyScan*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     if ( fabs(func( *it)-mass)<diff )
       result->SetPoint(i++, x(*it), y(*it));
  } 
  return result;
}
