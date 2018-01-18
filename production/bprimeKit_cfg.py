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

options.setDefault('maxEvents', 1000 )

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
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
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
#   Settings for Egamma Identification
#-------------------------------------------------------------------------------
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

dataFormat = DataFormat.MiniAOD
switchOnVIDPhotonIdProducer(process, dataFormat)
switchOnVIDElectronIdProducer(process, dataFormat)

my_elid_modules  = []
my_phoid_modules = []

my_elid_modules.append( mysetting.ElectronIDModule )
my_elid_modules.append( mysetting.ElectronIDHEEPModule )

my_phoid_modules.append( mysetting.PhotonIDModule )

for idmod in my_elid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

for idmod in my_phoid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)
#Avoid not to find 
#RecoEgamma/PhotonIdentification/data/Fall17/HggPhoId_92X_barrel_BDT.weights.xml
#RecoEgamma/PhotonIdentification/data/Fall17/HggPhoId_92X_endcap_BDT.weights.xml
#https://github.com/lsoffi/cmssw/blob/CMSSW_9_4_0_pre3_TnP/RecoEgamma/PhotonIdentification/python/Identification/mvaPhotonID_RunIIFall17_v1_cff.py#L29
#which are not used by bprimekit.
delattr(process, 'photonMVAValueMapProducer')


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

process.Path = cms.Path(
    process.egmGsfElectronIDSequence*
    process.egmPhotonIDSequence*
    process.JetToolBoxSequence*
    process.bprimeKit
)
