import FWCore.ParameterSet.Config as cms

finalPlot = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  ''),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 ),
     #
     Towers           = cms.InputTag("towerMaker"),
     Tracks           = cms.InputTag("ctfWithMaterialTracks"),
     GroupNTowers     = cms.int32( 3 ),
)

