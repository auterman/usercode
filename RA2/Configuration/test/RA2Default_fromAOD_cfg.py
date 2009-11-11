import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2")


## Call PAT if running on AOD --------------------------------------
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *
#restrictInputToAOD(process)
#removeMCMatching(process, 'Muons')
#removeAllPATObjectsBut(process, ['Muons'])
#removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])
from PhysicsTools.PatAlgos.tools.tauTools import *
switchTo31Xdefaults(process)

process.maxEvents.input = 100
process.source.fileNames = [
       #'/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/3291E09D-67B7-DE11-9ED6-003048678C9A.root'
       'file:/rdata2/uhh-cms014/data/auterman/data/RelVal3_3_0TTbar-GEN-SIM-RECO-STARTUP31X_V8-v1.root'
    ]
#process.MessageLogger.cout = cms.untracked.PSet(
#        default = cms.untracked.PSet(
#            limit = cms.untracked.int32(-1) )) ## show all messages in the log
#process.MessageLogger.destinations = ['cout']

## An event weight producer ----------------------------------------
process.load("RA2.WeightProducer.weightproducer_cfi")
process.weightProducer.weight = 2.0  # weight per event for this sample

## The RA2 Selection -----------------------------------------------
process.load("RA2.Selection.Selection_cff")
process.PatTagNames.jetTag       = 'selectedLayer1Jets'
process.PatTagNames.metTag       = 'layer1METs'
process.SelectionDefault.selections.selectors.MET.minMET = 20.
#process.SelectionDefault.filterSelection = ['JetPreSelection','MET']


## Produce root file with 'final plots' for the current sample -----
process.TFileService = cms.Service("TFileService",
                       fileName = cms.string("finalplots.root") )
process.load("RA2.FinalPlots.finalplots_cfi")
process.finalPlot.Jet  = 'selectedLayer1Jets'
process.finalPlot.MET  = 'layer1METs'


## Call all modules ------------------------------------------------  
process.p = cms.Path( 
                      process.patDefaultSequence
                    * process.weightProducer
		    * process.SelectionDefault
		    * process.finalPlot
		    )

## process.outpath is set in PhysicsTools.PatAlgos.patTemplate_cfg
## No EDM-output file
del process.outpath
