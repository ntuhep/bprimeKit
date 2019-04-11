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
fileNames = cms.untracked.vstring(options.sample))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

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
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
setupEgammaPostRecoSeq(
    process,
    runVID = True,
    runEnergyCorrections = True if mysetting.Year == '2017' else False,
    era = '2017-Nov17ReReco' if mysetting.Year == '2017' else '2016-Legacy'
    )

#-------------------------------------------------------------------------------
#   Settings for MET bug fixing
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETUncertaintyPrescription#Instructions_for_9_4_X_X_9_for_2
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD (
    process,
    isData = mysetting.isData,
    fixEE2017 = mysetting.isData,
    fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139},
    postfix = "ModifiedMET"
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

process.bugfixingSequence = cms.Sequence()
if ( mysetting.Year == '2017' ):
    #process.bugfixingSequence += process.egammaPostRecoSeq
    process.bugfixingSequence += process.fullPatMetSequenceModifiedMET

process.Path = cms.Path(
    #process.egmGsfElectronIDSequence*
    #process.egmPhotonIDSequence*
    process.egammaPostRecoSeq*
    process.bugfixingSequence*
    process.JetToolBoxSequence*
    process.bprimeKit
)
