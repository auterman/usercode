import FWCore.ParameterSet.Config as cms

finalPlot = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  ''),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

finalPlot_JEC_UP = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_JEC_UP'),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

finalPlot_JEC_DN = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_JEC_DN'),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

finalPlot_method_UP = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_method_UP'),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

finalPlot_method_DN = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_method_DN'),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)



##E.g. for closure tests:
finalPlot_MCtruth = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  ''),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

finalPlot_Prediction = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  ''),
     weightName       = cms.InputTag('weight'),
     JetPtMin         = cms.double( 30.0 ),
     JetEtaMax        = cms.double( 2.5 )
)

