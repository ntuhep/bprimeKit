#*************************************************************************
#
#  Filename    : bprimeKit_MC25ns_MiniAOD_76X.py
#  Description : Default settings for DataProcessing Flag MC25ns_MiniAOD_76X
#  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
#
#  Ditching original cfg framework 2016-03
#
#*************************************************************************
import bpkFrameWork.bprimeKit.Ntuplizer_cfi as ntpl
import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------
#   Tag settings
#-------------------------------------------------------------------------
GlobalTag = "80X_mcRun2_asymptotic_2016_TrancheIV_v6"
ElectronIDHEEPModule = "RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff"
ElectronIDModule = "RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff"

#-------------------------------------------------------------------------
#   Loading ntuplizer settings
#-------------------------------------------------------------------------
ak4jet = ntpl.ak4jetbase.clone()
ak8jet = ntpl.ak8jetbase.clone()
ca8jet = ntpl.ca8jetbase.clone()
ak4jet.jecversion = cms.string("Summer16_23Sep2016V3_MC")
ak8jet.jecversion = cms.string("Summer16_23Sep2016V3_MC")
ca8jet.jecversion = cms.string("Summer16_23Sep2016V3_MC")

#-------------------------------------------------------------------------
#   BprimeKit input settings
#-------------------------------------------------------------------------
bprimeKit = cms.EDAnalyzer(
    "bprimeKit",

    lherunsrc=cms.InputTag('externalLHEProducer'),

    evtgensetting  = ntpl.evtgenbase,
    vtxsetting     = ntpl.vertexbase,
    triggersetting = ntpl.triggerbase,

    #----- Photon information -----------------------------------------------
    photonsettings = cms.VPSet(
        ntpl.photonbase
    ),

    #----- Lepton related information ---------------------------------------
    leptonsettings=cms.VPSet(
        ntpl.leptonbase
    ),

    #----- Jet Information --------------------------------------------------
    jetsettings=cms.VPSet(
        ak4jet,
        ak8jet,
        ca8jet
    )
)
