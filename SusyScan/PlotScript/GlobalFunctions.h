#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>


double Luminosity = 31.2; //[pb^-1]
double Mzero(const SusyScan* p){ return p->Mzero; }
double Mhalf(const SusyScan* p){ return p->Mhalf; }
double MGluino(const SusyScan* p){ return p->MGL; }
double MSquarkL(const SusyScan* p){ return p->MUL; }
double MSquarkR(const SusyScan* p){ return p->MUR; }
double MChi1(const SusyScan* p){ return p->MZ1; }
double MChi2(const SusyScan* p){ return p->MZ2; }
double MChi3(const SusyScan* p){ return p->MZ3; }
double MChi4(const SusyScan* p){ return p->MZ4; }
double MCha1(const SusyScan* p){ return p->MW1; }
double MCha2(const SusyScan* p){ return p->MW2; }
double Xsection(const SusyScan* p){ return p->Xsection; }
double ExpXsecLimit(const SusyScan* p){ return p->ExpXsecLimit; }
double ObsXsecLimit(const SusyScan* p){ return p->ObsXsecLimit; }
double ExpExclusion(const SusyScan* p){ return (ExpXsecLimit(p)<Xsection(p)&&ExpXsecLimit(p)>0.01?1:-1); }
double ObsExclusion(const SusyScan* p){ return (ObsXsecLimit(p)<Xsection(p)&&ObsXsecLimit(p)>0.01?1:-1); }
double SoverSqrtB(const SusyScan* p){ return p->signal/(sqrt(p->background)+p->background_uncertainty+p->signal_uncertainty); }
double XsecOverObserved(const SusyScan* p){ return (ObsXsecLimit(p)==0 ? 9999. : Xsection(p)/ObsXsecLimit(p)); }
double XsecOverExpected(const SusyScan* p){ return (ObsXsecLimit(p)==0 ? 9999. : Xsection(p)/ObsXsecLimit(p)); }
double SignalAcceptance(const SusyScan* p){ return  p->signal / (Luminosity*Xsection(p)); }
double NSignExpLimit(const SusyScan* p){ return  p->signal * ExpXsecLimit(p)/Xsection(p); }





double Mzero(const GeneratorMasses* p){ return p->Mzero; }
double Mhalf(const GeneratorMasses* p){ return p->Mhalf; }
double MGluino(const GeneratorMasses* p){ return p->MGL; }
double MSquarkL(const GeneratorMasses* p){ return p->MUL; }
double MSquarkR(const GeneratorMasses* p){ return p->MUR; }
double MChi1(const GeneratorMasses* p){ return p->MZ1; }
double MChi2(const GeneratorMasses* p){ return p->MZ2; }
double MChi3(const GeneratorMasses* p){ return p->MZ3; }
double MChi4(const GeneratorMasses* p){ return p->MZ4; }
double MCha1(const GeneratorMasses* p){ return p->MW1; }
double MCha2(const GeneratorMasses* p){ return p->MW2; }
