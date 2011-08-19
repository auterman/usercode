//system
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>

//cut on MET > 100
//Aug11, cut on HT400, 1.143/fb, jet cut extended to eta 2.6
const double data = 57;
const double background = 68.36;
const double backgroundUncertainty = sqrt(pow(9.54, 2) + pow(8.96, 2));
//cut on MET > 200
//Aug11, cut on HT400, 1.143/fb, jet cut extended to eta 2.6
const double data200 = 7;
const double background200 = 7.24;
const double backgroundUncertainty200 = sqrt(1.53 * 1.53 + 2.6 * 2.6);
//cut on MET > 350
//Aug11, cut on HT400, 1.143/fb, jet cut extended to eta 2.6
const double data350 = 1;
const double background350 = 1.08;
const double backgroundUncertainty350 = sqrt(0.14 * 0.14 + 0.89 * 0.89);
//LUMI
const double luminosity = 1143.0;
const double luminosityUncertainty = 0.045 * luminosity; //4.5% as recommended for LP
const double scaleUncertainty = 0.2; //suggested by Yuri, see mail June 30th. !Note:obsolete, is not used for limit calculation

struct point {
	double gluino;
	double squark;
	double chi;
	double xsec;
	double xsecNLO;
	double Oldxsec;
	double OldxsecNLO;
	double Oldkfactor;
	double lumi;
	double u_lumi;
	double kfactor;
	double u_NLO;
	double u_NLO_up;
	double u_NLO_dn;
	double acc;
	double u_acc;
	double u_pdf;
	double u_pdfacc;
	double u_stat;
	double signal_contamination;
	double signal_controlregion;
	double back;
	double u_back;
	double data;
	double ngen;
	double triggAcc;
	double triggAcc2;
};

class points {
public:
	point* Get(double gl, double sq, double chi) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
			if (it->gluino == gl && it->squark == sq && it->chi == chi)
				return &(*it);
		return 0;
	}
	void Add(point p) {
		p_.push_back(p);
	}
	void Reset() {
		p_.clear();
	}
	void Write(const std::string dir) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
			std::ofstream ofile;
			std::stringstream ss;
			ss << dir << "_" << it->squark << "_" << it->gluino << "_"
					<< it->chi << ".txt";
			ofile.open(ss.str().c_str());
			ofile << "gluino = " << it->gluino << "\n";
			ofile << "squark = " << it-> squark << "\n";
			ofile << "chi1 = " << it-> chi << "\n";
			ofile << "Xsection = " << it-> xsec << "\n";
			ofile << "NLOXsection = " << it-> xsecNLO << "\n";
			ofile << "OldXsection = " << it-> Oldxsec << "\n";
			ofile << "OldNLOXsection = " << it-> OldxsecNLO << "\n";
			ofile << "Luminosity = " << it-> lumi << "\n";
			ofile << "Luminosity.uncertainty = " << it-> u_lumi << "\n";
			ofile << "Oldkfactor = " << it-> Oldkfactor << "\n";
			ofile << "kfactor = " << it-> kfactor << "\n";
			ofile << "signal.scale.uncertainty = " << it-> u_NLO << "\n";
			ofile << "signal.scale.uncertainty.up = " << it-> u_NLO_up << "\n";
			ofile << "signal.scale.uncertainty.dn = " << it-> u_NLO_dn << "\n";
			ofile << "signal.acceptance = " << it-> acc << "\n";
			ofile << "signal.Stat.uncertainty = " << it->u_stat << "\n";
			ofile << "signal.acceptance.uncertainty = " << it-> u_acc << "\n";
			ofile << "signal.PDF.uncertainty = " << it-> u_pdf << "\n";
			ofile << "signal.PDFacc.uncertainty = " << it-> u_pdfacc << "\n";
			ofile << "background = " << it-> back << "\n";
			ofile << "background.uncertainty = " << it-> u_back << "\n";
			ofile << "data = " << it-> data << "\n";
			ofile << "signal.ngen = " << it->ngen << "\n";

			ofile << "signal.triggAcc = " << it->triggAcc << "\n";
			ofile << "signal.triggAcc2 = " << it->triggAcc2 << "\n";

			ofile << "signal.contamination = " << it->signal_contamination
					<< "\n";
			std::cout << "WRITE acc:" << it->acc << std::endl;
			std::cout << "WRITE GL:" << it->gluino << std::endl;
			std::cout << "WRITE SQ:" << it->squark << std::endl;
			ofile << "signal.controlregion = " << it->signal_controlregion
					<< "\n";
			ofile.close();
		}
	}
private:
	std::vector<point> p_;
} Points;

void ReadXsec(const std::string filelist, bool useCut2 = false, bool useCut3 =
		false, double neutr = 375) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.data = data;
	p.back = background;
	p.u_back = backgroundUncertainty;
	if (useCut2) {
		p.data = data200;
		p.back = background200;
		p.u_back = backgroundUncertainty200;
	}
	if (useCut3) {
		p.data = data350;
		p.back = background350;
		p.u_back = backgroundUncertainty350;
	}
	if (useCut2 && useCut3) {
		std::cout
				<< "ERROR---------which cut do you want to use for Acceptance?!?"
				<< std::endl;
	}
	p.lumi = luminosity;
	p.u_lumi = luminosityUncertainty;
	p.u_NLO = scaleUncertainty;
	//  grid[ig].xsecRSErrorPos = rsp/xsec;  // convert to relative error
	//grid[ig].xsecRSErrorNeg = rsm/xsec;  // convert to relative error

	double nAcc;
	double LO_dn, LO_up;
	while (1) {
		masses_file >> p.squark >> p.gluino >> p.chi >> p.xsec >> LO_up
				>> LO_dn >> p.xsecNLO >> p.u_NLO_up >> p.u_NLO_dn;

		if (!masses_file.good())
			break;

		//p.kfactor = p.xsecNLO / p.xsec;
		std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
		std::cout << "sq" << p.squark << std::endl;
		std::cout << "gl" << p.gluino << std::endl;
		std::cout << "neutr" << p.chi << std::endl;
		if (p.chi == neutr) {
			point * a = 0;
			a = Points.Get(p.gluino, p.squark, p.chi);
			if (!a) {
				Points.Add(p);
			}

		}

	}

}
void ReadSignalAcceptance(const std::string filelist,
		double applyFilterEffNEvts = 0, bool useCut2 = false, bool useCut3 =
				false, double neutralinomass = 375) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		double muell;
		double acc200, sigcont200;
		double acc350, sigcont350;
		masses_file >> p.squark >> p.gluino >> p.acc >> p.signal_contamination
				>> p.ngen >> acc200 >> sigcont200 >> acc350 >> sigcont350
				>> muell >> muell >> muell >> muell >> muell >> muell >> muell
				>> muell >> muell >> muell >> p.triggAcc >> p.triggAcc2;
		std::cout << "neutralino:" << neutralinomass << std::endl;
		std::cout << "squark:" << p.squark << std::endl;
		std::cout << "gl:" << p.gluino << std::endl;
		std::cout << "ngen:" << p.ngen << std::endl;
		if (applyFilterEffNEvts > 0) {
			p.ngen = applyFilterEffNEvts;
		}
		std::cout << "ngen:" << p.ngen << std::endl;
		if (useCut2) {
			p.acc = acc200;
			p.signal_contamination = sigcont200;
		}
		if (useCut3) {
			p.acc = acc350;
			p.signal_contamination = sigcont350;
		}
		if (useCut2 && useCut3) {
			std::cout
					<< "ERROR---------which cut do you want to use for Acceptance?!?"
					<< std::endl;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, neutralinomass);
		if (p.ngen == 0 && p.acc != 0) {
			std::cout << "ERROR---------n gen == 0 ?!?" << std::endl;
			//break;
		}
		if (a) {
			a->ngen = p.ngen;
			a->u_stat = sqrt(p.acc) / p.ngen;

			a->triggAcc = p.triggAcc / p.ngen;
			a->triggAcc2 = p.triggAcc2 / p.ngen;

			a->acc = p.acc / p.ngen;
			double perc_uncert_JES = 0.02;
			double scale_Photon_dataMC = 0.953;
			double scale_Photon_dataMC_uncert = 0.068;
			double perc_uncert_Photon_dataMC = scale_Photon_dataMC_uncert
					/ scale_Photon_dataMC;
			//rescale acceptance & signal cont with photon data/MC scale
			a->acc = (a->acc) * scale_Photon_dataMC;
			a->u_stat = (a->u_stat) * scale_Photon_dataMC;

			a->signal_contamination = p.signal_contamination / p.ngen;
			a->signal_contamination = a->signal_contamination
					* scale_Photon_dataMC;

			//calculate combined error
			a->u_acc = a->acc * sqrt(pow(a->u_stat, 2) + pow(a->u_pdfacc, 2)
					+ pow(perc_uncert_JES, 2) + pow(perc_uncert_Photon_dataMC,
					2));
			bool addPDFXSecUncertToAcc = false;
			if (addPDFXSecUncertToAcc) {
				double u_renormalisation = ((a->u_NLO_up / a->u_NLO)
						+ (a->u_NLO_up / a->u_NLO)) / 2;
				a->u_acc = a->acc * sqrt(pow(a->u_stat, 2) + pow(a->u_pdfacc, 2) + pow(
						a->u_pdf, 2) + pow(u_renormalisation, 2) +
						pow(perc_uncert_JES, 2) + pow(
						perc_uncert_Photon_dataMC, 2));
			}

			std::cout << "lumi=" << a->lumi << ", xsec=" << a->xsec << ", eff="
					<< a->acc << " +- " << a->u_acc << ", SC="
					<< a->signal_contamination << ", Ngen=" << a->ngen
					<< ", trigger acceptance=" << a->triggAcc2 << std::endl;
		}

	}
	masses_file.close();
}
void ReadPDF(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.u_pdf;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->u_pdf = 0.01 * p.u_pdf;
		point * a150 = 0;
		a150 = Points.Get(p.gluino, p.squark, 150);
		if (a150)
			a150->u_pdf = 0.01 * p.u_pdf;

	}
	masses_file.close();
}
void ReadPDFAcceptance(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.u_pdfacc;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->u_pdfacc = 0.01 * p.u_pdfacc;
		point * a150 = 0;
		a150 = Points.Get(p.gluino, p.squark, 150);
		if (a150)
			a150->u_pdfacc = 0.01 * p.u_pdfacc;

	}
	masses_file.close();
}

void ReadOldkFactor(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.Oldkfactor;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->Oldkfactor = p.Oldkfactor;

	}
	masses_file.close();
}

int main(void) {
	////////////////Bino Limits
	//MET>100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt");
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	ReadSignalAcceptance("signalAcceptanceBino_V7.dat");
	Points.Write("limits/GMSBBino100");
	//MET>200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt", true, false);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBino_V7.dat", 0, true, false);
	Points.Write("limits/GMSBBino200");
	//MET>350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt", false, true);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBino_V7.dat", 0, false, true);
	Points.Write("limits/GMSBBino350");
	////////////////Wino Limits
	//MET >100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt");
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V7.dat", 60000);
	Points.Write("limits/GMSBWino100");
	//MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt", true, false);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V7.dat", 60000, true, false);
	Points.Write("limits/GMSBWino200");
	//MET >350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt", false, true);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V7.dat", 60000, false, true);
	Points.Write("limits/GMSBWino350");

	//Limits with Neutralino Mass 150
	//Wino, MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr150.txt", true, false, 150);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	ReadSignalAcceptance("signalAcceptanceWinoNeutr150_V8.dat", 20000, true,
			false, 150);
	Points.Write("limits/GMSBWino200");
	//Wino, MET >100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr150.txt", false, false, 150);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	ReadSignalAcceptance("signalAcceptanceWinoNeutr150_V8.dat", 20000, false,
			false, 150);
	Points.Write("limits/GMSBWino100");
	//Bino, MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr150.txt", true, false, 150);
	ReadPDF("PDFcross.txt");
	ReadPDFAcceptance("PDFacceptance.txt");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBinoNeutr150_V6.dat", 0, true, false,
			150);
	Points.Write("limits/GMSBBino200");
}
