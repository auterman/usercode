// $Id: QcdClosure.h,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef QCDCLOSURE_H
#define QCDCLOSURE_H


#include "Analyzer.h"

#include <map>
#include <string>

class QcdWeights : public Analyzer {
 public:
   QcdWeights(Variable*weight=0);//:v1_(new FloatVariable("met")),weight_(weight){};
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Process(const Event*evt);
   virtual void EndJob();
   void AddSample(const Sample*sample,const std::string& name="");
 private:
   Variable * v1_, * weight_;
   std::map<const Sample*, std::string> file_;
   std::map<const Sample*, TH1*> hist_;
};




class QcdPrediction : public Analyzer {
 public:
   QcdPrediction(Variable*weight=0);
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Produce(Event*evt);
   void AddSample(const Sample*sample, const std::string& fact, const std::string& norm );
 private:
   Variable * v1_, * weight_;
   std::map<const Sample*, std::string> norm_, fact_;
   std::map<const Sample*, TH1*> hist_, w1m_, w1p_;
};



#endif
