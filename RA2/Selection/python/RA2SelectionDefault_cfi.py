import FWCore.ParameterSet.Config as cms

from RA2.Selection.PatTagNames_cfi import *


## Default RA2 Selectors
RA2SelectionDefault = cms.PSet(
    # Sequence of selectors
    selectionSequence = cms.vstring(
    'JetPreSelection',
    'PrimaryVertex', 
    'JetEMFrac', 
    'ChargedFrac',
    'LeptonVeto',
    'JetSelection',
    'MET',
    'METdPhiMin',
    #'HLTJet180',
    #'HLTJet250'
    #'HLTHT240'
    ),
    
    # Selector's list
    selectors = cms.PSet(
    # 1. Preselection
    JetPreSelection  = cms.PSet( selector = cms.string('GenericJetEventSelector'),
                               jetTag = PatTagNames.jetTag,
                               minPt = cms.vdouble(50., 50., 50.),
                               maxEta = cms.vdouble(2.5, 2.5, 2.5),
                               minEMFraction = cms.vdouble(0.05, 0.05, 0.05),             
                               maxEMFraction = cms.vdouble(0.95, 0.95, 0.95)
                              ),
            
    PrimaryVertex = cms.PSet( selector = cms.string('PrimaryVertexEventSelector'),
                              vertexTag = PatTagNames.vertexTag
                              ),
            
            
    JetEMFrac     = cms.PSet( selector = cms.string('EMFractionSelector'),
                              jetTag = PatTagNames.jetTag,
                              maxEta = cms.double(5.0),
                              minFraction = cms.double(0.175),
                              minPt = cms.double(10.0),
                              ),
    
    ChargedFrac   = cms.PSet( selector = cms.string('ChargedFractionSelector'),
                              jetTag = PatTagNames.jetTag,
                              minPt = cms.double(10.0),
                              maxEta = cms.double(5.0),
                              minFraction = cms.double(0.1),
                              minTracks = cms.uint32(1)
                              ),
    
    LeptonVeto = cms.PSet( selector = cms.string('LeptonVetoSelector'),
                              electronTag = PatTagNames.electronTag,
                              muonTag = PatTagNames.muonTag,
                              beamSpot = PatTagNames.beamSpot,
                              minMuonPt = cms.double(10.),
                              maxMuonEta = cms.double(2.4),
                              muonIsolation = cms.double(0.1),
                              muonMinHits = cms.uint32(11),
                              maxMuonDxy = cms.double(0.2),
                              minElectronPt = cms.double(15.0),
                              maxElectronEta   = cms.double(2.5),   
                              electronIsolation = cms.double(0.5),
                              maxElectronDxy = cms.double(0.2)
                              ),
    
    JetSelection  = cms.PSet( selector = cms.string('GenericJetEventSelector'),
                              jetTag = PatTagNames.jetTag,
                              minPt = cms.vdouble(180., 150., 50.),
                              maxEta = cms.vdouble(2.5, 2.5, 2.5),
                              minEMFraction = cms.vdouble(0.05, 0.05, 0.05),       
                              maxEMFraction = cms.vdouble(0.95, 0.95, 0.95)
                              ),
    

    
    METdPhiMin    = cms.PSet( selector = cms.string('MetJetEventSelector'),
                              jetTag = PatTagNames.jetTag,
                              metTag = PatTagNames.metTag,
                              uncorrTypeMet  = cms.string('uncorrMUON'),
                              metDPhiMin     = cms.double(0.3),
                              rDistJetsMin   = cms.double(0.),
                              dPhiJet2MetMin = cms.double(0.),
                              NJets_metIso   = cms.uint32(3),
                              ),
    
    MET           = cms.PSet( selector = cms.string('MetEventSelector'),
                              metTag = PatTagNames.metTag,
                              uncorrType = cms.string('uncorrMUON'),
                              minMET = cms.double(150.0),
                              ),
  
    HLTJet250     = cms.PSet( selector = cms.string('HLTEventSelector'),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              pathNames = cms.vstring('HLT_Jet250','HLT_DiJetAve220'),
                              ),
    HLTJet180     = cms.PSet( selector = cms.string('HLTEventSelector'),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              pathNames = cms.vstring('HLT_Jet180','HLT_DiJetAve130'),
                              ),
    HLTHT240      = cms.PSet( selector = cms.string('HLTEventSelector'),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              pathNames = cms.vstring('HLT_HT200','HLT_HT240')
                              ),
    
    ) # End selector's list
    )
