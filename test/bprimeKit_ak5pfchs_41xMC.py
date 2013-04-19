import FWCore.ParameterSet.Config as cms

process = cms.Process("BPKIT")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.PyReleaseValidation.autoCond import autoCond
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
                               )

#process.outpath = cms.EndPath(process.out)


from PhysicsTools.PatAlgos.tools.coreTools import *

## Source
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
      'file:/nasdata2/yjli/test_aod_1_1_bLn.root'
   )
)
inputJetCorrLabel = ('AK5PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L7Parton'])

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.GlobalTag.globaltag = cms.string('START41_V0::All')

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )
# HB + HE noise filtering
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
# Modify defaults setting to avoid an over-efficiency in the presence of OFT PU
process.HBHENoiseFilter.minIsolatedNoiseSumE = cms.double(999999.)
process.HBHENoiseFilter.minNumIsolatedNoiseChannels = cms.int32(999999)
process.HBHENoiseFilter.minIsolatedNoiseSumEt = cms.double(999999.)


pvSrc = 'offlinePrimaryVertices'
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag(pvSrc),
                                           minimumNDOF = cms.uint32(3) , # here is >3
                                           maxAbsZ = cms.double(24), 
                                           maxd0 = cms.double(2) 
                                           )


from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector

process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( maxZ = cms.double(24.0),
                                     minNdof = cms.double(4.0) # here is  >=4
                                     ),
    src=cms.InputTag(pvSrc)
    )


###############################
#### Jet RECO includes ########
###############################

from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.CaloJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.CATopJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *

###############################
########## PF Setup ###########
###############################

# Default PF2PAT with AK5 jets. Make sure to turn ON the L1fastjet stuff. 
from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix)
process.pfPileUpPFlow.Enable = True
process.pfPileUpPFlow.Vertices = 'goodOfflinePrimaryVertices'
process.pfJetsPFlow.doAreaFastjet = True
process.pfJetsPFlow.doRhoFastjet = False
process.patJetCorrFactorsPFlow.payload = inputJetCorrLabel[0]
process.patJetCorrFactorsPFlow.levels = inputJetCorrLabel[1]
process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
#process.pfPileUpPFlow.checkClosestZVertex = False #Not for 41x

#muons
applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
#electrons
applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)

# In order to have a coherent semileptonic channel also, add
# some "loose" leptons to do QCD estimates.
process.pfIsolatedMuonsLoosePFlow = process.pfIsolatedMuonsPFlow.clone(
    combinedIsolationCut = cms.double(999.0) 
    )

process.patMuonsLoosePFlow = process.patMuonsPFlow.clone(
   pfMuonSource = cms.InputTag("pfIsolatedMuonsLoosePFlow"),
   genParticleMatch = cms.InputTag("muonMatchLoosePFlow")
   )

tmp = process.muonMatchPFlow.src
adaptPFMuons( process, process.patMuonsLoosePFlow, "PFlow")
process.muonMatchPFlow.src = tmp

process.muonMatchLoosePFlow = process.muonMatchPFlow.clone(
    src = cms.InputTag("pfIsolatedMuonsLoosePFlow")
    )
process.muonMatchPFlow.src = "pfIsolatedMuonsPFlow"

process.selectedPatMuonsLoosePFlow = process.selectedPatMuonsPFlow.clone(
    src = cms.InputTag("patMuonsLoosePFlow")
    )

process.pfIsolatedElectronsLoosePFlow = process.pfIsolatedElectronsPFlow.clone(
    combinedIsolationCut = cms.double(999.0) 
    )

process.patElectronsLoosePFlow = process.patElectronsPFlow.clone(
    pfElectronSource = cms.InputTag("pfIsolatedElectronsLoosePFlow")
    )
adaptPFElectrons( process, process.patElectronsLoosePFlow, "PFlow")

process.selectedPatElectronsLoosePFlow = process.selectedPatElectronsPFlow.clone(
    src = cms.InputTag("patElectronsLoosePFlow")
    )


process.looseLeptonSequence = cms.Sequence(
    process.pfIsolatedMuonsLoosePFlow +
    process.muonMatchLoosePFlow +
    process.patMuonsLoosePFlow +
    process.selectedPatMuonsLoosePFlow +    
    process.pfIsolatedElectronsLoosePFlow +
    process.patElectronsLoosePFlow +
    process.selectedPatElectronsLoosePFlow
    )

usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix="PF")
process.pfPileUpPF.Enable = True
process.pfPileUpPF.Vertices = 'goodOfflinePrimaryVertices'
process.pfJetsPF.doAreaFastjet = True
process.pfJetsPF.doRhoFastjet = False
process.patJetCorrFactorsPF.payload = inputJetCorrLabel[0]
process.patJetCorrFactorsPF.levels = inputJetCorrLabel[1]
process.patJetCorrFactorsPF.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
#getattr(process,"pfNoPileUpPF").enable = False
getattr(process,"pfNoMuonPF").enable = False
getattr(process,"pfNoElectronPF").enable = False
getattr(process,"pfNoTauPF").enable = False

#removeMCMatching(process, ['All'])


###############################
###### Bare KT 0.6 jets #######
###############################

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsPFlow = kt4PFJets.clone(
    rParam = cms.double(0.6),
    src = cms.InputTag('pfNoElectron'+postfix),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
    )

getattr(process,"patPF2PATSequence"+postfix).replace( getattr(process,"pfNoElectron"+postfix), getattr(process,"pfNoElectron"+postfix)*getattr(process,"kt6PFJets" + postfix) )


## New Electron ID
process.load("RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi")
from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import *
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesFixedIsolationElectronIdentificationV06_cfi") ## Cut In Category
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi") ## Cut In Category (MC tuned)
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")        ## simpleEID
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
#process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.electronMatch*process.patElectrons
#)
for iele in [ process.patElectrons,
      process.patElectronsPFlow ] :
   iele.addElectronID = cms.bool(True)
   iele.electronIDSources = cms.PSet(
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


#process.patJetCorrFactors.payload = 'AK5PF'
#process.patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute']
#process.patJetCorrFactors.useRho = False
#process.patJetCorrFactors.rho = cms.InputTag('')

######################
###Trigger Matching###
######################
process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi" )
process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi" )

process.TriggerMatchSingleMu = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatMuons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    'path( "HLT_Mu8_v*") || path( "HLT_Mu17_v1") '
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
    ' path( "HLT_DoubleMu7_v*" ) ||  path( "HLT_DoubleMu5_v1" ) ||  path( "HLT_Mu13_Mu8_v*") '
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
    ' path( "HLT_Ele10_SW_L1R_v2") || path( "HLT_Ele8_*" ) '
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
    ' path( "HLT_DoubleEle17_SW_L1R_v1" ) || path("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v*")'
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.TriggerMatchEGMuForEl = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatElectrons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    ' path( "HLT_Mu8_Ele17_CaloIdL_v*") || path( "HLT_Mu8_Ele8_v1")'
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.TriggerMatchEGMuForMu = cms.EDProducer(
  "PATTriggerMatcherDRDPtLessByR"                 # match by DeltaR only, best match by DeltaR
, src     = cms.InputTag( "cleanPatMuons" )
, matched = cms.InputTag( "patTrigger" )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
, matchedCuts                = cms.string(
    ' path( "HLT_Mu8_Ele17_CaloIdL_v*" ) || path( "HLT_Mu8_Ele8_v1")'
  )
, maxDPtRel = cms.double( 0.5 )
, maxDeltaR = cms.double( 0.5 )
, resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
, resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

process.patTriggerEvent.patTriggerMatches = cms.VInputTag( "TriggerMatchSingleMu","TriggerMatchDoubleMu","TriggerMatchSingleEl","TriggerMatchDoubleEl","TriggerMatchEGMuForEl","TriggerMatchEGMuForMu" )

#process.patTrigger.processName = 'REDIGI311X'
#process.patTriggerEvent.processName = 'REDIGI311X'

process.bpkitTriggerMatcher = cms.Sequence(
          process.TriggerMatchSingleMu 
          + process.TriggerMatchDoubleMu 
          + process.TriggerMatchSingleEl 
          + process.TriggerMatchDoubleEl
          + process.TriggerMatchEGMuForEl
          + process.TriggerMatchEGMuForMu
              )

process.bpkitTriggerSequence = cms.Sequence(
          process.patTrigger 
          * process.bpkitTriggerMatcher 
          * process.patTriggerEvent
        )

process.patTrigger.processName = 'REDIGI311X'
process.patTriggerEvent.processName = 'REDIGI311X'

from MyAna.bprimeKit.HitFitParameters_cfi import *
from MyAna.bprimeKit.SelectionParameters_cfi import *

##bprimeKit process
process.bprimeKit = cms.EDAnalyzer("bprimeKit",
      MCtag = cms.untracked.bool(False),
      muonlabel = cms.InputTag("cleanPatMuons"),
      eleclabel = cms.InputTag("cleanPatElectrons"),
      pfmuonlabel = cms.InputTag("selectedPatMuonsLoosePFlow"),
      pfeleclabel = cms.InputTag("selectedPatElectronsLoosePFlow"),
      pftaulabel = cms.InputTag("selectedPatTausPFlow"),
      jetlabel= cms.InputTag("selectedPatJets"),
      pfjetlabel= cms.InputTag("selectedPatJetsPFlow"),
      metlabel  = cms.InputTag("patMETs"),
      pfmetlabel  = cms.InputTag("patMETsPFlow"),
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
      rhocorrectionlabel = cms.InputTag("kt6PFJetsPFlow","rho"),
      sigmaLabel = cms.InputTag("kt6PFJetsPFlow","sigma"),
      puInfoLabel = cms.InputTag("addPileupInfo"),
      doHitFit = cms.untracked.bool(False),
      HitFitParameters = defaultHitFitParameters.clone(),
      SelectionParameters = defaultSelectionParameters.clone()
      )


##Output file
process.TFileService = cms.Service("TFileService",
      fileName = cms.string('results.root')
      )

process.p = cms.Path(
      process.scrapingVeto *
      process.HBHENoiseFilter *
      process.goodOfflinePrimaryVertices *
      process.patElectronIDs *
      process.primaryVertexFilter *
      process.patDefaultSequence *
      getattr(process,"patPF2PATSequencePFlow") *
      process.looseLeptonSequence *
      getattr(process,"patPF2PATSequencePF") *
      process.bpkitTriggerSequence *
      process.bprimeKit
      )
