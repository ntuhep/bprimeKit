import FWCore.ParameterSet.Config as cms

defaultHitFitParameters = cms.PSet(
    Debug              = cms.untracked.bool(False),
    Default            = cms.untracked.FileInPath("MyAna/bpkHitFit/data/setting/RunHitFitConfiguration.txt"),
    ElectronResolution = cms.untracked.FileInPath("TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt"),
    ElectronObjRes     = cms.untracked.bool(False),
    MuonResolution     = cms.untracked.FileInPath("TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt"),
    MuonObjRes         = cms.untracked.bool(False),
    UdscJetResolution  = cms.untracked.FileInPath("TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt"),
    BJetResolution     = cms.untracked.FileInPath("TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt"),
    JetObjRes          = cms.untracked.bool(False),
    JetCorrectionLevel = cms.untracked.string('L7Parton'),
    UdscJES            = cms.untracked.double(1.0),
    BJES               = cms.untracked.double(1.0),
    METResolution      = cms.untracked.FileInPath("TopQuarkAnalysis/TopHitFit/data/resolution/tqafKtResolution.txt"),
    METsObjRes         = cms.untracked.bool(False),
    LepWMass           = cms.untracked.double(80.4),
    HadWMass           = cms.untracked.double(80.4),
    TopMass            = cms.untracked.double(0.0),
    NuSolution         = cms.untracked.int32(2),
    MinLeptonPt        = cms.untracked.double(15.0),
    MinJetPt           = cms.untracked.double(15.0),
    MinMET             = cms.untracked.double(0.0),
    MaxNJet            = cms.untracked.uint32(4)
    )
