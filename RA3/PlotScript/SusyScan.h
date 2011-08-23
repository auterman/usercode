#ifndef SUSYSCAN_H
#define SUSYSCAN_H

#include <string>
#include <vector>

class SusyScan {
public:
	SusyScan();
	SusyScan(const SusyScan&);
	SusyScan(std::string file);

private:
	std::vector<double*> p;
public:
	const SusyScan operator*(const double f) const;
	const SusyScan operator+(const SusyScan& f) const;

	double Mgluino;
	double Msquark;
	double Mchi1;
	double signal_acceptance;
	double signal_contamination;
	double triggerEffN90Hits, triggerEff;
	double ExpXsecLimit;
	double ObsXsecLimit;
	double ExpXsecLimitM1;
	double ExpXsecLimitP1;
	double ExpXsecLimitM2;
	double ExpXsecLimitP2;

	double ExpXsecLimitSigCont;
	double ObsXsecLimitSigCont;
	double ExpXsecLimitM1SigCont;
	double ExpXsecLimitP1SigCont;
	double ExpXsecLimitM2SigCont;
	double ExpXsecLimitP2SigCont;

	double NLOXsection;
	double NLOXSecUp, NLOXSecDown;
	double PDFXsectionErr;

	double Luminosity;
	double background;

private:
	void SetPtr();

};

#endif
