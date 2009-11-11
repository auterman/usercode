import FWCore.ParameterSet.Config as cms

from RA2.Selection.RA2SelectionDefault_cfi import *
SelectionDefault = cms.EDFilter("Selection",
    selections = RA2SelectionDefault,
    filterSelection = cms.vstring(
      'JetPreSelection',
      'PrimaryVertex', 
      'JetEMFrac', 
      'ChargedFrac',
      'LeptonVeto',
      'JetSelection',
      'MET',
      'METdPhiMin'
      #'HLTHT240'
    ),
    weightName = cms.InputTag('weightProducer:weight') 
)
RA2SelectionDefault.selectionSequence = SelectionDefault.filterSelection


from RA2.Selection.RA2SelectionQCDSmear_cfi import *
SelectionQCDSmear = cms.EDFilter("Selection",
    selections = RA2SelectionQCDSmear,
    filterSelection = cms.vstring(
      'JetPreSelection',
      'PrimaryVertex', 
      'JetEMFrac', 
      'ChargedFrac',
      'LeptonVeto',
      'JetSelection',
      'MET',
      'METdPhiMin'
      #'HLTHT240'
    ),
    weightName = cms.InputTag('weightProducer:weight')
)
RA2SelectionQCDSmear.selectionSequence = SelectionQCDSmear.filterSelection

