import FWCore.ParameterSet.Config as cms

## Default QCD Smearing method
QCDfromSmearing = cms.EDProducer('QCDSmearProd',
     jetType           = cms.string( 'GenJet'),
     Jet               = cms.InputTag('selectedLayer1Jets'),
     MET               = cms.InputTag('layer1METs'),
     fileName          = cms.string( 'data/SmearFunctions.root'),
     ## if plot 'plot' is in a tree 'tree' use as plotName 'tree/plot'!
     plotName          = cms.string( 'smearFunc'),
     uncertaintyName   = cms.string( ''),
     ptBinNames        = cms.vstring(
                             'pt100',
                             'pt500',
			     'pt3000' ),
     ptBins            = cms.vdouble(
                             100.0,
                             500.0,
			     3000.0 )
)

## JEC + 1 sigma systematic uncertainty
QCDfromSmearingJECUp = cms.EDProducer('QCDSmearProd',
     uncertaintyName   = cms.string( 'JECUP'),
     plotName          = cms.string( 'smearFunc'),
     fileName          = cms.string( 'data/SmearFunctions.root'),
     jetType           = QCDfromSmearing.jetType,
     Jet               = QCDfromSmearing.Jet,
     MET               = QCDfromSmearing.MET,
     ptBinNames        = QCDfromSmearing.ptBinNames,
     ptBins            = QCDfromSmearing.ptBins
)

## JEC - 1 sigma systematic uncertainty
QCDfromSmearingJECDn = cms.EDProducer('QCDSmearProd',
     uncertaintyName   = cms.string( 'JECDN'),
     plotName          = cms.string( 'smearFunc'),
     fileName          = cms.string( 'data/SmearFunctions.root'),
     jetType           = QCDfromSmearing.jetType,
     Jet               = QCDfromSmearing.Jet,
     MET               = QCDfromSmearing.MET,
     ptBinNames        = QCDfromSmearing.ptBinNames,
     ptBins            = QCDfromSmearing.ptBins
)

## handle all other systematic likewise:
# ...
