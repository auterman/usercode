#ifndef CrossCleanerResult_h
#define CrossCleanerResult_h

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/RefToBase.h"

struct CrossCleanerValue{ //ValueMap
  bool keep; //probably not necessary: can also check size of 'objects'
  std::vector<const reco::Candidate*> objects;
};
typedef edm::ValueMap<CrossCleanerValue> CrossCleanerResult; //ValueMap

#endif
