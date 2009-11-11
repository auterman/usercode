import FWCore.ParameterSet.Config as cms
process = cms.Process("RA2")

##Only summary
process.load("FWCore.MessageService.MessageLogger_cfi")

## show all messages in the log
#process.MessageLogger = cms.Service("MessageLogger",
#    cout = cms.untracked.PSet(
#        default = cms.untracked.PSet(
#            limit = cms.untracked.int32(-1) ) ),
#    destinations = cms.untracked.vstring('cout') )				     

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       #'/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/3291E09D-67B7-DE11-9ED6-003048678C9A.root'
       'file:/rdata2/uhh-cms014/data/auterman/data/PATLayer1_Output.fromAOD_full.root'
    )
)


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
process.p = cms.Path( process.weightProducer
                    * process.SelectionDefault
		    * process.finalPlot
		    )

## No EDM event output
