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
#   Declaring variables
#-------------------------------------------------------------------------------
options = opts.VarParsing ('analysis')

myOptions.initB2GOptions( options )
myOptions.initBPKOptions( options )

options.parseArguments()

myParser = bpkFrameWork.bprimeKit.OptionParser.OptionParser( options )

print "\nRunning with DataProcessing option ", myParser.GetProcess(), " and with global tag", myParser.GetSetting('GlobalTag'), "\n"

#####################################
process = cms.Process("bprimeKit")
#####################################

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
if( options.Debug ):
   process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.categories.append('HLTrigReport')
### Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(options.wantSummary) )
process.options.allowUnscheduled = cms.untracked.bool(True) ## Run all processes regardless of definition
### Number of maximum events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
### Source file
process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
        options.sample
        )
)

#process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("RecoEgamma/PhotonIdentification/PhotonIDValueMapProducer_cfi")
process.load("RecoEgamma.ElectronIdentification.ElectronIDValueMapProducer_cfi")
### Setting global tag
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag.globaltag = myParser.GetSetting('GlobalTag')


print """
#-------------------------------------------------------------------------------
#   Reprocessing Jets
#-------------------------------------------------------------------------------
"""
process.load('CommonTools/PileupAlgos/Puppi_cff')
process.puppi.candName = cms.InputTag('packedPFCandidates')
process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
process.puppiOnTheFly = process.puppi.clone()
process.puppiOnTheFly.useExistingWeights = True

from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

# NOTE: Disable selection to fix subjet issue
# ak4Cut='pt>20  && abs(eta)<5.'
# ak8Cut='pt>100 && abs(eta)<5.'

run_on_data = ('Data' in options.DataProcessing )

listBtagDiscriminators = [
		'pfJetProbabilityBJetTags',
		'pfCombinedInclusiveSecondaryVertexV2BJetTags',
		'pfCombinedMVAV2BJetTags',
		'pfBoostedDoubleSecondaryVertexAK8BJetTags',
		'pfCombinedCvsLJetTags',
		'pfCombinedCvsBJetTags'
		]


print "Add AK4 Jets"
jetToolbox( process, 'ak4', 'jetToolBox_ak4', 'edmOut',
      runOnMC            = run_on_data ,
      addQGTagger        = True,
      bTagDiscriminators = listBtagDiscriminators ,
      Cut                = ''
      )

print "Add AK8 Jets, soft drop"
jetToolbox( process, 'ak8', 'jetToolBox_ak8', 'edmOut',
      runOnMC            = run_on_data ,
      addSoftDropSubjets = True,
      addTrimming        = True,
      rFiltTrim          = 0.1,
      addPruning         = True,
      addFiltering       = True,
      addSoftDrop        = True,
      addNsub            = True,
      addCMSTopTagger    = True,
      bTagDiscriminators = listBtagDiscriminators ,
      Cut                = '' )

print "Add AK8 Jets, top tag"
jetToolbox( process, 'ca8', 'jetToolBox_ak8_toptag', 'edmOut',
      runOnMC            = run_on_data ,
      addMassDrop        = True,
      addCMSTopTagger    = True,
      bTagDiscriminators = listBtagDiscriminators ,
      Cut                = ''
      )

print "Add ak8 jets, puppi"
jetToolbox( process, 'ak8', 'jetToolBox_ak8_puppi', 'edmOut',
      runOnMC             = run_on_data ,
      PUMethod            = 'Puppi',
      addTrimming         = True,
      addPruning          = True,
      addFiltering        = True,
      addSoftDrop         = True,
      addSoftDropSubjets  = True,
      addNsub             = True,
      bTagDiscriminators  = listBtagDiscriminators ,
      Cut                 = ''
      )


print """
#-------------------------------------------------------------------------------
#   Settings for Egamma Identification
#-------------------------------------------------------------------------------"""
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

dataFormat = DataFormat.MiniAOD
switchOnVIDPhotonIdProducer(process, dataFormat)
switchOnVIDElectronIdProducer(process, dataFormat)

my_elid_modules  = []
my_phoid_modules = []

my_elid_modules.append( myParser.GetElectronIDModule( "other"     ) )
my_elid_modules.append( myParser.GetElectronIDModule( "heep" ) )


my_phoid_modules.append( 'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring15_50ns_V1_cff' )
pho_loose_id_label   = "egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-loose"
pho_medium_id_label  = "egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-medium"
pho_tight_id_label   = "egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-tight"

for idmod in my_elid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

for idmod in my_phoid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

## Adapt primary vertex collection
from PhysicsTools.PatAlgos.tools.pfTools import *
adaptPVs(process, pvCollection=cms.InputTag('offlineSlimmedPrimaryVertices'))


print """
#-------------------------------------------------------------------------------
#   bprimeKit configuration importing
#-------------------------------------------------------------------------------"""

process.TFileService = cms.Service("TFileService",
        fileName = cms.string( options.outputLabel )
        )

if options.Debug :
   process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

#-------------------------------------------------------------------------------
#   For Default Label settings, see the "bprimeKit/data/Process*.cfg" files
#-------------------------------------------------------------------------------

process.bprimeKit = cms.EDAnalyzer(
      "bprimeKit",
      #----- Operation paramters --------------------------------------------------------------------------
      MCtag           = cms.untracked.bool( "Data" not in options.DataProcessing ),
      PairCollection  = cms.untracked.int32(1),
      IncludeL7       = cms.untracked.bool(False),
      Debug           = cms.untracked.int32( options.Debug ),
      runOnB2G        = cms.untracked.bool( options.b2gPreprocess ),
      runMuonJetClean = cms.bool( options.RunMuonJetClean ),

      #----- Event level objects --------------------------------------------------------------------------
      rhoLabel            = cms.InputTag( myParser.GetSetting('RhoLabel') ),
      hltLabel            = cms.InputTag( myParser.GetSetting('HLTLabel') ),
      metLabel            = cms.InputTag( myParser.GetSetting('MetLabel') ),
      puInfoLabel         = cms.InputTag( myParser.GetSetting('PileUpLabel') ),

      #----- Vertex related  ------------------------------------------------------------------------------
      offlinePVLabel      = cms.InputTag( myParser.GetSetting('PVLabel')),#CMSSW73X "offlinePrimaryVertices",
      offlinePVBSLabel    = cms.InputTag( myParser.GetSetting('PVBSLabel')),# CMSSW73X"offlinePrimaryVerticesWithBS",
      offlineBSLabel      = cms.InputTag( myParser.GetSetting('BSLabel')),
      conversionsLabel    = cms.InputTag( *myParser.GetMultiSetting('ConversionsLabel') ),

      #----- MC Generation information --------------------------------------------------------------------
      genLabel    = cms.InputTag("prunedGenParticles"),
      genevtLabel = cms.InputTag("generator"),
      gtdigiLabel = cms.InputTag("gtDigis"),
      lheLabel    = cms.InputTag( myParser.GetSetting('LHELabel') ),
      lheRunLabel = cms.InputTag( myParser.GetSetting('LHELabel') ),

      #----- Photon information ------------------------------------------------------------------------
      PhoCollections            = cms.vstring('PhotonInfo'),
      phoLabel                  = cms.VInputTag( myParser.GetSetting('PhotonLabel') ),
      phoLooseIdMap             = cms.InputTag( pho_loose_id_label     ) ,
      phoMediumIdMap            = cms.InputTag( pho_medium_id_label    ) ,
      phoTightIdMap             = cms.InputTag( pho_tight_id_label     ) ,
      phoChargedIsolation       = cms.InputTag( "photonIDValueMapProducer:phoChargedIsolation"                          ) ,
      phoNeutralHadronIsolation = cms.InputTag( "photonIDValueMapProducer:phoNeutralHadronIsolation"                    ) ,
      phoPhotonIsolation        = cms.InputTag( "photonIDValueMapProducer:phoPhotonIsolation"                           ) ,
      full5x5SigmaIEtaIEtaMap   = cms.InputTag( "photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta"),
      effAreaChHadFile          = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt"),
      effAreaNeuHadFile         = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt"),
      effAreaPhoFile            = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt"),

      #----- Lepton related information -------------------------------------------------------------------
      LepCollections  = cms.vstring( 'LepInfo'             ) ,
      muonLabel       = cms.VInputTag( myParser.GetSetting('MuonLabel')       ) ,
      elecLabel       = cms.VInputTag( myParser.GetSetting('ElectronLabel')   ) ,
      tauLabel        = cms.VInputTag( myParser.GetSetting('TauLabel')        ) ,
      eleVetoIdMap    = cms.InputTag( myParser.GetElectronIDLabel( "veto"   )  ) ,
      eleLooseIdMap   = cms.InputTag( myParser.GetElectronIDLabel( "loose"  )  ) ,
      eleMediumIdMap  = cms.InputTag( myParser.GetElectronIDLabel( "medium" )  ) ,
      eleTightIdMap   = cms.InputTag( myParser.GetElectronIDLabel( "tight"  )  ) ,
      eleHEEPIdMap    = cms.InputTag( myParser.GetElectronIDLabel( "heep"   )  ) ,

      #----- Jet Information ------------------------------------------------------------------------------
      JetSettings = cms.VPSet(
         cms.PSet(
            jetCollection = cms.string( 'JetInfo' ),
            jetLabel      = cms.InputTag( myParser.GetSetting('JetLabel') ),
            subjetLabel   = cms.InputTag(''), ## No tag for this collection..
            ),
         cms.PSet(
            jetCollection = cms.string( 'AK8BosonJetInfo' ),
            jetLabel      = cms.InputTag( myParser.GetSetting('FatJetLabel') ),
            subjetLabel   = cms.InputTag( *myParser.GetMultiSetting('SoftDropSubJetLabel') ),
            ),
         cms.PSet(
            jetCollection = cms.string( 'CA8TopJetInfo' ),
            jetLabel      = cms.InputTag( myParser.GetSetting('FatJetLabel') ),
            subjetLabel   = cms.InputTag( *myParser.GetMultiSetting('TopSubJetLabel') ),
            ),
         cms.PSet(
            jetCollection = cms.string( 'AK8BosonJetInfoPuppi' ),
            jetLabel      = cms.InputTag( myParser.GetSetting('PuppiFatJetLabel') ),
            subjetLabel   = cms.InputTag( *myParser.GetMultiSetting('PuppiSoftDropSubJetLabel') ),
            ),
            )
      )

process.edmOut = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('edmOut.root'),
    outputCommands = cms.untracked.vstring("drop *"),
    dropMetaData = cms.untracked.string('ALL'),
    )

if not options.b2gPreprocess:
   print "Running with original pat tuples"
   process.Path = cms.Path(process.bprimeKit)
else:
   print "Running with b2g defined filters"
   process.bprimeKit.muonlabel = cms.VInputTag("muonUserData")
   process.bprimeKit.eleclabel = cms.VInputTag("electronUserData")
   process.bprimeKit.pholabel  = cms.VInputTag("photonUserData")
   process.bprimeKit.jetlabel = cms.VInputTag( "jetUserData" , "jetUserDataAK8" , "jetUserDataAK8" )
   process.QGTagger.srcJets = cms.InputTag( "jetUserData" )
   process.endPath = cms.Path(process.bprimeKit)
