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
from bpkFrameWork.bprimeKit.jetmet_settings import *
jet_settings( process, not mysetting.isData )
print '\nFinished jet toolbox setup.....\n'

print '\nBeginning met setup.....\n'
met_settings( process, mysetting.isData, mysetting.Year == '2017' )
print '\n\nFinished met setup.....\n'
#-------------------------------------------------------------------------------
#   Settings for Egamma Identification and Energy Correction bug fixing
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPostRecoRecipes#106X
from EgammaUser.EgammaPostRecoTools.EgammaPostRecoTools import setupEgammaPostRecoSeq

Era = ''
if mysetting.Year == '2016':
    Era = '2016-Legacy'
elif mysetting.Year == '2017':
    Era = '2017-Nov17ReReco'
elif mysetting.Year == '2018':
    Era = '2018-Prompt'

fall17V2PhoIDModules = [
    'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Fall17_94X_V2_cff',
    'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff'
    ]
fall17V2EleIDModules = [
    'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff',
    'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff',
    'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff',
    'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff'
    ]

setupEgammaPostRecoSeq(
    process,
    runVID               = True,
    eleIDModules         = fall17V2EleIDModules,
    phoIDModules         = fall17V2PhoIDModules,
    runEnergyCorrections = True,
    era                  = Era
    )

#-------------------------------------------------------------------------------
#   Level 1 ECAL prefiring
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/viewauth/CMS/L1ECALPrefiringWeightRecipe
from PhysicsTools.PatUtils.l1ECALPrefiringWeightProducer_cfi import l1ECALPrefiringWeightProducer
process.prefiringweight = l1ECALPrefiringWeightProducer.clone(
    DataEra                      = cms.string('2017BtoF' if mysetting.Year == '2017' else '2016BtoH'),
    UseJetEMPt                   = cms.bool(False),
    PrefiringRateSystematicUncty = cms.double(0.2),
    SkipWarnings = False
)

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
if mysetting.Year == '2016' or mysetting.Year == '2017':
    if not mysetting.isData:
        process.externalSequence *= process.prefiringweight

if mysetting.Year == '2017' or mysetting.Year == '2018':
    process.externalSequence *= process.ecalBadCalibReducedMINIAODFilter

process.Path = cms.Path(
    process.egammaPostRecoSeq*
    process.JetToolBoxSequence*
    process.MetSequence*
    process.externalSequence*
    process.bprimeKit
    )
