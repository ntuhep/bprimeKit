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
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts
import copy

options = opts.VarParsing ('analysis')

options.register('maxEvts',
                 1000, ## For all events processing use -1
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.int,
                 'Number of events to process')

options.register('sample',
                 '/store/cmst3/user/gpetrucc/miniAOD/Spring15MiniAODv2/CMSSW_7_4_12/miniAOD-TTJets_madgraphMLM_25ns-40k_PAT.root',
                 #'/store/cmst3/user/gpetrucc/miniAOD/Spring15MiniAODv2/CMSSW_7_4_12/miniAOD-data-SingleMuon-251252_PAT_job5.root', 
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 'Sample to analyze')

options.register('lheLabel',
                 'externalLHEProducer',
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 'LHE module label')

options.register('outputLabel',
                 'B2GEDMNtuple.root',
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 'Output label')

options.register('globalTag',
                 '', ## Optional, could be decided automatically
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 'Global Tag')

options.register('isData',
                 False,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'Is data?')

options.register('DataProcessing',
                 "MC25ns",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 'Data processing type')

options.register('useNoHFMET',
                 True,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'Adding met without HF and relative jets')

options.register('usePrivateSQLite',
                 True,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'Take Corrections from private SQL file')

options.register('forceResiduals',
                 None,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'Whether to force residuals to be applied')

options.register('LHE',
                 False,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'Keep LHEProducts')

options.register('Debug',
                 0,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.int,
                 'Debugging output level' )

options.register('b2gPreprocess',
      False,
      opts.VarParsing.multiplicity.singleton,
      opts.VarParsing.varType.bool,
      'Where to use the filters and producers defined by b2g group')


options.parseArguments()

print """
#------------------------------------------------------------------------------- 
#   Setting up input labels
#------------------------------------------------------------------------------- """
rhoLabel               = "fixedGridRhoFastjetAll"
muLabel                = 'slimmedMuons'
elLabel                = 'slimmedElectrons'
jLabel                 = 'slimmedJets'
jLabelNoHF             = 'slimmedJets'
jLabelAK8              = 'slimmedJetsAK8'
pvLabel                = 'offlineSlimmedPrimaryVertices'
convLabel              = 'reducedEgamma:reducedConversions'
particleFlowLabel      = 'packedPFCandidates'
metLabel               = 'slimmedMETs'
metNoHFLabel           = 'slimmedMETsNoHF'
triggerResultsLabel    = "TriggerResults"
triggerSummaryLabel    = "hltTriggerSummaryAOD"
hltMuonFilterLabel     = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f40QL3crIsoRhoFiltered0p15"
hltPathLabel           = "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL"
hltElectronFilterLabel = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8"
lheLabel               = "externalLHEProducer"


print """
#------------------------------------------------------------------------------- 
#   Begin option parsing for process
#------------------------------------------------------------------------------- """
if(options.isData):options.LHE = False
runOnData        = options.isData           #data/MC switch
useHFCandidates  = not options.useNoHFMET   #create an additionnal NoHF slimmed MET collection if the option is set to false
usePrivateSQlite = options.usePrivateSQLite #use external JECs (sqlite file)
applyResiduals   = options.isData           #application of residual corrections. 
                                            #   Have to be set to True once the 13 TeV residual corrections are available. 
                                            #   False to be kept meanwhile. Can be kept to False later for private tests
                                            #   or for analysis checks and developments (not the official recommendation!).
if not (options.forceResiduals == None):
  applyResiduals = (options.forceResiduals == True)

#####################################
process = cms.Process("bprimeKit")
#####################################

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.categories.append('HLTrigReport')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvts) )
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

if options.globalTag == "": 
   if options.DataProcessing=="MC50ns":
      process.GlobalTag.globaltag="MCRUN2_74_V9A"
   elif options.DataProcessing=="MC25ns":
      process.GlobalTag.globaltag="MCRUN2_74_V9"
   elif options.DataProcessing=="Data50ns":
      process.GlobalTag.globaltag="74X_dataRun2_Prompt_v0"
   elif options.DataProcessing=="Data25ns":
      process.GlobalTag.globaltag="74X_dataRun2_Prompt_v1"
   else:
      print "Choose any of the following options for 'DataProcessing'", "MC50ns,  MC25ns, Data50ns, Data25ns" 
else: 
   print "You have chosen globalTag as", options.globalTag, ". Please check if this corresponds to your dataset."
   process.GlobalTag.globaltag = options.globalTag 


print """
#------------------------------------------------------------------------------- 
#   External Jet Energy Corrections.
#------------------------------------------------------------------------------- """
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

if runOnData:
  process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'

corrections = ['L1FastJet', 'L2Relative', 'L3Absolute']
if (applyResiduals == True):
  corrections = ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual']

if usePrivateSQlite:
    jLabel    = 'updatedPatJetsAK4'
    jLabelAK8 = 'updatedPatJetsAK8'
    
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os
    if runOnData:
      era="Summer15_50nsV4_DATA"
    else:
      era="Summer15_50nsV4_MC"
    dBFile = era+".db"
    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                               connect = cms.string( "sqlite_file:"+dBFile ),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
                label= cms.untracked.string("AK4PFchs")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK8PF"),
                label= cms.untracked.string("AK8PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK8PFchs"),
                label= cms.untracked.string("AK8PFchs")
                ),
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')
    
    process.load("PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff")
    from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated, patJetsUpdated
    process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
      rho = cms.InputTag("fixedGridRhoFastjetAll"),
      src = cms.InputTag("slimmedJets"),
      levels = corrections )
    process.updatedPatJetsAK4 = patJetsUpdated.clone(
      jetSource = cms.InputTag("slimmedJets"),
      jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
      )
    
    process.patJetAK8CorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
      src = cms.InputTag("slimmedJetsAK8"),
      rho = cms.InputTag("fixedGridRhoFastjetAll"),
      levels = corrections )
    process.updatedPatJetsAK8 = patJetsUpdated.clone(
      jetSource = cms.InputTag("slimmedJetsAK8"),
      jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetAK8CorrFactorsReapplyJEC"))
      )
jecUncertaintyFile="PhysicsTools/PatUtils/data/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt"

print """
#------------------------------------------------------------------------------- 
#   Since 7.4.0, only AK8 jets need to make gen jets, all others are stored 
#   in MiniAOD directly
#------------------------------------------------------------------------------- """
## Filter out neutrinos from packed GenParticles
process.packedGenParticlesForJetsNoNu = cms.EDFilter(
        "CandPtrSelector", 
        src = cms.InputTag("packedGenParticles"), 
        cut = cms.string("abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16")
        )

## Fat GenJets
if not options.isData : 
    from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
    process.ak8GenJetsNoNu = ak4GenJets.clone(
        rParam = cms.double(0.8),
        src = cms.InputTag("packedGenParticlesForJetsNoNu")
    )

    ## SoftDrop fat GenJets (two jet collections are produced, fat jets and subjets)
    from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
    from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
    process.ak8GenJetsNoNuSoftDrop = ak4GenJets.clone(
        rParam = cms.double(0.8),
        src = cms.InputTag("packedGenParticlesForJetsNoNu"),
        useSoftDrop = cms.bool(True),
        zcut = cms.double(0.1),
        beta = cms.double(0.0),
        R0   = cms.double(0.8),
        useExplicitGhosts = cms.bool(True),
        writeCompound = cms.bool(True),
        jetCollInstanceName=cms.string("SubJets")    
    )

print """
### ---------------------------------------------------------------------------
### Removing the HF from the MET computation as from 7 Aug 2015 recommendations
### ---------------------------------------------------------------------------"""
if not useHFCandidates:
   process.noHFCands = cms.EDFilter(
           "CandPtrSelector",
           src=cms.InputTag("packedPFCandidates"),
           cut=cms.string("abs(pdgId)!=1 && abs(pdgId)!=2 && abs(eta)<3.0")
           )

#jets are rebuilt from those candidates by the tools, no need to do anything else
### =================================================================================

from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

#default configuration for miniAOD reprocessing, change the isData flag to run on data
#for a full met computation, remove the pfCandColl input
runMetCorAndUncFromMiniAOD(
        process,
        isData=runOnData,
        )

if not useHFCandidates:
    runMetCorAndUncFromMiniAOD(
            process,
            isData=runOnData,
            pfCandColl=cms.InputTag("noHFCands"),
            postfix="NoHF"
            )
    jLabelNoHF = 'patJetsNoHF'

print """
### -------------------------------------------------------------------
### the lines below remove the L2L3 residual corrections when processing data
### -------------------------------------------------------------------"""
if (applyResiduals == True):
  #Take new pat jets as input of the entuples
  process.patJetCorrFactors.levels = corrections 
  if not useHFCandidates:
    process.patJetCorrFactorsNoHF.levels = corrections 

if not ( applyResiduals ==True):
    process.patPFMetT1T2Corr.jetCorrLabelRes          = cms.InputTag("L3Absolute")
    process.patPFMetT1T2SmearCorr.jetCorrLabelRes     = cms.InputTag("L3Absolute")
    process.patPFMetT2Corr.jetCorrLabelRes            = cms.InputTag("L3Absolute")
    process.patPFMetT2SmearCorr.jetCorrLabelRes       = cms.InputTag("L3Absolute")
    process.shiftedPatJetEnDown.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
    process.shiftedPatJetEnUp.jetCorrLabelUpToL3Res   = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")

    if not useHFCandidates:
          process.patPFMetT1T2CorrNoHF.jetCorrLabelRes          = cms.InputTag("L3Absolute")
          process.patPFMetT1T2SmearCorrNoHF.jetCorrLabelRes     = cms.InputTag("L3Absolute")
          process.patPFMetT2CorrNoHF.jetCorrLabelRes            = cms.InputTag("L3Absolute")
          process.patPFMetT2SmearCorrNoHF.jetCorrLabelRes       = cms.InputTag("L3Absolute")
          process.shiftedPatJetEnDownNoHF.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
          process.shiftedPatJetEnUpNoHF.jetCorrLabelUpToL3Res   = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")

print """
#------------------------------------------------------------------------------- 
#   B2GAnaFW EDFilter settings
#------------------------------------------------------------------------------- """
process.skimmedPatMuons = cms.EDFilter(
    "PATMuonSelector",
    src = cms.InputTag(muLabel),
    cut = cms.string("pt > 0.0 && abs(eta) < 2.4")
    )
process.skimmedPatPhotons = cms.EDFilter(
    "PATPhotonSelector",
    src = cms.InputTag("slimmedPhotons"),
    cut = cms.string("pt > 30 && abs(eta) < 2.4"),

)
process.skimmedPatElectrons = cms.EDFilter(
    "PATElectronSelector",
    src = cms.InputTag(elLabel),
    cut = cms.string("pt > 10 && abs(eta) < 2.5")
    )
process.skimmedPatMET = cms.EDFilter(
    "PATMETSelector",
    src = cms.InputTag(metLabel),
    cut = cms.string("")
    )
process.skimmedPatMETNoHF = cms.EDFilter(
    "PATMETSelector",
    src = cms.InputTag(metNoHFLabel),
    cut = cms.string("")
    )
process.skimmedPatJets = cms.EDFilter(
    "PATJetSelector",
    src = cms.InputTag(jLabel),
    cut = cms.string(" pt > 25 && abs(eta) < 5.")
    )
process.skimmedPatJetsAK8 = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag(jLabelAK8),
    cut = cms.string("pt > 100 && abs(eta) < 5.")    
    )

print """
#------------------------------------------------------------------------------- 
#   B2GAnaFW EDProducer settings
#------------------------------------------------------------------------------- """
process.eventUserData = cms.EDProducer(
      'EventUserData',
      pileup = cms.InputTag("slimmedAddPileupInfo"),
      pvSrc = cms.InputTag("offlineSlimmedPrimaryVertices")
      )

process.muonUserData = cms.EDProducer(
      'MuonUserData',
      muonLabel = cms.InputTag("skimmedPatMuons"),
      pv        = cms.InputTag(pvLabel),
      packedPFCands = cms.InputTag("packedPFCandidates"),
      ### TTRIGGER ###
      triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
      triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
      hltMuonFilter  = cms.InputTag(hltMuonFilterLabel),
      hltPath            = cms.string("HLT_IsoMu40_eta2p1_v11"),
      hlt2reco_deltaRmax = cms.double(0.1),
      # mainROOTFILEdir    = cms.string("../data/")
      )

process.jetUserData = cms.EDProducer(
      'JetUserData',
      jetLabel  = cms.InputTag(jLabel),
      ### TTRIGGER ###
      triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
      triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
      hltJetFilter       = cms.InputTag("hltSixCenJet20L1FastJet"),
      hltPath            = cms.string("HLT_QuadJet60_DiJet20_v6"),
      hlt2reco_deltaRmax = cms.double(0.2),
      )

process.jetUserDataNoHF = cms.EDProducer(
      'JetUserData',
      jetLabel  = cms.InputTag(jLabelNoHF),
      ### TTRIGGER ###
      triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
      triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
      hltJetFilter       = cms.InputTag("hltSixCenJet20L1FastJet"),
      hltPath            = cms.string("HLT_QuadJet60_DiJet20_v6"),
      hlt2reco_deltaRmax = cms.double(0.2),
      )

process.jetUserDataAK8 = cms.EDProducer(
      'JetUserData',
      jetLabel  = cms.InputTag(jLabelAK8),
      pv        = cms.InputTag(pvLabel),
      ### TTRIGGER ###
      triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
      triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
      hltJetFilter       = cms.InputTag("hltSixCenJet20L1FastJet"),
      hltPath            = cms.string("HLT_QuadJet60_DiJet20_v6"),
      hlt2reco_deltaRmax = cms.double(0.2)
      )

process.electronUserData = cms.EDProducer(
      'ElectronUserData',
      eleLabel            = cms.InputTag("skimmedPatElectrons"),
      pv                  = cms.InputTag(pvLabel),
      packedPFCands       = cms.InputTag("packedPFCandidates"),
      conversion          = cms.InputTag(convLabel),
      rho                 = cms.InputTag(rhoLabel),
      triggerResults      = cms.InputTag(triggerResultsLabel),
      triggerSummary      = cms.InputTag(triggerSummaryLabel),
      hltElectronFilter   = cms.InputTag(hltElectronFilterLabel),  ##trigger matching code to be fixed!
      hltPath             = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL"),
      electronVetoIdMap   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-veto"),
      electronLooseIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-loose"),
      electronMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-medium"),
      electronTightIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-tight"),
      electronHEEPIdMap   = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60")
      )

process.photonUserData = cms.EDProducer(
      'PhotonUserData',
      rho                     = cms.InputTag(rhoLabel),
      pholabel                = cms.InputTag("slimmedPhotons"),
      phoLooseIdMap           = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose"),
      phoMediumIdMap          = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium"),
      phoTightIdMap           = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight"),
      phoChgIsoMap            = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation"),
      phoPhoIsoMap            = cms.InputTag("photonIDValueMapProducer:phoPhotonIsolation"),
      phoNeuIsoMap            = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation"),
      effAreaChHadFile        = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt"),
      effAreaNeuHadFile       = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt"),
      effAreaPhoFile          = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt"),
      full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta")
      )

process.photonJets = cms.EDProducer(
      'PhotonJets',
      phoLabel = cms.InputTag("skimmedPatPhotons"),
      pv        = cms.InputTag(pvLabel),
      rho               = cms.InputTag(rhoLabel),
      packedPFCands = cms.InputTag("packedPFCandidates"),
      jetLabel  = cms.InputTag("slimmedJetsAK8"),
      ebReducedRecHitCollection = cms.InputTag("reducedEgamma:reducedEBRecHits"),
      eeReducedRecHitCollection = cms.InputTag("reducedEgamma:reducedEERecHits")
      )

process.vertexInfo = cms.EDProducer(
      'VertexInfo',
      src  = cms.InputTag(pvLabel),
      )

print """
#------------------------------------------------------------------------------- 
#   Settings for QGTagger
#------------------------------------------------------------------------------- """
### Including QGL: ensuring the database object can be accessed
qgDatabaseVersion = 'v1' # check https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion

from CondCore.DBCommon.CondDBSetup_cfi import *
QGPoolDBESSource = cms.ESSource("PoolDBESSource",
      CondDBSetup,
      toGet = cms.VPSet(),
      connect = cms.string('frontier://FrontierProd/CMS_COND_PAT_000'),
)

for type in ['AK4PFchs','AK4PFchs_antib']:
   QGPoolDBESSource.toGet.extend(cms.VPSet(cms.PSet(
      record = cms.string('QGLikelihoodRcd'),
      tag    = cms.string('QGLikelihoodObject_'+qgDatabaseVersion+'_'+type),
      label  = cms.untracked.string('QGL_'+type)
   )))

process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.srcJets  = cms.InputTag("jetUserData")  # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
process.QGTagger.jetsLabel = cms.string('QGL_AK4PFchs')  # Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
process.QGTaggerNoHF = copy.deepcopy(process.QGTagger)
process.QGTaggerNoHF.srcJets  = cms.InputTag("jetUserDataNoHF")


print """
#------------------------------------------------------------------------------- 
#   Settings for Egamma Identification
#------------------------------------------------------------------------------- """
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)

my_id_modules = [
   'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V2_cff',
   'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV51_cff']
for idmod in my_id_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_PHYS14_PU20bx25_V2_cff']
for idmod in my_phoid_modules:
   setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

from PhysicsTools.PatAlgos.tools.pfTools import *
## Adapt primary vertex collection
adaptPVs(process, pvCollection=cms.InputTag('offlineSlimmedPrimaryVertices'))

print """
#------------------------------------------------------------------------------- 
#   Trigger and MET Settings
#------------------------------------------------------------------------------- """
from PhysicsTools.CandAlgos.EventShapeVars_cff import *
process.eventShapePFVars = pfEventShapeVars.clone()
process.eventShapePFVars.src = cms.InputTag(particleFlowLabel)

process.eventShapePFJetVars = pfEventShapeVars.clone()
process.eventShapePFJetVars.src = cms.InputTag("skimmedPatJets")

process.TriggerUserData = cms.EDProducer(
      'TriggerUserData',
      bits = cms.InputTag("TriggerResults","","HLT"),
      prescales = cms.InputTag("patTrigger"),
      storePrescales = cms.untracked.bool(True), 
      hltProcName = cms.untracked.string("HLT"), 
      objects = cms.InputTag("selectedPatTrigger")
      )                                 

hltProcForMETUserData = "PAT"

process.METUserData = cms.EDProducer(
      'TriggerUserData',
      bits = cms.InputTag("TriggerResults","",hltProcForMETUserData),
      prescales = cms.InputTag("patTrigger"),
      storePrescales = cms.untracked.bool(False), 
      hltProcName = cms.untracked.string(hltProcForMETUserData), 
      objects = cms.InputTag("selectedPatTrigger")
      )

process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)

print """
#------------------------------------------------------------------------------- 
#   bprimeKit configuration importing 
#------------------------------------------------------------------------------- """
resultsFile = 'bprimeKit_ntuples.root'
process.TFileService = cms.Service("TFileService",
    fileName = cms.string( resultsFile )
)

from MyAna.bprimeKit.ObjectParameters_cfi import *
process.bprimeKit = cms.EDAnalyzer(
   "bprimeKit",
   #----- Operation paramters --------------------------------------------------------------------------
   MCtag               = cms.untracked.bool( not options.isData ),
   PairCollection      = cms.untracked.int32(1),
   IncludeL7           = cms.untracked.bool(False),
   SelectionParameters = defaultObjectParameters.clone(),
   Debug               = cms.untracked.int32( options.Debug ),
   runOnB2G            = cms.untracked.bool( options.b2gPreprocess ),
  
   #----- Event level objects --------------------------------------------------------------------------
   rhoLabel            = cms.InputTag( rhoLabel ),
   metlabel            = cms.VInputTag("slimmedMETs" ),
   hltlabel            = cms.VInputTag("TriggerResults::HLT"),
   offlinePVlabel      = cms.VInputTag("offlineSlimmedPrimaryVertices"),#CMSSW73X "offlinePrimaryVertices"),
   offlinePVBSlabel    = cms.VInputTag("offlinePrimaryVerticesWithBS"),# CMSSW73X"offlinePrimaryVerticesWithBS"),
   offlineBSlabel      = cms.VInputTag("offlineBeamSpot"),
   pfCands             = cms.InputTag("packedPFCandidates"),
   puInfoLabel         = cms.VInputTag("addPileupInfo"),
   conversionsInputTag = cms.InputTag("reducedEgamma","reducedConversions"),

   #----- Photon information ------------------------------------------------------------------------ 
   PhoCollections            = cms.vstring('PhotonInfo'),
   pholabel                  = cms.VInputTag('slimmedPhotons'),
   phoLooseIdMap             = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose"     ) ,
   phoMediumIdMap            = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium"    ) ,
   phoTightIdMap             = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight"     ) ,
   phoChargedIsolation       = cms.InputTag( "photonIDValueMapProducer:phoChargedIsolation"                          ) ,
   phoNeutralHadronIsolation = cms.InputTag( "photonIDValueMapProducer:phoNeutralHadronIsolation"                    ) ,
   phoPhotonIsolation        = cms.InputTag( "photonIDValueMapProducer:phoPhotonIsolation"                           ) ,
   full5x5SigmaIEtaIEtaMap   = cms.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta"),
   effAreaChHadFile          = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt"),
   effAreaNeuHadFile         = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt"),
   effAreaPhoFile            = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt"),
   
   #----- Lepton related information -------------------------------------------------------------------
   LepCollections  = cms.vstring( 'LepInfo'),
   muonlabel       = cms.VInputTag('slimmedMuons'     ) ,
   eleclabel       = cms.VInputTag('slimmedElectrons' ) ,
   taulabel        = cms.VInputTag('slimmedTaus'      ) ,
   eleVetoIdMap    = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-veto"   ) ,
   eleLooseIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose"  ) ,
   eleMediumIdMap  = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium" ) ,
   eleTightIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight"  ) ,
   eleHEEPIdMap    = cms.InputTag( "egmGsfElectronIDs:heepElectronID-HEEPV51"                                  ) ,

   #----- Jet Information ------------------------------------------------------------------------------
   jetlabel       = cms.VInputTag( 'slimmedJets' , 'slimmedJetsAK8' , 'slimmedJetsAK8') ,
   JetCollections = cms.vstring  ( 'JetInfo'     , 'AK8BosonJetInfo', 'CA8TopJetInfo' ) ,

   #----- MC Generation information --------------------------------------------------------------------
   genlabel    = cms.VInputTag("prunedGenParticles"),
   genevtlabel = cms.VInputTag("generator"),
   gtdigilabel = cms.VInputTag("gtDigis"),

   EIDMVAInputTags = cms.vstring(
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat1.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat2.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat3.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat4.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat5.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat6.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat1.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat2.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat3.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat4.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat5.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat6.weights.xml'
   ),
)

if not options.b2gPreprocess:
   print "Running with original pat tuples"
   process.QGTagger.srcJets = cms.InputTag( "slimmedJets")
   process.endPath = cms.Path(
         process.QGTagger * 
         process.egmGsfElectronIDSequence * 
         process.egmPhotonIDSequence * 
         process.bprimeKit
         )
else:
   print "Running with b2g defined filters"
   process.bprimeKit.muonlabel = cms.VInputTag("muonUserData")
   process.bprimeKit.eleclabel = cms.VInputTag("electronUserData")
   process.bprimeKit.pholabel  = cms.VInputTag("photonUserData")
   process.bprimeKit.jetlabel = cms.VInputTag( "jetUserData" , "jetUserDataAK8" , "jetUserDataAK8" )
   process.QGTagger.srcJets = cms.InputTag( "jetUserData" ) 
   process.endPath = cms.Path(
         #----- Lepton User data -----------------------------------------------------------------------------
         process.skimmedPatMuons *
         process.muonUserData  *
         process.skimmedPatElectrons *
         process.electronUserData *
         #----- Photon User data -----------------------------------------------------------------------------
         process.skimmedPatPhotons *
         process.photonUserData *
         #----- jet sub routines -----------------------------------------------------------------------------
         process.patJetCorrFactorsReapplyJEC *
         process.patJetAK8CorrFactorsReapplyJEC * 
         process.updatedPatJetsAK4 *
         process.updatedPatJetsAK8 * 
         process.skimmedPatJets *
         process.skimmedPatJetsAK8 * 
         process.jetUserData *
         process.jetUserDataAK8 *
         #----- Originam routines ----------------------------------------------------------------------------
         process.QGTagger *
         process.egmGsfElectronIDSequence * 
         process.egmPhotonIDSequence *
         process.bprimeKit
   )

