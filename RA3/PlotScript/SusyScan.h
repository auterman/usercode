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
	double signal_acceptance;
	double ExpXsecLimit;
	double ObsXsecLimit;
	double ExpXsecLimitM1;
	double ExpXsecLimitP1;
	double Xsection;

private:
	void SetPtr();

};

#endif
