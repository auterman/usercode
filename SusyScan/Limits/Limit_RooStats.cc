//User
#include "cls.h"
#include "ConfigFile.h"
#include "table.h"

//system
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <time.h>

//ROOT
#include "TH1.h"
#include "TCanvas.h"
#include "TRint.h"
#include "RooRealVar.h"
#include "RooUniform.h"
#include "RooDataSet.h"
#include "RooConstVar.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "RooPoisson.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/HybridCalculatorOriginal.h"
#include "RooStats/HypoTestInverterResult.h"
#include "RooStats/HypoTestInverter.h"

using namespace std;
using namespace RooStats;


void Limit(int argc, char *argv[])
{
  if (argc<1){ 
    std::cerr<<"Error: Expected '"<<argv[0]<<" <limit config>'!"<<std::endl;
    exit(1);
  }
  
  for (int file=1; file<argc; ++file){   
    std::cout << "opening: "<<argv[file]<<std::endl;
    ConfigFile config(argv[file]);

    int    m0 = config.read<int>("Mzero");
    int    m12 = config.read<int>("Mhalf");
    double CLsTLimit  = config.read<double>("NLO.CLs@xsec");
    int    ndata = config.read<int>("data");
    double nback  = config.read<double>("background");
    double bkgUncert = config.read<double>("background.uncertainty");
    double signal  = config.read<double>("signal.LO");
    double sigUncert = config.read<double>("signal.LO.uncertainty");
    double sigSignal = config.read<double>("signal.LO.signalregion.IsoMuon");
    double sigSigTau  = config.read<double>("signal.LO.signalregion.Tau");
    double signalNLO  = config.read<double>("signal.NLO");
    double sigUncertNLO = config.read<double>("signal.NLO.uncertainty");
    double sigSignalNLO = config.read<double>("signal.NLO.signalregion.IsoMuon");
    double sigSigTauNLO = config.read<double>("signal.NLO.signalregion.Tau");
    double bkgControl = config.read<double>("background.controlregion.IsoMuon");
    double bkgSignal  = config.read<double>("background.signalregion.IsoMuon");
  
    double xsec = config.read<double>("Xsection");
    double kfactor = config.read<double>("signal.kFactor");

    //sigUncertNLO =  1e-3;
    //bkgUncert = 5;
    // sigSignalNLO = sigSigTauNLO = 1e-3;

    cout << " background = " << nback << " +/- " << bkgUncert << endl;
    cout << " expected signal = " << signalNLO << " +/- " << sigUncertNLO << endl;
    cout << " contamination = " << sigSignalNLO+sigSigTauNLO << ", corrected background=" << nback-sigSignalNLO-sigSigTauNLO << endl;
    cout << " data = " << ndata << endl;
    // RooFit Model
    // constants
    RooConstVar sigInBackground ("sigInBackground", "sigInBackground", -sigSignalNLO-sigSigTauNLO); sigInBackground.Print("");
    RooConstVar signalUncertainy ("signalUncertainy", "signalUncertainy", sigUncertNLO); signalUncertainy.Print();
    RooConstVar signalEvents ("signalEvents", "signalEvents", signalNLO); signalEvents.Print("");
    RooConstVar backgroundControl ("backgroundControl", "backgroundControl", nback); backgroundControl.Print("");
    RooConstVar backgroundUncertainy ("backgroundUncertainy", "backgroundUncertainy", bkgUncert); backgroundUncertainy.Print("");

   // variables
    RooRealVar signalScale ("signalScale", "signalScale", 0, 0, 10); signalScale.Print(); // POI
    RooRealVar nObserved ("nObserved", "nObserved", ndata, 0, 10*ndata); nObserved.Print(); // observed events
    double minSignal = signalEvents.getVal() - 5*signalUncertainy.getVal();
    if (minSignal < 0) minSignal = 0;
    RooRealVar signalEventsVar ("signalEventsVar", "signalEventsVar", signalEvents.getVal(), minSignal, signalEvents.getVal() + 5*signalUncertainy.getVal()); signalEventsVar.Print(); 
    RooProduct signalMean ("signalMean", "signalMean", RooArgSet(signalScale, signalEventsVar)); signalMean.Print(); 
    double minBkg = nback - 5*bkgUncert;
    if (minBkg < 0) minBkg = 0;
    RooRealVar backgroundVar ("backgroundVar", "backgroundVar", nback, minBkg, nback + 5*bkgUncert); backgroundVar.Print(); 
    RooProduct bkgContamination ("bkgContamination","bkgContamination", RooArgSet(signalScale, sigInBackground)); bkgContamination.Print(); 
    RooAddition backgroundAll ("backgroundAll", "backgroundAll", RooArgSet (backgroundVar, bkgContamination)); backgroundAll.Print(); 
    RooAddition allEvents ("allEvents", "allEvents", RooArgSet (signalMean, backgroundAll)); allEvents.Print(); 

    // pdf
    RooUniform signalPrior ("signalPrior", "signalPrior", signalScale); signalPrior.Print(); 
    RooGaussian backgroundPrior ("backgroundPrior", "backgroundPrior", backgroundVar, backgroundControl, backgroundUncertainy); backgroundPrior.Print(); 
    RooGaussian signalEventsPrior ("signalEventsPrior", "signalEventsPrior", signalEventsVar, signalEvents, signalUncertainy); signalEventsPrior.Print(); 
    RooArgSet nuisances (backgroundVar, signalEventsVar); nuisances.Print(); 
    RooProdPdf prior2 ("prior2", "prior2", RooArgSet (backgroundPrior, signalEventsPrior)); prior2.Print(); 
    RooProdPdf prior3 ("prior3", "prior3", RooArgSet (signalPrior, backgroundPrior, signalEventsPrior)); prior3.Print(); 
    RooPoisson sbPdf ("sbPdf", "sbPdf", nObserved, allEvents); sbPdf.Print(); 
    RooPoisson bPdf ("bPdf", "bPdf", nObserved, backgroundVar); bPdf.Print(); 

    RooDataSet data ("data", "data", nObserved);
    nObserved = ndata;
    data.add (nObserved);

    BayesianCalculator bcalc(data, sbPdf, RooArgSet(signalScale), prior3, &nuisances);
    bcalc.SetConfidenceLevel (0.9);
    RooAbsPdf* posteriorPdf = bcalc.GetPosteriorPdf();
    for (int i = 0; i < 20; ++i) {
      signalScale = i*0.2;
      cout << "posterior " << signalScale.getVal() << " -> " <<  posteriorPdf->getVal() << endl;
    }
    SimpleInterval* interval = bcalc.GetInterval();
    double result = interval->UpperLimit();
    cout << "Bayesian 95%CL UL = " << result << endl;

    HybridCalculatorOriginal hcalc(data, sbPdf, bPdf, &nuisances, &prior2);
    hcalc.PatchSetExtended(false);
    hcalc.UseNuisance(true);
    // hcalc.PrintMore("v");
    signalScale = 1.;
    hcalc.SetNumberOfToys (10000);
    HybridResult* hr = hcalc.GetHypoTest();
    cout << "signal=1 -> CLs=" << hr->CLs() << "+/-" << hr->CLsError()
	 << " CLsb=" << hr->CLsplusb() << "+/-" << hr->CLsplusbError() 
	 << " CLb=" << hr->CLb() << "+/-" << hr->CLbError() << endl;

    cout << m0 << "   "
         << m12 << "   "
         << signalNLO<< "   "
	 << sigSignalNLO+sigSigTauNLO<< "   "
         << CLsTLimit << "   "
         << hr->CLs()
	 << std::endl;

    config.add("NLOHybrid_CLs@xsec",	hr->CLs());
    config.add("NLOHybrid_CLs_error@xsec", hr->CLsError());
    config.add("NLOHybrid_CLsb@xsec",	hr->CLsplusb());
    config.add("NLOHybrid_CLsb_error@xsec",hr->CLsplusbError());
    config.add("NLOHybrid_CLb@xsec",	hr->CLb());
    config.add("NLOHybrid_CLb_error@xsec", hr->CLbError());
    //write stuff:  
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    ofstream ofile;
    ofile.open (argv[file]);
    ofile << config.getComment() << asctime (timeinfo) 
	  << config.getComment()<< "\n"
	  << config;
    ofile.close();

/*
     hcalc.SetNumberOfToys (2000);
     HypoTestInverter myInverter(hcalc, signalScale);
     myInverter.UseCLs(true);
     double targetP = 0.05; // 95% C.L.
     myInverter.RunAutoScan(0, 10, targetP, 0.1*targetP);  
     HypoTestInverterResult* results = myInverter.GetInterval();
     results->SetConfidenceLevel (1-2.*targetP);
     cout << "95%CL Hybrid UL: " <<  results->UpperLimit() << endl;
*/
  }
}


int main(int argc, char *argv[])
{
  int argc_fake = 1;
  //TRint *theApp = new TRint("ROOT example", &argc_fake, argv);
  
  // Init Intrinsics, build all windows, and enter event loop
  // theApp->Run();
  Limit(argc,argv);
}
