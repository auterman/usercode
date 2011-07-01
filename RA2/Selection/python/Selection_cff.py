import FWCore.ParameterSet.Config as cms

from RA2.Selection.RA2SelectionDefault_cfi import *

SelectionDefault = cms.EDFilter("Selection",
    selections = RA2SelectionDefault,
    
# only store what passes these selections
# If empty, stores information on all events
    filterSelection = cms.vstring(
    'JetPreSelection',
    'PrimaryVertex', 
    'DirectLeptonVeto',
    'JetSelection',
    'MET',
    'METdPhiMin'
  ),

    # Weight to add to each event
    #weightSource= cms.InputTag("Summer08WeightProducer"),
    #eventWeight = cms.double(1)
)
