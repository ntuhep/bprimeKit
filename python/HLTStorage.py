#*************************************************************************
#
#  Filename    : HLTStorage.py
#  Description : List of HLT objects to store
#  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
#
#*************************************************************************

import FWCore.ParameterSet.Config as cms

triggerlist = cms.VPSet(
    cms.PSet(
        HLTPath=cms.string('HLT_IsoMu27_v*'),
        HLTFilter=cms.string(
            "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09"),
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
        HLTPath=cms.string('HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v*'),
        HLTFilter=cms.string('hltDiPFJet50') ## For jets
    ),
    cms.PSet(
        HLTPath=cms.string('HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v*'),
        HLTFilter=cms.string('hltEle45CaloIdVTGsfTrkIdTGsfDphiFilter') ## For Electron
    ),
)
