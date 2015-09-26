import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

ivars = opts.VarParsing('analysis')

ivars.register( 'sample' , 
   '/store/data/Run2015B/Charmonium/AOD/PromptReco-v1/000/251/168/00000/1602316B-CF26-E511-BCBA-02163E011BF3.root',
   opts.VarParsing.multiplicity.singleton,
   opts.VarParsing.varType.string,
   'input file to run'
)

ivars.register('globalTag',
   '74X_dataRun2_Prompt_v0',
   opts.VarParsing.multiplicity.singleton,
   opts.VarParsing.varType.string,
   'Global Tag'
)

ivars.outputFile='hltOnly.root'

ivars.parseArguments()

process = cms.Process("demo")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(4))
process.TFileService = cms.Service("TFileService",fileName = cms.string(ivars.outputFile))
process.source = cms.Source(
   "PoolSource",
   skipEvents=cms.untracked.uint32(0),
   fileNames = cms.untracked.vstring(ivars.sample)
)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, ivars.globalTag , '')

process.load('HLTrigger.HLTanalyzers.HLTBitAnalyser_cfi')
process.hltbitanalysis.UseTFileService = cms.untracked.bool(True)

process.hltAna = cms.Path(process.hltbitanalysis)
process.schedule = cms.Schedule(process.hltAna)
