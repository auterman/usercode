import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.TFileService = cms.Service("TFileService", 
                       fileName = cms.string("plots.root") )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
  #      'file:/rdata2/uhh-cms014/data/auterman/data/RelVal3_3_0ZmumuJets_Pt_20_300_GEN-SIM-RECO_LowLumiPileUp-v1.root'
  #      '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_2360GeV-v2/0004/FED63CFD-18DE-DE11-B2B4-002618943957.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_2360GeV-v2/0004/FC2A02B4-18DE-DE11-A2C0-0026189438FE.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_2360GeV-v2/0004/FAAFEE64-20DE-DE11-A190-002618FDA237.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_2360GeV-v2/0004/FA77B88B-17DE-DE11-AD68-00248C0BE014.root',
        '/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_2360GeV-v2/0004/E67E0EDD-18DE-DE11-8833-0026189438CF.root',
    )
)

## configure geometry
process.load("Configuration.StandardSequences.Geometry_cff")

## configure conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('STARTUP_V7::All')

process.GlobalTag.globaltag = cms.string('START3X_V26::All')

## load magnetic field
process.load("Configuration.StandardSequences.MagneticField_cff")

##------------------------------------------------------------------
## Call PAT if running on AOD --------------------------------------
#from PhysicsTools.PatAlgos.patTemplate_cfg import *
#from PhysicsTools.PatAlgos.tools.coreTools import *
##restrictInputToAOD(process)
##removeMCMatching(process, 'Muons')
##removeAllPATObjectsBut(process, ['Muons'])
##removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])
#from PhysicsTools.PatAlgos.tools.tauTools import *
##switchTo31Xdefaults(process)
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
##run33xOn31xMC( process,
##	       jetSrc = cms.InputTag("antikt5CaloJets"),
##	       jetIdTag = "antikt5"
##	       )
#
#process.maxEvents.input = -1
#process.source.fileNames = [
#        '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
#    ]
#
#process.TFileService = cms.Service("TFileService", 
#                       fileName = cms.string("plots.root") )
#
#from JetMETCorrections.Configuration.JetCorrectionEra_cff import *
#JetCorrectionEra.era = 'Summer09_7TeV'
#process.load('JetMETCorrections.Configuration.JetCorrectionProducers_cff')

process.load("Analysis.FinalPlots.finalplots_cfi")
process.finalPlot.Jet  = 'ak5CaloJets'
process.finalPlot.MET  = 'met'
process.finalPlot.JetPtMin = 5.0
process.finalPlot.JetEtaMax = 2.4
process.finalPlot.Tracks = 'generalTracks'

process.reco = cms.Path(	  
                         #process.patDefaultSequence *
			 process.finalPlot
                       )
