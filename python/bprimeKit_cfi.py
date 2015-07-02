import FWCore.ParameterSet.Config as  cms
from MyAna.bprimeKit.ObjectParameters_cfi import *

bprimeKit = cms.EDAnalyzer(
   "bprimeKit",
   MCtag                     = cms.untracked.bool(False),
   reducedEBRecHitCollection = cms.InputTag("reducedEgamma","reducedEBRecHits"),
   reducedEERecHitCollection = cms.InputTag("reducedEgamma","reducedEERecHits"),
   rhoLabel                  = cms.InputTag("fixedGridRhoFastjetAll"), 
   #------------------------------  Photon Information  -------------------------------
   PhoCollections            = cms.vstring('PhotonInfo'),
   pholabel                  = cms.VInputTag('slimmedPhotons'),
   phoLooseIdMap             = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose"     ) ,
   phoMediumIdMap            = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium"    ) ,
   phoTightIdMap             = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight"     ) ,
   phoMVAValuesMap           = cms.InputTag( "photonMVAValueMapProducer:PhotonMVAEstimatorRun2Spring15NonTrigValues" ) ,
   phoChargedIsolation       = cms.InputTag( "photonIDValueMapProducer:phoChargedIsolation"                          ) ,
   phoNeutralHadronIsolation = cms.InputTag( "photonIDValueMapProducer:phoNeutralHadronIsolation"                    ) ,
   phoPhotonIsolation        = cms.InputTag( "photonIDValueMapProducer:phoPhotonIsolation"                           ) ,
   #------------------------------  Lepton Information  -------------------------------
   LepCollections  = cms.vstring('LepInfo'),
   muonlabel       = cms.VInputTag('slimmedMuons'     ) ,
   eleclabel       = cms.VInputTag('slimmedElectrons' ) ,
   taulabel        = cms.VInputTag('slimmedTaus'      ) ,
   eleVetoIdMap    = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-veto"   ) ,
   eleLooseIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose"  ) ,
   eleMediumIdMap  = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium" ) ,
   eleTightIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight"  ) ,
   eleHEEPIdMap    = cms.InputTag( "egmGsfElectronIDs:heepElectronID-HEEPV51"                                  ) ,
   eleMVAValuesMap = cms.InputTag( "electronMVAValueMapProducer:ElectronMVAEstimatorRun2Phys14NonTrigValues"   ) ,

   #--------------------------------  Jet information  --------------------------------      
   jetlabel                  = cms.VInputTag('slimmedJets' ), ##Requires adding 'selectedPatJetsAK8PFCHSPrunedPacked','patJetsCMSTopTagCHSPacked'
   JetCollections            = cms.vstring('JetInfo' ) ,      ##Requires adding 'AK8BosonJetInfo','CA8TopJetInfo'
   JetType                   = cms.vint32(0,2,3),

   PairCollection            = cms.untracked.int32(1),
   metlabel                  = cms.VInputTag("slimmedMETs"),
   pfmetlabel                = cms.VInputTag("slimmedMETs"),	## No PF MET for this moment in CMSSW73X
   genlabel                  = cms.VInputTag("prunedGenParticles"),
   hltlabel                  = cms.VInputTag("TriggerResults::HLT"),
   pathltlabel               = cms.VInputTag("patTriggerEvent"),
   offlinePVlabel            = cms.VInputTag("offlineSlimmedPrimaryVertices"),#CMSSW73X "offlinePrimaryVertices"),
   offlinePVBSlabel          = cms.VInputTag("offlineSlimmedPrimaryVertices"),# CMSSW73X"offlinePrimaryVerticesWithBS"),
   offlineBSlabel            = cms.VInputTag("offlineBeamSpot"),
   #pixelvtxlabel            = cms.VInputTag("pixelVertices"), #CMSSW73X
   tracklabel                = cms.VInputTag("generalTracks"),# not used in CMSSW73X "generalTracks"),
   pfCands                   = cms.InputTag("packedPFCandidates"),
   dcslabel                  = cms.VInputTag("scalersRawToDigi"),
   genevtlabel               = cms.VInputTag("generator"),
   gtdigilabel               = cms.VInputTag("gtDigis"),
   rhocorrectionlabel        = cms.VInputTag("kt6PFJetsForIsolation:rho","kt6PFJetsForIsolation:rho"),  #not used in 73x [electron,muon]
   sigmaLabel                = cms.VInputTag("kt6PFJetsForIsolation:sigma","kt6PFJetsForIsolation:rho"),    #not used in 73x [electron,muon]
   puInfoLabel               = cms.VInputTag("addPileupInfo"),
   #conversionsInputTag      = cms.InputTag("allConversions"),
   conversionsInputTag       = cms.InputTag("reducedEgamma","reducedConversions"),
   rhoIsoInputTag            = cms.InputTag("kt6PFJetsForIsolation", "rho"),	# not used in 73x
   
   isoValInputTags = cms.VInputTag( 
      cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
      cms.InputTag('elPFIsoValueGamma03PFIdPFIso'  ),
      cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')
   ),

   EIDMVAInputTags = cms.vstring(
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat1.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat2.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat3.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat4.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat5.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat6.weights.xml' ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat1.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat2.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat3.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat4.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat5.weights.xml'    ,
      'dataEIDMVA/Electrons_BDTG_TrigV0_Cat6.weights.xml'
   ),

   IncludeL7           = cms.untracked.bool(False),
   SelectionParameters = defaultObjectParameters.clone(),
   Debug               = cms.untracked.int32(100),
)

