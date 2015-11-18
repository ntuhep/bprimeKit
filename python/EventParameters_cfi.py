import FWCore.ParameterSet.Config as cms

from bpkFrameWork.bprimeKit.ObjectParameters_cfi import *

defaultEventParameters = cms.PSet(
    ObjectParameters = defaultObjectParameters.clone(),
    MinDRmuonJet     = cms.untracked.double(0.3),
    MinDRjetElectron = cms.untracked.double(0.3),
    CleanDRjetMuon   = cms.untracked.double(-1.),
    MuonTriggers     = cms.untracked.vstring('HLT_IsoMu17_v'),
    MuTrigLowerRun   = cms.untracked.vint32(0),
    ElectronTriggers = cms.untracked.vstring(),
    CutLevels        = cms.untracked.vstring('Initial','Trigger','Vertex','Lepton','MuonVeto','ElectronVeto','ConversionVeto','MinJets'),
    ElTrigLowerRun   = cms.untracked.vint32(),
    MinNJets         = cms.untracked.int32(4),
	CheckJetPtOrder  = cms.untracked.bool(False),
    MinJetPTs        = cms.untracked.vdouble(0.),
    MinMET           = cms.untracked.double(0.),
    Debug            = cms.untracked.bool(False)
    )

