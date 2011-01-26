#include <cmath>
#include <iostream>
#include "solve.h"
#include "cls.h"
#include "significance.h"


template <class TClass> 
double TSolve<TClass>::_bisection(int n)
{
   int i=0;
   double a=_left, b=_right;
   do {
     double xmid  = a+(b-a)*0.5;
     double mid = (*_pt2Object.*_function)(xmid,_par);
     //std::cout<<"TSolve<TClass>::bisection(left="<<a<<", right="<<b<<"); f(mid)="<<mid<<std::endl;
     if ((*_pt2Object.*_function)(a,_par)*mid<0.) b = xmid;
     if ((*_pt2Object.*_function)(b,_par)*mid<0.) a = xmid;
     if (b-a<epsilon) return xmid;
   } while ( i<n );
   return 0;
}

template <class TClass> 
double TSolve<TClass>::operator()()
{
   //std::cout<<"TSolve<TClass>::operator() " <<std::endl;
   if (!_isBracketed(_left,_right)) BracketFunction(_left,_right);
   return _bisection(); 
}

template <class TClass>
bool TSolve<TClass>::_isBracketed(double l, double r)
{
    //std::cerr<<"TSolve<TClass>::isBracketed(double l="<<l<<", double r="<<r<<") ? "
     //        <<"CLs(l*sigma) ="<<(*_pt2Object.*_function)(l,_par)+0.05 
	//     <<" <-> CLs(r*sigma)="<< (*_pt2Object.*_function)(r,_par)+0.05<<std::endl;
   return (*_pt2Object.*_function)(l,_par) * (*_pt2Object.*_function)(r,_par) <= 0.0;
}


template <class TClass>
void TSolve<TClass>::BracketFunction(double l, double r)
{
  //std::cout<<"TSolve<TClass>::BracketFunction(double l="<<l<<", double r="<<r<<")"<<std::endl;
  //double ix = epsilon;
  double ix = 1;
  int i=0;
  do {
    if (i>5) throw TSolveException("Not converged! Cannot be bracketed!");

    ix *= 2.;
    ++i;
//  } while(!_isBracketed(l-ix,r+ix) && ix<100000);
//  _left  = l-ix;
//  _right = r+ix;
  } while(!_isBracketed(l/ix,r*ix) && ix<100000 );
  _left  = l/ix;
  _right = r*ix;
}

template class TSolve<cls>;
template class TSolve<TSignificance>;
