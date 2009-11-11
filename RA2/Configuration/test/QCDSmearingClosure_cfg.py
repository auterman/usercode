import FWCore.ParameterSet.Config as cms

process = cms.Process("RA2QCD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       'file:/rdata2/uhh-cms014/data/auterman/data/RelVal3_3_0TTbar-GEN-SIM-RECO-STARTUP31X_V8-v1.root'
       #'/store/relval/CMSSW_3_3_0/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/3291E09D-67B7-DE11-9ED6-003048678C9A.root'
    )
)


## -> QCD Smearing method <-
process.load("RA2.QCDSmearProd.qcdsmearprod_cfi")
process.QCDfromSmearing.fileName = 'Summer08DijetResponse.root'
process.QCDfromSmearing.Jet = 'ak5GenJets'
process.QCDfromSmearing.MET = 'genMetIC5GenJets'
process.QCDfromSmearing.uncertaintyName = ''
process.QCDfromSmearing.plotName = 'hResponse'
process.QCDfromSmearing.ptBinNames = ('Pt0_Eta0','Pt1_Eta0','Pt2_Eta0','Pt3_Eta0','Pt4_Eta0')
process.QCDfromSmearing.ptBins = (20.0, 50.0, 100.0, 200.0, 7000.0 )


## Create 'Final Plots' for smeared GenJets and CaloJets:
process.TFileService = cms.Service("TFileService",
                       fileName = cms.string("finalplots.root") )
process.load("RA2.FinalPlots.finalplots_cfi")
process.finalPlot_MCtruth.Jet  = 'ak5CaloJets'
process.finalPlot_MCtruth.MET  = 'metNoHFHO'
process.finalPlot_Prediction.Jet  = 'QCDfromSmearing:SmearedJet'
process.finalPlot_Prediction.MET  = 'QCDfromSmearing:SmearedMET'

  
process.p = cms.Path(  process.QCDfromSmearing 
                    * process.finalPlot_MCtruth
		    * process.finalPlot_Prediction
		    )

#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string('out_QCDSmearProd.root')
#                               #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
#                               #outputCommands = cms.untracked.vstring('drop *', *patEventContent ) 
#                               )
#
#process.e = cms.EndPath(process.out )
