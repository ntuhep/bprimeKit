/*******************************************************************************
 *
 *  Filename    : bprimeKit.h
 *  Description : The b' analysis kit
 *  Details     : Strips the standard CMSSW data format into a standalone 
 *                fBaseTree ntuple
 *  Authors     : May 31, 2015 - Split info mutifile format by Enoch.
 *                Jul 05, 2010 - Add Electron ID by Jacky, Add PFMet by Chiyi
 *                May 19, 2010 - Add Muon Iso variables, MCTag, and PF Jet  - by Dmitry Hits & Yeong-jyi Kei
 *                Apr 15, 2010 - Update to CMSSW_3_5_6, fix bTag bug  - by Yeong-jyi Lei
 *                Sep 24, 2009 - Updates label name, and triggerbook. Add fPhotonInfo - by Yeong-jyi Lei
 *                Sep 11, 2009 - Updates with CMSSW_3_1_2 - by Kai-Feng Chen
 *                May 29, 2009 - Include fGenInfo, electron classification, bug fix on vtx - by Yeong-jyi Lei
 *                Mar 10, 2009 - Change the trigger table
 *                Feb 20, 2009 - Protection added for missing InnerTrack() from muons, adding MC top mass, bug fix on fJetInfo.
 *                Jan 03, 2009 - Updates according to CMSSW_2_2_3 release (clean 2_2_3 + PhysicsTools/PatAlgos V04-14-15)
 *
*******************************************************************************/
#ifndef __BPRIMEKIT_H__
#define __BPRIMEKIT_H__

//----- ED Analyzer requirements  ----------------------------------------------
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

//----- CMS Software libraries for special variables  --------------------------
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h" 
#include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//----- Jet Energy corrections  ------------------------------------------------
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

//----- Custom classes  --------------------------------------------------------
#include "bpkFrameWork/bprimeKit/interface/Types.h"
#include "bpkFrameWork/bprimeKit/interface/format.h"
#include "bpkFrameWork/bprimeKit/interface/TriggerBooking.h"
#include <map>
#include <TTree.h>


//------------------------------------------------------------------------------ 
//   Global typedefs, enums and macros 
//------------------------------------------------------------------------------ 
#define MAX_LEPCOLLECTIONS 3
#define MAX_PHOCOLLECTIONS 3
#define MAX_JETCOLLECTIONS 3

//------------------------------------------------------------------------------ 
//   BprimeKit definitions 
//------------------------------------------------------------------------------ 
class bprimeKit : public edm::EDAnalyzer {
public:
   explicit bprimeKit( const edm::ParameterSet& ) ;
   ~bprimeKit();

private:
   //-------------------------------------------------------------------------------------------------- 
   //   Inheritance methods
   //-------------------------------------------------------------------------------------------------- 
   virtual void beginJob() ;
   virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
   virtual void endJob() ;
   virtual void beginRun( edm::Run const& iRun, edm::EventSetup const& iSetup );
   virtual void endRun( edm::Run const&, edm::EventSetup const& );

   //-------------------------------------------------------------------------------------------------- 
   //   Helper Methods
   //--------------------------------------------------------------------------------------------------
   //----- Ntuplization functions  ------------------------------------------------
   void InitTree();
   void ClearTree();
   void InitJetEnergyCorrectors();
   void ClearJetEnergyCorrector();
   void GetEdmObjects (const edm::Event&, const EventSetup& );
   bool FillVertex    ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillPhoton    ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillLepton    ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillMuon      ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool FillElectron  ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool FillTau       ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool FillJet       ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillLepPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillJetPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillfGenInfo   ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillEvent     ( const edm::Event&, const edm::EventSetup& ) ;
   bool FillGenGeneric();
   bool FillfPairInfo  ( const int , const int , math::XYZTLorentzVector& );
   bool FillPairGen   ( const reco::GenParticle* , const reco::GenParticle* );

   //----- Custom algorithms  -----------------------------------------------------
   bool IsSelectedMuon( const MuonIterator& );
   TLorentzVector CleanedJetP4( JetIterator, bool = false );

   //-------------------------------------------------------------------------------------------------- 
   //   Private data members
   //-------------------------------------------------------------------------------------------------- 

   //----- Ntuple interaction variables  --------------------------------------------------------------
   TTree*             fBaseTree                      ;
   EvtInfoBranches    fEvtInfo                       ;
   GenInfoBranches    fGenInfo                       ;
   LepInfoBranches    fLepInfo   [MAX_LEPCOLLECTIONS] ;
   PhotonInfoBranches fPhotonInfo[MAX_PHOCOLLECTIONS] ;
   JetInfoBranches    fJetInfo   [MAX_JETCOLLECTIONS] ;
   VertexInfoBranches fVertexInfo                     ;
   PairInfoBranches   fPairInfo                       ;
   
   //----- Event variable setup  ----------------------------------------------------------------------
   edm::InputTag  fMETLabel    ;
   edm::InputTag  fRhoLabel    ;
   edm::InputTag  fHLTLabel    ;
   edm::InputTag  fPileupLabel ;
   
   //----- Vertex variable setup  ---------------------------------------------------------------------
   edm::InputTag  fPrimaryVertexLabel            ;
   edm::InputTag  fPrimVertex_withBeamSpot_Label ;
   edm::InputTag  fBeamspotLabel                 ;
   
   //----- Generation variables setup  ----------------------------------------------------------------
   edm::InputTag  fGenEventLabel    ;
   edm::InputTag  fGenParticleLabel ;
   edm::InputTag  fGenDigiLabel     ;

   //----- Jet variable setup  ------------------------------------------------------------------------
   std::vector<std::string>                fJetCollections      ;
   std::vector<edm::InputTag>              fJetLabels           ;
   edm::EDGetTokenT<edm::ValueMap<float>>  fQGLikelihoodToken   ;
   edm::EDGetTokenT<edm::ValueMap<float>>  fQGAxis2Token        ;
   edm::EDGetTokenT<edm::ValueMap<int  >>  fQGMultiplicityToken ;
   edm::EDGetTokenT<edm::ValueMap<float>>  fQGPtDToken          ;

   //----- Photon variable setup  ---------------------------------------------------------------------
   std::vector<std::string>               fPhotonCollections                 ;
   std::vector<edm::InputTag>             fPhotonLabels                      ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fPhotonLooseIDToken                ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fPhotonMediumIDToken               ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fPhotonTightIDToken                ;
   edm::EDGetTokenT<edm::ValueMap<float>> fPhotonIsolation_Charged_Token     ;
   edm::EDGetTokenT<edm::ValueMap<float>> fPhotonIsolation_Neutral_Token     ;
   edm::EDGetTokenT<edm::ValueMap<float>> fPhotonIsolation_Photon_Token      ;
   edm::EDGetTokenT<edm::ValueMap<float>> fPhotonSignaIEtaIEtaToken          ;
   EffectiveAreas                         fPhotonEffectiveArea_ChargeHadron  ;
   EffectiveAreas                         fPhotonEffectiveArea_NeutralHadron ;
   EffectiveAreas                         fPhotonEffectiveArea_Photons       ;

   //----- Lepton variable setup  ---------------------------------------------------------------------
   std::vector<std::string>               fLeptonCollections     ;
   std::vector<edm::InputTag>             fMuonLabels            ;
   std::vector<edm::InputTag>             fElectronLabels        ;
   std::vector<edm::InputTag>             fTauLabels             ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fElectronIDVetoToken   ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fElectronIDLooseToken  ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fElectronIDMediumToken ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fElectronIDTightToken  ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  fElectronIDHEEPToken   ;
   edm::InputTag                          fConversionsTag        ;

   //------------------------------------------------------------------------------ 
   //   EDM Handles
   //------------------------------------------------------------------------------
   //----- Event Wide Handles  ----------------------------------------------------
   edm::Handle<double> fRho_H;
   METHandle           fMET_H;
   METHandle           fMETTempPlus_H;
   METHandle           fMETTempDown_H;
   PileupHandle        fPileup_H;
   TriggerHandle       fTrigger_H;
   BeamSpotHandle      fBeamSpot_H ;
   RecordHandle        fRecord_H;
  
   //----- fGenInfo Handles  -------------------------------------------------------
   GenHandle     fGenParticle_H;
   GenInfoHandle fGenEvent_H;
  
   //----- Vertex Handles  --------------------------------------------------------
   VertexHandle fVertex_H   ;
   VertexHandle fVertexWithBeamSpot_H ;
  
   //----- Jet related Handles  ---------------------------------------------------
	JetHandleList     fJetList_Hs;
   edm::Handle<edm::ValueMap<float>> fQGLikelihood_H;
   edm::Handle<edm::ValueMap<float>> fQGAxis2_H;
   edm::Handle<edm::ValueMap<int  >> fQGMultiplicity_H;
   edm::Handle<edm::ValueMap<float>> fQGPtD_H;

   //----- Lepton related Handles  ------------------------------------------------
   std::vector<MuonHandle>      fMuonList_Hs;
   std::vector<ElectronHandle>  fElectronList_Hs;
   std::vector<GsfHandle>       fGsfElectronList_Hs;
   std::vector<TauHandle>       fTauList_Hs;
   edm::Handle<reco::ConversionCollection> fConversions_H;
   edm::ESHandle<TransientTrackBuilder>    fTrackBuilder_H;
   edm::Handle<edm::ValueMap<bool>>  fElectronIDVeto_H   ;
   edm::Handle<edm::ValueMap<bool>>  fElectronIDLoose_H  ;
   edm::Handle<edm::ValueMap<bool>>  fElectronIDMedium_H ;
   edm::Handle<edm::ValueMap<bool>>  fElectronIDTight_H  ;
   edm::Handle<edm::ValueMap<bool>>  fElectronIDHEEP_H   ;

   //----- Photon related Handles  ------------------------------------------------
   std::vector<PhotonHandle>         fPhotonList_Hs;
   edm::Handle<edm::ValueMap<bool>>  fPhotonIDLoose;
   edm::Handle<edm::ValueMap<bool>>  fPhotonIDMedium;
   edm::Handle<edm::ValueMap<bool>>  fPhotonIDTight;
   edm::Handle<edm::ValueMap<float>> fPhotonIsolation_Charged_H;
   edm::Handle<edm::ValueMap<float>> fPhotonIsolation_Neutral_H;
   edm::Handle<edm::ValueMap<float>> fPhotonIsolation_Photon_H;
   edm::Handle<edm::ValueMap<float>> fPhotonSigmaIEtaIEta_H;
   
   reco::Vertex                         fPrimaryVertex        ;
   reco::Vertex                         fPrimaryVertex_BS     ;
   reco::BeamSpot                       fBeamSpot       ;
   map<std::string,int>                 fHighLevelTriggerMap;
   map<std::string,int>::iterator       fHighLevelTriggerMap_pr;
   HLTConfigProvider                    fHighLevelTriggerConfig;

   //----- Helper variables for muon-jet cleaning  --------------------------------
   std::vector<edm::Ptr<pat::Muon>>  fMySelectedMuons          ;
   FactorizedJetCorrector*        fJetCorrector             ;
   FactorizedJetCorrector*        fJetCorrectorAK8          ;
	JetCorrectionUncertainty*      fJetCorrectionUncertainty ;
   
   //----- Configuration flags  -----------------------------------------------------------------------
   int  fPairCollectionType ;
   bool fSkipfGenInfo        ;
   bool fIncludeL7          ;
   int  fDebug              ;
   bool fRunOnB2G           ;
   bool fIsData             ;
   bool fRunMuonJetCleaning ;
};

#endif /* end of include guard: __BPRIMEKIT_H__ */
