#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import FWCore.ParameterSet.Config     as cms
from   PhysicsTools.PatAlgos.tools.jetTools  import updateJetCollection
from   JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
from   PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

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

listBtagDiscriminatorsDeepFlavour = [
    'pfDeepFlavourJetTags:probb',
    'pfDeepFlavourJetTags:probbb',
    'pfDeepFlavourJetTags:problepb',
    'pfDeepFlavourJetTags:probc',
    'pfDeepFlavourJetTags:probuds',
    'pfDeepFlavourJetTags:probg'
]

def jet_settings( process , runMC ):

    JETCorrLevels = ['L2Relative', 'L3Absolute']

    if not runMC:
        JETCorrLevels += ['L2L3Residual']

    jetToolbox( process, 'ak4', 'JetToolBoxSequence', 'noOutput',
        updateCollection   = 'slimmedJets',
        PUMethod           = 'CHS',
        dataTier           = 'miniAOD',
        JETCorrPayload     = 'AK4PFchs',
        JETCorrLevels      = ['L1FastJet'] + JETCorrLevels,
        bTagDiscriminators = listBtagDiscriminators + listBtagDiscriminatorsDeepFlavour,
        addQGTagger        = True,
        runOnMC            = runMC
    )

    jetToolbox( process, 'ak4', 'JetToolBoxSequence', 'noOutput',
        updateCollection   = 'slimmedJetsPuppi',
        PUMethod           = 'Puppi',
        dataTier           = 'miniAOD',
        JETCorrPayload     = 'AK4PFPuppi',
        JETCorrLevels      = JETCorrLevels,
        bTagDiscriminators = listBtagDiscriminators + listBtagDiscriminatorsDeepFlavour,
        runOnMC            = runMC
    )

    jetToolbox( process, 'ak8', 'JetToolBoxSequence', 'noOutput',
        updateCollection        = 'slimmedJetsAK8',
        updateCollectionSubjets = 'slimmedJetsAK8PFPuppiSoftDropPacked:SubJets',
        PUMethod                = 'Puppi',
        dataTier                = 'miniAOD',
        JETCorrPayload          = 'AK8PFPuppi',
        subJETCorrPayload       = 'AK4PFPuppi',
        JETCorrLevels           = JETCorrLevels,
        subJETCorrLevels        = JETCorrLevels,
        bTagDiscriminators      = listBtagDiscriminators + listBtagDiscriminatorsDeepFlavour,
        subjetBTagDiscriminators= ['pfDeepCSVJetTags:probb', 'pfDeepCSVJetTags:probbb'],
        runOnMC                 = runMC
    )

    jetToolbox( process, 'ca8', 'JetToolBoxSequence', 'noOutput',
        PUMethod                = 'Puppi',
        addMassDrop             = True,
        addCMSTopTagger         = True,
        GetJetMCFlavour         = True,
        GetSubjetMCFlavour      = True,
        bTagDiscriminators      = listBtagDiscriminators,
        runOnMC                 = runMC
    )

def met_settings( process, runData, is2017 = False ):

    runMetCorAndUncFromMiniAOD(
            process,
            isData          = runData,
            fixEE2017       = is2017,
            fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139} if is2017 else None,
            postfix         = 'Updated'
    )

    from PhysicsTools.PatAlgos.slimming.puppiForMET_cff import makePuppiesFromMiniAOD
    makePuppiesFromMiniAOD( process, True )

    runMetCorAndUncFromMiniAOD(
            process,
            isData    = runData,
            metType   = 'Puppi',
            jetFlavor = 'AK4PFPuppi',
            postfix   = 'PuppiUpdated'
    )

    MetSequence = cms.Sequence(  process.puppiMETSequence * process.fullPatMetSequenceUpdated * process.fullPatMetSequencePuppiUpdated )
    #MetSequence = cms.Sequence( process.fullPatMetSequenceUpdated )
    setattr( process, 'MetSequence', MetSequence )
