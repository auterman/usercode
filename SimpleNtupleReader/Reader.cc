#include "Reader.h"
#include "Interface.h"
#include "Tools.h"

#include "TChain.h"
#include "TError.h"

const static bool ONLY_QCD = false;
const static std::string version = "V03.13";

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
  Plotter<GJets_Photon> gjets_g(version,"GJets_Photon");
  Weighter<GJets_Photon> weights_gj_g("Weighter_GJets_Photon");
  Closure<GJets_Photon> direct_gj("","Closure_Direct_GJets", "Direct Simulation");
  Cutter<GJets_Photon> presel_gj_g("Presel_GJets_Photon");
  Closure<GJets_Photon> finaldirect_gj("","Final_Direct_GJets", "Direct Simulation");
  FinalCuts<GJets_Photon> final_gj_g("Final_GJets_Photon");
  DoubleCountFilter<GJets_Photon> double_gj_g("DoublicateFilter_GJets_Photon");
  Cutter_tightID<GJets_Photon> tightID_gj_g("TightPhotonId_GJets_Photon");
  finaldirect_gj.Book();
  direct_gj.Book();
  gjets_g.Book();
  v_gjets_g.push_back( &status_gjets_g );
  v_gjets_g.push_back( &tightID_gj_g );
  v_gjets_g.push_back( &double_gj_g );
  v_gjets_g.push_back( &presel_gj_g );
  v_gjets_g.push_back( &gjets_g );
  v_gjets_g.push_back( &weights_gj_g );
  v_gjets_g.push_back( &direct_gj );
  v_gjets_g.push_back( &final_gj_g );
  v_gjets_g.push_back( &finaldirect_gj );
  if (!ONLY_QCD){
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  //Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  //Process<GJets_Photon>("photonTree",v_gjets_g,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1 );
  gjets_g.Write();
  direct_gj.Write();
  tight_g.Add( weights_gj_g.GetYields() );
  }
  
  std::cout << "\nQCD Photon Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Photon>*> v_qcd_g;
  Status<QCD_Photon> status_qcd_g("Status QCD_Photon");
  Plotter<QCD_Photon> qcd_g(version,"Qcd_Photon");
  Weighter<QCD_Photon> weights_qcd_g("Qcd_Photon");
  Closure<QCD_Photon> direct_qcd("","Direct_QCD", "Direct Simulation");
  Cutter<QCD_Photon> presel_qcd_g("Presel_QCD_Photon");
  DoubleCountFilter<QCD_Photon> double_qcd_g("DoublicateFilter_QCD_Photon");
  Cutter_tightID<QCD_Photon> tightID_qcd_g("TightPhotonId_QCD_Photon");
  Closure<GJets_Photon> finaldirect_qcd("","Final_Direct_QCD", "Direct Simulation");
  FinalCuts<GJets_Photon> final_qcd_g("Final_QCD_Photon");
  finaldirect_qcd.Book();
  direct_qcd.Book();
  qcd_g.Book();
  v_qcd_g.push_back( &status_qcd_g );
  v_qcd_g.push_back( &tightID_qcd_g );
//  v_qcd_g.push_back( &double_qcd_g );
  v_qcd_g.push_back( &presel_qcd_g );
  v_qcd_g.push_back( &qcd_g );
  v_qcd_g.push_back( &weights_qcd_g );
  v_qcd_g.push_back( &direct_qcd );
  v_gjets_g.push_back( &final_qcd_g );
  v_gjets_g.push_back( &finaldirect_qcd );
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1);
  //Process<QCD_Photon>("photonTree",v_qcd_g,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);
  qcd_g.Write();
  direct_qcd.Write();
  tight_g.Add( weights_qcd_g.GetYields() );
  
  std::cout << "\nPhoton-Jet Jet Tree\n======================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> v_gjets_j;
  Status<GJets_Jet> status_gjets_j("Status GJets_Jet");
  Plotter<GJets_Jet> gjets_j(version,"GJets_Jet");
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
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  //Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  //Process<GJets_Jet>("photonJetTree",v_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1);
  gjets_j.Write();
  loose_g.Add( weights_gj_j.GetYields() );
  }
  
  std::cout << "\nQCD Jet Tree\n======================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> v_qcd_j;
  Status<QCD_Jet> status_qcd_j("Status QCD_Jet");
  Plotter<QCD_Jet> qcd_j(version,"Qcd_Jet");
  Weighter<QCD_Jet> weights_qcd_j("Qcd_Jet");
  Cutter<QCD_Jet> presel_qcd_j("Presel_QCD_Jet");
  DoubleCountFilter<QCD_Jet> double_qcd_j("DoublicateFilter_QCD_Jet");
  Cutter_looseID<QCD_Jet> looseID_qcd_j("LoosePhotonId_QCD_Jet");
  double_qcd_j.Set( double_qcd_g.Get() );
  qcd_j.Book();
  v_qcd_j.push_back( &status_qcd_j );
  v_qcd_j.push_back( &looseID_qcd_j );
//  v_qcd_j.push_back( &double_qcd_j );
  v_qcd_j.push_back( &presel_qcd_j );
  v_qcd_j.push_back( &qcd_j );
  v_qcd_j.push_back( &weights_qcd_j );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1 );
  //Process<QCD_Jet>("photonJetTree",v_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);
  qcd_j.Write();
  loose_g.Add( weights_qcd_j.GetYields() );

  
  ///Closure 2nd pass ---------------------------------------------------------------------------------
    

  Closure<QCD_Jet> closure(version,"Closure_Combined", "Prediction");
  Closure<QCD_Jet> closure_qcd(version,"Closure_QCD", "Prediction");
  Closure<GJets_Jet> closure_gj(version,"Closure_GJets", "Prediction");
  Closure<QCD_Jet> final(version,"Final_Combined", "Prediction");
  Closure<QCD_Jet> final_qcd(version,"Final_QCD", "Prediction");
  Closure<GJets_Jet> final_gj(version,"Final_GJets", "Prediction");
  closure_gj.SetNominator( weights_gj_g.GetYields());   //Zähler, tight isolated
  closure_gj.SetDenominator( weights_gj_j.GetYields()); //Nenner, loose isolated
  closure_gj.AddDirectYields( direct_gj.GetYields());   //Signal
  closure_gj.Book();
  final_gj.Book();

  closure_qcd.SetNominator( weights_qcd_g.GetYields() );   //Zähler, tight isolated
  closure_qcd.SetDenominator( weights_qcd_j.GetYields() ); //Nenner, loose isolated
  closure_qcd.AddDirectYields( direct_qcd.GetYields() );   //Signal
  closure_qcd.Book();
  final_qcd.Book();

  if (!ONLY_QCD){
  std::cout << "\nPhoton-Jet Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<GJets_Jet>*> vc_gjets_j;
  Cutter               <GJets_Jet>   cut_gj(    "Cutter");
  FinalCuts            <GJets_Jet>   finalcut_gj("FinalCuts");
  DoubleCountFilter    <GJets_Jet>   double_gj( "DoublicateFilter_GJets");
  Cutter_looseID       <GJets_Jet>   looseID_gj("LoosePhotonId_GJets");
  double_gj.Set( double_gj_g.Get() );
  vc_gjets_j.push_back( &status_gjets_j );
  vc_gjets_j.push_back( &looseID_gj );
  vc_gjets_j.push_back( &double_gj );
  vc_gjets_j.push_back( &cut_gj );
  vc_gjets_j.push_back( &closure_gj );
  vc_gjets_j.push_back( &finalcut_gj );
  vc_gjets_j.push_back( &final_gj );
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",0.32466417277);
  Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",0.0502103290278 );
  //Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_200_400_"+version+"_tree.root",1);
  //Process<GJets_Jet>("photonJetTree",vc_gjets_j,"data/"+version+"/GJets_400_inf_"+version+"_tree.root",1);
  closure_gj.Write();
  }
    
  std::cout << "\nQCD Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  std::vector<Processor<QCD_Jet>*> vc_qcd_j;
  Cutter               <QCD_Jet>   cut_qcd(    "Cutter");
  FinalCuts            <QCD_Jet> finalcut_qcd("FinalCuts");
  DoubleCountFilter    <QCD_Jet>   double_qcd( "DoublicateFilter_QCD");
  Cutter_looseID       <QCD_Jet>   looseID_qcd("LoosePhotonId_QCD");
  //double_qcd.Set( double_qcd_g.Get() );
  vc_qcd_j.push_back( &status_qcd_j );
  vc_qcd_j.push_back( &looseID_qcd );
//  vc_qcd_j.push_back( &double_qcd );
  vc_qcd_j.push_back( &cut_qcd );
  vc_qcd_j.push_back( &closure_qcd );
  vc_qcd_j.push_back( &finalcut_qcd );
  vc_qcd_j.push_back( &final_qcd );
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",201.935697478);
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",5.45224379701 );
  Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",0.291768273061);
  //Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_250_500_"+version+"_tree.root",1);
  //Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_500_1000_"+version+"_tree.root",1);
  //Process<QCD_Jet>("photonJetTree",vc_qcd_j,"data/"+version+"/QCD_1000_inf_"+version+"_tree.root",1);

  closure_qcd.Write();
  if (!ONLY_QCD) {
   closure.SetNominator(    &tight_g );   //Zähler, tight isolated
   closure.SetDenominator(  &loose_g );   //Nenner, loose isolated
   closure.AddDirectYields( direct_qcd.GetYields() ); 
   closure.AddDirectYields( direct_gj.GetYields() ); 
   closure.Book();
   closure.AddRef(          closure_gj.GetYields());
   closure.AddRef(          closure_qcd.GetYields());
   closure.Write();

   final.SetNominator(    &tight_g );   //Zähler, tight isolated
   final.SetDenominator(  &loose_g );   //Nenner, loose isolated
   final.AddDirectYields( finaldirect_qcd.GetYields() ); 
   final.AddDirectYields( finaldirect_gj.GetYields() ); 
   final.Book();
   final.AddRef(          final_gj.GetYields());
   final.AddRef(          final_qcd.GetYields());
   final.Write();
  }
  
  return 0;
}

int do_data(){

  std::cout << "\nISR Photon Tree\n======================" <<std::endl;
  std::vector<Processor<ISR_Photon>*> v_ISR_g;
  Status<ISR_Photon> status_isr_g("Status ISR_Photon");
  Plotter<ISR_Photon> ISR_g(version,"ISR_Photon");
  ISR_prediction<ISR_Photon> direct_isr("","Direct_ISR", "ISR t#bar{t}#gamma, W#gamma, Z#gamma");
//    ISR_prediction<ISR_Photon> direct_isr("","Direct_ISR", "ISR");
  Cutter<ISR_Photon> presel_isr_g("Presel_ISR_Photon");
  DoubleCountFilter<ISR_Photon> double_isr_g("DoublicateFilter_ISR_Photon");
  Cutter_tightID<ISR_Photon> tightID_isr_g("TightPhotonId_ISR_Photon");
  direct_isr.Book();
  ISR_g.Book();
  v_ISR_g.push_back( &status_isr_g );
  v_ISR_g.push_back( &tightID_isr_g );
  v_ISR_g.push_back( &double_isr_g );
  v_ISR_g.push_back( &presel_isr_g );
  v_ISR_g.push_back( &ISR_g );
  v_ISR_g.push_back( &direct_isr );
//  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.06/TTGamma_V03.06_tree.root",2.166*19789/1719954);       //2.166*19800/1719954
//  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.06/WGamma_50_130_V03.06_tree.root",1.17*19789/1135698); //1.17*19800/1135698
//  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.06/WGamma_130_inf_V03.06_tree.root",0.2571*19789/471458);//0.2571*19800/471458
//  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.06/ZGammaNuNu_V03.06_tree.root",0.074*19789/489474);    //0.074*19800/489474
  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.13/TTGamma_V03.13_tree.root",1.5*  2.166*19789/1719954);       //2.166*19800/1719954
  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.13/WGamma_50_130_V03.13_tree.root",1.5* 1.17*19789/1135698); //1.17*19800/1135698
  Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.13/WGamma_130_inf_V03.13_tree.root",1.5* 0.2571*19789/471458);//0.2571*19800/471458
  Process<ISR_Photon>("photonTree",v_ISR_g,"data/modifiedZGammaNuNu_V03.13_tree.root",1.5* 0.074*19789/489474);    //0.074*19800/489474
  Process<ISR_Photon>("photonTree",v_ISR_g,"data/ZGammaLL_V02.19b_tree.root",1.5* 132.6*19789/6588161);    //0.074*19800/489474
//  Process<ISR_Photon>("photonTree",v_ISR_g,"data/modifiedZGammaLL_V02.19b_tree.root",1.5*3* 132.6*19789/6588161);    //0.074*19800/489474
  //Process<ISR_Photon>("photonTree",v_ISR_g,"data/V03.13/ZGammaNuNu_V03.13_tree.root",1.5* 0.074*19789/489474);    //0.074*19800/489474
  ISR_g.Write();

  std::cout << "\nisr2 Photon Tree\n======================" <<std::endl;
  std::vector<Processor<ISR_Photon>*> v_isr2_g;
  Status<ISR_Photon> status_isr2_g("Status ISR2_Photon");
  Plotter<ISR_Photon> isr2_g(version,"ISR2_Photon");
  ISR_prediction<ISR_Photon> direct_isr2("","Direct_isr2", "ISR2");
  Cutter<ISR_Photon> presel_isr2_g("Presel_ISR2_Photon");
  DoubleCountFilter<ISR_Photon> double_isr2_g("DoublicateFilter_ISR2_Photon");
  Cutter_tightID_SansLeptonVeto<ISR_Photon> tightIDsansLveta_isr2_g("TightPhotonId_ISR2_Photon");
  direct_isr2.Book();
  isr2_g.Book();
  v_isr2_g.push_back( &status_isr2_g );
  v_isr2_g.push_back( &tightIDsansLveta_isr2_g );
  v_isr2_g.push_back( &double_isr2_g );
  v_isr2_g.push_back( &presel_isr2_g );
  v_isr2_g.push_back( &isr2_g );
  v_isr2_g.push_back( &direct_isr2 );
  Process<ISR_Photon>("photonTree",v_isr2_g,"data/modifiedZGammaLL_V02.20b_tree.root",1.5*2.979 * 132.6*19789/6588161);    //0.074*19800/489474
  isr2_g.Write();
  direct_isr.AddRef( direct_isr2.GetYields() );

  std::cout << "\nSignal Photon Tree\n======================" <<std::endl;
  std::vector<Processor<Signal_Photon>*> v_Signal_g;
  Status<Signal_Photon> status_signal_g("Status Signal_Photon");
  Plotter<Signal_Photon> Signal_g(version,"Signal_Photon Plotter");
  Weighter<Signal_Photon> weights_signal_g("Signal_Photon");
  Closure<Signal_Photon> direct_signal("","Direct_Signal", "Signal: Wino 1700 720");
  Cutter<Signal_Photon> presel_signal_g("Presel_Signal_Photon");
  DoubleCountFilter<Signal_Photon> double_signal_g("DoublicateFilter_Signal_Photon");
  Cutter_tightID<Signal_Photon> tightID_signal_g("TightPhotonId_Signal_Photon");
  direct_signal.Book();
  Signal_g.Book();
  v_Signal_g.push_back( &status_signal_g );
  v_Signal_g.push_back( &tightID_signal_g );
  v_Signal_g.push_back( &double_signal_g );
  v_Signal_g.push_back( &presel_signal_g );
  v_Signal_g.push_back( &Signal_g );
  v_Signal_g.push_back( &direct_signal );
  v_Signal_g.push_back( &weights_signal_g );
  //Process<Signal_Photon>("photonTree",v_Signal_g,"data/B_1700_1120_375_V03.06_tree.root",0.01920353672);
  Process<Signal_Photon>("photonTree",v_Signal_g,"data/W_1700_720_375_V03.06_tree.root", 0.3164*19712/60000 ); //0.3164*19712/60000
  //direct_signal.AddSignalYields( direct_signal.GetYields());   //Signal

  Signal_g.Write();
  std::cout << "\nData Photon Tree\n======================" <<std::endl;
  std::vector<Processor<Data_Photon>*> v_Data_g;
  Status<Data_Photon> status_data_g("Status Data_Photon");
  Plotter<Data_Photon> Data_g(version,"Data_Photon");
  Weighter<Data_Photon> weights_data_g("Data_Photon");
  Closure<Data_Photon> direct_data("","Direct_Data","Data,  20 fb^{-1}, 8 TeV");
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


  std::cout << "\nData Electron Tree\n======================" <<std::endl;
  std::vector<Processor<Data_Electron>*> v_EWK_g;
  Status<Data_Electron> status_ewk_g("Status Data_Electron");
  Plotter<Data_Electron> EWK_g(version,"Data_Electron");
  EWK_prediction<Data_Electron> prediction_ewk("","Prediction_EWK","e#rightarrow#gamma fakes");
//    EWK_prediction<Data_Electron> prediction_ewk("","Prediction_EWK","EWK");
  Cutter<Data_Electron> presel_ewk_g("Presel_Data_Electron");
  DoubleCountFilter<Data_Electron> double_ewk_g("DoublicateFilter_Data_Electron");
  Cutter_electronID<Data_Electron> eID_ewk_g("TightPhotonId_Data_Electron");
  prediction_ewk.Book();
  double_ewk_g.Set( double_data_g.Get() );
  EWK_g.Book();
  v_EWK_g.push_back( &status_ewk_g );
  v_EWK_g.push_back( &eID_ewk_g );
  v_EWK_g.push_back( &double_ewk_g );
  v_EWK_g.push_back( &presel_ewk_g );
  v_EWK_g.push_back( &EWK_g );
  v_EWK_g.push_back( &prediction_ewk );
  Process<Data_Electron>("photonElectronTree",v_EWK_g,"data/"+version+"/PhotonHadA_"+version+"_tree.root",1.0);
  Process<Data_Electron>("photonElectronTree",v_EWK_g,"data/"+version+"/PhotonHadB_"+version+"_tree.root",1.0);
  Process<Data_Electron>("photonElectronTree",v_EWK_g,"data/"+version+"/PhotonHadC_"+version+"_tree.root",1.0);
  Process<Data_Electron>("photonElectronTree",v_EWK_g,"data/"+version+"/PhotonHadD_"+version+"_tree.root",1.0);
  EWK_g.Write();

  std::cout << "\nData Jet Tree\n======================" <<std::endl;
  std::vector<Processor<Data_Jet>*> v_Data_j;
  Status<Data_Jet> status_data_j("Status Data_Jet");
  Plotter<Data_Jet> Data_j(version,"Data_Jet");
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

  std::cout << "\nSignal Jet Tree\n======================" <<std::endl;
  std::vector<Processor<Signal_Jet>*> v_Signal_j;
  Status<Signal_Jet> status_signal_j("Status Signal_Jet");
  Plotter<Signal_Jet> Signal_j(version,"Signal_Jet");
  Weighter<Signal_Jet> weights_signal_j("Signal_Jet");
  Cutter<Signal_Jet> presel_signal_j("Presel_Signal_Jet");
  DoubleCountFilter<Signal_Jet> double_signal_j("DoublicateFilter_Signal_Jet");
  Cutter_tightID<Signal_Jet> tightID_signal_j("TightPhotonId_Signal_Jet");
  Signal_j.Book();
  v_Signal_j.push_back( &status_signal_j );
  v_Signal_j.push_back( &tightID_signal_j );
  v_Signal_j.push_back( &double_signal_j );
  v_Signal_j.push_back( &presel_signal_j );
  v_Signal_j.push_back( &Signal_j );
  v_Signal_j.push_back( &weights_signal_j );  
  //Process<Signal_Jet>("photonJetTree",v_Signal_j,"data/B_1700_1120_375_V03.06_tree.root",0.01920353672);
  Process<Signal_Photon>("photonTree",v_Signal_g,"data/W_1700_720_375_V03.06_tree.root", 0.3164*19712/60000 ); //0.3164*19712/60000
  Signal_j.Write();


  std::cout << "\nData Jet Tree (2nd pass for closure)\n===================================" <<std::endl;
  Closure<Data_Jet> closure_data(version,"Closure_Data","Total Prediction");
  closure_data.SetNominator( weights_data_g.GetYields());   //Zähler, tight isolated
  closure_data.SetDenominator( weights_data_j.GetYields()); //Nenner, loose isolated
  direct_data.ResultName("selected");
  closure_data.AddDirectYields( direct_data.GetYields());   //Direct sim
  closure_data.ResultName( "QCD" );
  direct_signal.LineColor( 3 );
  direct_isr.ResultName( "ISR" );
  direct_isr.FillColor( 4 );
  direct_isr2.FillColor( 4 );
  prediction_ewk.ResultName( "data EWK" );
  prediction_ewk.FillColor( 5 );
  closure_data.AddSignalYields( direct_signal.GetYields());   //Signal
  closure_data.AddOtherYields(  direct_isr.GetYields());      //ISR
  //closure_data.AddOtherYields(  direct_isr2.GetYields());      //ISR2
  closure_data.AddOtherYields(  prediction_ewk.GetYields());  //EWK
  closure_data.Book();
  std::vector<Processor<Data_Jet>*> vc_data_j;
  Cutter               <Data_Jet>   cut_data(    "Cutter");//Presel
  DoubleCountFilter    <Data_Jet>   double_data( "DoublicateFilter_Data");
  Cutter_looseID       <Data_Jet>   looseID_data("LoosePhotonId_Data");
  double_data.Set( double_data_g.Get() );
  vc_data_j.push_back( &status_data_j );
  vc_data_j.push_back( &looseID_data );
  vc_data_j.push_back( &double_data );
  vc_data_j.push_back( &cut_data );
  vc_data_j.push_back( &closure_data );
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadA_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadB_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadC_"+version+"_tree.root",1.);
  Process<Data_Jet>("photonJetTree",vc_data_j,"data/"+version+"/PhotonHadD_"+version+"_tree.root",1.);
  closure_data.SetLegTitel("#bf{CMS preliminary}");
  closure_data.Write();


  std::cout << "\nSignal Jet Tree (2nd pass for contamination)\n===================================" <<std::endl;
  Yields tight_g("tightly isolated photon");
  Yields loose_g("loosely isolated photon");
  tight_g.Add( weights_data_g.GetYields() );
  tight_g.Add( weights_signal_g.GetYields() );
  loose_g.Add( weights_data_j.GetYields() );
  loose_g.Add( weights_signal_j.GetYields() );
  Closure<Signal_Jet> contamination(version,"contamination","Total Contamination");
  contamination.SetNominator(    &tight_g ); //Zähler, tight isolated
  contamination.SetDenominator(  &loose_g ); //Nenner, loose isolated
  contamination.AddDirectYields( direct_signal.GetYields() );   //Direct sim
  contamination.Book();
  std::vector<Processor<Signal_Jet>*> vc_contamin_j;
  Cutter               <Signal_Jet>   cut_contamin(    "Cutter");//Presel
  DoubleCountFilter    <Signal_Jet>   double_contamin( "DoublicateFilter_contamin");
  Cutter_looseID       <Signal_Jet>   looseID_contamin("LoosePhotonId_contamin");
  double_contamin.Set( double_signal_g.Get() );
  vc_contamin_j.push_back( &status_signal_j );
  vc_contamin_j.push_back( &looseID_contamin );
  vc_contamin_j.push_back( &double_contamin );
  vc_contamin_j.push_back( &cut_contamin );
  vc_contamin_j.push_back( &contamination );
  Process<Signal_Photon>("photonTree",v_Signal_g,"data/W_1700_720_375_V03.06_tree.root", 0.3164*19712/60000 ); //0.3164*19712/60000
  //Process<Signal_Jet>("photonJetTree",v_Signal_j,"data/B_1700_1120_375_V03.06_tree.root",0.01920353672);
  contamination.SetLegTitel("#bf{CMS preliminary}");
  contamination.Write();

  return 0;
}

int main()
{
  gErrorIgnoreLevel = 5000;
 
  //int n=0;
  //for (float i=0; i<50; ++i) {std::cout<<-200+8*i<<", "; }
  //std::cout <<"\n n="<<n<<std::endl;
  
  
  
  Reader();
  do_data();
  
  return 0;
}



