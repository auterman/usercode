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

////Aug11, cut on HT400, 1.143/fb, jet cut extended to eta 2.6
////cut on MET > 100
//const double data = 57;
//const double background = 68.36;
//const double backgroundUncertainty = sqrt(pow(9.54, 2) + pow(8.96, 2));
////cut on MET > 200
//const double data200 = 7;
//const double background200 = 7.24;
//const double backgroundUncertainty200 = sqrt(1.53 * 1.53 + 2.6 * 2.6);
////cut on MET > 350
//const double data350 = 1;
//const double background350 = 1.08;
//const double backgroundUncertainty350 = sqrt(0.14 * 0.14 + 0.89 * 0.89);
//cut on MET > 100

//Sep6, cut on HT450, 1.71/fb, jet cut extended to eta 2.6,>=1photon,>=3 jets
//cut on MET > 100
const double data = 96;
const double background = 127.3;
const double backgroundUncertainty = sqrt(pow(16.9, 2) + pow(9.8, 2));
//cut on MET > 200
const double data200 = 12;
const double background200 = 16.09;
const double backgroundUncertainty200 = sqrt(4.93 * 4.93 + 4.26 * 4.26);
//cut on MET > 350
const double data350 = 2;
const double background350 = 2.3;
const double backgroundUncertainty350 = sqrt(0.37 * 0.37 + 1.52 * 1.52);

//Sep6, cut on HT450, 1.71/fb, jet cut extended to eta 2.6,>=1photon,>=2 jets
//cut on MET > 100
const double data2j = 190;
const double background2j = 254.6;
const double backgroundUncertainty2j = sqrt(pow(36.3, 2) + pow(15.9, 2));
//cut on MET > 200
const double data2002j = 22;
const double background2002j = 32.9;
const double backgroundUncertainty2002j = sqrt(7.1 * 7.1 + 6.4 * 6.4);
//cut on MET > 350
const double data3502j = 3;
const double background3502j = 5.6;
const double backgroundUncertainty3502j = sqrt(0.95 * 0.95 + 2.74 * 2.74);

//LUMI
const double luminosity = 1714.0;
const double luminosityUncertainty = 0.045 * luminosity; //4.5% as recommended for LP
const double scaleUncertainty = 0.2; //suggested by Yuri, see mail June 30th. !Note:obsolete, is not used for limit calculation

struct point {
	//general stuff, independent from selection
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
	double u_pdf;
	double u_pdfacc;
	//information dependent on pre-Selection/MC sample
	double ngen;
	double triggAcc;
	double triggAcc2;
	//information dependent on selection (3-jet)
	double acc;
	double u_acc;
	double u_stat;
	double back;
	double u_back;
	double data;
	double signal_contamination;
	//information dependent on selection (2-jet)
	double acc2j;
	double u_acc2j;
	double u_stat2j;
	double back2j;
	double u_back2j;
	double data2j;
	double signal_contamination2j;
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

			ofile << "signal.PDF.uncertainty = " << it-> u_pdf << "\n";
			ofile << "signal.PDFacc.uncertainty = " << it-> u_pdfacc << "\n";

			ofile << "signal.ngen = " << it->ngen << "\n";
			ofile << "signal.triggAcc = " << it->triggAcc << "\n";
			ofile << "signal.triggAcc2 = " << it->triggAcc2 << "\n";

			ofile << "*********>=3 jets" << "\n";
			ofile << "signal.acceptance = " << it-> acc << "\n";
			ofile << "signal.Stat.uncertainty = " << it->u_stat << "\n";
			ofile << "signal.acceptance.uncertainty = " << it-> u_acc << "\n";
			ofile << "background = " << it-> back << "\n";
			ofile << "background.uncertainty = " << it-> u_back << "\n";
			ofile << "data = " << it-> data << "\n";
			ofile << "signal.contamination = " << it->signal_contamination
					<< "\n";
			ofile << "*********>=2 jets" << "\n";
			ofile << "signal.acceptance2j = " << it-> acc2j << "\n";
			ofile << "signal.Stat.uncertainty2j = " << it->u_stat2j << "\n";
			ofile << "signal.acceptance.uncertainty2j = " << it-> u_acc2j
					<< "\n";
			ofile << "background2j = " << it-> back2j << "\n";
			ofile << "background.uncertainty2j = " << it-> u_back2j << "\n";
			ofile << "data2j = " << it-> data2j << "\n";
			ofile << "signal.contamination2j = " << it->signal_contamination2j
					<< "\n";

			std::cout << "WRITE acc:" << it->acc << std::endl;
			std::cout << "WRITE acc2j:" << it->acc2j << std::endl;
			std::cout << "WRITE GL:" << it->gluino << std::endl;
			std::cout << "WRITE SQ:" << it->squark << std::endl;
			ofile.close();
		}
	}
private:
	std::vector<point> p_;
} Points;

void ReadXsec(const std::string filelist, bool useCut2, bool useCut3,
		double neutr) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.data = data;
	p.back = background;
	p.u_back = backgroundUncertainty;
	p.data2j = data2j;
	p.back2j = background2j;
	p.u_back2j = backgroundUncertainty2j;
	if (useCut2) {
		p.data = data200;
		p.back = background200;
		p.u_back = backgroundUncertainty200;
		p.data2j = data2002j;
		p.back2j = background2002j;
		p.u_back2j = backgroundUncertainty2002j;
	}
	if (useCut3) {
		p.data = data350;
		p.back = background350;
		p.u_back = backgroundUncertainty350;
		p.data2j = data3502j;
		p.back2j = background3502j;
		p.u_back2j = backgroundUncertainty3502j;
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
		masses_file >> p.squark >> p.gluino >> p.chi >> p.xsec >> LO_up >> LO_dn >> p.xsecNLO
			>> p.u_NLO_up >> p.u_NLO_dn;

		if (!masses_file.good())
			break;

		//p.kfactor = p.xsecNLO / p.xsec;
		std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
		std::cout << "sq" << p.squark << std::endl;
		std::cout << "gl" << p.gluino << std::endl;
		std::cout << "neutr" << p.chi << std::endl;
		if (p.chi == neutr || neutr == 0) {
			//reject points were neutralino is heavier than squark/gluino
			if (p.chi < p.squark && p.chi < p.gluino) {

				point * a = 0;
				a = Points.Get(p.gluino, p.squark, p.chi);
				if (!a) {
					Points.Add(p);
				}
			}
		}

	}

}
void mergeBins(std::vector<double> glmasses) {
	double sqmass = 400;

	double nacc = 0;
	double naccu = 0;
	double nsigc = 0;

	for (int i = 0; i < glmasses.size(); i++) {
		std::cout << "merge bin;" << glmasses.at(i);

		point * a = 0;
		a = Points.Get(glmasses.at(i), sqmass, 375);
		nacc = nacc + a->acc;
		naccu = naccu + a->u_acc;
		nsigc = nsigc + a->signal_contamination;
	}
	for (int i = 0; i < glmasses.size(); i++) {
		std::cout << "refill bin;" << glmasses.at(i);

		point * a = 0;
		a = Points.Get(glmasses.at(i), sqmass, 375);
		a->acc = nacc / glmasses.size();
		a->signal_contamination = nsigc / glmasses.size();
		a->u_acc = naccu / glmasses.size();
	}

}
void ReadSignalAcceptance(const std::string filelist,
		double applyFilterEffNEvts, bool useCut2, bool useCut3,
		double neutralinomass) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		double muell;
		double acc200, sigcont200;
		double acc350, sigcont350;
		double acc2002j, sigcont2002j;
		double acc3502j, sigcont3502j;
		masses_file >> p.squark >> p.gluino >> p.chi >> p.ngen >> p.acc
				>> p.signal_contamination >> acc200 >> sigcont200 >> acc350
				>> sigcont350 >> p.acc2j >> p.signal_contamination2j
				>> acc2002j >> sigcont2002j >> acc3502j >> sigcont3502j
				>> p.triggAcc >> p.triggAcc2;

		std::cout << "neutralino:" << p.chi << std::endl;
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
			p.acc2j = acc2002j;
			p.signal_contamination2j = sigcont2002j;
		}
		if (useCut3) {
			p.acc = acc350;
			p.signal_contamination = sigcont350;
			p.acc2j = acc3502j;
			p.signal_contamination2j = sigcont3502j;
		}
		if (useCut2 && useCut3) {
			std::cout
					<< "ERROR---------which cut do you want to use for Acceptance?!?"
					<< std::endl;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (p.ngen == 0 && p.acc != 0) {
			std::cout << "ERROR---------n gen == 0 ?!?" << std::endl;
			//break;
		}
		if (a) {
			a->ngen = p.ngen;
			a->u_stat = sqrt(p.acc) / p.ngen;
			a->u_stat2j = sqrt(p.acc2j) / p.ngen;

			a->triggAcc = p.triggAcc / p.ngen;
			a->triggAcc2 = p.triggAcc2 / p.ngen;

			a->acc = p.acc==0?0:p.acc / p.ngen;
			a->acc2j = p.acc==0?0:p.acc2j / p.ngen;
			double perc_uncert_JES = 0.02;
			double scale_Photon_dataMC = 0.953;
			double scale_Photon_dataMC_uncert = 0.068;
			double perc_uncert_Photon_dataMC = scale_Photon_dataMC_uncert
					/ scale_Photon_dataMC;
			//rescale acceptance & signal cont with photon data/MC scale
			a->acc = (a->acc) * scale_Photon_dataMC;
			a->u_stat = (a->u_stat) * scale_Photon_dataMC;
			a->acc2j = (a->acc2j) * scale_Photon_dataMC;
			a->u_stat2j = (a->u_stat2j) * scale_Photon_dataMC;

			a->signal_contamination = p.signal_contamination / p.ngen;
			a->signal_contamination = a->signal_contamination
					* scale_Photon_dataMC;
			a->signal_contamination2j = p.signal_contamination2j / p.ngen;
			a->signal_contamination2j = a->signal_contamination2j
					* scale_Photon_dataMC;

			//calculate combined error
			a->u_acc = a->acc * sqrt(pow(a->u_stat, 2) + pow(a->u_pdfacc, 2)
					+ pow(perc_uncert_JES, 2) + pow(perc_uncert_Photon_dataMC,
					2));
			a->u_acc2j = a->acc2j * sqrt(pow(a->u_stat2j, 2) + pow(a->u_pdfacc,
					2) + pow(perc_uncert_JES, 2) + pow(
					perc_uncert_Photon_dataMC, 2));
			bool addPDFXSecUncertToAcc = false;
			if (addPDFXSecUncertToAcc) {
				double u_renormalisation = ((a->u_NLO_up / a->u_NLO)
						+ (a->u_NLO_up / a->u_NLO)) / 2;
				a->u_acc = a->acc * sqrt(pow(a->u_stat, 2)
						+ pow(a->u_pdfacc, 2) + pow(a->u_pdf, 2) + pow(
						u_renormalisation, 2) + pow(perc_uncert_JES, 2) + pow(
						perc_uncert_Photon_dataMC, 2));
				a->u_acc2j = a->acc2j * sqrt(pow(a->u_stat2j, 2) + pow(
						a->u_pdfacc, 2) + pow(a->u_pdf, 2) + pow(
						u_renormalisation, 2) + pow(perc_uncert_JES, 2) + pow(
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
void ReadPDF(const std::string filelist, double neutralinomass) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> p.u_pdf;
		} else {
			masses_file >> p.gluino >> p.squark >> p.u_pdf;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a)
			a->u_pdf = 0.01 * p.u_pdf;

	}
	masses_file.close();
}
void ReadPDFAcceptance(const std::string filelist, double neutralinomass) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> p.u_pdfacc;
		} else {
			masses_file >> p.gluino >> p.squark >> p.u_pdfacc;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a)
			a->u_pdfacc = 0.01 * p.u_pdfacc;

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
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt", false, false, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceBinoNeutr375_V10.dat", 0, false,
			false, 375);
	Points.Write("limits/GMSBBino100");
	//MET>200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt", true, false, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceBinoNeutr375_V10.dat", 0, true,
			false, 375);
	//	std::vector<double> glmasses;
	//glmasses.push_back(800);
	//glmasses.push_back(880);
	//glmasses.push_back(960);
	//glmasses.push_back(1040);
	// mergeBins(glmasses);
	//glmasses.clear();
	//
	//glmasses.push_back(1120);
	//glmasses.push_back(1200);
	//glmasses.push_back(1280);
	//glmasses.push_back(1360);
	// mergeBins(glmasses);
	//glmasses.clear();
	//glmasses.push_back(1440);
	//glmasses.push_back(1520);
	//glmasses.push_back(1600);
	//glmasses.push_back(1680);
	////mergeBins(glmasses);
	////glmasses.clear();
	//
	//glmasses.push_back(1760);
	//glmasses.push_back(1840);
	//glmasses.push_back(1920);
	//glmasses.push_back(2000);
	//mergeBins(glmasses);
	//glmasses.clear();


	Points.Write("limits/GMSBBino200");
	//MET>350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr375.txt", false, true, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceBinoNeutr375_V10.dat", 0, false,
			true, 375);
	Points.Write("limits/GMSBBino350");
	////////////////Wino Limits
	//MET >100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt", false, false, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceWinoNeutr375_V10.dat", 60000, false,
			false, 375);
	Points.Write("limits/GMSBWino100");
	//MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt", true, false, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceWinoNeutr375_V10.dat", 60000, true,
			false, 375);
	Points.Write("limits/GMSBWino200");
	//MET >350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr375.txt", false, true, 375);
	ReadPDF("PDFcross.txt", 375);
	ReadPDFAcceptance("PDFacceptance.txt", 375);
	ReadSignalAcceptance("signalAcceptanceWinoNeutr375_V10.dat", 60000, false,
			true, 375);
	Points.Write("limits/GMSBWino350");

	//Limits with Neutralino Mass 150
	//Wino, MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr150.txt", true, false, 150);
	ReadPDF("PDFcross.txt", 150);
	ReadPDFAcceptance("PDFacceptance.txt", 150);
	ReadSignalAcceptance("signalAcceptanceWinoNeutr150_V10.dat", 20000, true,
			false, 150);
	Points.Write("limits/GMSBWino200");
	//Wino, MET >100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Wino_Neutr150.txt", false, false, 150);
	ReadPDF("PDFcross.txt", 150);
	ReadPDFAcceptance("PDFacceptance.txt", 150);
	ReadSignalAcceptance("signalAcceptanceWinoNeutr150_V10.dat", 20000, false,
			false, 150);
	Points.Write("limits/GMSBWino100");
	//Bino, MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_Neutr150.txt", true, false, 150);
	ReadPDF("PDFcross.txt", 150);
	ReadPDFAcceptance("PDFacceptance.txt", 150);
	ReadSignalAcceptance("signalAcceptanceBinoNeutr150_V10.dat", 0, true,
			false, 150);
	Points.Write("limits/GMSBBino200");

	//Bino, MET>200, Neutralino-Scan
	Points.Reset();
	ReadXsec("NLOProspinoXsecs_Bino_NeutrScan.txt", true, false, 0);
	ReadPDF("PDFcrossBino_NeutrScan.txt", 0);
	ReadPDFAcceptance("PDFacceptance_NeutrScan.txt", 0);
	ReadSignalAcceptance("signalAcceptanceBinoNeutrScan_V10.dat", 0, true,
			false, 0);
	Points.Write("limits/GMSBBinoNeutrScan200");

}
