#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import FWCore.ParameterSet.Config     as cms
from   JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

listBTagInfos = [
    'pfInclusiveSecondaryVertexFinderTagInfos',
]

listBtagDiscriminators = [
    'pfJetProbabilityBJetTags',
    'pfCombinedInclusiveSecondaryVertexV2BJetTags',
    'pfCombinedMVAV2BJetTags',
    'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    'pfCombinedCvsLJetTags',
    'pfCombinedCvsBJetTags'
]

ak4Cut='pt > 25 && abs(eta) < 5.'
ak8Cut='pt > 100 && abs(eta) < 5.'

def jettoolbox_settings( process , runMC ):
    process.load('CommonTools/PileupAlgos/Puppi_cff')
    process.puppi.candName = cms.InputTag('packedPFCandidates')
    process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
    process.puppi.useExistingWeights = cms.bool(True)

    jetToolbox( process, 'ak4', 'analysisPath', 'edmOut',
        runOnMC            = runMC,
        addQGTagger        = True,
        bTagInfos          = listBTagInfos,
        bTagDiscriminators = listBtagDiscriminators,
        Cut                = ak4Cut )

    #jetToolbox( process, 'ak4', 'analysisPath', 'edmOut',
    #    runOnMC=runMC,
    #    PUMethod='Puppi',
    #    newPFCollection=True,
    #    nameNewPFCollection='puppi',
    #    bTagInfos=listBTagInfos,
    #    bTagDiscriminators=listBtagDiscriminators,
    #    Cut=ak4Cut )

    jetToolbox( process, 'ak8', 'analysisPath', 'edmOut',
        runOnMC            = runMC,
        addSoftDropSubjets = True,
        addTrimming        = True,
        rFiltTrim          = 0.1,
        addPruning         = True,
        addFiltering       = True,
        addSoftDrop        = True,
        addNsub            = True,
        bTagInfos          = listBTagInfos,
        bTagDiscriminators = listBtagDiscriminators,
        addCMSTopTagger    = True,
        Cut                = ak8Cut ,
        addNsubSubjets     = True,
        subjetMaxTau       = 4 )

    #jetToolbox( process, 'ak8', 'analysisPath', 'edmOut',
    #    runOnMC=runMC,
    #    PUMethod='Puppi',
    #    newPFCollection=True,
    #    nameNewPFCollection='puppi',
    #    addSoftDropSubjets=True,
    #    addTrimming=True,
    #    addPruning=True,
    #    addFiltering=True,
    #    addSoftDrop=True,
    #    addNsub=True,
    #    bTagInfos=listBTagInfos,
    #    bTagDiscriminators=listBtagDiscriminators,
    #    addCMSTopTagger=True,
    #    Cut=ak8Cut,
    #    addNsubSubjets=True,
    #    subjetMaxTau=4
    #    )

    jetToolbox( process, 'ca8', 'analysisPath', 'edmOut',
        runOnMC            = runMC ,
        addMassDrop        = True,
        addCMSTopTagger    = True,
        GetJetMCFlavour    = True,
        GetSubjetMCFlavour = True,
        bTagDiscriminators = listBtagDiscriminators ,
        Cut                = ''
    )

    # jetToolbox( process, 'ca8', 'analysisPath', 'edmOut',
    #     runOnMC            = runMC ,
    #     PUMethod='Puppi',
    #     newPFCollection=True,
    #     nameNewPFCollection='puppi',
    #     addMassDrop        = True,
    #     addCMSTopTagger    = True,
    #     GetJetMCFlavour    = True,
    #     GetSubjetMCFlavour = True,
    #     bTagDiscriminators = listBtagDiscriminators ,
    #     Cut                = ''
    # )
