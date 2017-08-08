#*******************************************************************************
 #
 #  Filename    : jettoolbox_settings.py
 #  Description : Configuration settings for jet settings
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
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

    jetToolbox( process, 'ak4', 'ak4chs', 'edmOut',
        runOnMC            = runMC,
        addQGTagger        = True,
        bTagInfos          = listBTagInfos,
        bTagDiscriminators = listBtagDiscriminators,
        Cut                = ak4Cut )

    jetToolbox( process, 'ak4', 'ak4puppi', 'edmOut',
        runOnMC=runMC,
        PUMethod='Puppi',
        newPFCollection=True,
        nameNewPFCollection='puppi',
        bTagInfos=listBTagInfos,
        bTagDiscriminators=listBtagDiscriminators,
        Cut=ak4Cut )

    jetToolbox( process, 'ak8', 'ak8chs', 'edmOut',
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

    jetToolbox( process, 'ak8', 'ak8puppi', 'edmOut',
        runOnMC=runMC,
        PUMethod='Puppi',
        newPFCollection=True,
        nameNewPFCollection='puppi',
        addSoftDropSubjets=True,
        addTrimming=True,
        addPruning=True,
        addFiltering=True,
        addSoftDrop=True,
        addNsub=True,
        bTagInfos=listBTagInfos,
        bTagDiscriminators=listBtagDiscriminators,
        addCMSTopTagger=True,
        Cut=ak8Cut,
        addNsubSubjets=True,
        subjetMaxTau=4
        )

    jetToolbox( process, 'ca8', 'ca8chs', 'edmOut',
        runOnMC            = runMC ,
        addMassDrop        = True,
        addCMSTopTagger    = True,
        GetJetMCFlavour    = True,
        GetSubjetMCFlavour = True,
        bTagDiscriminators = listBtagDiscriminators ,
        Cut                = ''
    )

    jetToolbox( process, 'ca8', 'ca8puppi', 'edmOut',
        runOnMC            = runMC ,
        PUMethod           = 'Puppi',
        newPFCollection    = True,
        nameNewPFCollection= 'puppi',
        addMassDrop        = True,
        addCMSTopTagger    = True,
        GetJetMCFlavour    = True,
        GetSubjetMCFlavour = True,
        bTagDiscriminators = listBtagDiscriminators ,
        Cut                = ''
    )

    #To avoid producing additional JetToolBox format files, kick out the "EndPath" attribute of process.
    #Aditionally remove the unscheduled mode added by JetToolBox which results in segmentation violation???
    #(temporary!!)
    delattr(process, 'endpath')

    # Required because allowedUnsheduled is broken?
    # Or the jetttoolbox is not adding all the sequences
    process.ak4chs = cms.Sequence(
        process.ak4PFJetsCHS *
        process.pfImpactParameterTagInfosAK4PFCHS *
        process.pfSecondaryVertexTagInfosAK4PFCHS *
        process.softPFElectronsTagInfosAK4PFCHS *
        process.softPFMuonsTagInfosAK4PFCHS *
        process.pfInclusiveSecondaryVertexFinderTagInfosAK4PFCHS *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHS *
        process.pfCombinedMVAV2BJetTagsAK4PFCHS *
        process.pfJetProbabilityBJetTagsAK4PFCHS *
        process.pfImpactParameterAK8TagInfosAK4PFCHS *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK4PFCHS *
        process.pfBoostedDoubleSVAK8TagInfosAK4PFCHS *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK4PFCHS *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK4PFCHS *
        process.pfCombinedCvsLJetTagsAK4PFCHS *
        process.pfCombinedCvsBJetTagsAK4PFCHS *
        process.ak4PFJetsCHSConstituents *
        process.patJetCorrFactorsAK4PFCHS *
        process.QGTaggerAK4PFCHS *
        process.patJetsAK4PFCHS *
        process.selectedPatJetsAK4PFCHS
    )

    process.ak4puppi = cms.Sequence(
        process.ak4PFJetsPuppi *
        process.pfImpactParameterTagInfosAK4PFPuppi *
        process.pfSecondaryVertexTagInfosAK4PFPuppi *
        process.softPFElectronsTagInfosAK4PFPuppi *
        process.softPFMuonsTagInfosAK4PFPuppi *
        process.pfInclusiveSecondaryVertexFinderTagInfosAK4PFPuppi *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK4PFPuppi *
        process.pfCombinedMVAV2BJetTagsAK4PFPuppi *
        process.pfJetProbabilityBJetTagsAK4PFPuppi *
        process.pfImpactParameterAK8TagInfosAK4PFPuppi *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK4PFPuppi *
        process.pfBoostedDoubleSVAK8TagInfosAK4PFPuppi *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK4PFPuppi *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK4PFPuppi *
        process.pfCombinedCvsLJetTagsAK4PFPuppi *
        process.pfCombinedCvsBJetTagsAK4PFPuppi *
        process.ak4PFJetsPuppiConstituents *
        process.patJetCorrFactorsAK4PFPuppi *
        # process.QGTaggerAK4PFPuppi *
        process.patJetsAK4PFPuppi *
        process.selectedPatJetsAK4PFPuppi
    )

    process.ak8chs = cms.Sequence(
        process.ak8PFJetsCHS *
        process.ak8PFJetsCHSConstituents *
        process.ak8PFJetsCHSSoftDrop *
        process.patJetCorrFactorsAK8PFCHSSoftDrop *
        process.patJetsAK8PFCHSSoftDrop *

        process.pfImpactParameterTagInfosAK8PFCHS *
        process.pfSecondaryVertexTagInfosAK8PFCHS *
        process.softPFElectronsTagInfosAK8PFCHS *
        process.softPFMuonsTagInfosAK8PFCHS *
        process.pfInclusiveSecondaryVertexFinderTagInfosAK8PFCHS *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK8PFCHS *
        process.pfCombinedMVAV2BJetTagsAK8PFCHS *
        process.pfJetProbabilityBJetTagsAK8PFCHS *
        process.pfImpactParameterAK8TagInfosAK8PFCHS *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK8PFCHS *
        process.pfBoostedDoubleSVAK8TagInfosAK8PFCHS *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK8PFCHS *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK8PFCHS *
        process.pfCombinedCvsLJetTagsAK8PFCHS *
        process.pfCombinedCvsBJetTagsAK8PFCHS *

        process.ak8PFJetsCHSPruned *
        process.ak8PFJetsCHSTrimmed *
        process.ak8PFJetsCHSFiltered *
        process.ak8PFJetsCHSFilteredMass *
        process.ak8PFJetsCHSTrimmedMass *
        process.ak8PFJetsCHSPrunedMass *
        process.ak8PFJetsCHSSoftDropMass *
        process.NjettinessAK8CHS *

        process.pfImpactParameterTagInfosAK8PFCHSSoftDropSubjets *
        process.pfSecondaryVertexTagInfosAK8PFCHSSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderTagInfosAK8PFCHSSoftDropSubjets *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK8PFCHSSoftDropSubjets *
        process.softPFElectronsTagInfosAK8PFCHSSoftDropSubjets *
        process.softPFMuonsTagInfosAK8PFCHSSoftDropSubjets *
        process.pfCombinedMVAV2BJetTagsAK8PFCHSSoftDropSubjets *
        process.pfJetProbabilityBJetTagsAK8PFCHSSoftDropSubjets *
        process.pfImpactParameterAK8TagInfosAK8PFCHSSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK8PFCHSSoftDropSubjets *
        process.pfBoostedDoubleSVAK8TagInfosAK8PFCHSSoftDropSubjets *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK8PFCHSSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK8PFCHSSoftDropSubjets *
        process.pfCombinedCvsLJetTagsAK8PFCHSSoftDropSubjets *
        process.pfCombinedCvsBJetTagsAK8PFCHSSoftDropSubjets *
        process.NsubjettinessAK8PFCHSSoftDropSubjets *

        process.patJetCorrFactorsAK8PFCHSSoftDropSubjets *
        process.patJetsAK8PFCHSSoftDropSubjets *
        process.selectedPatJetsAK8PFCHSSoftDropSubjets *

        process.patJetCorrFactorsAK8PFCHS *
        process.patJetsAK8PFCHS *
        process.selectedPatJetsAK8PFCHS *
        process.selectedPatJetsAK8PFCHSSoftDrop *
        process.selectedPatJetsAK8PFCHSSoftDropPacked
    )

    process.ca8chs = cms.Sequence(
        process.ca8PFJetsCHS *
        process.ca8PFJetsCHSConstituents *
        process.cmsTopTagPFJetsCHS *

        process.pfImpactParameterTagInfosCMSTopTagCHS *
        process.pfSecondaryVertexTagInfosCMSTopTagCHS *
        process.softPFElectronsTagInfosCMSTopTagCHS *
        process.softPFMuonsTagInfosCMSTopTagCHS *
        process.pfInclusiveSecondaryVertexFinderTagInfosCMSTopTagCHS *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsCMSTopTagCHS *
        process.pfCombinedMVAV2BJetTagsCMSTopTagCHS *
        process.pfJetProbabilityBJetTagsCMSTopTagCHS *
        process.pfImpactParameterAK8TagInfosCMSTopTagCHS *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosCMSTopTagCHS *
        process.pfBoostedDoubleSVAK8TagInfosCMSTopTagCHS *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsCMSTopTagCHS *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosCMSTopTagCHS *
        process.pfCombinedCvsLJetTagsCMSTopTagCHS *
        process.pfCombinedCvsBJetTagsCMSTopTagCHS *


        # process.ak8PFJetsPuppiPruned *
        # process.ak8PFJetsPuppiTrimmed *
        # process.ak8PFJetsPuppiFiltered *
        # process.ak8PFJetsPuppiFilteredMass *
        # process.ak8PFJetsPuppiTrimmedMass *
        # process.ak8PFJetsPuppiPrunedMass *
        # process.ak8PFJetsPuppiSoftDropMass *
        process.patJetCorrFactorsCMSTopTagCHS *
        process.patJetsCMSTopTagCHS *

        process.pfImpactParameterTagInfosCMSTopTagCHSSubjets *
        process.pfSecondaryVertexTagInfosCMSTopTagCHSSubjets *
        process.softPFElectronsTagInfosCMSTopTagCHSSubjets *
        process.softPFMuonsTagInfosCMSTopTagCHSSubjets *
        process.pfInclusiveSecondaryVertexFinderTagInfosCMSTopTagCHSSubjets *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsCMSTopTagCHSSubjets *
        process.pfCombinedMVAV2BJetTagsCMSTopTagCHSSubjets *
        process.pfJetProbabilityBJetTagsCMSTopTagCHSSubjets *
        process.pfImpactParameterAK8TagInfosCMSTopTagCHSSubjets *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosCMSTopTagCHSSubjets *
        process.pfBoostedDoubleSVAK8TagInfosCMSTopTagCHSSubjets *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsCMSTopTagCHSSubjets *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosCMSTopTagCHSSubjets *
        process.pfCombinedCvsLJetTagsCMSTopTagCHSSubjets *
        process.pfCombinedCvsBJetTagsCMSTopTagCHSSubjets *

        process.patJetCorrFactorsCMSTopTagCHSSubjets *
        process.patJetsCMSTopTagCHSSubjets *


        process.patJetsCMSTopTagCHSPacked
    )

    process.ak8puppi = cms.Sequence(
        process.ak8PFJetsPuppi *
        process.ak8PFJetsPuppiConstituents *
        process.pfImpactParameterTagInfosAK8PFPuppi *
        process.pfSecondaryVertexTagInfosAK8PFPuppi *
        process.softPFElectronsTagInfosAK8PFPuppi *
        process.softPFMuonsTagInfosAK8PFPuppi *

        process.pfInclusiveSecondaryVertexFinderTagInfosAK8PFPuppi *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK8PFPuppi *
        process.pfCombinedMVAV2BJetTagsAK8PFPuppi *
        process.pfJetProbabilityBJetTagsAK8PFPuppi *
        process.pfImpactParameterAK8TagInfosAK8PFPuppi *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK8PFPuppi *
        process.pfBoostedDoubleSVAK8TagInfosAK8PFPuppi *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK8PFPuppi *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK8PFPuppi *
        process.pfCombinedCvsLJetTagsAK8PFPuppi *
        process.pfCombinedCvsBJetTagsAK8PFPuppi *

        process.ak8PFJetsPuppiSoftDrop *
        process.patJetCorrFactorsAK8PFPuppiSoftDrop *
        process.patJetsAK8PFPuppiSoftDrop *
        process.selectedPatJetsAK8PFPuppiSoftDrop *
        process.patJetCorrFactorsAK8PFPuppi *

        process.pfImpactParameterTagInfosAK8PFPuppiSoftDropSubjets *
        process.pfSecondaryVertexTagInfosAK8PFPuppiSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderTagInfosAK8PFPuppiSoftDropSubjets *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsAK8PFPuppiSoftDropSubjets *
        process.softPFElectronsTagInfosAK8PFPuppiSoftDropSubjets *
        process.softPFMuonsTagInfosAK8PFPuppiSoftDropSubjets *
        process.pfCombinedMVAV2BJetTagsAK8PFPuppiSoftDropSubjets *
        process.pfJetProbabilityBJetTagsAK8PFPuppiSoftDropSubjets *
        process.pfImpactParameterAK8TagInfosAK8PFPuppiSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosAK8PFPuppiSoftDropSubjets *
        process.pfBoostedDoubleSVAK8TagInfosAK8PFPuppiSoftDropSubjets *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsAK8PFPuppiSoftDropSubjets *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosAK8PFPuppiSoftDropSubjets *
        process.pfCombinedCvsLJetTagsAK8PFPuppiSoftDropSubjets *
        process.pfCombinedCvsBJetTagsAK8PFPuppiSoftDropSubjets *
        process.NsubjettinessAK8PFPuppiSoftDropSubjets *

        process.patJetCorrFactorsAK8PFPuppiSoftDropSubjets *
        process.patJetsAK8PFPuppiSoftDropSubjets *
        process.selectedPatJetsAK8PFPuppiSoftDropSubjets *

        process.ak8PFJetsPuppiPruned *
        process.ak8PFJetsPuppiTrimmed *
        process.ak8PFJetsPuppiFiltered *
        process.ak8PFJetsPuppiFilteredMass *
        process.ak8PFJetsPuppiTrimmedMass *
        process.ak8PFJetsPuppiPrunedMass *
        process.ak8PFJetsPuppiSoftDropMass *

        process.NjettinessAK8Puppi *
        process.patJetsAK8PFPuppi *

        process.selectedPatJetsAK8PFPuppi*

        process.selectedPatJetsAK8PFPuppiSoftDropPacked
    )

    process.ca8puppi = cms.Sequence(
        process.ca8PFJetsPuppi *
        process.ca8PFJetsPuppiConstituents *
        process.cmsTopTagPFJetsPuppi *
        process.patJetCorrFactorsCMSTopTagPuppi *

        process.pfImpactParameterTagInfosCMSTopTagPuppi *
        process.pfSecondaryVertexTagInfosCMSTopTagPuppi *
        process.softPFElectronsTagInfosCMSTopTagPuppi *
        process.softPFMuonsTagInfosCMSTopTagPuppi *
        process.pfInclusiveSecondaryVertexFinderTagInfosCMSTopTagPuppi *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsCMSTopTagPuppi *
        process.pfCombinedMVAV2BJetTagsCMSTopTagPuppi *
        process.pfJetProbabilityBJetTagsCMSTopTagPuppi *
        process.pfImpactParameterAK8TagInfosCMSTopTagPuppi *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosCMSTopTagPuppi *
        process.pfBoostedDoubleSVAK8TagInfosCMSTopTagPuppi *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsCMSTopTagPuppi *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosCMSTopTagPuppi *
        process.pfCombinedCvsLJetTagsCMSTopTagPuppi *
        process.pfCombinedCvsBJetTagsCMSTopTagPuppi *

        process.CATopTagInfos *

        process.patJetsCMSTopTagPuppi *

        process.pfImpactParameterTagInfosCMSTopTagPuppiSubjets *
        process.pfSecondaryVertexTagInfosCMSTopTagPuppiSubjets *
        process.softPFElectronsTagInfosCMSTopTagPuppiSubjets *
        process.softPFMuonsTagInfosCMSTopTagPuppiSubjets *
        process.pfInclusiveSecondaryVertexFinderTagInfosCMSTopTagPuppiSubjets *
        process.pfCombinedInclusiveSecondaryVertexV2BJetTagsCMSTopTagPuppiSubjets *
        process.pfCombinedMVAV2BJetTagsCMSTopTagPuppiSubjets *
        process.pfJetProbabilityBJetTagsCMSTopTagPuppiSubjets *
        process.pfImpactParameterAK8TagInfosCMSTopTagPuppiSubjets *
        process.pfInclusiveSecondaryVertexFinderAK8TagInfosCMSTopTagPuppiSubjets *
        process.pfBoostedDoubleSVAK8TagInfosCMSTopTagPuppiSubjets *
        process.pfBoostedDoubleSecondaryVertexAK8BJetTagsCMSTopTagPuppiSubjets *
        process.pfInclusiveSecondaryVertexFinderCvsLTagInfosCMSTopTagPuppiSubjets *
        process.pfCombinedCvsLJetTagsCMSTopTagPuppiSubjets *
        process.pfCombinedCvsBJetTagsCMSTopTagPuppiSubjets *

        process.patJetCorrFactorsCMSTopTagPuppiSubjets *
        process.patJetsCMSTopTagPuppiSubjets *
        process.patJetsCMSTopTagPuppiPacked
    )

    return cms.Sequence(
        process.chs *
        process.puppi    *
        process.inclusiveCandidateVertexFinderCvsL *
        process.candidateVertexMergerCvsL *
        process.candidateVertexArbitratorCvsL *
        process.slimmedSecondaryVerticesCvsL *
        process.ak4puppi *
        process.ak8puppi *
        process.ca8puppi *
        process.ak4chs *
        process.ak8chs *
        process.ca8chs
    )
