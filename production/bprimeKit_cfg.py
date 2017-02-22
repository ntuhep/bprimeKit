import sys,os,importlib  # Python2.7 libaries
import copy
import ConfigParser
import FWCore.ParameterSet.Config            as cms
import FWCore.ParameterSet.VarParsing        as opts
import bpkFrameWork.bprimeKit.optionInit     as myOptions

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------
options = opts.VarParsing ('analysis')
myOptions.initB2GOptions( options )
myOptions.initBPKOptions( options )
options.parseArguments()

mysetting = importlib.import_module('bpkFrameWork.bprimeKit.bprimeKit_' + options.DataProcessing )

print "\nRunning with DataProcessing option ", options.DataProcessing, " and with global tag", mysetting.GlobalTag, "\n"

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------
process = cms.Process("bprimeKit")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.sample))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(options.wantSummary) )
process.options.allowUnscheduled = cms.untracked.bool(True) ## Run all processes regardless of definition

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
if( options.Debug ):
   process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")
process.load("RecoEgamma.ElectronIdentification.ElectronIDValueMapProducer_cfi")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag.globaltag = mysetting.GlobalTag

#-------------------------------------------------------------------------------
#   Reprocessing Jets
#     For settings, see the bprimeKit/python/jettoolbox_settings.py
#-------------------------------------------------------------------------------
from bpkFrameWork.bprimeKit.jettoolbox_settings import *
jettoolbox_settings( process, "Data" not in options.DataProcessing )

#-------------------------------------------------------------------------------
#   Settings for Egamma Identification
#-------------------------------------------------------------------------------
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

dataFormat = DataFormat.MiniAOD
switchOnVIDPhotonIdProducer(process, dataFormat)
switchOnVIDElectronIdProducer(process, dataFormat)

my_elid_modules  = []
my_phoid_modules = []

my_elid_modules.append( mysetting.ElectronIDModule )
my_elid_modules.append( mysetting.ElectronIDHEEPModule )

my_phoid_modules.append( 'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring15_50ns_V1_cff' )

for idmod in my_elid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

for idmod in my_phoid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

#-------------------------------------------------------------------------------
#   bprimeKit configuration importing
#-------------------------------------------------------------------------------
process.TFileService = cms.Service("TFileService",
        fileName = cms.string( options.outputLabel )
        )

# See the file python/bprimeKit_* default settings for the various DataProcessings
process.bprimeKit = mysetting.bprimeKit

#-------------------------------------------------------------------------------
#   Final output settings
#-------------------------------------------------------------------------------
# process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

process.Path = cms.Path(
    process.bprimeKit
)
