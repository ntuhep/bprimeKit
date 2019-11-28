#*************************************************************************
#
#  Filename    : Ntuplizer_cfi.
#  Description : Settings for common ntuplizer settings
#  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
#
#*************************************************************************
import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------------
#   Common objects
#-------------------------------------------------------------------------------
rhosrc            = cms.InputTag('fixedGridRhoFastjetAll')
rhocalosrc        = cms.InputTag('fixedGridRhoFastjetCentralCalo')
rhonofastjetsrc   = cms.InputTag('fixedGridRhoAll')
vtxsrc            = cms.InputTag('offlineSlimmedPrimaryVertices')
beamspotsrc       = cms.InputTag('offlineBeamSpot')
gensrc            = cms.InputTag('prunedGenParticles')

#-------------------------------------------------------------------------------
#   EvtGen settings
#-------------------------------------------------------------------------------
evtgenbase = cms.PSet(
    rhosrc          = rhosrc,
    rhonofastjetsrc = rhonofastjetsrc,
    metsrc          = cms.InputTag('slimmedMETs'),
    puppimetsrc     = cms.InputTag('slimmedMETsPuppi'),
    pusrc           = cms.InputTag('slimmedAddPileupInfo'),
    beamspotsrc     = beamspotsrc,
    prefwgtsrc      = cms.InputTag('prefiringweight:nonPrefiringProb'),
    prefwgtupsrc    = cms.InputTag('prefiringweight:nonPrefiringProbUp'),
    prefwgtdownsrc  = cms.InputTag('prefiringweight:nonPrefiringProbDown'),
    genevtsrc       = cms.InputTag('generator'),
    gensrc          = gensrc,
    lhesrc          = cms.InputTag('externalLHEProducer'),
    mettriggersrc   = cms.InputTag('TriggerResults','','RECO'),
    ecalBadCalibReducedMINIAODFiltersrc   = cms.InputTag('ecalBadCalibReducedMINIAODFilter')
)

#-------------------------------------------------------------------------------
#   Vertex settings
#-------------------------------------------------------------------------------
vertexbase = cms.PSet(
    vtxsrc = vtxsrc,
)

#-------------------------------------------------------------------------------
#   Trigger object settings
#-------------------------------------------------------------------------------
triggerbase = cms.PSet(
    triggersrc           = cms.InputTag('TriggerResults::HLT'),
    triggerprescalessrc  = cms.InputTag('patTrigger'),
    triggerobjsrc        = cms.InputTag('slimmedPatTrigger'),
    gtdigisrc            = cms.InputTag('gtDigis'),
    triggerlist          = cms.VPSet(
        cms.PSet(
            HLTPath=cms.string('HLT_IsoMu27_v*'),
            HLTFilter=cms.string('hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_IsoMu30_v*'),
            HLTFilter=cms.string('hltL3crIsoL1sMu22Or25L1f0L2f10QL3f30QL3trkIsoFiltered0p07')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Mu50_v*'),
            HLTFilter=cms.string('hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Mu55_v*'),
            HLTFilter=cms.string('hltL3fL1sMu22Or25L1f0L2f10QL3Filtered55Q')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele35_WPTight_Gsf_v*'),
            HLTFilter=cms.string('hltEle35noerWPTightGsfTrackIsoFilter')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Photon200_v*'),
            HLTFilter=cms.string('hltEG200HEFilter')
        ),
    )
)

#-------------------------------------------------------------------------------
#   Photon settings
#-------------------------------------------------------------------------------
photonbase = cms.PSet(
    photonname                = cms.string('PhotonInfo'),
    photonsrc                 = cms.InputTag('slimmedPhotons'),
    rhosrc                    = rhosrc,
    phoLooseIdMap             = cms.string('cutBasedPhotonID-Fall17-94X-V2-loose'),
    phoMediumIdMap            = cms.string('cutBasedPhotonID-Fall17-94X-V2-medium'),
    phoTightIdMap             = cms.string('cutBasedPhotonID-Fall17-94X-V2-tight'),
    phoIdMvaMap               = cms.string('PhotonMVAEstimatorRunIIFall17v2Values'),
    effAreaChHadFile          = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfChargedHadrons_90percentBased_V2.txt'),
    effAreaNeuHadFile         = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased_V2.txt'),
    effAreaPhoFile            = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Fall17/effAreaPhotons_cone03_pfPhotons_90percentBased_V2.txt'),
)

#-------------------------------------------------------------------------------
#   Lepton settings
#-------------------------------------------------------------------------------
leptonbase = cms.PSet(
    leptonname             = cms.string('LepInfo'),
    muonsrc                = cms.InputTag('slimmedMuons'),
    elecsrc                = cms.InputTag('slimmedElectrons'),
    tausrc                 = cms.InputTag('slimmedTaus'),
    packedsrc              = cms.InputTag('packedPFCandidates'),
    eleVetoIdMap           = cms.string('cutBasedElectronID-Fall17-94X-V2-veto'),
    eleLooseIdMap          = cms.string('cutBasedElectronID-Fall17-94X-V2-loose'),
    eleMediumIdMap         = cms.string('cutBasedElectronID-Fall17-94X-V2-medium'),
    eleTightIdMap          = cms.string('cutBasedElectronID-Fall17-94X-V2-tight'),
    eleHEEPIdMap           = cms.string('heepElectronID-HEEPV70'),
    eleffAreaNeuHadPhoFile = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_94X.txt'),
    eleffAreaHLTecalFile   = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_ecalPFClusterIso.txt'),
    eleffAreaHLThcalFile   = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_hcalPFClusterIso.txt'),
    mueffAreaNeuHadPhoFile = cms.FileInPath('bpkFrameWork/bprimeKit/data/effAreaMuons_cone03_pfNeuHadronsAndPhotons_94X.txt'),
    rhosrc                 = rhosrc,
    rhocalosrc             = rhocalosrc,
    vtxsrc                 = vtxsrc,
    gensrc                 = gensrc,
    beamspotsrc            = beamspotsrc,
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
ak4jetbase.jetsrc  = cms.InputTag('slimmedJets')

ak4jetpuppi = jetcommon.clone()
ak4jetpuppi.jetname = cms.string('JetInfoPuppi')
ak4jetpuppi.jettype = cms.string('AK4PFPuppi')
ak4jetpuppi.jetsrc  = cms.InputTag('slimmedJetsPuppi')

#-------------------------------------------------------------------------
#   AK8/CA8 jet common settings
#-------------------------------------------------------------------------
ak8jetpuppi = jetcommon.clone()
ak8jetpuppi.jetname   = cms.string('JetAK8Puppi')
ak8jetpuppi.jettype   = cms.string('AK8PFPuppi')
ak8jetpuppi.jetsrc    = cms.InputTag('slimmedJetsAK8')
ak8jetpuppi.subjetsrc = cms.InputTag('slimmedJetsAK8PFPuppiSoftDropPacked', 'SubJets', 'RECO')

ca8jetpuppi = jetcommon.clone()
ca8jetpuppi.jetname   = cms.string('JetCA8Puppi')
ca8jetpuppi.jettype   = cms.string('AK8PFPuppi')
ca8jetpuppi.jetsrc    = cms.InputTag('slimmedJetsAK8')
ca8jetpuppi.subjetsrc = cms.InputTag('patJetsCMSTopTagPuppiPacked')
