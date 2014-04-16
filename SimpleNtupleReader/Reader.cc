#include "Reader.h"
#include "Interface.h"
#include "Tools.h"

#include "TChain.h"
#include "TError.h"

const static bool ONLY_QCD = true;

template <typename T>
bool Status(T*t, Long64_t i, Long64_t n, double w)
{
   if (i==0)             std::cout <<" > "<< n << " events: " << std::flush;
   if (n&&(n/10)&&(i%(n/10))==0) std::cout << i/(n/10)*10 << "% "<< std::flush;
   if (i==n-1)           std::cout<<std::endl;
   return true;
}


template <typename T>
void Process(const std::string& str_chain, std::vector<Processor<T>*>& proc, const std::string& file, double w)
{
    std::cout<<"File: "<<file<<std::endl<<"----------------------------------------------------------"<<std::endl;
    TChain * chain = new TChain(str_chain.c_str());
    chain->Add(file.c_str());
    Interface<T> * t = new Interface<T>( chain, w );
    t->Process( proc );
    t->Terminate();  
    delete t;
    delete chain;
}




int Reader()
{ 
  gErrorIgnoreLevel = 5000;
  
  const std::string version = "V03.16";

  ///todo: -Recoil HT, phi; MHT
  ///      -full repro all samples
  ///      -ggf weighting in phi(met,g), recoil, ...
  
  Yields tight_g("tightly isolated photon");
  Yields loose_g("loosely isolated photon");

  std::cout << "Photon-Jet Photon Tree\n======================" <<std::endl;
  std::vector<Processor<GJets_Photon>*> v_gjets_g;
  Plotter<GJets_Photon> gjets_g("plots/"+version+"/GJets_Photon");
  Weighter<GJets_Photon> weights_gj_g("GJets_Photon");
  Closure<GJets_Photon> direct_gj("Direct_GJets");
  Cutter<GJets_Photon> presel_gj_g("Presel_GJets_Photon");
  DoubleCountFilter<GJets_Photon> double_gj_g("DoublicateFilter_GJets_Photon");
  Cutter_tightID<GJets_Photon> tightID_gj_g("TightPhotonId_GJets_Photon");
  direct_gj.Book();
  gjets_g.Book();
  gjets_g.Add( Status<GJets_Photon> );
  v_gjets_g.push_back( &tightID_gj_g );
  v_gjets_g.push_back( &double_gj_g );
  v_gjets_g.push_back( &presel_gj_g );
  v_gjets_g.push_back( &gjets_g );
  v_gjets_g.push_back( &weights_gj_g );
  v_gjets_g.push_back( &direct_gj );
  if (!ONLY_QCD){
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  gjets_g.Write();
  tight_g.Add( weights_gj_g.GetYields() );
  }
  
  std::cout << "QCD Photon Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Photon>*> v_qcd_g;
  Plotter<QCD_Photon> qcd_g("plots/"+version+"/Qcd_Photon");
  Weighter<QCD_Photon> weights_qcd_g("Qcd_Photon");
  Closure<QCD_Photon> direct_qcd("Direct_QCD");
  Cutter<QCD_Photon> presel_qcd_g("Presel_QCD_Photon");
  DoubleCountFilter<QCD_Photon> double_qcd_g("DoublicateFilter_QCD_Photon");
  Cutter_tightID<QCD_Photon> tightID_qcd_g("TightPhotonId_QCD_Photon");
  direct_qcd.Book();
  qcd_g.Book();
  qcd_g.Add( Status<QCD_Photon> );
  v_qcd_g.push_back( &tightID_qcd_g );
  v_qcd_g.push_back( &double_qcd_g );
  v_qcd_g.push_back( &presel_qcd_g );
  v_qcd_g.push_back( &qcd_g );
  v_qcd_g.push_back( &weights_qcd_g );
  v_qcd_g.push_back( &direct_qcd );
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  qcd_g.Write();
  tight_g.Add( weights_qcd_g.GetYields() );
  
  std::cout << "Photon-Jet Jet Tree\n======================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> v_gjets_j;
  Plotter<GJets_Jet> gjets_j("plots/"+version+"/GJets_Jet");
  Weighter<GJets_Jet> weights_gj_j("GJets_Jet");
  Cutter<GJets_Jet> presel_gj_j("Presel_GJets_Jet");
  DoubleCountFilter<GJets_Jet> double_gj_j("DoublicateFilter_GJets_Jet");
  Cutter_looseID<GJets_Jet> looseID_gj_j("LoosePhotonId_GJets_Jet");
  double_gj_j.Set( double_gj_g.Get() );
  gjets_j.Book();
  gjets_j.Add( Status<GJets_Jet> );
  v_gjets_j.push_back( &looseID_gj_j );
  v_gjets_j.push_back( &double_gj_j );
  v_gjets_j.push_back( &presel_gj_j );
  v_gjets_j.push_back( &gjets_j );
  v_gjets_j.push_back( &weights_gj_j );
  if (!ONLY_QCD){
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  gjets_j.Write();
  loose_g.Add( weights_gj_j.GetYields() );
  }
  
  std::cout << "QCD Jet Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> v_qcd_j;
  Plotter<QCD_Jet> qcd_j("plots/"+version+"/Qcd_Jet");
  Weighter<QCD_Jet> weights_qcd_j("Qcd_Jet");
  Cutter<QCD_Jet> presel_qcd_j("Presel_QCD_Jet");
  DoubleCountFilter<QCD_Jet> double_qcd_j("DoublicateFilter_QCD_Jet");
  Cutter_looseID<QCD_Jet> looseID_qcd_j("LoosePhotonId_QCD_Jet");
  double_qcd_j.Set( double_qcd_g.Get() );
  qcd_j.Book();
  qcd_j.Add( Status<QCD_Jet> );
  v_qcd_j.push_back( &looseID_qcd_j );
  v_qcd_j.push_back( &double_qcd_j );
  v_qcd_j.push_back( &presel_qcd_j );
  v_qcd_j.push_back( &qcd_j );
  v_qcd_j.push_back( &weights_qcd_j );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  qcd_j.Write();
  loose_g.Add( weights_qcd_j.GetYields() );

  
  ///Closure 2nd pass ---------------------------------------------------------------------------------
    

  Closure<QCD_Jet> closure("plots/"+version+"/Closure_Combined");
  Closure<QCD_Jet> closure_qcd("plots/"+version+"/Closure_QCD");
  Closure<GJets_Jet> closure_gj("plots/"+version+"/Closure_GJets");
  direct_gj.Write();
  closure_gj.SetNominator( weights_gj_g.GetYields());   //Zähler, tight isolated
  closure_gj.SetDenominator( weights_gj_j.GetYields()); //Nenner, loose isolated
  closure_gj.AddSignalYields( direct_gj.GetYields());   //Signal
  closure_gj.Book();
  closure_gj.Add( Status<GJets_Jet> );

  direct_qcd.Write();
  closure_qcd.SetNominator( weights_qcd_g.GetYields() );   //Zähler, tight isolated
  closure_qcd.SetDenominator( weights_qcd_j.GetYields() ); //Nenner, loose isolated
  closure_qcd.AddSignalYields( direct_qcd.GetYields() );   //Signal
  closure_qcd.Book();
  closure_qcd.Add( Status<QCD_Jet> );

  if (!ONLY_QCD){
  std::cout << "Photon-Jet Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> vc_gjets_j;
  Cutter<GJets_Jet> cut_gj("Cutter");
  DoubleCountFilter<GJets_Jet> double_gj("DoublicateFilter_GJets");
  Cutter_looseID<GJets_Jet> looseID_gj("LoosePhotonId_GJets");
  double_gj.Set( double_gj_g.Get() );
  vc_gjets_j.push_back( &looseID_gj );
  vc_gjets_j.push_back( &double_gj );
  vc_gjets_j.push_back( &cut_gj );
  vc_gjets_j.push_back( &closure_gj );
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  }
    
  std::cout << "QCD Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> vc_qcd_j;
  Cutter<QCD_Jet> cut_qcd("Cutter");
  DoubleCountFilter<QCD_Jet> double_qcd("DoublicateFilter_QCD");
  Cutter_looseID<QCD_Jet> looseID_qcd("LoosePhotonId_QCD");
  vc_qcd_j.push_back( &looseID_qcd );
  vc_qcd_j.push_back( &double_qcd );
  vc_qcd_j.push_back( &cut_qcd );
  vc_qcd_j.push_back( &closure_qcd );
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  //Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);

  closure_qcd.Write();
  if (!ONLY_QCD) {
  closure_gj.Write();
  
  closure.SetNominator(   &tight_g );   //Zähler, tight isolated
  closure.SetDenominator( &loose_g );   //Nenner, loose isolated
  //Histograms signal_combined("Combined Direct Simulation");
  //signal_combined.Add( direct_qcd.GetHists() );
  //signal_combined.Add( direct_gj.GetHists() );
  //closure.SetSignalHists( &signal_combined );   //Signal
  closure.AddSignalYields( direct_qcd.GetYields() ); 
  closure.AddSignalYields( direct_gj.GetYields() ); 
  closure.Book();
  closure.AddRef(closure_gj.GetYields());
  closure.AddRef(closure_qcd.GetYields());
  closure.Write();
  }
  
  return 0;
}


int main()
{
  for (int i=0; i<=100; i+=2) std::cout<<i<<",";
  std::cout<<std::endl;
  return Reader();
}
