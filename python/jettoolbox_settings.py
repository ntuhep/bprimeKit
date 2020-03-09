#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import FWCore.ParameterSet.Config     as cms
from   JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

listBtagDiscriminators = [
    #AK8 double b-tagging discriminator
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    'pfMassIndependentDeepDoubleBvLJetTags:probHbb',
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
