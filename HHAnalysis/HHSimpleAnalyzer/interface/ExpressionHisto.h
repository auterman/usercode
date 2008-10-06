// -*- C++ -*-
//
// Package:    HistoAnalyzer
// Class:      HistoAnalyzer
//
/**\class HistoAnalyzer HistoAnalyzer.cc Prototyping/HistoAnalyzer/src/HistoAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Benedikt HEGNER
//         Created:  Mon May  7 15:31:00 CEST 2007
// $Id: ExpressionHisto.h,v 1.1.1.4 2008/02/25 15:54:04 auterman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// code specific includes
#include "PhysicsTools/Parser/interface/MethodMap.h"
#include "Reflex/Type.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/Parser/interface/MethodMap.h"
#include "PhysicsTools/Parser/interface/cutParser.h"
#include "PhysicsTools/Parser/interface/ExpressionVar.h"

//#include "Object.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1.h"
#include <string>
#include <sstream>
#include <ostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>


//
// class decleration
//
template<typename T>
class ExpressionHisto {

 private:
  int nbins;
  double min, max;
  std::string name, plotquantity;
  TH1F * hist;
  ROOT::Reflex::Type & type;
  reco::parser::ExpressionVar * expr;

 public:
  ExpressionHisto(const edm::ParameterSet& iConfig);
  ~ExpressionHisto();

  void initialize(edm::Service<TFileService>& fs);
  void fill( T element);
};

template<typename T>
ExpressionHisto<T>::ExpressionHisto(const edm::ParameterSet& iConfig)
{
    min = iConfig.template getUntrackedParameter<double>("min");
    max = iConfig.template getUntrackedParameter<double>("max");
    nbins = iConfig.template getUntrackedParameter<int>("nbins");
    name = iConfig.template getUntrackedParameter<std::string>("name");
    plotquantity = iConfig.template getUntrackedParameter<std::string>("plotquantity");
    reco::MethodMap methods_ = reco::MethodMap::methods<T>();
    reco::MethodMap::const_iterator m = methods_.find(plotquantity);
    type = methods_.type();
    if (m==methods_.end()) 
      throw cms::Exception("BadConfig") << "ExpressionHisto: Cannot find expression ("
				        << plotquantity << ", " << name << ").";
    expr = new reco::parser::ExpressionVar( m->second );
}

template<typename T>
ExpressionHisto<T>::~ExpressionHisto()
{
  delete expr;
}


template<typename T>
void ExpressionHisto<T>::initialize(edm::Service<TFileService>& fs)
{
  hist = fs->template make<TH1F>(name.c_str(),plotquantity.c_str(),nbins,min,max);
}

template<typename T>
void ExpressionHisto<T>::fill(T element)
{ 
  hist->Fill(expr->value(ROOT::Reflex::Object(type, &element))); 
}


