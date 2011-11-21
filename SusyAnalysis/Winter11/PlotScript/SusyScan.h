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
	double ngen;
	double ExpR;
	double ObsR;
	double ExpRM1;
	double ExpRP1;
	double ExpRM2;
	double ExpRP2;
	double ExpRasym;
	double ObsRasym;
	double ExpRasymM1;
	double ExpRasymP1;
	double ExpRasymM2;
	double ExpRasymP2;
	double NLOXsection;
	double LOXsection;
	double Luminosity;
	double background;

private:
	void SetPtr();

};

#endif
