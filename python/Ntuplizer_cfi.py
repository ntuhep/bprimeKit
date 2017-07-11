#*************************************************************************
#
#  Filename    : Ntuplizer_cfi.
#  Description : Settings for common ntuplizer settings
#  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
#
#*************************************************************************
import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------------
#   Common objects
#-------------------------------------------------------------------------------
rhosrc      = cms.InputTag( 'fixedGridRhoFastjetAll' )
vtxsrc      = cms.InputTag( 'offlineSlimmedPrimaryVertices' )
vtxBSsrc    = cms.InputTag( 'offlinePrimaryVerticesWithBS' )
beamspotsrc = cms.InputTag( 'offlineBeamSpot' )
hltsrc      = cms.InputTag( 'TriggerResults::HLT' )
gensrc      = cms.InputTag( 'prunedGenParticles' )


#-------------------------------------------------------------------------------
#   EvtGen settings
#-------------------------------------------------------------------------------
evtgenbase = cms.PSet(
    rhosrc        = rhosrc,
    metsrc        = cms.InputTag('slimmedMETs'),
    puppimetsrc   = cms.InputTag('slimmedMETsPuppi'),
    pusrc         = cms.InputTag('slimmedAddPileupInfo'),
    hltsrc        = hltsrc,
    beamspotsrc   = beamspotsrc,
    genevtsrc     = cms.InputTag('generator'),
    gensrc        = gensrc,
    gtdigisrc     = cms.InputTag('gtDigis'),
    lhesrc        = cms.InputTag('externalLHEProducer'),
    mettriggersrc = cms.InputTag("TriggerResults","","RECO"),
    ## Disabling on-the-fly MET filters until available
    # metbadchadsrc = cms.InputTag("BadChargedCandidateFilter"),
    # metbadmusrc   = cms.InputTag("BadPFMuonFilter"),

    )

#-------------------------------------------------------------------------------
#   Vertex settings
#-------------------------------------------------------------------------------
vertexbase = cms.PSet(
    vtxsrc = vtxsrc,
    vtxBSsrc = vtxBSsrc,
)

#-------------------------------------------------------------------------------
#   Trigger object settings
#-------------------------------------------------------------------------------
triggerbase = cms.PSet(
    triggersrc    = hltsrc,
    triggerobjsrc = cms.InputTag( 'slimmedPatTrigger'),
    triggerlist   = cms.VPSet(
                cms.PSet(
            HLTPath=cms.string('HLT_Mu50_v*'),
            HLTFilter=cms.string('hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele35_WPTight_Gsf_v*'),
            HLTFilter=cms.string('hltEle40noerWPTightGsfTrackIsoFilter')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele40_WPTight_Gsf_v*'),
            HLTFilter=cms.string('hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q')
        ),
    )
)

#-------------------------------------------------------------------------------
#   Photon settings
#-------------------------------------------------------------------------------
photonbase = cms.PSet(
    photonname = cms.string('PhotonInfo'),
    photonsrc  = cms.InputTag('slimmedPhotons'),
    rhosrc     = rhosrc,
    phoLooseIdMap  = cms.InputTag(  'egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-loose'),
    phoMediumIdMap = cms.InputTag( 'egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-medium'),
    phoTightIdMap  = cms.InputTag(  'egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-tight'),
    phoChargedIsolation       = cms.InputTag( "photonIDValueMapProducer:phoChargedIsolation"),
    phoNeutralHadronIsolation = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation"),
    phoPhotonIsolation        = cms.InputTag( "photonIDValueMapProducer:phoPhotonIsolation"),
    full5x5SigmaIEtaIEtaMap   = cms.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta"),
    effAreaChHadFile  = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfChargedHadrons_V2.txt"),
    effAreaNeuHadFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfNeutralHadrons_V2.txt"),
    effAreaPhoFile    = cms.FileInPath( "RecoEgamma/PhotonIdentification/data/PHYS14/effAreaPhotons_cone03_pfPhotons_V2.txt"),

)

#-------------------------------------------------------------------------------
#   Lepton settings
#-------------------------------------------------------------------------------
leptonbase = cms.PSet(
    leptonname     = cms.string('LepInfo'),
    muonsrc        = cms.InputTag('slimmedMuons'),
    elecsrc        = cms.InputTag('slimmedElectrons'),
    tausrc         = cms.InputTag('slimmedTaus'),
    packedsrc      = cms.InputTag('packedPFCandidates'),
    eleVetoIdMap   = cms.InputTag( 'egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto'),
    eleLooseIdMap  = cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose'),
    eleMediumIdMap = cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium'),
    eleTightIdMap  = cms.InputTag('egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight'),
    eleHEEPIdMap   = cms.InputTag('egmGsfElectronIDs:heepElectronID-HEEPV60'),
    conversionsrc  = cms.InputTag('reducedEgamma', 'reducedConversions'),
    rhosrc         = rhosrc,
    vtxsrc         = vtxsrc,
    gensrc         = gensrc,
    beamspotsrc    = beamspotsrc,
)


#-------------------------------------------------------------------------------
#   Jet settings
#-------------------------------------------------------------------------------
jetcommon = cms.PSet(
    jetname=cms.string('JetInfo'),
    jettype=cms.string(''),
    muonsrc=cms.InputTag('slimmedMuons'),
    vtxsrc=vtxsrc,
    rhosrc=rhosrc,
    jetsrc=cms.InputTag(''),
    subjetsrc=cms.InputTag(''),
    jecversion=cms.string(''),
)

#-------------------------------------------------------------------------
#   AK4 Jet common settings
#-------------------------------------------------------------------------
ak4jetbase = jetcommon.clone()
ak4jetbase.jettype = cms.string('AK4PFchs')
ak4jetbase.jetsrc  = cms.InputTag('selectedPatJetsAK4PFCHS')

ak4jetpuppi = jetcommon.clone()
ak4jetpuppi.jettype = cms.string('AK4PFPuppi')
ak4jetpuppi.jetsrc  = cms.InputTag('selectedPatJetsAK4PFPuppi')

#-------------------------------------------------------------------------
#   AK8/CA8 jet common settings
#-------------------------------------------------------------------------
ak8jetbase = jetcommon.clone()
ak8jetbase.jetname   = cms.string('JetAK8Info')
ak8jetbase.jettype   = cms.string('AK8PFchs')
ak8jetbase.jetsrc    = cms.InputTag('selectedPatJetsAK8PFCHS')
ak8jetbase.subjetsrc = cms.InputTag('selectedPatJetsAK8PFCHSSoftDropPacked')


ak8jetpuppi = jetcommon.clone()
ak8jetpuppi.jetname   = cms.string('JetAK8Puppi')
ak8jetpuppi.jettype   = cms.string('AK8PFPuppi')
ak8jetpuppi.jetsrc    = cms.InputTag('selectedPatJetsAK8PFPuppi')
ak8jetpuppi.subjetsrc = cms.InputTag('selectedPatJetsAK8PFPuppiSoftDropPacked')

################

ca8jetbase = jetcommon.clone()
ca8jetbase.jetname   = cms.string('JetCA8Info')
ca8jetbase.jettype   = cms.string('AK8PFchs')
ca8jetbase.jetsrc    = cms.InputTag('selectedPatJetsAK8PFCHS')
ca8jetbase.subjetsrc = cms.InputTag('patJetsCMSTopTagCHSPacked')

ca8jetpuppi = jetcommon.clone()
ca8jetpuppi.jetname   = cms.string('JetCA8Puppi')
ca8jetpuppi.jettype   = cms.string('AK8PFPuppi')
ca8jetpuppi.jetsrc    = cms.InputTag('selectedPatJetsAK8PFPuppi')
ca8jetpuppi.subjetsrc = cms.InputTag('patJetsCMSTopTagPuppiPacked')
