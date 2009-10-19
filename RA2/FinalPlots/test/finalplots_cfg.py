import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(0) )

process.TFileService = cms.Service("TFileService", 
                       fileName = cms.string("data.root") )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/rdata2/uhh-cms014/data/auterman/data/RelVal3_3_0ZmumuJets_Pt_20_300_GEN-SIM-RECO_LowLumiPileUp-v1.root'
    )
)

process.load("RA2.FinalPlots.finalplots_cfi")
process.finalPlot.Jet  = 'iterativeCone5CaloJets'
#process.finalPlot.Jet  = 'ak5CaloJets'
process.finalPlot.MET  = 'metOptNoHFHO'

process.p = cms.Path(process.finalPlot)
