#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import FWCore.ParameterSet.Config     as cms
from   JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

listBtagDiscriminators = [
    'pfJetProbabilityBJetTags',
    'pfCombinedInclusiveSecondaryVertexV2BJetTags',
    'pfCombinedMVAV2BJetTags',
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    'pfCombinedCvsLJetTags',
    'pfCombinedCvsBJetTags'
]

def jettoolbox_settings( process , RunOnMC ):
    process.load('CommonTools/PileupAlgos/Puppi_cff')
    process.puppi.candName   = cms.InputTag('packedPFCandidates')
    process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
    process.puppiOnTheFly    = process.puppi.clone()
    process.puppiOnTheFly.useExistingWeights = True

    # print "Add AK4 Jets"
    # jetToolbox( process, 'ak4', 'jetToolBox_ak4', 'edmOut',
    #     runOnMC            = RunOnMC ,
    #     addQGTagger        = True,
    #     bTagDiscriminators = listBtagDiscriminators ,
    #     Cut                = ''
    # )

    # print "Add AK8 Jets, soft drop"
    # jetToolbox( process, 'ak8', 'jetToolBox_ak8', 'edmOut',
    # runOnMC            = RunOnMC,
    # addSoftDropSubjets = True,
    # addTrimming        = True,
    # rFiltTrim          = 0.1,
    # addPruning         = True,
    # addFiltering       = True,
    # addSoftDrop        = True,
    # addNsub            = True,
    # GetJetMCFlavour    = True,
    # GetSubjetMCFlavour = True,
    # bTagDiscriminators = listBtagDiscriminators ,
    # Cut                = '' )

    print "Add CA8 Subjet collection for top tagging"
    jetToolbox( process, 'ca8', 'jetToolBox_ca8_toptag', 'edmOut',
        runOnMC            = RunOnMC ,
        addMassDrop        = True,
        addCMSTopTagger    = True,
        GetJetMCFlavour    = True,
        GetSubjetMCFlavour = True,
        bTagDiscriminators = listBtagDiscriminators ,
        Cut                = ''
    )

    # print "Add ak8 jets, puppi"
    # jetToolbox( process, 'ak8', 'jetToolBox_ak8_puppi', 'edmOut',
    # runOnMC            = RunOnMC ,
    # PUMethod           = 'Puppi',
    # addTrimming        = True,
    # addPruning         = True,
    # addFiltering       = True,
    # addSoftDrop        = True,
    # addSoftDropSubjets = True,
    # addNsub            = True,
    # GetJetMCFlavour    = True,
    # GetSubjetMCFlavour = True,
    # bTagDiscriminators = listBtagDiscriminators ,
    # Cut                = ''
    # )
