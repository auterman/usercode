#include "Rebalance.h"

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"

#include <vector>
#include <map>
#include <iostream>

bool Rebalance( const Event* evt, Event* rebalanced,  JetResolution * JetRes=0;
 )
{
   //// Interface to KinFitter
   TKinFitter* myFit = new TKinFitter();
   std::vector<TLorentzVector*> lvec_m;
   std::vector<TMatrixD*> covMat_m;
   std::vector<TFitParticleEtEtaPhi*> fitted;
   std::vector<TFitParticleEtEtaPhi*> measured;
   
   bool result = true;
   double MHTx_high = 0;
   double MHTy_high = 0;

   //// Fill measured particles to vector
   for (int i = 0; i<evt->NrecoJet; ++i) {

      if (evt->recoJetPt[i] >= 20. ) {  //use all jets >= 20. GeV
         MHTx_high -= evt->recoJetPx[i];
         MHTy_high -= evt->recoJetPy[i];

         // The particles before fitting
         double tmppx, tmppy, tmppz, tmpe;
         tmppx = evt->recoJetPx[i];
         tmppy = evt->recoJetPy[i];
         tmppz = evt->recoJetPz[i];
         tmpe  = evt->recoJetE[i];

         TLorentzVector* lv = new TLorentzVector(tmppx, tmppy, tmppz, tmpe);
         lvec_m.push_back(lv);
         TMatrixD* cM = new TMatrixD(3, 3);
         (*cM)(0, 0) = JetRes->JetResolution_Pt2(it->pt(), it->eta(), i);
         (*cM)(1, 1) = JetRes->JetResolution_Eta2(it->energy(), it->eta());
         (*cM)(2, 2) = JetRes->JetResolution_Phi2(it->energy(), it->eta());
//         (*cM)(0, 0) = sqrt( evt->recoJetPt[i] ); //JetResolution_Pt2(it->pt(), it->eta(), i);
//         (*cM)(1, 1) = 0.05;                 //JetResolution_Eta2(it->energy(), it->eta());
//         (*cM)(2, 2) = 0.05;                 //JetResolution_Phi2(it->energy(), it->eta());
         covMat_m.push_back(cM);
         char name[10];
         sprintf(name, "jet%i", i);
         TFitParticleEtEtaPhi* jet1 = new TFitParticleEtEtaPhi(name, name, lvec_m.back(), covMat_m.back());
         measured.push_back(jet1);
         TFitParticleEtEtaPhi* jet2 = new TFitParticleEtEtaPhi(name, name, lvec_m.back(), covMat_m.back());
         fitted.push_back(jet2);
         myFit->addMeasParticle(fitted.back());
         ++i;
      }
   }

   //// Add momentum constraints
   double MET_constraint_x = 0.;
   double MET_constraint_y = 0.;
   //// default: rebalance MHT of fitted jets
   MET_constraint_x = 0.;
   MET_constraint_y = 0.;
   TFitConstraintEp* momentumConstr1 = new TFitConstraintEp("px", "px", 0, TFitConstraintEp::pX, MET_constraint_x);
   TFitConstraintEp* momentumConstr2 = new TFitConstraintEp("py", "py", 0, TFitConstraintEp::pY, MET_constraint_y);
   for (unsigned int i = 0; i < fitted.size(); ++i) {
      momentumConstr1->addParticle(fitted.at(i));
      momentumConstr2->addParticle(fitted.at(i));
   }
   myFit->addConstraint(momentumConstr1);
   myFit->addConstraint(momentumConstr2);

   //// Set fit parameters
   myFit->setVerbosity(0);
   myFit->setMaxNbIter(100);
   myFit->setMaxF(0.01 * 2);
   myFit->setMaxDeltaS(1.e-3);
   myFit->fit();
   int status = myFit->getStatus();

   double chi2 = 0;
   double F = 0;
   double prob = 0;
   //if (status == 0 || status == 1) {
   if (status == 0) {
      chi2 = myFit->getS();
      F = myFit->getF();
      int dof = myFit->getNDF();
      prob = TMath::Prob(chi2, dof);
      //if (prob < 1.e-8) result = false;
      //cout << "chi2, prop, F = " << chi2 << " " << prob << " " << F << endl;
   } else {
      chi2 = 99999;
      prob = 0;
      F = 99999;
      result = false;
      //cout << "chi2, prop, F = " << chi2 << " " << prob << " " << F << endl;
   }
   //   h_fitProb->Fill(prob);  /// plot fit probability

   //// Get the output of KinFitter
   rebalanced->CopyEvent( *evt );
   rebalanced->NrecoJet = measured.size();
   for (unsigned int i = 0; i < measured.size(); ++i) {
     rebalanced->recoJetPx[i] = fitted.at(i)->getCurr4Vec()->Px();
     rebalanced->recoJetPy[i] = fitted.at(i)->getCurr4Vec()->Py();
     rebalanced->recoJetPz[i] = fitted.at(i)->getCurr4Vec()->Pz();
     rebalanced->recoJetE[i]  = fitted.at(i)->getCurr4Vec()->E();
     rebalanced->recoJetPt[i] = sqrt( rebalanced->recoJetPx[i]*rebalanced->recoJetPx[i]+
                                      rebalanced->recoJetPy[i]*rebalanced->recoJetPy[i] );
   }
   //std::cout<<"Fit Status="<<status
   //	    <<", old HT="<<evt->HT()<<", MHT="<<evt->MHT()
   //	    <<";  new HT="<<rebalanced->HT()<<", MHT="<<rebalanced->MHT()<<std::endl
   //	    <<std::endl;


   delete myFit;
   for (unsigned int i = 0; i < measured.size(); ++i) {
      delete lvec_m.at(i);
      delete covMat_m.at(i);
      delete measured.at(i);
      delete fitted.at(i);
   }
   delete momentumConstr1;
   delete momentumConstr2;

   return result;
}




void Rebalance( const std::vector<Event*>& evts, std::vector<Event*>& rebalanced_events )
{
  int not_converged=0;
  JetResolution * JetRes = new JetResolution();
  for (std::vector<Event*>::const_iterator it=evts.begin(); it!=evts.end(); ++it){
    Event * rebalanced = new Event;
    if ( Rebalance( *it, rebalanced, JetRes ) )
      rebalanced_events.push_back( rebalanced );
    else {
      ++not_converged;
      delete rebalanced;
    } 
    if ((it-evts.begin())%(evts.size()/10)==0)std::cerr<<"->"<<(it-evts.begin())/10<<"%"; 
  }
  delete JetRes;
  std::cout << "\nSuccessfully rebalanced "<<rebalanced_events.size()<<" out of "<<evts.size()
            <<" events. Fit failed for "<<not_converged<<" events."<<std::endl;
}
