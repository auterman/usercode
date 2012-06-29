#include "SusyScan.h"
#include "ConfigFile.h"

SusyScan::SusyScan() {
	SetPtr();
	for (std::vector<double*>::iterator it = p.begin(); it != p.end(); ++it)
		**it = 0.;
}
SusyScan::SusyScan(const SusyScan& c) {
	SetPtr();

	std::vector<double*>::iterator it = p.begin();
	std::vector<double*>::const_iterator ct = c.p.begin();
	for (; ct != c.p.end(); ++it, ++ct)
		**it = **ct;
}

SusyScan::SusyScan(std::string filename) {
	SetPtr();
	ConfigFile config(filename);

	Mgluino = config.read<double> ("gluino", 0);
	Msquark = config.read<double> ("squark", 0);
	Mchi1 = config.read<double> ("chi1", 0);

	ngen =config.read<double> ("signal.ngen", 0);
	if(ngen<1)ngen=0;
	signal_acceptance = ngen==0?0:config.read<double> ("signal.acceptance", 0);
	signal_contamination = signal_acceptance == 0 ? 0
		: config.read<double> ("signal.contamination", 0);
	signal_acceptance2j = ngen==0?0:config.read<double> ("signal.acceptance2j", 0);
	//std::cout <<"SigAcc 2jet:"<< signal_acceptance2j <<std::endl;
	signal_contamination2j = signal_acceptance2j == 0 ? 0
		: config.read<double> ("signal.contamination2j", 0);
	triggerEffN90Hits = signal_acceptance == 0 ? 0 : config.read<double> ("signal.triggAcc2", 0);
	triggerEff = signal_acceptance == 0 ? 0 : config.read<double> ("signal.triggAcc", 0);
	//std::cout <<"CONT:"<< signal_contamination << ": "<< signal_contamination <<std::endl;

	//Xsection 	= config.read<double>("Xsection", 0);
	NLOXsection = config.read<double> ("NLOXsection", 0);
	NLOXSecUp = config.read<double> ("signal.scale.uncertainty.up", 0);
	NLOXSecDown = config.read<double> ("signal.scale.uncertainty.dn", 0);
	PDFXsectionErr = config.read<double> ("signal.PDF.uncertainty", 0);

	ExpXsecLimit = signal_acceptance == 0 ? 9999 : config.read<double> ("limit.cls.expected", 9999);
	ObsXsecLimit = signal_acceptance == 0 ? 9999 : config.read<double> ("limit.cls.observed", 9999);

	ExpXsecLimitM1 = signal_acceptance == 0 ? 9999
		: config.read<double> ("limit.cls.expected.m1sigma", 9999);
	ExpXsecLimitP1 = signal_acceptance == 0 ? 9999
		: config.read<double> ("limit.cls.expected.p1sigma", 9999);
	ExpXsecLimitM2 = signal_acceptance == 0 ? 9999
		: config.read<double> ("limit.cls.expected.m2sigma", 9999);
	ExpXsecLimitP2 = signal_acceptance == 0 ? 9999
		: config.read<double> ("limit.cls.expected.p2sigma", 9999);

	ExpXsecLimitSigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.expected", 9999);
	ObsXsecLimitSigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.observed", 9999);

	ExpXsecLimitM1SigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.expected.m1sigma", 9999);
	ExpXsecLimitP1SigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.expected.p1sigma", 9999);

	ExpXsecLimitM2SigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.expected.m2sigma", 9999);
	ExpXsecLimitP2SigCont = signal_acceptance == 0 ? 9999
		: config.read<double> ("limitSC.cls.expected.p2sigma", 9999);

	ExpXsecLimitSigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.expected", 9999);
	ObsXsecLimitSigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.observed", 9999);

	ExpXsecLimitM1SigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.expected.m1sigma", 9999);
	ExpXsecLimitP1SigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.expected.p1sigma", 9999);

	ExpXsecLimitM2SigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.expected.m2sigma", 9999);
	ExpXsecLimitP2SigCont2j = signal_acceptance2j == 0 ? 9999
		: config.read<double> ("limitSC2j.cls.expected.p2sigma", 9999);

	Luminosity = config.read<double> ("Luminosity", 9999);

	background = config.read<double> ("background", 9999);
	background2j = config.read<double> ("background2j", 9999);
	//std::cout <<"SigAcc 2jet -B:"<< signal_acceptance2j <<std::endl;
}

void SusyScan::SetPtr() {
	if (p.size() != 0)
		return;

	p.push_back(&Mgluino);
	p.push_back(&Msquark);
	p.push_back(&Mchi1);
	p.push_back(&ngen);
	p.push_back(&signal_acceptance);
	p.push_back(&signal_contamination);
	p.push_back(&triggerEffN90Hits);
	p.push_back(&triggerEff);
	p.push_back(&NLOXsection);
	p.push_back(&NLOXSecUp);
	p.push_back(&NLOXSecDown);
	p.push_back(&PDFXsectionErr);

	p.push_back(&ObsXsecLimit);
	p.push_back(&ExpXsecLimit);
	p.push_back(&ExpXsecLimitM1);
	p.push_back(&ExpXsecLimitP1);
	p.push_back(&ExpXsecLimitM2);
	p.push_back(&ExpXsecLimitP2);

	p.push_back(&ObsXsecLimitSigCont);
	p.push_back(&ExpXsecLimitSigCont);

	p.push_back(&ExpXsecLimitM1SigCont);
	p.push_back(&ExpXsecLimitP1SigCont);
	p.push_back(&ExpXsecLimitM2SigCont);
	p.push_back(&ExpXsecLimitP2SigCont);

	p.push_back(&signal_acceptance2j);
	p.push_back(&signal_contamination2j);

	p.push_back(&ObsXsecLimitSigCont2j);
	p.push_back(&ExpXsecLimitSigCont2j);

	p.push_back(&ExpXsecLimitM1SigCont2j);
	p.push_back(&ExpXsecLimitP1SigCont2j);
	p.push_back(&ExpXsecLimitM2SigCont2j);
	p.push_back(&ExpXsecLimitP2SigCont2j);

	p.push_back(&Luminosity);
	p.push_back(&background);
	p.push_back(&background2j);
}

const SusyScan SusyScan::operator*(const double f) const {
	SusyScan res(*this);
	for (std::vector<double*>::iterator it = res.p.begin(); it != res.p.end(); ++it)
		**it *= f;
	return res;
}

const SusyScan SusyScan::operator+(const SusyScan& f) const {
	SusyScan res(*this);
	std::vector<double*>::iterator it = res.p.begin();
	std::vector<double*>::const_iterator fi = f.p.begin();
	for (; it != res.p.end(); ++it, ++fi)
		**it += **fi;
	return res;
}