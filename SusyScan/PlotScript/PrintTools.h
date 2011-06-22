#ifndef PRINTTOOLS_H
#define PRINTTOOLS_H

#include <vector>
#include <cmath>
#include "TGraph.h"


class SusyScan;
class TH2;


template<class T>
class PrintTools {
 public:

  class Criteria{
    public:
      Criteria(double(*x)(const T*), double(*y)(const T*), TGraph* g, double r=0.):x_(x),y_(y),g_(g),r2_(r*r){}
      Criteria(double(*x)(const T*), double v, double r=0.):x_(x),v_(v),r_(r){}
      Criteria(double v, double r=0.):v_(v),r_(r){}
      virtual ~Criteria(){}
      virtual bool operator()(const T*t) const = 0;
      virtual bool operator()(double x, double y) const = 0;
      
    protected:  
      bool deltaR_(const T*t) const {
           for (int j=0; j<g_->GetN(); ++j) {
              double gx, gy;
              g_->GetPoint(j, gx, gy);
              if ( (x_(t)-gx)*(x_(t)-gx) +  (y_(t)-gy)*(y_(t)-gy) < r2_)
                return true;
           }
	   return false;
      }
      bool equal_(const T*t) const {
	   return std::abs( x_(t)-v_) < r_;
      }
      bool Xbinning_(double x) const {
           return fmod(x,v_) < r_;
      }
      
    private:
      Criteria();
      double (*x_)(const T*), (*y_)(const T*);
      TGraph *g_;
      double v_,r_,r2_;
  };
  class deltaR: public Criteria{
    public:
      deltaR(double(*x)(const T*), double(*y)(const T*), TGraph* g, double r=0.) : Criteria(x,y,g,r){}
      bool operator()(const T*t) const { return deltaR_(t); }
      bool operator()(double x, double y) const {return false;}
  };
  class equal: public Criteria{
    public:
      equal(double(*x)(const T*), double v, double r=0.) : Criteria(x,v,r){}
      bool operator()(const T*t) const { return equal_(t); }
      bool operator()(double x, double y) const {return false;}
  };
  class Xbinning: public Criteria{
    public:
      Xbinning(double v, double r) : Criteria(v,r){}
      bool operator()(double x, double y) const { return Criteria::Xbinning_(x); }
      bool operator()(const T*t) const { return false; }
  };


  PrintTools(std::vector<T*> * scan):_scan(scan){}
  
  void AddMatch(const Criteria*c){ crits_.push_back(c);};
  void AddVar(  double(*x)(const T*) ){ var_  .push_back(x);};

  void Print(double(*x)(const T*));
  void Print(const TGraph *g,const std::string& x="", const std::string& y="");

 private:
  std::vector<T*> * _scan; 
  std::vector< const Criteria* > crits_;
  std::vector< double(*)(const T*) > var_;
  
 class sort_by{
  public:
   sort_by(double(*x)(const T*)):_f(x){}
   bool operator()(const T*a, const T*b){ return _f(a)<_f(b); }
  private:
   double(*_f)(const T*);
 };
 
 class sort_TGraph{
  public:
   sort_TGraph(){}
   bool operator()(const TGraph*g1, const TGraph*g2);
 };
 
};

#endif
