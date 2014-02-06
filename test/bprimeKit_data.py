import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.GlobalTag.globaltag = cms.string('GR_R_311_V2::All')

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.out = cms.OutputModule("PoolOutputModule",
      fileName = cms.untracked.string('test.root'),
      SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
      outputCommands = cms.untracked.vstring('drop *') 
      )

# load the coreTools of PAT
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process, 
      cms.InputTag('ak5PFJets'),   
      doJTA            = True,            
      doBTagging       = True,            
      jetCorrLabel     = ('AK5PF', ['L2Relative', 'L3Absolute', 'L2L3Residual']),  
      doType1MET       = False,            
#      genJetCollection = cms.InputTag("ak5GenJets"),
      doJetID      = True,
      jetIdLabel   = "ak5"
      ) 

##Electron ID
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.electronMatch*process.patElectrons
)
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
)

from PhysicsTools.PatAlgos.tools.coreTools import *
runOnData(process, ['All'], outputInProcess = False)

from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

# source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
         'file:/nasdata2/yjli/Run2011A_SingleElectron_AOD.root'
         ),
      )
process.options = cms.untracked.PSet(
      SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

#bptimeKit process
process.bprimeKit = cms.EDAnalyzer("bprimeKit",
      MCtag = cms.untracked.bool(False),
      muonlabel = cms.InputTag("cleanPatMuons"),
      eleclabel = cms.InputTag("cleanPatElectrons"),
      pfjetlabel= cms.InputTag("cleanPatJets"),
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
      gtdigilabel = cms.InputTag("gtDigis")

      )

process.TFileService = cms.Service("TFileService",
      fileName = cms.string('results.root')
      )

process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi" )
process.load( "PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi" )

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


#process.patTriggerMatcher += process.TriggerMatchSingleMu
#process.patTriggerMatcher += process.TriggerMatchDoubleMu
#process.patTriggerMatcher += process.TriggerMatchSingleEl
#process.patTriggerMatcher += process.TriggerMatchDoubleEl
process.patTriggerEvent.patTriggerMatches = cms.VInputTag( "TriggerMatchSingleMu","TriggerMatchDoubleMu","TriggerMatchSingleEl","TriggerMatchDoubleEl" )

#process.patTrigger.processName = 'REDIGI311X'
#process.patTriggerEvent.processName = 'REDIGI311X'

process.bpkitTriggerMatcher = cms.Sequence(
          process.TriggerMatchSingleMu + process.TriggerMatchDoubleMu + process.TriggerMatchSingleEl + process.TriggerMatchDoubleEl
              )

process.bpkitTriggerSequence = cms.Sequence(
          process.patTrigger * process.bpkitTriggerMatcher * process.patTriggerEvent
        )

#from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
#switchOnTrigger( process , hltProcess = '*' )

process.p = cms.Path(
      process.patDefaultSequence
      * process.bpkitTriggerSequence
      * process.bprimeKit
      )


