import FWCore.ParameterSet.Config as cms

finalPlot = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  ''),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.4 ),
     #
     Towers           = cms.InputTag("towerMaker"),
     Tracks           = cms.InputTag("ctfWithMaterialTracks"),
     GroupNTowers     = cms.int32( 3 ),
     TowerDeltaEtaMax = cms.double( 0.3 ),
     TowerDeltaPhiMax = cms.double( 0.3 ),
     #
     TrackEtaMax      = cms.double( 2.4 ),
     TrackPtMin       = cms.double( 0.0 ),
     TrackChiSquareMax= cms.double( 8.0 ),
     TrackNumOfHitsMin= cms.int32( 10 ),
     TrackDeltaRMin   = cms.double( 0.5 ),
)

