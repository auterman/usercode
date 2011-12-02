#ifndef SUSYSCAN_H
#define SUSYSCAN_H

#include <string>
#include <vector>

class SusyScan {
public:
	SusyScan();
	SusyScan(const SusyScan&);
	SusyScan(std::string file);
	//use file with best expected limit from both files for setting SusyScan
	SusyScan(std::string file,std::string file2);

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
	//default: 1, index of choosen inputfile
	//filled if SusyScan is filled with two input files
	double File1or2;
	//add XS uncertainties
	double NLOXSecUp, NLOXSecDown;
	double PDFXsectionErr;

private:
	void SetPtr();

};

#endif