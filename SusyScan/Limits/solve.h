#ifndef TSOLVE_H
#define TSOLVE_H

#include <string>
#include <exception>


class TSolveException: public std::exception
{
 private:
  std::string except;
 public:
  TSolveException( const std::string& except_ = std::string() ) : except(except_){} 
  virtual const char* what() const throw()  {
    return except.c_str();
  }
  virtual ~TSolveException() throw(){}
};

//Klasse fuer die Extremwertbestimmung
template <class TClass> class TSolve{
 
 public:

  TSolve(TClass* pt2Object, double(TClass::*fpt)(double, const double*), const double*par=0, double l=0., double r=0.):
          _par(par),_left(l),_right(r),epsilon(0.0001)
         { _pt2Object = pt2Object; _function=fpt;};
  
  ~TSolve(){}; //Destructor

  double operator()();

  double GetLeft(){return _left;};
  double GetRight(){return _right;};
  void   SetLeft(double const left){_left=left;};
  void   SetRight(double const right){_right=right;};
  void   SetPar(double const *par){_par=par;};
  void   SetPrecision(double const eps){epsilon=eps;};
  void   BracketFunction(double l, double r);
  
  struct exception {
	 std::string except;
         std::string what(){return except;}		
	 exception( const std::string& except_ = std::string() )
			: except(except_){} };
private:
  TClass* _pt2Object;                      // pointer to object

  bool   _isBracketed(double l, double r);
  double _bisection(int n=100);

  double (TClass::*_function)(double, const double*);  
  const double *_par;
  double _left, _right;
  double epsilon;
};


#endif
