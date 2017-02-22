#*************************************************************************
#
#  Filename    : Ntuplizer_cfi.
#  Description : Settings for common ntuplizer settings
#  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
#
#*************************************************************************
import FWCore.ParameterSet.Config as cms

jetcommon = cms.PSet(
    jetname=cms.string('JetInfo'),
    jettype=cms.string(''),
    muonsrc=cms.InputTag('slimmedMuons'),
    vtxsrc=cms.InputTag('offlineSlimmedPrimaryVertices'),
    rhosrc=cms.InputTag('fixedGridRhoFastjetAll'),
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

#-------------------------------------------------------------------------
#   AK8/CA8 jet common settings
#-------------------------------------------------------------------------
ak8jetbase = jetcommon.clone()
ak8jetbase.jetname   = cms.string('JetAK8Info')
ak8jetbase.jettype   = cms.string('AK8PFchs')
ak8jetbase.jetsrc    = cms.InputTag('selectedPatJetsAK8PFCHS')
ak8jetbase.subjetsrc = cms.InputTag('selectedPatJetsAK8PFCHSSoftDropPacked')
##
ca8jetbase = jetcommon.clone()
ca8jetbase.jetname   = cms.string('JetCA8Info')
ca8jetbase.jettype   = cms.string('AK8PFchs')
ca8jetbase.jetsrc    = cms.InputTag('selectedPatJetsAK8PFCHS')
ca8jetbase.subjetsrc = cms.InputTag('patJetsCMSTopTagCHSPacked')
