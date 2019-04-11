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
    genevtsrc       = cms.InputTag('generator'),
    gensrc          = gensrc,
    lhesrc          = cms.InputTag('externalLHEProducer'),
    mettriggersrc   = cms.InputTag('TriggerResults','','RECO'),
    metfilter       = cms.vstring(
                        "Flag_HBHENoiseFilter",
                        "Flag_HBHENoiseIsoFilter",
                        "Flag_EcalDeadCellTriggerPrimitiveFilter",
                        "Flag_goodVertices",
                        "Flag_globalSuperTightHalo2016Filter",
                        "Flag_BadPFMuonFilter",
                    )
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
            HLTFilter=cms.string("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09"),
            ),
        cms.PSet(
            HLTPath=cms.string('HLT_Mu45_eta2p1_v*'),
            HLTFilter=cms.string('hltL3fL1sMu22Or25L1f0L2f10QL3Filtered45e2p1Q')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele45_WPLoose_Gsf_v*'),
            HLTFilter=cms.string('hltEle45WPLooseGsfTrackIsoFilter')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Mu50_v*'),
            HLTFilter=cms.string('hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_TkMu50_v*'),
            HLTFilter=cms.string('hltL3fL1sMu25f0TkFiltered50Q')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele27_eta2p1_WPLoose_Gsf_v*'),
            HLTFilter=cms.string('hltEle27erWPLooseGsfTrackIsoFilter')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele32_eta2p1_WPTight_Gsf_v*'),
            HLTFilter=cms.string('hltEle32WPTightGsfTrackIsoFilter')
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v*'),
            HLTFilter=cms.string('hltDiPFJet50') ## For jets
        ),
        cms.PSet(
            HLTPath=cms.string('HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v*'),
            HLTFilter=cms.string('hltEle45CaloIdVTGsfTrkIdTGsfDphiFilter') ## For Electron
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
    phoLooseIdMap  = cms.string('cutBasedPhotonID-Spring16-V2p2-loose'),
    phoMediumIdMap = cms.string('cutBasedPhotonID-Spring16-V2p2-medium'),
    phoTightIdMap  = cms.string('cutBasedPhotonID-Spring16-V2p2-tight'),
    phoIdMvaMap    = cms.string('PhotonMVAEstimatorRun2Spring16NonTrigV1Values'),
    effAreaChHadFile  = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt'),
    effAreaNeuHadFile = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt'),
    effAreaPhoFile    = cms.FileInPath('RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt'),
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
    eleVetoIdMap           = cms.string('cutBasedElectronID-Summer16-80X-V1-veto'),
    eleLooseIdMap          = cms.string('cutBasedElectronID-Summer16-80X-V1-loose'),
    eleMediumIdMap         = cms.string('cutBasedElectronID-Summer16-80X-V1-medium'),
    eleTightIdMap          = cms.string('cutBasedElectronID-Summer16-80X-V1-tight'),
    eleHEEPIdMap           = cms.string('heepElectronID-HEEPV70'),
    eleffAreaNeuHadPhoFile = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt'),
    eleffAreaHLTecalFile   = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_ecalPFClusterIso.txt'),
    eleffAreaHLThcalFile   = cms.FileInPath('RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_HLT_hcalPFClusterIso.txt'),
    mueffAreaNeuHadPhoFile = cms.FileInPath('bpkFrameWork/bprimeKit/data/effAreaMuons_cone03_pfNeuHadronsAndPhotons_80X.txt'),
    conversionsrc          = cms.InputTag('reducedEgamma', 'reducedConversions'),
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
ak4jetbase.jetsrc  = cms.InputTag('selectedPatJetsAK4PFCHS')

ak4jetpuppi = jetcommon.clone()
ak4jetpuppi.jetname = cms.string('JetInfoPuppi')
ak4jetpuppi.jettype = cms.string('AK4PFPuppi')
#ak4jetpuppi.jetsrc  = cms.InputTag('selectedPatJetsAK4PFPuppi')
ak4jetpuppi.jetsrc  = cms.InputTag('updatedPatJetsSelectedAK4PFPuppi')

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
