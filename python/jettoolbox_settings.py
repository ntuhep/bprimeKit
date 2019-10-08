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

    process.load('CommonTools/PileupAlgos/Puppi_cff')
    process.puppi.candName = cms.InputTag('packedPFCandidates')
    process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
    process.puppi.useExistingWeights = cms.bool(True)

    jetToolbox( process, 'ca8', 'ca8puppi', 'edmOut',
        runOnMC             = runMC,
        PUMethod            = 'Puppi',
        newPFCollection     = True,
        nameNewPFCollection = 'puppi',
        addMassDrop         = True,
        addCMSTopTagger     = True,
        GetJetMCFlavour     = True,
        GetSubjetMCFlavour  = True,
        bTagDiscriminators  = listBtagDiscriminators,
        Cut                 = ''
    )

    #Additional QGTagger Information
    #How to include QGL database : https://twiki.cern.ch/twiki/bin/view/CMS/QuarkGluonLikelihood#Step_0_Load_database_object_only
    from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup

    #Latest QGL database : https://twiki.cern.ch/twiki/bin/view/CMS/QuarkGluonLikelihood#2017_data_CMSSW_9_4_X_training
    process.QGPoolDBESSource = cms.ESSource("PoolDBESSource",
        CondDBSetup,
        toGet = cms.VPSet(
            cms.PSet(
                record = cms.string('QGLikelihoodRcd'),
                tag    = cms.string('QGLikelihoodObject_v1_AK4'),
                label  = cms.untracked.string('QGL_AK4PFchs')
            ),
        ),
        connect = cms.string('sqlite_fip:bpkFrameWork/bprimeKit/data/QGL_AK4chs_94X.db')
    )
    process.es_prefer_qg = cms.ESPrefer('PoolDBESSource','QGPoolDBESSource')

    JetToolBoxSequence = cms.Sequence( process.jetTask )
    setattr( process, 'JetToolBoxSequence', JetToolBoxSequence)
