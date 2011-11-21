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
	ngen  = config.read<double> ("signal.ngen", 0);
	signal_acceptance    = config.read<double> ("signal.acceptance", 0);
	signal_contamination = config.read<double> ("signal.contamination", 0);


	//Xsection 	= config.read<double>("Xsection", 0);
	NLOXsection = config.read<double> ("Xsection.NLO", 0);
	LOXsection = config.read<double> ("Xsection.LO", 0);

	ExpR = config.read<double> ("CLs expected", 9999);
	ObsR = config.read<double> ("CLs observed", 9999);
	ExpRM1 = config.read<double> ("CLs expected m1sigma", 9999);
	ExpRP1 = config.read<double> ("CLs expected p1sigma", 9999);
	ExpRM2 = config.read<double> ("CLs expected m2sigma", 9999);
	ExpRP2 = config.read<double> ("CLs expected p2sigma", 9999);

	ExpRasym   = config.read<double> ("CLs expected asymptotic", 9999);
	ObsRasym   = config.read<double> ("CLs observed asymptotic", 9999);
	ExpRasymM1 = config.read<double> ("CLs expected asymptotic m1sigma", 9999);
	ExpRasymP1 = config.read<double> ("CLs expected asymptotic p1sigma", 9999);
	ExpRasymM2 = config.read<double> ("CLs expected asymptotic m2sigma", 9999);
	ExpRasymP2 = config.read<double> ("CLs expected asymptotic p2sigma", 9999);


	Luminosity = config.read<double> ("Luminosity", 9999);

	background = config.read<double> ("background", 9999);
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
	p.push_back(&NLOXsection);
	p.push_back(&LOXsection);

	p.push_back(&ObsR);
	p.push_back(&ExpR);
	p.push_back(&ExpRM1);
	p.push_back(&ExpRP1);
	p.push_back(&ExpRM2);
	p.push_back(&ExpRP2);

	p.push_back(&ObsRasym);
	p.push_back(&ExpRasym);
	p.push_back(&ExpRasymM1);
	p.push_back(&ExpRasymP1);
	p.push_back(&ExpRasymM2);
	p.push_back(&ExpRasymP2);
	p.push_back(&Luminosity);
	p.push_back(&background);
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
