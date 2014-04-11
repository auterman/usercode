#include "Reader.h"
#include "Interface.h"
#include "Tools.h"

#include "TChain.h"


template <typename T>
bool Status(T*t, Long64_t i, Long64_t n, double w)
{
   if ( (i%(n/10))==0)
   std::cout << i/(n/10)*10 << "% / " << n << " events" << std::endl;
   return 0;
}


template <typename T>
void Process(const std::string& str_chain, std::vector<Processor<T>*>& proc, const std::string& file, double w)
{
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
 
  std::cout << "Photon-Jet Photon Tree" <<std::endl;
  std::vector<Processor<GJets_Photon>*> v_gjets_g;
  Plotter<GJets_Photon> gjets_g("GJets_Photon");
  Weighter<GJets_Photon> weights_gj_g("GJets_Photon");
  Closure<GJets_Photon> direct_gj("Direct_GJets");
  gjets_g.Book();
  gjets_g.Add( Status<GJets_Photon> );
  v_gjets_g.push_back( &gjets_g );
  v_gjets_g.push_back( &weights_gj_g );
  v_gjets_g.push_back( &direct_gj );
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/GJets_200_400_V03.06_tree.root",0.32466417277);
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/GJets_400_inf_V03.06_tree.root",0.0502103290278 );
  gjets_g.Write();

  
  std::cout << "QCD Photon Tree" <<std::endl;
  std::vector<Processor<QCD_Photon>*> v_qcd_g;
  Plotter<QCD_Photon> qcd_g("Qcd_Photon");
  Weighter<QCD_Photon> weights_qcd_g("Qcd_Photon");
  Closure<QCD_Photon> direct_qcd("Direct_QCD");
  qcd_g.Book();
  qcd_g.Add( Status<QCD_Photon> );
  v_qcd_g.push_back( &qcd_g );
  v_qcd_g.push_back( &weights_qcd_g );
  v_qcd_g.push_back( &direct_qcd );
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/QCD_250_500_V03.06_tree.root",201.935697478);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/QCD_500_1000_V03.06_tree.root",5.45224379701 );
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/QCD_1000_inf_V03.06_tree.root",0.291768273061);
  qcd_g.Write();
  
  Yields tight_g("tightly isolated photon");
  tight_g.Add( weights_gj_g.GetYields() );
  tight_g.Add( weights_qcd_g.GetYields() );
  
  std::cout << "Photon-Jet Jet Tree" <<std::endl;
  std::vector<Processor<GJets_Jet>*> v_gjets_j;
  Plotter<GJets_Jet> gjets_j("GJets_Jet");
  Weighter<GJets_Jet> weights_gj_j("GJets_Jet");
  gjets_j.Book();
  gjets_j.Add( Status<GJets_Jet> );
  v_gjets_j.push_back( &gjets_j );
  v_gjets_j.push_back( &weights_gj_j );
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/GJets_200_400_V03.06_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/GJets_400_inf_V03.06_tree.root",0.0502103290278 );
  gjets_j.Write();
  
  
  std::cout << "QCD Jet Tree" <<std::endl;
  std::vector<Processor<QCD_Jet>*> v_qcd_j;
  Plotter<QCD_Jet> qcd_j("Qcd_Jet");
  Weighter<QCD_Jet> weights_qcd_j("Qcd_Jet");
  qcd_j.Book();
  qcd_j.Add( Status<QCD_Jet> );
  v_qcd_j.push_back( &qcd_j );
  v_qcd_j.push_back( &weights_qcd_j );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/QCD_250_500_V03.06_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/QCD_500_1000_V03.06_tree.root",5.45224379701 );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/QCD_1000_inf_V03.06_tree.root",0.291768273061);
  qcd_j.Write();
  
  Yields loose_g("loosely isolated photon");
  loose_g.Add( weights_gj_j.GetYields() );
  loose_g.Add( weights_qcd_j.GetYields() );


  Closure<GJets_Jet> closure_gj("Closure_GJets");
  Closure<QCD_Jet> closure_qcd("Closure_QCD");
  Closure<GJets_Jet> closure("Closure_Combined");
  closure_gj.Book();
  direct_gj.Book();
  direct_gj.Write();
  closure_gj.SetNominator( weights_gj_g.GetYields());   //Zähler, tight isolated
  closure_gj.SetDenominator( weights_gj_j.GetYields()); //Nenner, loose isolated
  closure_gj.SetSignalHists( direct_gj.GetHists());   //Signal

  closure_qcd.Book();
  direct_qcd.Book();
  direct_qcd.Write();
  closure_qcd.SetNominator( weights_qcd_g.GetYields() );   //Zähler, tight isolated
  closure_qcd.SetDenominator( weights_qcd_j.GetYields() ); //Nenner, loose isolated
  closure_qcd.SetSignalHists( direct_qcd.GetHists() );   //Signal



  std::cout << "Photon-Jet Jet Tree (2nd pass for closure)" <<std::endl;
  std::vector<Processor<GJets_Jet>*> vc_gjets_j;
  closure_gj.Add( Status<GJets_Jet> );
  vc_gjets_j.push_back( &closure_gj );
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/GJets_200_400_V03.06_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/GJets_400_inf_V03.06_tree.root",0.0502103290278 );
  
  
  std::cout << "QCD Jet Tree (2nd pass for closure)" <<std::endl;
  std::vector<Processor<QCD_Jet>*> vc_qcd_j;
  closure_qcd.Add( Status<QCD_Jet> );
  vc_qcd_j.push_back( &closure_qcd );
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/QCD_250_500_V03.06_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/QCD_500_1000_V03.06_tree.root",5.45224379701 );
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/QCD_1000_inf_V03.06_tree.root",0.291768273061);

  closure_gj.Write();
  closure_qcd.Write();

  closure.Book();
  Histograms signal_combined("Combined Direct Simulation");
  signal_combined.Add( direct_qcd.GetHists() );
  signal_combined.Add( direct_gj.GetHists() );
  closure.SetNominator(   &tight_g );   //Zähler, tight isolated
  closure.SetDenominator( &loose_g ); //Nenner, loose isolated
  closure.SetSignalHists( &signal_combined );   //Signal
  closure.GetYields()->Add(closure_gj.GetYields());
  closure.GetYields()->Add(closure_qcd.GetYields());
  closure.Write();

  return 0;
}


int main()
{
  return Reader();
}
