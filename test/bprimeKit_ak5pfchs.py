import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

process.GlobalTag.globaltag = cms.string('START42_V12::All')

#load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

#process.out = cms.OutputModule("PoolOutputModule",
#      fileName = cms.untracked.string('test.root'),
#      SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
#      outputCommands = cms.untracked.vstring('drop *') 
#      )

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
      applyfilter = cms.untracked.bool(True),
      debugOn = cms.untracked.bool(False),
      numtrack = cms.untracked.uint32(10),
      thresh = cms.untracked.double(0.2)
      )

# HB + HE noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')


# rho
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)
process.otherStuff = cms.Sequence( process.kt6PFJets )


pvSrc = 'offlinePrimaryVertices'
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
      vertexCollection = cms.InputTag(pvSrc),
      minimumNDOF = cms.uint32(7) ,
      maxAbsZ = cms.double(24), 
      maxd0 = cms.double(2) 
      )

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( maxZ = cms.double(24.0),
                                     minNdof = cms.double(7.0)
                                     ),
    src=cms.InputTag(pvSrc)
    )

# Default PF2PAT with AK5 jets. Make sure to turn ON the L1fastjet stuff. 
from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix)
process.pfPileUpPFlow.Enable = True
process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
process.pfJetsPFlow.doAreaFastjet = True
process.pfJetsPFlow.doRhoFastjet = False
process.patJetCorrFactorsPFlow.payload = 'AK5PFchs'
process.patJetCorrFactorsPFlow.levels = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L7Parton']
process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
process.pfPileUpPFlow.checkClosestZVertex = False

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsPFlow = kt4PFJets.clone(
    rParam = cms.double(0.6),
    src = cms.InputTag('pfNoElectron'+postfix),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True),
    voronoiRfact = cms.double(0.9),
    )

getattr(process,"patPF2PATSequence"+postfix).replace( getattr(process,"pfNoElectron"+postfix), getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow )

usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix="PF")
process.patJetCorrFactorsPF.payload = 'AK5PFchs'
process.patJetCorrFactorsPF.levels = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L7Parton']
process.patJetCorrFactorsPF.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
getattr(process,"pfNoPileUpPF").enable = False
getattr(process,"pfNoMuonPF").enable = False
getattr(process,"pfNoElectronPF").enable = False
getattr(process,"pfNoTauPF").enable = False

## New Electron ID
process.load("RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi")
from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import *
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesFixedIsolationElectronIdentificationV06_cfi") ## Cut In Category
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi") ## Cut In Category (MC tuned)
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")	## simpleEID
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence +
		process.eidVeryLoose + 
		process.eidLoose + 
		process.eidMedium + 
		process.eidTight + 
		process.eidSuperTight + 
		process.eidHyperTight1 +
		process.eidHyperTight2 +
		process.eidHyperTight3 +
		process.eidHyperTight4 +
		process.eidVeryLooseMC + 
		process.eidLooseMC + 
		process.eidMediumMC + 
		process.eidTightMC + 
		process.eidSuperTightMC + 
		process.eidHyperTight1MC +
		process.eidHyperTight2MC +
		process.eidHyperTight3MC +
		process.eidHyperTight4MC 
)
#process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.electronMatch*process.patElectrons)
process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
		simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
		simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
		simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
		simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
		simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
		simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
		simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
		simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
		simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
		simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
		simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
		simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),

		## For CIC
		eidVeryLoose = cms.InputTag("eidVeryLoose"),
		eidLoose = cms.InputTag("eidLoose"),
		eidMedium = cms.InputTag("eidMedium"),
		eidTight = cms.InputTag("eidTight"),
		eidSuperTight = cms.InputTag("eidSuperTight"),
		eidHyperTight1 = cms.InputTag("eidHyperTight1"),
		eidHyperTight2 = cms.InputTag("eidHyperTight2"),
		eidHyperTight3 = cms.InputTag("eidHyperTight3"),
		eidHyperTight4 = cms.InputTag("eidHyperTight4"),
		# For CIC with ISO
		eidVeryLooseMC = cms.InputTag("eidVeryLooseMC"),
		eidLooseMC = cms.InputTag("eidLooseMC"),
		eidMediumMC = cms.InputTag("eidMediumMC"),
		eidTightMC = cms.InputTag("eidTightMC"),
		eidSuperTightMC = cms.InputTag("eidSuperTightMC"),
		eidHyperTight1MC = cms.InputTag("eidHyperTight1MC"),
		eidHyperTight2MC = cms.InputTag("eidHyperTight2MC"),
		eidHyperTight3MC = cms.InputTag("eidHyperTight3MC"),
		eidHyperTight4MC = cms.InputTag("eidHyperTight4MC")


)

#Turn off charge match in PAT
process.electronMatch.checkCharge = cms.bool(False)
process.muonMatch.checkCharge = cms.bool(False)

# source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
#         'file:/disk2/yjlei/cmsdata/testsample/RECO/424RECO.root'
         'file:/disk2/yjlei/cmsdata/testsample/RECO/42xMCttbar/50AC4DBF-746D-E011-8CF9-00248C55CC62.root'
         #'rfio:/castor/cern.ch/cms/store/mc/Fall10/MinBias_TuneZ2_7TeV-pythia6/DQM/START38_V12-v1/0004/F400EBC3-C5C8-DF11-BB9D-001A92971B36.root'
         ),
      noEventSort = cms.untracked.bool(True),
      duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
      )
process.options = cms.untracked.PSet(
      SkipEvent = cms.untracked.vstring('ProductNotFound')
)

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#bptimeKit process
process.bprimeKit = cms.EDAnalyzer("bprimeKit",
      MCtag = cms.untracked.bool(True),
      muonlabel = cms.InputTag("cleanPatMuons"),
      eleclabel = cms.InputTag("cleanPatElectrons"),
      pfjetlabel= cms.InputTag("selectedPatJetsPF"),
      metlabel  = cms.InputTag("patMETs"),
      pfmetlabel  = cms.InputTag("patMETsPF"),
      genlabel  = cms.InputTag("genParticles"),
      hltlabel  = cms.InputTag("TriggerResults::HLT"),
      pathltlabel = cms.InputTag("patTriggerEvent"),
      offlinePVlabel = cms.InputTag("offlinePrimaryVertices"),
      offlinePVBSlabel = cms.InputTag("offlinePrimaryVerticesWithBS"),
      offlineBSlabel = cms.InputTag("offlineBeamSpot"),
      pixelvtxlabel = cms.InputTag("pixelVertices"),
      tracklabel = cms.InputTag("generalTracks"),
      dcslabel = cms.InputTag("scalersRawToDigi"),
      genevtlabel = cms.InputTag("generator"),
      gtdigilabel = cms.InputTag("gtDigis"),
      rhocorrectionlabel = cms.InputTag("kt6PFJets","rho"),
      sigmaLabel = cms.InputTag("kt6PFJets","sigma"),
      puInfoLabel = cms.InputTag("addPileupInfo")
      )

process.TFileService = cms.Service("TFileService",
      fileName = cms.string('results.root')
      )

process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff" )
process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi")

process.TriggerMatchSingleMu = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatMuons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    'path( "HLT_Mu9") || path( "HLT_Mu15_v1") '
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)
		
process.TriggerMatchDoubleMu = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatMuons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    ' path( "HLT_DoubleMu3" ) || path( "HLT_DoubleMu3_v2" ) '
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.TriggerMatchSingleEl = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatElectrons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    ' path( "HLT_Ele15_LW_L1R") '
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.TriggerMatchDoubleEl = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatElectrons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    ' path( "HLT_DoubleEle5_SW_L1R" ) || path( "HLT_DoubleEle10_SW_L1R" ) || path( "HLT_DoubleEle15_SW_L1R_v1" ) || path( "HLT_DoubleEle17_SW_L1R_v1" ) '
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.patTriggerEvent.patTriggerMatches = cms.VInputTag( "TriggerMatchSingleMu","TriggerMatchDoubleMu","TriggerMatchSingleEl","TriggerMatchDoubleEl" )

#process.patTrigger.processName = 'REDIGI311X'
#process.patTriggerEvent.processName = 'REDIGI311X'

process.bpkitTriggerMatcher = cms.Sequence(
          process.TriggerMatchSingleMu + process.TriggerMatchDoubleMu + process.TriggerMatchSingleEl + process.TriggerMatchDoubleEl
              )

process.bpkitTriggerSequence = cms.Sequence(
          process.patTrigger * process.bpkitTriggerMatcher * process.patTriggerEvent
        )

process.p = cms.Path(
#        process.offlinePrimaryVertices
#       process.offlinePrimaryVerticesDAF
       process.goodOfflinePrimaryVertices
      * process.kt6PFJets
      * process.patElectronIDs
      * process.patDefaultSequence
      * process.patPF2PATSequencePFlow
      * process.patPF2PATSequencePF
      * process.bpkitTriggerSequence
      * process.bprimeKit
      )


