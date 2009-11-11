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


## Produce new jet/MET collection, RA2 background method specific --
## -> QCD Smearing method <-
process.load("RA2.QCDSmearProd.qcdsmearprod_cfi")
process.QCDfromSmearing.fileName = 'Summer08DijetResponse.root'
process.QCDfromSmearing.Jet      = 'ak5GenJets'
process.QCDfromSmearing.MET      = 'genMetIC5GenJets'
process.QCDfromSmearing.uncertaintyName = ''
process.QCDfromSmearing.plotName = 'hResponse'
process.QCDfromSmearing.ptBinNames = ('Pt0_Eta0','Pt1_Eta0','Pt2_Eta0','Pt3_Eta0','Pt4_Eta0')
process.QCDfromSmearing.ptBins   = (20.0, 50.0, 100.0, 200.0, 7000.0 )
##JEC UP    (Note: It's currently using the same smear functions as above; the syst. uncertainty due to JEC + 1 sigma will therefore be 0! This is just an example!)
process.QCDfromSmearingJECUp.fileName = 'Summer08DijetResponse.root'
process.QCDfromSmearingJECUp.uncertaintyName = ''
process.QCDfromSmearingJECUp.plotName = 'hResponse'
##JEC DOWN  (Note: It's currently using the same smear functions as above; the syst. uncertainty due to JEC + 1 sigma will therefore be 0! This is just an example!)
process.QCDfromSmearingJECDn.fileName = 'Summer08DijetResponse.root'
process.QCDfromSmearingJECDn.uncertaintyName = ''
process.QCDfromSmearingJECDn.plotName = 'hResponse'


## Produce root file with 'final plots' for the current sample -----
process.TFileService = cms.Service("TFileService",
                       fileName = cms.string("finalplots.root") )
process.load("RA2.FinalPlots.finalplots_cfi")
process.finalPlot.Jet        = 'QCDfromSmearing:SmearedJet'
process.finalPlot.MET        = 'QCDfromSmearing:SmearedMET'
process.finalPlot_JEC_UP.Jet = 'QCDfromSmearingJECUp:SmearedJet'
process.finalPlot_JEC_UP.MET = 'QCDfromSmearingJECUp:SmearedMET'
process.finalPlot_JEC_DN.Jet = 'QCDfromSmearingJECDn:SmearedJet'
process.finalPlot_JEC_DN.MET = 'QCDfromSmearingJECDn:SmearedMET'
#(Note: For testing, in 'finalPlot_JEC_UP' (FinalPlots.cc, line 123ff) the plots are scaled by +8%, and in 'finalPlot_JEC_DN' by -8%!)


## Call all modules ------------------------------------------------  
process.p = cms.Path( 
                      process.patDefaultSequence
                    * process.weightProducer
		    * process.SelectionQCDSmear
		    * process.QCDfromSmearing
		    * process.QCDfromSmearingJECUp
		    * process.QCDfromSmearingJECDn
		    * process.finalPlot
		    * process.finalPlot_JEC_UP
		    * process.finalPlot_JEC_DN
		    )

## process.outpath is set in PhysicsTools.PatAlgos.patTemplate_cfg
## No EDM-output file
del process.outpath
