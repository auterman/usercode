import FWCore.ParameterSet.Config as cms

weightProducer = cms.EDProducer('WeightProducer',
   weight = cms.double( 1.0 )
)
