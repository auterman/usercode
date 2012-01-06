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

//use file with best expected limit from both files for setting SusyScan
//needed eg for combination of 2j/3j limit
SusyScan::SusyScan(std::string filename,std::string filename2) {
	SetPtr();
	ConfigFile config1(filename);
	ConfigFile config2(filename2);
	//std::cout<<"make susy scan from two files..."<<std::endl;
	//check which file gives the best expected limit at this point - and use it to set the values
	double ExpLimit1= config1.read<double> ("CLs expected", 9999);
	double ExpLimit2= config2.read<double> ("CLs expected", 9999);
	//std::cout<<"exp limit 1..."<<ExpLimit1<<std::endl;
	//std::cout<<"exp limit 2..."<<ExpLimit2<<std::endl;
	std::string chosenFilename=filename;
	if(ExpLimit2<ExpLimit1){
		chosenFilename=filename2;
	}
	//std::cout<<"best limit in..."<<chosenFilename<<std::endl;
	const SusyScan * bestScan = new SusyScan(chosenFilename);

	//std::cout<<"created SusyScan..."<<chosenFilename<<std::endl;
	std::vector<double*>::iterator it = p.begin();
		std::vector<double*>::const_iterator ct = bestScan->p.begin();
		for (; ct != bestScan->p.end(); ++it, ++ct)
			**it = **ct;
	//std::cout<<"MGluino..."<<Mgluino<<std::endl;
	if(chosenFilename==filename2){
	 File1or2=2;
	}
}

SusyScan::SusyScan(std::string filename) {
	//std::cout<<"create SusyScan..."<<filename<<std::endl;
	ConfigFile config(filename);

	SetPtr();


	Mgluino = config.read<double> ("gluino", 0);
	Msquark = config.read<double> ("squark", 0);
	Mchi1 = config.read<double> ("chi1", 0);
	ngen  = config.read<double> ("signal.ngen", 0);
	signal_acceptance    = config.read<double> ("signal.acceptance", 0);
	if(signal_acceptance==0)signal_acceptance    =config.read<double> ("acceptance", 0);
	signal_acceptance_uncertainty = config.read<double> ("signal.acceptance.uncertainty", 0);
	signal_contamination = config.read<double> ("signal.contamination", 0);
	signal_expectation = config.read<double> ("signal", 0);

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

	NLOXSecUp = config.read<double> ("signal.scale.uncertainty.UP", 0);
	NLOXSecDown = config.read<double> ("signal.scale.uncertainty.DN", 0);
	PDFXsectionErr = config.read<double> ("signal.PDF.uncertainty", 0);
  PDFAccErr =  config.read<double> ("signal.PDFacc.uncertainty", 0);
	File1or2=1;

}



void SusyScan::SetPtr() {
	if (p.size() != 0)
		return;

	p.push_back(&Mgluino);
	p.push_back(&Msquark);
	p.push_back(&Mchi1);
	p.push_back(&ngen);
	p.push_back(&signal_acceptance);
	p.push_back(&signal_acceptance_uncertainty);
	p.push_back(&signal_contamination);
	p.push_back(&signal_expectation);
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

	p.push_back(&NLOXSecUp);
	p.push_back(&NLOXSecDown);
	p.push_back(&PDFXsectionErr);
	p.push_back(&PDFAccErr);

	p.push_back(&File1or2);
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
