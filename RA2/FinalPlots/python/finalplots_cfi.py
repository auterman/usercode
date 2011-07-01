import FWCore.ParameterSet.Config as cms

finalPlot = cms.EDAnalyzer('FinalPlots',
     Jet              = cms.InputTag('selectedLayer1Jets'),
     MET              = cms.InputTag('layer1METs'),
     uncertainty_name = cms.string(  '')
)
