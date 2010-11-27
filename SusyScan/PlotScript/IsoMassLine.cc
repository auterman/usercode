#include "IsoMassLine.h"
#include "TGraph.h"
#include "SusyScan.h"

#include <cmath>
#include <algorithm>


template<class T>
TGraph * IsoMassLine<T>::get( double(*x)(const T*), double(*y)(const T*), 
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

template class IsoMassLine<SusyScan>;
