#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import FWCore.ParameterSet.Config     as cms
from   JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

listBtagDiscriminators = [
    #AK8 double b-tagging discriminator
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
#    'pfMassIndependentDeepDoubleBvLJetTags:probHbb',
    #Deep CSV
    'pfDeepCSVJetTags:probb',
    'pfDeepCSVJetTags:probbb',
    'pfDeepCSVJetTags:probc',
    'pfDeepCSVJetTags:probudsg'
]

def jettoolbox_settings( process , runMC ):

    jetToolbox( process, 'ca8', 'JetToolBoxSequence', 'noOutput',
        PUMethod            = 'Puppi',
        addMassDrop         = True,
        addCMSTopTagger     = True,
        GetJetMCFlavour     = True,
        GetSubjetMCFlavour  = True,
        bTagDiscriminators  = listBtagDiscriminators,
        runOnMC             = runMC,
        Cut                 = ''
    )

    # Add Pujet ID for AK4 CHS with UL18 training
    # https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/PatAlgos/test/patTuple_updateJets_fromMiniAOD_cfg.py#L49L65
    from RecoJets.JetProducers.PileupJetID_cfi import pileupJetId, _chsalgos_106X_UL18
    process.pileupJetId = pileupJetId.clone(
        jets                = cms.InputTag("slimmedJets"),
        inputIsCorrected    = True,
        applyJec            = False,
        vertexes            = cms.InputTag("offlineSlimmedPrimaryVertices"),
        algos               = cms.VPSet(_chsalgos_106X_UL18),
    )
    updateJetCollection(
        process,
        labelName = '',
        jetSource = cms.InputTag('slimmedJets'),
    )
    process.updatedPatJets.userData.userInts.src = ['pileupJetId:fullId']
    process.updatedPatJets.userData.userFloats.src = ['pileupJetId:fullDiscriminant']
