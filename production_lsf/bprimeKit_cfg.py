# *****************************************************************************************
# Usage:
#
# cmsRun bprimeKit_cfg.py maxEvts=N
#
# The globalTag is automatically chosen according to the input 'DataProcessing' value.
# However it can be explictily specified to override the default option.
#
#  Running on 25 ns MC (default settings):
#  cmsRun b2gedmntuples_cfg.py isData=False DataProcessing='MC25ns'
#  Running on 25 ns data:
#  cmsRun b2gedmntuples_cfg.py isData=True DataProcessing='Data25ns'
#  Running on 50 ns MC:
#  cmsRun b2gedmntuples_cfg.py isData=False DataProcessing='MC50ns'
#  Running on 50 ns data:
#  cmsRun b2gedmntuples_cfg.py isData=True DataProcessing='Data50ns'
#
# *****************************************************************************************
import sys
import copy
import ConfigParser
import FWCore.ParameterSet.Config     as cms
import FWCore.ParameterSet.VarParsing as opts
import bpkFrameWork.bprimeKit.optionInit     as myOptions
import bpkFrameWork.bprimeKit.OptionParser

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------
options = opts.VarParsing ('analysis')
myOptions.initB2GOptions( options )
myOptions.initBPKOptions( options )

options.register('RunRange',
    0,
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.int,
    'Run ID to process'
    )

options.parseArguments()
myParser = bpkFrameWork.bprimeKit.OptionParser.OptionParser( options )

print "\nRunning with DataProcessing option ", myParser.GetProcess(), " and with global tag", myParser.GetSetting('GlobalTag'), "\n"

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------
process = cms.Process("bprimeKit")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.sample)
    )

if options.RunRange:
    input_string =  "{0}:1-{0}:max".format(options.RunRange)
    print "Running with RunRange:", options.RunRange , "( ", input_string , ")"

    process.source.eventsToProcess = cms.untracked.VEventRange( input_string ); # All events in process

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(options.wantSummary) )
process.options.allowUnscheduled = cms.untracked.bool(True) ## Run all processes regardless of definition

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.cerr.FwkReport.reportEvery = 1
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
process.GlobalTag.globaltag = myParser.GetSetting('GlobalTag')

#-------------------------------------------------------------------------------
#   Reprocessing Jets
#     For settings, see the bprimeKit/python/jettoolbox_settings.py
#-------------------------------------------------------------------------------
# from bpkFrameWork.bprimeKit.jettoolbox_settings import *
# jettoolbox_settings( process, myParser.IsMC() )

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

my_elid_modules.append( myParser.GetSetting('ElectronIDModule') )
my_elid_modules.append( myParser.GetSetting('ElectronIDHEEPModule') )

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
process.load('bpkFrameWork.bprimeKit.bprimeKit_cfg_test' )
# Passing input options to bprimeKit
process.bprimeKit.runMuonJetClean = cms.bool( options.RunMuonJetClean )
process.bprimeKit.runOnB2G        = cms.bool( options.b2gPreprocess )
process.bprimeKit.Debug           = cms.int32( options.Debug )

#-------------------------------------------------------------------------------
#   Final output settings
#-------------------------------------------------------------------------------
if options.Debug :
   process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

if not options.b2gPreprocess:
   print "Running with original pat tuples"
   process.Path = cms.Path( process.bprimeKit )
else:
   print "Running with b2g defined filters"
   process.bprimeKit.muonlabel = cms.VInputTag("muonUserData")
   process.bprimeKit.eleclabel = cms.VInputTag("electronUserData")
   process.bprimeKit.pholabel  = cms.VInputTag("photonUserData")
   process.bprimeKit.jetlabel = cms.VInputTag( "jetUserData" , "jetUserDataAK8" , "jetUserDataAK8" )
   process.QGTagger.srcJets = cms.InputTag( "jetUserData" )
   process.endPath = cms.Path(process.bprimeKit)