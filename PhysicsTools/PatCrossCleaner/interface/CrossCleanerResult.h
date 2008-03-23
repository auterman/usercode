#ifndef CrossCleanerResult_h
#define CrossCleanerResult_h

//#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/RefToBase.h"

namespace edm{
  bool operator< (const edm::RefToBase<reco::Candidate> & r1,
		  const edm::RefToBase<reco::Candidate> & r2);
}

struct CrossCleanerValue{ //ValueMap
  bool keep; //probably not necessary: can also check size of 'objects'
  std::vector<edm::RefToBase<reco::Candidate> > objects;
};

//typedef edm::ValueMap<CrossCleanerValue> CrossCleanerResult; //ValueMap

typedef std::map<edm::RefToBase<reco::Candidate>, CrossCleanerValue>
        CrossCleanerMap; //std::map


#endif
