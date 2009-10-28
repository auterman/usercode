import FWCore.ParameterSet.Config as cms

finalPlot = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '')
)

finalPlot_JEC_UP = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_JEC_UP')
)

finalPlot_JEC_DN = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_JEC_DN')
)

finalPlot_method_UP = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_method_UP')
)

finalPlot_method_DN = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '_method_DN')
)



##E.g. for closure tests:
finalPlot_MCtruth = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '')
)

finalPlot_Prediction = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '')
)

