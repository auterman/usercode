import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", 
                       fileName = cms.string("zmumu.root") )

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

process.finalPlot_JEC_UP.Jet  = 'iterativeCone5CaloJets'
process.finalPlot_JEC_UP.MET  = 'metOptNoHFHO'
process.finalPlot_JEC_DN.Jet  = 'iterativeCone5CaloJets'
process.finalPlot_JEC_DN.MET  = 'metOptNoHFHO'
process.finalPlot_method_UP.Jet  = 'iterativeCone5CaloJets'
process.finalPlot_method_UP.MET  = 'metOptNoHFHO'
process.finalPlot_method_DN.Jet  = 'iterativeCone5CaloJets'
process.finalPlot_method_DN.MET  = 'metOptNoHFHO'

process.p = cms.Path(process.finalPlot * 
                     process.finalPlot_JEC_UP * 
		     process.finalPlot_JEC_DN * 
		     process.finalPlot_method_UP * 
		     process.finalPlot_method_DN 
		     )
