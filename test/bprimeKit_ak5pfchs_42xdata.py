import FWCore.ParameterSet.Config as cms

process = cms.Process("BPKIT")

#User Options
runOnMC = False
resultsFile = 'results.root'

maxInputEvents = -1

pfD0Cut = 0.2
pfDzCut = 0.5
pfMuonSelectionCut = 'pt > 5.'
pfMuonIsoConeR = 0.3
pfMuonCombIsoCut = 0.2
pfElectronSelectionCut = 'pt > 5.'
pfElectronSelectionCut += ' && gsfTrackRef.isNonnull'
pfElectronSelectionCut += ' && gsfTrackRef.trackerExpectedHitsInner.numberOfLostHits < 2'
pfElectronIsoConeR = 0.3
pfElectronCombIsoCut = 0.2

useTrigger = False

usePFnoPU = True
useNoMuon = True
useNoElectron = True
useNoJet = True
useNoTau = False
jetAlgo = 'AK5'

### JEC levels

# levels to be accessible from the jets
# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
# and remain uncorrected, if none of these levels is enabled here
useL1FastJet    = True  # needs useL1Offset being off, error otherwise
useL1Offset     = False # needs useL1FastJet being off, error otherwise
useL2Relative   = True
useL3Absolute   = True
useL2L3Residual = True  # takes effect only on data
useL5Flavor     = False
useL7Parton     = True
if useL1FastJet and useL1Offset:
  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )

fwkReportEvery = 100
wantSummary = False

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = fwkReportEvery

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(wantSummary) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
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
      #'file:/home/grundler/samples/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola_AODSIM_PU_S4_START42_V11-v1/AODSIM/PU_24_START42_V11-v1/0000/FEEE3638-F297-E011-AAF8-00304867BEC0.root'
      #'file:/home/grundler/samples/Summer11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0001/FED96BE1-859A-E011-836E-001A92971B56.root'
      'file:/home/grundler/samples/Run2011A/SingleMu/AOD/PromptReco-v6/000/172/620/92127CFF-41C0-E011-ABC2-BCAEC532971F.root'
   )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(maxInputEvents) )

if runOnMC:
  process.GlobalTag.globaltag = cms.string('START42_V13::All')
else:
  process.GlobalTag.globaltag = cms.string('GR_R_42_V19::All')

#Trigger selection
l1Tag  = cms.InputTag( '' ) # skip L1 results, since conflicts with the GlobalTag can occur
hltTag = cms.InputTag( 'TriggerResults::HLT' )
from HLTrigger.HLTfilters.triggerResultsFilter_cfi import *
process.triggerResults = triggerResultsFilter.clone( hltResults = hltTag
                                                     , l1tResults = l1Tag
                                                     , throw      = False
                                                     , triggerConditions = [ 'HLT_IsoMu17_v*' ]
                                           )

# Good vertex selection
pvSrc = 'offlinePrimaryVertices'
pfVertices = 'goodOfflinePrimaryVertices'
pvSelection = cms.PSet(
  minNdof = cms.double( 4. )
, maxZ    = cms.double( 24. )
, maxRho  = cms.double( 2. )
)

process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelection,
    src=cms.InputTag(pvSrc),
    filter = cms.bool(False)
    )


## # Event cleaning
## process.scrapingVeto = cms.EDFilter("FilterOutScraping",
##                                     applyfilter = cms.untracked.bool(True),
##                                     debugOn = cms.untracked.bool(False),
##                                     numtrack = cms.untracked.uint32(10),
##                                     thresh = cms.untracked.double(0.2)
##                                     )
## # HB + HE noise filtering
## process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
## # Modify defaults setting to avoid an over-efficiency in the presence of OFT PU
## process.HBHENoiseFilter.minIsolatedNoiseSumE = cms.double(999999.)
## process.HBHENoiseFilter.minNumIsolatedNoiseChannels = cms.int32(999999)
## process.HBHENoiseFilter.minIsolatedNoiseSumEt = cms.double(999999.)

process.load( "MyAna.bprimeKit.eventCleaning_cfi" )
#HBHENoiseFilter + scrapingFilter will now be included under process.eventCleaning

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
pfSuffix = 'PF'
jecSetPF = jetAlgo + pfSuffix
if usePFnoPU:
    jecSetPF += 'chs'

jecLevels = []
if useL1FastJet:
  jecLevels.append( 'L1FastJet' )
if useL1Offset:
  jecLevels.append( 'L1Offset' )
if useL2Relative:
  jecLevels.append( 'L2Relative' )
if useL3Absolute:
  jecLevels.append( 'L3Absolute' )
if useL2L3Residual and not runOnMC:
  jecLevels.append( 'L2L3Residual' )
if useL5Flavor:
  jecLevels.append( 'L5Flavor' )
if useL7Parton:
  jecLevels.append( 'L7Parton' )

# Default PF2PAT with AK5 jets. Make sure to turn ON the L1fastjet stuff. 
from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process
          , runPF2PAT=True
          , jetAlgo='AK5'
          , runOnMC=runOnMC
          , postfix=postfix
          , jetCorrections = (jecSetPF,jecLevels) )
applyPostfix( process, 'pfNoPileUp'   , postfix ).enable = usePFnoPU
applyPostfix( process, 'pfNoMuon'     , postfix ).enable = useNoMuon
applyPostfix( process, 'pfNoElectron' , postfix ).enable = useNoElectron
applyPostfix( process, 'pfNoJet'      , postfix ).enable = useNoJet
applyPostfix( process, 'pfNoTau'      , postfix ).enable = useNoTau
applyPostfix( process, 'pfPileUp', postfix ).Vertices = cms.InputTag( pfVertices )
applyPostfix( process, 'pfPileUp', postfix ).checkClosestZVertex = False
applyPostfix( process, 'pfJets', postfix ).doAreaFastjet = True
applyPostfix( process, 'pfJets', postfix ).doRhoFastjet  = False
applyPostfix( process, 'pfMuonsFromVertex'    , postfix ).vertices = cms.InputTag( pfVertices )
applyPostfix( process, 'pfMuonsFromVertex'    , postfix ).d0Cut    = pfD0Cut
applyPostfix( process, 'pfMuonsFromVertex'    , postfix ).dzCut    = pfDzCut
applyPostfix( process, 'pfSelectedMuons'      , postfix ).cut = pfMuonSelectionCut
applyPostfix( process, 'isoValMuonWithCharged', postfix ).deposits[0].deltaR = pfMuonIsoConeR
applyPostfix( process, 'isoValMuonWithNeutral', postfix ).deposits[0].deltaR = pfMuonIsoConeR
applyPostfix( process, 'isoValMuonWithPhotons', postfix ).deposits[0].deltaR = pfMuonIsoConeR
applyPostfix( process, 'pfIsolatedMuons'      , postfix ).combinedIsolationCut = pfMuonCombIsoCut
applyPostfix( process, 'pfElectronsFromVertex'    , postfix ).vertices = cms.InputTag( pfVertices )
applyPostfix( process, 'pfElectronsFromVertex'    , postfix ).d0Cut    = pfD0Cut
applyPostfix( process, 'pfElectronsFromVertex'    , postfix ).dzCut    = pfDzCut
applyPostfix( process, 'pfSelectedElectrons'      , postfix ).cut = pfElectronSelectionCut
applyPostfix( process, 'isoValElectronWithCharged', postfix ).deposits[0].deltaR = pfElectronIsoConeR
applyPostfix( process, 'isoValElectronWithNeutral', postfix ).deposits[0].deltaR = pfElectronIsoConeR
applyPostfix( process, 'isoValElectronWithPhotons', postfix ).deposits[0].deltaR = pfElectronIsoConeR
applyPostfix( process, 'pfIsolatedElectrons'      , postfix ).combinedIsolationCut = pfElectronCombIsoCut
applyPostfix( process, 'patElectrons', postfix ).pvSrc = cms.InputTag( pfVertices )
applyPostfix( process, 'patMuons', postfix ).pvSrc = cms.InputTag( pfVertices )
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
kt6PFJetsChs = kt6PFJets.clone(
    rParam        = cms.double( 0.6 )
  , src           = cms.InputTag( 'pfNoElectron' )
  , doAreaFastjet = cms.bool( True )
  , doRhoFastjet  = cms.bool( True )
  , voronoiRfact  = cms.double( -0.9 )
  )
applyPostfix( process, 'patJetCorrFactors', postfix ).primaryVertices = cms.InputTag( pfVertices )
kt6PFJetsPFChs = kt6PFJetsChs.clone( src = cms.InputTag( 'pfNoElectron' + postfix ) )
setattr( process, 'kt6PFJetsChs' + postfix, kt6PFJetsPFChs )
getattr( process, 'patPF2PATSequence' + postfix).replace( getattr( process, 'patJetCorrFactors' + postfix )
                                                        , getattr( process, 'kt6PFJetsChs' + postfix ) * getattr( process, 'patJetCorrFactors' + postfix )
                                                          )
applyPostfix( process, 'patJetCorrFactors', postfix ).rho = cms.InputTag( 'kt6PFJetsChs' + postfix, 'rho' )

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



if not runOnMC:
  removeMCMatching(process, ['All'])
  process.looseLeptonSequence.remove( process.muonMatchLoosePFlow )

## New Electron ID
process.load("RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi")
#from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import *
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesFixedIsolationElectronIdentificationV06_cfi") ## Cut In Category
process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi") ## Cut In Category (MC tuned)
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")        ## simpleEID
process.patElectronIDs = cms.Sequence(
      process.simpleEleIdSequence
    + process.eidVeryLoose
    + process.eidLoose
    + process.eidMedium
    + process.eidTight
    + process.eidSuperTight
    + process.eidHyperTight1
    + process.eidHyperTight2
    + process.eidHyperTight3
    + process.eidHyperTight4
    + process.eidVeryLooseMC
    + process.eidLooseMC
    + process.eidMediumMC
    + process.eidTightMC
    + process.eidSuperTightMC
    + process.eidHyperTight1MC
    + process.eidHyperTight2MC
    + process.eidHyperTight3MC
    + process.eidHyperTight4MC
    )

for iele in [ process.patElectrons,
      process.patElectronsPFlow,
      process.patElectronsLoosePFlow ] :
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

#process.patElectrons.addElectronID = cms.bool(True)
#process.patElectrons.electronIDSources = electronIDSources

#applyPostfix( process, 'patElectrons', postfix ).electronIDSources = electronIDSources

process.patJetCorrFactors.payload = 'AK5PF'
process.patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute','L7Parton']
process.patJetCorrFactors.useRho = False
process.patJetCorrFactors.rho = cms.InputTag('')

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

from MyAna.bprimeKit.HitFitParameters_cfi import *
from MyAna.bprimeKit.ObjectParameters_cfi import *

##bprimeKit process
process.bprimeKit = cms.EDAnalyzer(
  "bprimeKit",
  MCtag = cms.untracked.bool(False),
  muonlabel = cms.VInputTag('selectedPatMuonsLoose'+postfix,'cleanPatMuons'),
  eleclabel = cms.VInputTag('selectedPatElectronsLoose'+postfix,'cleanPatElectrons'),
  taulabel = cms.VInputTag('selectedPatTaus'+postfix),
  LepCollections = cms.vstring('PFLepInfo','LepInfo'),
  jetlabel= cms.VInputTag('selectedPatJets'+postfix,'selectedPatJets'),
  JetCollections = cms.vstring('PFJetInfo','JetInfo'),
  PFJets = cms.vint32(1,0),
  PairCollection = cms.untracked.int32(1),
  metlabel  = cms.VInputTag("patMETs"),
  pfmetlabel  = cms.VInputTag("patMETsPFlow"),
  genlabel  = cms.VInputTag("genParticles"),
  hltlabel  = cms.VInputTag("TriggerResults::HLT"),
  pathltlabel = cms.VInputTag("patTriggerEvent"),
  offlinePVlabel = cms.VInputTag('offlinePrimaryVertices'),#"offlinePrimaryVertices"),
  offlinePVBSlabel = cms.VInputTag("offlinePrimaryVerticesWithBS"),
  offlineBSlabel = cms.VInputTag("offlineBeamSpot"),
  pixelvtxlabel = cms.VInputTag("pixelVertices"),
  tracklabel = cms.VInputTag("generalTracks"),
  dcslabel = cms.VInputTag("scalersRawToDigi"),
  genevtlabel = cms.VInputTag("generator"),
  gtdigilabel = cms.VInputTag("gtDigis"),
  rhocorrectionlabel = cms.VInputTag("kt6PFJetsPFlow","rho"),
  sigmaLabel = cms.VInputTag("kt6PFJetsPFlow","sigma"),
  puInfoLabel = cms.VInputTag("addPileupInfo"),
  doHitFit = cms.untracked.bool(False),
  HitFitParameters = defaultHitFitParameters.clone(),
  SelectionParameters = defaultObjectParameters.clone(),
  Debug = cms.untracked.int32(0)
  )


##Output file
process.TFileService = cms.Service("TFileService",
      fileName = cms.string( resultsFile )
      )

## process.p = cms.Path(
##       #process.triggerResults *
## ##       process.scrapingVeto *
## ##       process.HBHENoiseFilter *
##       process.goodOfflinePrimaryVertices *
##       process.patElectronIDs *
##       #process.primaryVertexFilter *
##       process.patDefaultSequence *
##       getattr(process,"patPF2PATSequencePFlow") *
##       #process.looseLeptonSequence *
##       getattr(process,"patPF2PATSequencePF") *
##       process.bpkitTriggerSequence *
##       process.bprimeKit
##       )

process.p = cms.Path()
if useTrigger:
  process.p += process.triggerResults
if not runOnMC:
  process.p += process.eventCleaning
process.p += process.goodOfflinePrimaryVertices
process.p += process.patElectronIDs
process.p += process.patDefaultSequence
process.p += getattr( process, 'patPF2PATSequence' + postfix)
process.p += process.looseLeptonSequence 
process.p += process.bpkitTriggerSequence
process.p += process.bprimeKit
