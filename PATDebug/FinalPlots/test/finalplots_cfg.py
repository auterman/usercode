import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", 
                       fileName = cms.string("plots.root") )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
  #      'file:/rdata2/uhh-cms014/data/auterman/data/RelVal3_3_0ZmumuJets_Pt_20_300_GEN-SIM-RECO_LowLumiPileUp-v1.root'
        '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
    )
)


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

process.load("PATDebug.FinalPlots.finalplots_cfi")
process.finalPlot_RECO  = process.finalPlot.clone()
process.finalPlot_RECO.Jet  = 'ak5CaloJets'
process.finalPlot_RECO.MET  = 'met'

process.finalPlot_PAT  = process.finalPlot.clone()
process.finalPlot_PAT.Jet  = 'allLayer1JetsAK5'
process.finalPlot_PAT.MET  = 'allLayer1METsAK5'


process.reco = cms.Path(	  process.finalPlot_RECO )

#process.pat  = cms.Path(		  process.patDefaultSequence 
#                          	* process.finalPlot_PAT )
