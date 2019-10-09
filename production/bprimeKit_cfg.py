import importlib  # Python2.7 libaries
import FWCore.ParameterSet.Config            as cms
import FWCore.ParameterSet.VarParsing        as opts

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------
options = opts.VarParsing ('analysis')

options.register('sample',
    '',
    opts.VarParsing.multiplicity.list,
    opts.VarParsing.varType.string,
    'Sample to analyze')

options.register('outputLabel',
    'bpk_ntuple.root',
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Output label')

options.register('DataProcessing',
    '',
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Data processing types. Options are file found in python/bprimeKit_*.py')

options.register('Debug',
    0,
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.int,
    'Debugging output level' )

options.setDefault('maxEvents', 100 )

options.parseArguments()

mysetting = importlib.import_module('bpkFrameWork.bprimeKit.bprimeKit_' + options.DataProcessing )

print '\nRunning with DataProcessing option [', options.DataProcessing, '] and with global tag [', mysetting.GlobalTag, ']\n'

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------
process = cms.Process('bprimeKit')

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(options.sample)
    )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if( options.Debug ):
   process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = mysetting.GlobalTag

print '\nFinished basic setups...\n'

#-------------------------------------------------------------------------------
#   Reprocessing Jets
#     For settings, see the bprimeKit/python/jettoolbox_settings.py
#-------------------------------------------------------------------------------
print '\nBeginning jet toolbox setup.....\n'
from bpkFrameWork.bprimeKit.jettoolbox_settings import *
jettoolbox_settings( process, not mysetting.isData )
print '\nFinished jet toolbox setup.....\n'

#-------------------------------------------------------------------------------
#   Settings for Egamma Identification and Energy Correction bug fixing
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2#2017_MiniAOD_V2
#from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
#setupEgammaPostRecoSeq(
#    process,
#    runVID = True,
#    runEnergyCorrections = True if mysetting.Year == '2017' else False,
#    era = '2017-Nov17ReReco' if mysetting.Year == '2017' else '2016-Legacy'
#    )

#-------------------------------------------------------------------------------
#   Settings for MET bug fixing
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETUncertaintyPrescription#Instructions_for_9_4_X_X_9_for_2
#from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
#runMetCorAndUncFromMiniAOD (
#    process,
#    isData = mysetting.isData,
#    fixEE2017 = True,
#    fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139},
#    postfix = "ModifiedMET"
#    )

#-------------------------------------------------------------------------------
#   Extra MET filter for 2017 and 2018
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#Declaring_the_producer_in_your_c
process.load('RecoMET.METFilters.ecalBadCalibFilter_cfi')

baddetEcallist = cms.vuint32(
    [872439604,872422825,872420274,872423218,
     872423215,872416066,872435036,872439336,
     872420273,872436907,872420147,872439731,
     872436657,872420397,872439732,872439339,
     872439603,872422436,872439861,872437051,
     872437052,872420649,872422436,872421950,
     872437185,872422564,872421566,872421695,
     872421955,872421567,872437184,872421951,
     872421694,872437056,872437057,872437313])

process.ecalBadCalibReducedMINIAODFilter = cms.EDFilter(
    "EcalBadCalibFilter",
    EcalRecHitSource = cms.InputTag("reducedEgamma:reducedEERecHits"),
    ecalMinEt        = cms.double(50.),
    baddetEcal       = baddetEcallist,
    taggingMode      = cms.bool(True),
    debug            = cms.bool(False)
)

#-------------------------------------------------------------------------------
#   bprimeKit configuration importing
#-------------------------------------------------------------------------------
process.TFileService = cms.Service('TFileService',
    fileName = cms.string( options.outputLabel )
    )

# See the file python/bprimeKit_* default settings for the various DataProcessings
process.bprimeKit = mysetting.bprimeKit

#-------------------------------------------------------------------------------
#   Final output settings
#-------------------------------------------------------------------------------
# process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',ignoreTotal = cms.untracked.int32(1) )

process.externalSequence = cms.Sequence()
#if ( mysetting.Year == '2017' ):
#    process.externalSequence *= process.fullPatMetSequenceModifiedMET
if ( mysetting.Year == '2017' or mysetting.Year == '2018' ):
    process.externalSequence *= process.ecalBadCalibReducedMINIAODFilter

process.Path = cms.Path(
#    process.egammaPostRecoSeq*
    process.externalSequence*
    process.JetToolBoxSequence*
    process.bprimeKit
    )
