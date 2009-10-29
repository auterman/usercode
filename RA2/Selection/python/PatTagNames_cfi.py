import FWCore.ParameterSet.Config as cms

### Definition of all tags here

PatTagNames = cms.PSet(
    tauTag        = cms.InputTag("allLayer1Taus"),
    electronTag   = cms.InputTag("allLayer1Electrons"),
    photonTag     = cms.InputTag("allLayer1Photons"),
    jetTag        = cms.InputTag("allLayer1JetsSC5"),
    muonTag       = cms.InputTag("allLayer1Muons"),
    metTag        = cms.InputTag("allLayer1METsSC5"),
    genJetTag     = cms.InputTag("sisCone5GenJets"),
    genPartTag    = cms.InputTag("genParticles"),
    trackTag      = cms.InputTag("generalTracks"),
    beamSpot      = cms.InputTag("offlineBeamSpot"),
    vertexTag     = cms.InputTag("offlinePrimaryVertices")
)
