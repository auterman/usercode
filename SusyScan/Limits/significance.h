#ifndef SIGNIFICANCE_H
#define SIGNIFICANCE_H

#include <cmath>

class TSignificance
{
 public:
  TSignificance(double b, double sigma):_n(sqrt(b+sigma*sigma)){};
  virtual ~TSignificance(){};
  double operator()(const double s, const double * par){
    return (_n ? s/_n : 0) - par[0]; }; //par[0] being the wished CL

 private:
  double _n;  
};


#endif
