#include "SusyScan.h"
#include "ConfigFile.h"


SusyScan::SusyScan()
{
  SetPtr();
  for (std::vector<double*>::iterator it=p.begin(); it!=p.end(); ++it)
    **it = 0.;
}
SusyScan::SusyScan(const SusyScan& c)
{
  SetPtr();


  std::vector<double*>::iterator it=p.begin();
  std::vector<double*>::const_iterator ct=c.p.begin();
  for (; ct!=c.p.end(); ++it, ++ct)
    **it = **ct;
}


SusyScan::SusyScan(std::string filename)
{
  SetPtr();
  ConfigFile config(filename);

  Mgluino	= config.read<double>("gluino", 0);
  Msquark	= config.read<double>("squark", 0);
 // std::cout <<"MASS:"<< Mgluino << ": "<< Msquark <<std::endl;

  signal_acceptance = config.read<double>("signal.acceptance", 0);
 // std::cout <<"ACC:"<< signal_acceptance << ": "<< signal_acceptance <<std::endl;

  Xsection 	= config.read<double>("Xsection", 0);
  ExpXsecLimit 	= config.read<double>("limit.expected", 0);
  ObsXsecLimit 	= config.read<double>("limit.observed", 0);

  ExpXsecLimitM1 	= config.read<double>("limit.expected.m1sigma", 0);
  ExpXsecLimitP1 	= config.read<double>("limit.expected.p1sigma", 0);


}

void SusyScan::SetPtr()
{
  if (p.size()!=0) return; 

  p.push_back( &Mgluino);
  p.push_back( &Msquark);
  p.push_back( &signal_acceptance);
  p.push_back( &Xsection);
  p.push_back( &ObsXsecLimit);
  p.push_back( &ExpXsecLimit);

  p.push_back( &ExpXsecLimitM1);
  p.push_back( &ExpXsecLimitP1);


}

const SusyScan SusyScan::operator*(const double f) const
{
  SusyScan res(*this);
  for (std::vector<double*>::iterator it=res.p.begin(); it!=res.p.end(); ++it)
    **it *= f;
  return res;
}

const SusyScan SusyScan::operator+(const SusyScan& f) const
{
  SusyScan res(*this);
  std::vector<double*>::iterator it=res.p.begin(); 
  std::vector<double*>::const_iterator fi=  f.p.begin(); 
  for (; it!=res.p.end(); ++it, ++fi)
    **it += **fi;
  return res;
}
