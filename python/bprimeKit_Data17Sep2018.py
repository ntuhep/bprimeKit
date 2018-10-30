#*******************************************************************************
 #
 #  Filename    : bprimeKit_MC_76X.py
 #  Description : Default settings for 76X
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
 #  Ditching original cfg framework 2016-03
 #
#*******************************************************************************
import FWCore.ParameterSet.Config        as cms
import bpkFrameWork.bprimeKit.Ntuplizer_cfi as ntpl

#-------------------------------------------------------------------------------
#   Additional tag settings
#-------------------------------------------------------------------------------
isData               = True
GlobalTag            = '102X_dataRun2_Sep2018Rereco_v1'
ElectronIDHEEPModule = 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff'
ElectronIDModule     = 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff'
PhotonIDModule       = 'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V1_TrueVtx_cff'

#-------------------------------------------------------------------------------
#   BprimeKit input tags
#-------------------------------------------------------------------------------
bprimeKit = cms.EDAnalyzer(
    'bprimeKit',

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
        ntpl.ak4jetbase,
        ntpl.ak8jetbase,
        ntpl.ca8jetbase,
        ntpl.ak4jetpuppi,
        ntpl.ak8jetpuppi,
        ntpl.ca8jetpuppi
    )
)
