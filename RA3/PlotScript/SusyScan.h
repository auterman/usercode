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
	double signal_acceptance2j;
		double signal_contamination2j;
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

	double ExpXsecLimitSigCont2j;
		double ObsXsecLimitSigCont2j;
		double ExpXsecLimitM1SigCont2j;
		double ExpXsecLimitP1SigCont2j;
		double ExpXsecLimitM2SigCont2j;
		double ExpXsecLimitP2SigCont2j;

	double NLOXsection;
	double NLOXSecUp, NLOXSecDown;
	double PDFXsectionErr;

	double Luminosity;
	double background;
	double background2j;

private:
	void SetPtr();

};

#endif
