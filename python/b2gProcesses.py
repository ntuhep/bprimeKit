import FWCore.ParameterSet.Config as cms

###inputTag labels
rhoLabel          = "fixedGridRhoFastjetAll"
muLabel           = 'slimmedMuons'
elLabel           = 'slimmedElectrons'
jLabel            = 'selectedPatJetsAK4PFCHS'
jLabelAK8 = 'selectedPatJetsAK8PFCHS'
jLabelAK8Puppi = 'selectedPatJetsAK8PFPuppi'

rhoLabel          = 'fixedGridRhoFastjetAll'
pvLabel           = 'offlineSlimmedPrimaryVertices'
convLabel         = 'reducedEgamma:reducedConversions'
particleFlowLabel = 'packedPFCandidates'
metLabel          = 'slimmedMETs'
metNoHFLabel      = 'slimmedMETsNoHF'

triggerResultsLabel = "TriggerResults"
triggerSummaryLabel = "hltTriggerSummaryAOD"
hltMuonFilterLabel  = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f40QL3crIsoRhoFiltered0p15"
hltPathLabel        = "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL"
hltElectronFilterLabel  = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8"

### Including QGL: ensuring the database onject can be accessed
qgDatabaseVersion = 'v1' # check https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion

triggerResultsLabel    = "TriggerResults"
triggerSummaryLabel    = "hltTriggerSummaryAOD"
hltMuonFilterLabel     = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f40QL3crIsoRhoFiltered0p15"
hltPathLabel           = "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL"
hltElectronFilterLabel = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8"

metProcess = "PAT"

#-------------------------------------------------------------------------------
#   Filters
#-------------------------------------------------------------------------------
skimmedPatMuons = cms.EDFilter(
    "PATMuonSelector",
    src = cms.InputTag('slimmedMuons'),
    cut = cms.string("pt > 0.0 && abs(eta) < 2.4")
    )


skimmedPatPhotons = cms.EDFilter(
    "PATPhotonSelector",
    src = cms.InputTag("slimmedPhotons"),
    cut = cms.string("pt > 30 && abs(eta) < 2.4"),
)

skimmedPatElectrons = cms.EDFilter(
    "PATElectronSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("pt > 10 && abs(eta) < 2.5")
    )

skimmedPatMET = cms.EDFilter(
    "PATMETSelector",
    src = cms.InputTag(metLabel, "", metProcess),
    cut = cms.string("")
    )

## Jet selection is handled by jet tool box

#-------------------------------------------------------------------------------
#   Producers
#-------------------------------------------------------------------------------
process.muonUserData = cms.EDProducer(
    'MuonUserData',
    muonLabel      = cms.InputTag("skimmedPatMuons"),
    pv             = cms.InputTag(pvLabel),
    packedPFCands  = cms.InputTag("packedPFCandidates"),
    ### TTRIGGER ###
    triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
    triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
    hltMuonFilter  = cms.InputTag(hltMuonFilterLabel),
    hltPath            = cms.string("HLT_IsoMu40_eta2p1_v11"),
    hlt2reco_deltaRmax = cms.double(0.1),
    # mainROOTFILEdir    = cms.string("../data/")
    )

process.electronUserData = cms.EDProducer(
    'ElectronUserData',
    eleLabel   = cms.InputTag("skimmedPatElectrons"),
    pv         = cms.InputTag(pvLabel),
    packedPFCands = cms.InputTag("packedPFCandidates"),
    beamSpot = cms.InputTag("offlineBeamSpot"),
    conversion = cms.InputTag(convLabel),
    rho        = cms.InputTag(rhoLabel),
    triggerResults = cms.InputTag(triggerResultsLabel),
    triggerSummary = cms.InputTag(triggerSummaryLabel),
    hltElectronFilter  = cms.InputTag(hltElectronFilterLabel),  ##trigger matching code to be fixed!
    hltPath             = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL"),
    electronVetoIdMap      = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
    electronLooseIdMap     = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    electronMediumIdMap    = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    electronTightIdMap     = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    electronHEEPIdMap      = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
    eleMediumIdFullInfoMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    eleIdVerbose           = cms.bool(False)
    )

process.photonUserData = cms.EDProducer(
    'PhotonUserData',
    rho                     = cms.InputTag(rhoLabel),
    pholabel                = cms.InputTag("slimmedPhotons"),
    phoLooseIdMap           = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-loose"),
    phoMediumIdMap          = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-medium"),
    phoTightIdMap           = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-tight"),
    phoChgIsoMap            = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation"),
    phoPhoIsoMap            = cms.InputTag("photonIDValueMapProducer:phoPhotonIsolation"),
    phoNeuIsoMap            = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation"),
    full5x5SigmaIEtaIEtaMap = cms.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta")
    )


process.jetUserData = cms.EDProducer(
    'JetUserData',
    jetLabel          = cms.InputTag(jLabel),
    rho               = cms.InputTag('fixedGridRhoAll'),
    getJERFromTxt     = cms.bool(True),
    jetCorrLabel      = cms.string(jetAlgo),
    jerLabel          = cms.string(jetAlgo),
    resolutionsFile   = cms.FileInPath(jerEra+'_PtResolution_'+jetAlgo+'.txt'),
    scaleFactorsFile  = cms.FileInPath(jerEra+'_SF_'+jetAlgo+'.txt'),
    ### TTRIGGER ###
    triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
    triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
    hltJetFilter       = cms.InputTag("hltPFHT"),
    hltPath            = cms.string("HLT_PFHT800"),
    hlt2reco_deltaRmax = cms.double(0.2),
    candSVTagInfos         = cms.string("pfInclusiveSecondaryVertexFinder"),
    )

process.jetUserDataAK8 = cms.EDProducer(
    'JetUserData',
    jetLabel          = cms.InputTag(jLabelAK8),
    rho               = cms.InputTag('fixedGridRhoAll'),
    getJERFromTxt     = cms.bool(True),
    jetCorrLabel      = cms.string(jetAlgoAK8),
    jerLabel          = cms.string(jetAlgoAK8),
    resolutionsFile   = cms.FileInPath(jerEra+'_PtResolution_'+jetAlgoAK8+'.txt'),
    scaleFactorsFile  = cms.FileInPath(jerEra+'_SF_'+jetAlgoAK8+'.txt'),
    ### TTRIGGER ###
    triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
    triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
    hltJetFilter       = cms.InputTag("hltAK8PFJetsTrimR0p1PT0p03"),
    hltPath            = cms.string("HLT_AK8PFHT650_TrimR0p1PT0p03Mass50"),
    hlt2reco_deltaRmax = cms.double(0.2),
    candSVTagInfos     = cms.string("pfInclusiveSecondaryVertexFinder"),
)
process.jetUserDataAK8Puppi = cms.EDProducer(
    'JetUserData',
    jetLabel          = cms.InputTag( jLabelAK8Puppi ),
    rho               = cms.InputTag('fixedGridRhoAll'),
    getJERFromTxt     = cms.bool(True),
    jetCorrLabel      = cms.string(jetAlgoAK8Puppi),
    jerLabel          = cms.string(jetAlgoAK8Puppi),
    resolutionsFile   = cms.FileInPath(jerEra+'_PtResolution_'+jetAlgoAK8Puppi+'.txt'),
    scaleFactorsFile  = cms.FileInPath(jerEra+'_SF_'+jetAlgoAK8Puppi+'.txt'),
    ### TTRIGGER ###
    triggerResults = cms.InputTag(triggerResultsLabel,"","HLT"),
    triggerSummary = cms.InputTag(triggerSummaryLabel,"","HLT"),
    hltJetFilter       = cms.InputTag("hltAK8PFJetsTrimR0p1PT0p03"),
    hltPath            = cms.string("HLT_AK8PFHT650_TrimR0p1PT0p03Mass50"),
    hlt2reco_deltaRmax = cms.double(0.2),
    candSVTagInfos         = cms.string("pfInclusiveSecondaryVertexFinder"),
    )
