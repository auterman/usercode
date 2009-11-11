import FWCore.ParameterSet.Config as cms

from RA2.Selection.RA2SelectionDefault_cfi import *

RA2SelectionQCDSmear = cms.PSet(
  # Sequence of selectors
  selectionSequence = cms.vstring(
    'JetPreSelection',
    'PrimaryVertex', 
    'JetEMFrac', 
    'ChargedFrac',
    'LeptonVeto',
    'JetSelection',
    'MET',
   #'METdPhiMin',
    #'HLTJet180',
    #'HLTJet250'
    #'HLTHT240'
  ),
    
  # Selector's list
  selectors = cms.PSet(
    
    ## For the QCD smearing control-sample, other MET selection cuts w.r.t. to the
    ## default selection are necessarry:
    METdPhiMin    = cms.PSet( selector = cms.string('MetJetEventSelector'),
                              jetTag = RA2SelectionDefault.selectors.METdPhiMin.jetTag,
                              metTag = RA2SelectionDefault.selectors.METdPhiMin.metTag,
                              uncorrTypeMet  = RA2SelectionDefault.selectors.METdPhiMin.uncorrTypeMet,
                              metDPhiMin     = cms.double(0.3),
                              rDistJetsMin   = cms.double(0.),
                              dPhiJet2MetMin = cms.double(0.),
                              NJets_metIso   = cms.uint32(3),
                              ),
    
    MET           = cms.PSet( selector = cms.string('MetEventSelector'),
                              metTag = RA2SelectionDefault.selectors.MET.metTag,
                              uncorrType = cms.string('uncorrMUON'),
                              minMET = cms.double(0.0)
                              ),
  
    ## Selection cuts similar for the QCD smaering and the default selection,
    ## if one cut is changed it is automatically changed for all methods!
    JetPreSelection = RA2SelectionDefault.selectors.JetPreSelection,
    PrimaryVertex   = RA2SelectionDefault.selectors.PrimaryVertex,
    JetEMFrac       = RA2SelectionDefault.selectors.JetEMFrac,
    ChargedFrac     = RA2SelectionDefault.selectors.ChargedFrac,
    LeptonVeto      = RA2SelectionDefault.selectors.LeptonVeto,
    JetSelection    = RA2SelectionDefault.selectors.JetSelection,
    HLTJet250       = RA2SelectionDefault.selectors.HLTJet250,
    HLTJet180       = RA2SelectionDefault.selectors.HLTJet180,
    HLTHT240        = RA2SelectionDefault.selectors.HLTHT240,
  ) # End selector's list
)
