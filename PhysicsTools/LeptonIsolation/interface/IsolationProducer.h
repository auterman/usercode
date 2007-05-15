/* \class IsolationProducer
 *
 * template class to store isolation
 *
 *
 */
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include <vector>

template <typename C1, typename C2, typename Alg>
class IsolationProducer : public edm::EDProducer {
public:
  IsolationProducer( const edm::ParameterSet & );
  ~IsolationProducer();

private:
  void produce( edm::Event&, const edm::EventSetup& );
  edm::InputTag src_, elements_;
  Alg alg_;
  typedef typename Alg::value_type value_type;
  typedef edm::AssociationVector<edm::RefProd<C1>, 
    std::vector<value_type> > IsolationCollection;
};

template <typename C1, typename C2, typename Alg>
IsolationProducer<C1,C2,Alg>::IsolationProducer( const edm::ParameterSet & cfg ) :
  src_( cfg.template getParameter<edm::InputTag>( "src" ) ),
  elements_( cfg.template getParameter<edm::InputTag>( "elements" ) ),
  alg_(cfg) {
//  Quick fix for version CMSSW_1_3_1:
//  produces<IsolationCollection>();     //CMSSW_1_5_0
  produces<std::vector<value_type> >();  //CMSSW_1_3_x
}

template <typename C1, typename C2, typename Alg>
IsolationProducer<C1,C2,Alg>::~IsolationProducer() {
}

template <typename C1, typename C2, typename Alg>
void IsolationProducer<C1,C2,Alg>::produce( edm::Event& evt, const edm::EventSetup& setup ) {
  using namespace edm;
  using namespace std;
  Handle<C1> src;
  Handle<C2> elements;
  evt.getByLabel( src_, src );
  evt.getByLabel( elements_, elements );
//  Quick fix fir version CMSSW_1_3_1:
//  auto_ptr<IsolationCollection> isolations( new IsolationCollection( edm::RefProd<C1>( src ) )  );
  auto_ptr<std::vector<value_type> > isolations( new std::vector<value_type>);
  
//  size_t i = 0;
  for( typename C1::const_iterator lep = src->begin(); lep != src->end(); ++ lep ) {
    value_type iso = alg_(*lep,*elements, setup); //Changed alg_ interface!
    
    //isolations->setValue( i++, iso ); //CMSSW_1_5_0
    isolations->push_back( iso );       //CMSSW_1_3_x
  }
  evt.put( isolations );
}

