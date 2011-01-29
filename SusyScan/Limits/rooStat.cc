#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooProfileLL.h"
#include "RooAbsPdf.h"
#include "RooStats/HypoTestResult.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooTreeDataStore.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStopwatch.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/UniformProposal.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/NumberCountingPdfFactory.h"
#include "RooStats/ConfInterval.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalFunction.h"
#include "RooStats/ProposalHelper.h"
#include "RooFitResult.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <time.h>
#include "ConfigFile.h"

// use this order for safety on library loading
using namespace RooFit ;
using namespace RooStats ;



double simpleProfile2(ConfigFile * config, string Type, 
                      double dat, double bkg, double bkg_uncertainty, double sig, double
		      sig_uncertainty, double xsec, double ExpNsigLimit ) //absolute uncertainties!
{
  TStopwatch t;
  t.Start();

  /////////////////////////////////////////
  // The Model building stage
  /////////////////////////////////////////
  char * name = new char[1024];
  RooWorkspace* wspace = new RooWorkspace();
  sprintf(name,"Poisson::countingModel(obs[50,0,200],sum(s[50,0,100]*ratioSigEff[1.,0,2.],b[50,0,100]*ratioBkgEff[1.,0.,2.]))");
  wspace->factory(name); // counting model
  //wspace->factory("Gaussian::sigConstraint(1,ratioSigEff,0.257)"); // 5% signal efficiency uncertainty
  //wspace->factory("Gaussian::bkgConstraint(1,ratioBkgEff,0.136)"); // 10% background efficiency uncertainty
  sprintf(name,"Gaussian::sigConstraint(1,ratioSigEff,%f)",sig_uncertainty/sig);
  wspace->factory(name); // 5% signal efficiency uncertainty
  sprintf(name,"Gaussian::bkgConstraint(1,ratioBkgEff,%f)",bkg_uncertainty/bkg);
  wspace->factory(name); // 10% background efficiency uncertainty
  wspace->factory("PROD::modelWithConstraints(countingModel,sigConstraint,bkgConstraint)"); // product of terms
  wspace->Print();

  RooAbsPdf* modelWithConstraints = wspace->pdf("modelWithConstraints"); // get the model
  RooRealVar* obs = wspace->var("obs"); // get the observable
  RooRealVar* s = wspace->var("s"); // get the signal we care about
  RooRealVar* b = wspace->var("b"); // get the background and set it to a constant.  Uncertainty included in ratioBkgEff
  b->setConstant();
  RooRealVar* ratioSigEff = wspace->var("ratioSigEff"); // get uncertaint parameter to constrain
  RooRealVar* ratioBkgEff = wspace->var("ratioBkgEff"); // get uncertaint parameter to constrain
  RooArgSet constrainedParams(*ratioSigEff, *ratioBkgEff); // need to constrain these in the fit (should change default behavior)

  // Create an example dataset with 160 observed events
  obs->setVal(dat);
  b->setVal(bkg);
  s->setVal(sig);
  RooDataSet* data = new RooDataSet("exampleData", "exampleData", RooArgSet(*obs));
  data->add(*obs);

  RooArgSet all(*s, *ratioBkgEff, *ratioSigEff);

  // not necessary
  modelWithConstraints->fitTo(*data, RooFit::Constrain(RooArgSet(*ratioSigEff, *ratioBkgEff)));

  // Now let's make some confidence intervals for s, our parameter of interest
  RooArgSet paramOfInterest(*s);

  ModelConfig modelConfig(new RooWorkspace());
  modelConfig.SetPdf(*modelWithConstraints);
  modelConfig.SetParametersOfInterest(paramOfInterest);


  // First, let's use a Calculator based on the Profile Likelihood Ratio
  //ProfileLikelihoodCalculator plc(*data, *modelWithConstraints, paramOfInterest); 
  ProfileLikelihoodCalculator plc(*data, modelConfig); 
  plc.SetTestSize(.10);
  ConfInterval* lrint = plc.GetInterval();  // that was easy.

  // Let's make a plot
  TCanvas* dataCanvas = new TCanvas("dataCanvas");
  dataCanvas->Divide(2,2);
  
  dataCanvas->cd(3);
  // --- Generate a toyMC sample from composite PDF ---
  //RooDataSet *pseudodata = modelWithConstraints->generate(*b,2000) ;
  // --- Perform extended ML fit of composite PDF to toy data ---
  //modelWithConstraints.fitTo(*pseudodata,Extended());  

  // don't skip drawing, doesnt work without!!!
  RooPlot* mesframe = b->frame();
  modelWithConstraints->plotOn(mesframe);
  mesframe->Draw();

  dataCanvas->cd(1);
  LikelihoodIntervalPlot plotInt((LikelihoodInterval*)lrint);
  plotInt.SetTitle("Profile Likelihood Ratio and Posterior for S");
  plotInt.Draw();

  // Second, use a Calculator based on the Feldman Cousins technique
//  FeldmanCousins fc(*data, modelConfig);
//  fc.UseAdaptiveSampling(true);
//  fc.FluctuateNumDataEntries(false); // number counting analysis: dataset always has 1 entry with N events observed
//  fc.SetNBins(100); // number of points to test per parameter
//  fc.SetTestSize(.10); //95% single sided
//  fc.AdditionalNToysFactor(5);
//  //  fc.SaveBeltToFile(true); // optional
//  ConfInterval* fcint = NULL;
//  fcint = fc.GetInterval();  // that was easy.

  RooFitResult* fit = modelWithConstraints->fitTo(*data, Save(true));

  // Third, use a Calculator based on Markov Chain monte carlo
  // Before configuring the calculator, let's make a ProposalFunction
  // that will achieve a high acceptance rate
  ProposalHelper ph;
  ph.SetVariables((RooArgSet&)fit->floatParsFinal());
  ph.SetCovMatrix(fit->covarianceMatrix());
  ph.SetUpdateProposalParameters(true);
  ph.SetCacheSize(100);
  ProposalFunction* pdfProp = ph.GetProposalFunction();  // that was easy

  MCMCCalculator mc(*data, modelConfig);
  mc.SetNumIters(100000); // steps to propose in the chain
  mc.SetTestSize(.10); // 95% CL single-sided
  mc.SetNumBurnInSteps(100); // ignore first N steps in chain as "burn in"
  mc.SetProposalFunction(*pdfProp);
  mc.SetLeftSideTailFraction(0.5);  // find a "central" interval
  MCMCInterval* mcInt = (MCMCInterval*)mc.GetInterval();  // that was easy

  // Get Lower and Upper limits from Profile Calculator
  double up = ((LikelihoodInterval*) lrint)->UpperLimit(*s);
  cout << Type << ":  d:"<<dat<<", b:"<<bkg<<"+-"<<bkg_uncertainty
       <<";  s:"<<sig<<"+-"<<sig_uncertainty <<std::endl;
  cout << "Profile lower limit on s = " << ((LikelihoodInterval*) lrint)->LowerLimit(*s) << endl;
  cout << "Profile upper limit on s = " << up << endl;
  config->add("RooSimpleProfile.signal."+Type+"UpperLimit", up);
  config->add("RooSimpleProfile.xsec."+Type+"UpperLimit", up/sig * xsec);

//  // Get Lower and Upper limits from FeldmanCousins with profile construction
//  if (fcint != NULL) {
//     double fcul = ((PointSetInterval*) fcint)->UpperLimit(*s);
//     double fcll = ((PointSetInterval*) fcint)->LowerLimit(*s);
//     cout << "FC lower limit on s = " << fcll << endl;
//     cout << "FC upper limit on s = " << fcul << endl;
//     config->add("RooFC.signal."+Type+"UpperLimit", fcul);
//     config->add("RooFC.xsec."+Type+"UpperLimit", fcul/sig * xsec);
//     //TLine* fcllLine = new TLine(fcll, 0, fcll, 1);
//     //TLine* fculLine = new TLine(fcul, 0, fcul, 1);
//     //fcllLine->SetLineColor(kRed);
//     //fculLine->SetLineColor(kRed);
//     //fcllLine->Draw("same");
//     //fculLine->Draw("same");
//     dataCanvas->Update();
//  }
/*

  // Plot MCMC interval and print some statistics
  MCMCIntervalPlot mcPlot(*mcInt);
  mcPlot.SetLineColor(kMagenta);
  mcPlot.SetLineWidth(2);
  mcPlot.Draw("same");
*/
  double mcul = mcInt->UpperLimit(*s);
  double mcll = mcInt->LowerLimit(*s);
  cout << "MCMC lower limit on s = " << mcll << endl;
  cout << "MCMC upper limit on s = " << mcul << endl;
  cout << "MCMC Actual confidence level: "
     << mcInt->GetActualConfidenceLevel() << endl;

  config->add("RooMCMC.signal."+Type+"UpperLimit", mcul);
  config->add("RooMCMC.xsec."+Type+"UpperLimit", mcul/sig * xsec);
/*
  // 3-d plot of the parameter points
  dataCanvas->cd(2);
  // also plot the points in the markov chain
  TTree& chain =  ((RooTreeDataStore*) mcInt->GetChainAsDataSet()->store())->tree();
  chain.SetMarkerStyle(6);
  chain.SetMarkerColor(kRed);
  chain.Draw("s:ratioSigEff:ratioBkgEff","weight_MarkovChain_local_","box"); // 3-d box proporional to posterior

  // the points used in the profile construction
  TTree& parameterScan =  ((RooTreeDataStore*) fc.GetPointsToScan()->store())->tree();
  parameterScan.SetMarkerStyle(24);
  parameterScan.Draw("s:ratioSigEff:ratioBkgEff","","same");

  delete wspace;
  delete lrint;
  delete mcInt;
  delete fcint;
  delete data;
*/
  /// print timing info
  t.Stop();
  t.Print();
  
  return up;
}

void rooStat(int argc, char *argv[])
{
  //myPDF();
  //rooStatOrig();
  if (argc<1){ 
    std::cerr<<"Error: Expected '"<<argv[0]<<" <limit config>'!"<<std::endl;
    exit(1);
  }
  
  for (int file=1; file<argc; ++file){   
    std::cout << "opening: "<<argv[file]<<std::endl;
    ConfigFile config(argv[file]);
    int data = config.read<int>("data");
    double bkg  = config.read<double>("background");
    double bkgUncert = config.read<double>("background.uncertainty");
    double sig  = config.read<double>("signal");
    double sigUncert = config.read<double>("signal.uncertainty");
    double xsec = config.read<double>("Xsection",1.0);
    double ExpNsigLimit = config.read<double>("ExpNsigLimit",100.0);

    simpleProfile2(&config, "Obs", data, bkg, bkgUncert, sig, sigUncert, xsec, ExpNsigLimit);
    simpleProfile2(&config, "Exp", bkg, bkg, bkgUncert, sig, sigUncert, xsec, ExpNsigLimit);

    //write stuff:  
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    ofstream ofile;
    ofile.open (argv[file]);
    if (ofile.good())
      std::cout << "writing '"<<argv[file]<<"'"<<std::endl;
    else if ( (ofile.rdstate() & ifstream::failbit ) != 0 )
      std::cerr << "ERROR opening '"<<argv[file]<<"'! Does the directory exist?"<<std::endl;
    ofile << config.getComment() << asctime (timeinfo) 
	  << config.getComment()<< "\n"
	  << config;
  }
}


int main(int argc, char *argv[])
{
  rooStat(argc,argv);
}
