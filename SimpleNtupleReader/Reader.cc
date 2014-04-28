#include "Reader.h"
#include "Interface.h"
#include "Tools.h"

#include "TChain.h"
#include "TError.h"

const static bool ONLY_QCD = false;
const static std::string version = "V03.06_slim";

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
  ///todo: -full repro all samples
  ///      -ggf weighting in phi(met,g), recoil, ...
  
  Yields tight_g("tightly isolated photon");
  Yields loose_g("loosely isolated photon");

  std::cout << "\nPhoton-Jet Photon Tree\n======================" <<std::endl;
  std::vector<Processor<GJets_Photon>*> v_gjets_g;
  Status<GJets_Photon> status_gjets_g("Status GJets_Photon");
  Plotter<GJets_Photon> gjets_g("plots/"+version,"GJets_Photon");
  Weighter<GJets_Photon> weights_gj_g("Weighter_GJets_Photon");
  Closure<GJets_Photon> direct_gj("","Closure_Direct_GJets");
  Cutter<GJets_Photon> presel_gj_g("Presel_GJets_Photon");
  DoubleCountFilter<GJets_Photon> double_gj_g("DoublicateFilter_GJets_Photon");
  Cutter_tightID<GJets_Photon> tightID_gj_g("TightPhotonId_GJets_Photon");
  direct_gj.Book();
  gjets_g.Book();
  v_gjets_g.push_back( &status_gjets_g );
  v_gjets_g.push_back( &tightID_gj_g );
  v_gjets_g.push_back( &double_gj_g );
  v_gjets_g.push_back( &presel_gj_g );
  v_gjets_g.push_back( &gjets_g );
  v_gjets_g.push_back( &weights_gj_g );
  v_gjets_g.push_back( &direct_gj );
  if (!ONLY_QCD){
  //Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  //Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1 );
  gjets_g.Write();
  direct_gj.Write();
  tight_g.Add( weights_gj_g.GetYields() );
  }
  
  std::cout << "\nQCD Photon Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Photon>*> v_qcd_g;
  Status<QCD_Photon> status_qcd_g("Status QCD_Photon");
  Plotter<QCD_Photon> qcd_g("plots/"+version,"Qcd_Photon");
  Weighter<QCD_Photon> weights_qcd_g("Qcd_Photon");
  Closure<QCD_Photon> direct_qcd("","Direct_QCD");
  Cutter<QCD_Photon> presel_qcd_g("Presel_QCD_Photon");
  DoubleCountFilter<QCD_Photon> double_qcd_g("DoublicateFilter_QCD_Photon");
  Cutter_tightID<QCD_Photon> tightID_qcd_g("TightPhotonId_QCD_Photon");
  direct_qcd.Book();
  qcd_g.Book();
  v_qcd_g.push_back( &status_qcd_g );
  v_qcd_g.push_back( &tightID_qcd_g );
  v_qcd_g.push_back( &double_qcd_g );
  v_qcd_g.push_back( &presel_qcd_g );
  v_qcd_g.push_back( &qcd_g );
  v_qcd_g.push_back( &weights_qcd_g );
  v_qcd_g.push_back( &direct_qcd );
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);
  qcd_g.Write();
  direct_qcd.Write();
  tight_g.Add( weights_qcd_g.GetYields() );
  
  std::cout << "\nPhoton-Jet Jet Tree\n======================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> v_gjets_j;
  Status<GJets_Jet> status_gjets_j("Status GJets_Jet");
  Plotter<GJets_Jet> gjets_j("plots/"+version,"GJets_Jet");
  Weighter<GJets_Jet> weights_gj_j("GJets_Jet");
  Cutter<GJets_Jet> presel_gj_j("Presel_GJets_Jet");
  DoubleCountFilter<GJets_Jet> double_gj_j("DoublicateFilter_GJets_Jet");
  Cutter_looseID<GJets_Jet> looseID_gj_j("LoosePhotonId_GJets_Jet");
  double_gj_j.Set( double_gj_g.Get() );
  gjets_j.Book();
  v_gjets_j.push_back( &status_gjets_j );
  v_gjets_j.push_back( &looseID_gj_j );
  v_gjets_j.push_back( &double_gj_j );
  v_gjets_j.push_back( &presel_gj_j );
  v_gjets_j.push_back( &gjets_j );
  v_gjets_j.push_back( &weights_gj_j );
  if (!ONLY_QCD){
  //Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  //Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1);
  gjets_j.Write();
  loose_g.Add( weights_gj_j.GetYields() );
  }
  
  std::cout << "\nQCD Jet Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> v_qcd_j;
  Status<QCD_Jet> status_qcd_j("Status QCD_Jet");
  Plotter<QCD_Jet> qcd_j("plots/"+version,"Qcd_Jet");
  Weighter<QCD_Jet> weights_qcd_j("Qcd_Jet");
  Cutter<QCD_Jet> presel_qcd_j("Presel_QCD_Jet");
  DoubleCountFilter<QCD_Jet> double_qcd_j("DoublicateFilter_QCD_Jet");
  Cutter_looseID<QCD_Jet> looseID_qcd_j("LoosePhotonId_QCD_Jet");
  double_qcd_j.Set( double_qcd_g.Get() );
  qcd_j.Book();
  v_qcd_j.push_back( &status_qcd_j );
  v_qcd_j.push_back( &looseID_qcd_j );
  v_qcd_j.push_back( &double_qcd_j );
  v_qcd_j.push_back( &presel_qcd_j );
  v_qcd_j.push_back( &qcd_j );
  v_qcd_j.push_back( &weights_qcd_j );
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1 );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);
  qcd_j.Write();
  loose_g.Add( weights_qcd_j.GetYields() );

  
  ///Closure 2nd pass ---------------------------------------------------------------------------------
    

  Closure<QCD_Jet> closure("plots/"+version,"Closure_Combined");
  Closure<QCD_Jet> closure_qcd("plots/"+version,"Closure_QCD");
  Closure<GJets_Jet> closure_gj("plots/"+version,"Closure_GJets");
  closure_gj.SetNominator( weights_gj_g.GetYields());   //Zähler, tight isolated
  closure_gj.SetDenominator( weights_gj_j.GetYields()); //Nenner, loose isolated
  closure_gj.AddSignalYields( direct_gj.GetYields());   //Signal
  closure_gj.Book();

  closure_qcd.SetNominator( weights_qcd_g.GetYields() );   //Zähler, tight isolated
  closure_qcd.SetDenominator( weights_qcd_j.GetYields() ); //Nenner, loose isolated
  closure_qcd.AddSignalYields( direct_qcd.GetYields() );   //Signal
  closure_qcd.Book();

  if (!ONLY_QCD){
  std::cout << "\nPhoton-Jet Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> vc_gjets_j;
  Cutter               <GJets_Jet>   cut_gj(    "Cutter");
  DoubleCountFilter    <GJets_Jet>   double_gj( "DoublicateFilter_GJets");
  Cutter_looseID       <GJets_Jet>   looseID_gj("LoosePhotonId_GJets");
  double_gj.Set( double_gj_g.Get() );
  vc_gjets_j.push_back( &status_gjets_j );
  vc_gjets_j.push_back( &looseID_gj );
  vc_gjets_j.push_back( &double_gj );
  vc_gjets_j.push_back( &cut_gj );
  vc_gjets_j.push_back( &closure_gj );
  //Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  //Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1);
  closure_gj.Write();
  }
    
  std::cout << "\nQCD Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> vc_qcd_j;
  Cutter               <QCD_Jet>   cut_qcd(    "Cutter");
  DoubleCountFilter    <QCD_Jet>   double_qcd( "DoublicateFilter_QCD");
  Cutter_looseID       <QCD_Jet>   looseID_qcd("LoosePhotonId_QCD");
  //double_qcd.Set( double_qcd_g.Get() );
  vc_qcd_j.push_back( &status_qcd_j );
  vc_qcd_j.push_back( &looseID_qcd );
  vc_qcd_j.push_back( &double_qcd );
  vc_qcd_j.push_back( &cut_qcd );
  vc_qcd_j.push_back( &closure_qcd );
//  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
//  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
//  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);

  closure_qcd.Write();
  if (!ONLY_QCD) {
   closure.SetNominator(    &tight_g );   //Zähler, tight isolated
   closure.SetDenominator(  &loose_g );   //Nenner, loose isolated
   closure.AddSignalYields( direct_qcd.GetYields() ); 
   closure.AddSignalYields( direct_gj.GetYields() ); 
   closure.Book();
   closure.AddRef(          closure_gj.GetYields());
   closure.AddRef(          closure_qcd.GetYields());
   closure.Write();
  }
  
  return 0;
}

int do_data(){
  std::cout << "\nData Photon Tree\n======================" <<std::endl;
  std::vector<Processor<Data_Photon>*> v_Data_g;
  Status<Data_Photon> status_data_g("Status Data_Photon");
  Plotter<Data_Photon> Data_g("plots/"+version,"Data_Photon");
  Weighter<Data_Photon> weights_data_g("Data_Photon");
  Closure<Data_Photon> direct_data("","Direct_Data");
  Cutter<Data_Photon> presel_data_g("Presel_Data_Photon");
  DoubleCountFilter<Data_Photon> double_data_g("DoublicateFilter_Data_Photon");
  Cutter_tightID<Data_Photon> tightID_data_g("TightPhotonId_Data_Photon");
  direct_data.Book();
  Data_g.Book();
  v_Data_g.push_back( &status_data_g );
  v_Data_g.push_back( &tightID_data_g );
  v_Data_g.push_back( &double_data_g );
  v_Data_g.push_back( &presel_data_g );
  v_Data_g.push_back( &Data_g );
  v_Data_g.push_back( &weights_data_g );
  v_Data_g.push_back( &direct_data );
  Process<Data_Photon>("photonTree",v_Data_g,"data/"+version+"/PhotonHadA_"+version+"_tree.root",1.0);
  Process<Data_Photon>("photonTree",v_Data_g,"data/"+version+"/PhotonHadB_"+version+"_tree.root",1.0);
  Process<Data_Photon>("photonTree",v_Data_g,"data/"+version+"/PhotonHadC_"+version+"_tree.root",1.0);
  Process<Data_Photon>("photonTree",v_Data_g,"data/"+version+"/PhotonHadD_"+version+"_tree.root",1.0);
  Data_g.Write();

  std::cout << "\nData Jet Tree\n======================" <<std::endl;
  std::vector<Processor<Data_Jet>*> v_Data_j;
  Status<Data_Jet> status_data_j("Status Data_Jet");
  Plotter<Data_Jet> Data_j("plots/"+version,"Data_Jet");
  Weighter<Data_Jet> weights_data_j("Data_Jet");
  Cutter<Data_Jet> presel_data_j("Presel_Data_Jet");
  DoubleCountFilter<Data_Jet> double_data_j("DoublicateFilter_Data_Jet");
  Cutter_looseID<Data_Jet> looseID_data_j("LoosePhotonId_Data_Jet");
  double_data_j.Set( double_data_g.Get() );
  Data_j.Book();
  v_Data_j.push_back( &status_data_j );
  v_Data_j.push_back( &looseID_data_j );
  v_Data_j.push_back( &double_data_j );
  v_Data_j.push_back( &presel_data_j );
  v_Data_j.push_back( &Data_j );
  v_Data_j.push_back( &weights_data_j );
  Process<Data_Jet>("photonJetTree",v_Data_j,"data/"+version+"/PhotonHadA_"+version+"_tree.root",1.0);
  Process<Data_Jet>("photonJetTree",v_Data_j,"data/"+version+"/PhotonHadB_"+version+"_tree.root",1.0);
  Process<Data_Jet>("photonJetTree",v_Data_j,"data/"+version+"/PhotonHadC_"+version+"_tree.root",1.0);
  Process<Data_Jet>("photonJetTree",v_Data_j,"data/"+version+"/PhotonHadD_"+version+"_tree.root",1.0);
  Data_j.Write();


  std::cout << "\nPhoton-Jet Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  Closure<Data_Jet> closure_data("plots/"+version,"Closure_Data");
  closure_data.SetNominator( weights_data_g.GetYields());   //Zähler, tight isolated
  closure_data.SetDenominator( weights_data_j.GetYields()); //Nenner, loose isolated
  closure_data.AddSignalYields( direct_data.GetYields());   //Signal
  closure_data.Book();
  std::vector<Processor<Data_Jet>*> vc_data_j;
  Cutter               <Data_Jet>   cut_data(    "Cutter");
  DoubleCountFilter    <Data_Jet>   double_data( "DoublicateFilter_Data");
  Cutter_looseID       <Data_Jet>   looseID_data("LoosePhotonId_Data");
  //double_data.Set( double_data_g.Get() );
  vc_data_j.push_back( &status_data_j );
  vc_data_j.push_back( &looseID_data );
  vc_data_j.push_back( &double_data );
  vc_data_j.push_back( &cut_data );
  vc_data_j.push_back( &closure_data );
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadA_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadB_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadC_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadD_"+version+"_tree.root",1.);
  closure_data.Write();

  return 0;
}

int main()
{
  gErrorIgnoreLevel = 5000;
 
  //int n=0;
  //for (float i=1; i<2000; i*=1.5) {std::cout<<i<<","; ++n;}
  //std::cout <<"\n n="<<n<<std::endl;
  
  
  
  Reader();
  //do_data();
  
  return 0;
}



