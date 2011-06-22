#include "PrintTools.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>
#include <algorithm>
#include <iostream>

#include "TGraph.h"
#include "TF1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"

 
template<class T>
void PrintTools<T>::Print(double(*x)(const T*))
{
   for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     bool match=1;
     for (typename std::vector<const Criteria*>::const_iterator crit=crits_.begin();crit!=crits_.end();++crit)
       match *= (**crit)(*it);
     if (!match) continue;  

     for (typename std::vector< double(*)(const T*) >::const_iterator vit=var_.begin();vit!=var_.end();++vit)
       std::cout << (*vit)(*it) << ", ";
     std::cout << std::endl;
     }
}


template<class T>
void PrintTools<T>::Print(const TGraph *g,const std::string& x, const std::string& y){
  for (int j=0; j<g->GetN(); ++j) {
    double gx, gy;
    g->GetPoint(j, gx, gy);
    
    bool match=1;
    for (typename std::vector<const Criteria*>::const_iterator crit=crits_.begin();crit!=crits_.end();++crit)
       match *= (**crit)(gx, gy);
    if (!match) continue;  

    std::cout << x << gx << y << gy << std::endl;
  }
}


template class PrintTools<SusyScan>;
template class PrintTools<GeneratorMasses>;
