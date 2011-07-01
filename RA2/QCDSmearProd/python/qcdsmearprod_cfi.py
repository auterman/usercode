import FWCore.ParameterSet.Config as cms

QCDfromSmearing = cms.EDProducer('QCDSmearProd',
     jetType           = cms.string( 'GenJet'),
     Jet               = cms.InputTag('selectedLayer1Jets'),
     MET               = cms.InputTag('layer1METs'),
     fileName          = cms.string( 'data/SmearFunctions.root'),
     ## if plot 'plot' is in a tree 'tree' use as plotName 'tree/plot'!
     plotName          = cms.string( 'smearFunc'),
     uncertaintyName   = cms.string( 'JEC.UP'),
     ptBinNames        = cms.vstring(
                             'pt100',
                             'pt500',
			     'pt3000' ),
     ptBins            = cms.vdouble(
                             100.0,
                             500.0,
			     3000.0 )
     
)
