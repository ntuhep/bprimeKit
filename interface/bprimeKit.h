/*******************************************************************************
 *
 *  Filename    : bprimeKit.h
 *  Description : The b' analysis kit
 *  Details     : Strips the standard CMSSW data format into a standalone 
 *                ROOT ntuple
 *  Authors     : May 31, 2015 - Split info mutifile format by Enoch.
 *                Jul 05, 2010 - Add Electron ID by Jacky, Add PFMet by Chiyi
 *                May 19, 2010 - Add Muon Iso variables, MCTag, and PF Jet  - by Dmitry Hits & Yeong-jyi Kei
 *                Apr 15, 2010 - Update to CMSSW_3_5_6, fix bTag bug  - by Yeong-jyi Lei
 *                Sep 24, 2009 - Updates label name, and triggerbook. Add PhotonInfo - by Yeong-jyi Lei
 *                Sep 11, 2009 - Updates with CMSSW_3_1_2 - by Kai-Feng Chen
 *                May 29, 2009 - Include GenInfo, electron classification, bug fix on vtx - by Yeong-jyi Lei
 *                Mar 10, 2009 - Change the trigger table
 *                Feb 20, 2009 - Protection added for missing InnerTrack() from muons, adding MC top mass, bug fix on JetInfo.
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
#include "MyAna/bprimeKit/interface/Types.h"
#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/TriggerBooking.h"
#include <map>
#include "TTree.h"


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
   void initTree();
   void clearTree();
   void initJetEnergyCorrectors();
   void clearJetEnergyCorrector();
   void GetEdmObjects(const edm::Event&, const EventSetup& );
   bool fillVertex   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillPhoton    ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillLepton    ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillMuon      ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillElectron  ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillTau       ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillJet       ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillLepPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillJetPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillGenInfo   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillEvent     ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillGenGeneric();
   bool fillPairInfo  ( const int , const int , math::XYZTLorentzVector& );
   bool fillPairGen   ( const reco::GenParticle* , const reco::GenParticle* );

   bool isSelectedMuon( const MuonIterator& ); 
   bool passMuonJetClean( JetIterator );
   TLorentzVector correctJet(const pat::Jet&,  bool = false);

   //-------------------------------------------------------------------------------------------------- 
   //   Private data members
   //-------------------------------------------------------------------------------------------------- 

   //----- Ntuple interaction variables  --------------------------------------------------------------
   TTree*                   root                           ;
   EvtInfoBranches          EvtInfo                        ;
   GenInfoBranches          GenInfo                        ;
   LepInfoBranches          LepInfo   [MAX_LEPCOLLECTIONS] ;
   PhotonInfoBranches       PhotonInfo[MAX_PHOCOLLECTIONS] ;
   JetInfoBranches          JetInfo   [MAX_JETCOLLECTIONS] ;
   VertexInfoBranches       VertexInfo                     ;
   PairInfoBranches         PairInfo                       ;
   
   //----- Event variable setup  ----------------------------------------------------------------------
   edm::InputTag  metLabel_    ;
   edm::InputTag  rhoLabel_    ;
   edm::InputTag  hltLabel_    ;
   edm::InputTag  puInfoLabel_ ;
   
   //----- Vertex variable setup  ---------------------------------------------------------------------
   edm::InputTag  offlinePVLabel_   ;
   edm::InputTag  offlinePVBSLabel_ ;
   edm::InputTag  offlineBSLabel_   ;
   
   //----- Generation variables setup  ----------------------------------------------------------------
   edm::InputTag  genevtLabel_         ;
   edm::InputTag  genLabel_            ;
   edm::InputTag  gtdigiLabel_         ;

   //----- Jet variable setup  ------------------------------------------------------------------------
   std::vector<std::string>                jetcollections_ ;
   std::vector<edm::InputTag>              jetLabel_       ;
   edm::EDGetTokenT<edm::ValueMap<float>>  qgLikelihoodToken_        ;
   edm::EDGetTokenT<edm::ValueMap<float>>  qgaxis2Token_ ;
   edm::EDGetTokenT<edm::ValueMap<int  >>  qgmultToken_  ;
   edm::EDGetTokenT<edm::ValueMap<float>>  qgptDToken_   ;
   JetCorrectorParameters*    L3JetPar_;
   JetCorrectorParameters*    L2JetPar_;
   JetCorrectorParameters*    L1JetPar_;
   JetCorrectorParameters*    L3JetParAK8_;
   JetCorrectorParameters*    L2JetParAK8_;
   JetCorrectorParameters*    L1JetParAK8_;
	JetCorrectionUncertainty*  jecUnc_;

   //----- Photon variable setup  ---------------------------------------------------------------------
   std::vector<std::string>               phocollections_                 ;
   std::vector<edm::InputTag>             phoLabel_                       ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  phoLooseIdMapToken_             ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  phoMediumIdMapToken_            ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  phoTightIdMapToken_             ;
   edm::EDGetTokenT<edm::ValueMap<float>> phoChargedIsolationToken_       ;
   edm::EDGetTokenT<edm::ValueMap<float>> phoNeutralHadronIsolationToken_ ;
   edm::EDGetTokenT<edm::ValueMap<float>> phoPhotonIsolationToken_        ;
   edm::EDGetTokenT<edm::ValueMap<float>> full5x5SigmaIEtaIEtaMapToken_   ;
   EffectiveAreas effAreaChHadrons_                                       ;
   EffectiveAreas effAreaNeuHadrons_                                      ;
   EffectiveAreas effAreaPhotons_                                         ;

   //----- Lepton variable setup  ---------------------------------------------------------------------
   std::vector<std::string>               lepcollections_       ;
   std::vector<edm::InputTag>             muonLabel_            ;
   std::vector<edm::InputTag>             elecLabel_            ;
   std::vector<edm::InputTag>             tauLabel_             ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  eleVetoIdMapToken_    ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  eleLooseIdMapToken_   ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  eleMediumIdMapToken_  ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  eleTightIdMapToken_   ;
   edm::EDGetTokenT<edm::ValueMap<bool>>  eleHEEPIdMapToken_    ;
   edm::EDGetTokenT<edm::ValueMap<float>> eleMVAValuesMapToken_ ;
   edm::InputTag                          conversionsInputTag_;

   //------------------------------------------------------------------------------ 
   //   Edm Handles
   //------------------------------------------------------------------------------
   //----- Event Wide Handles  ----------------------------------------------------
   edm::Handle<double> _rhoHandle;
   METHandle           _metHandle;
   METHandle           _metHandle_TempPlus;
   METHandle           _metHandle_TempDown;
   PileupHandle        _pileupHandle;
   TriggerHandle       _triggerHandle;
   BeamSpotHandle      _beamSpotHandle ;
   RecordHandle        _gtRecord;
  
   //----- GenInfo Handles  -------------------------------------------------------
   GenHandle           _genHandle;
   GenInfoHandle       _genInfoHandle;
  
   //----- Vertex Handles  --------------------------------------------------------
   VertexHandle        _vertexHandle   ;
   VertexHandle        _bsVertexHandle ;
  
   //----- Jet related Handles  ---------------------------------------------------
	JetHandleList     _jetHandleList;
   edm::Handle<edm::ValueMap<float>> qgLikelihoodHandle;
   edm::Handle<edm::ValueMap<float>> qgaxis2Handle;
   edm::Handle<edm::ValueMap<int  >> qgmultHandle;
   edm::Handle<edm::ValueMap<float>> qgptDHandle;

   //----- Lepton related Handles  ------------------------------------------------
   std::vector<MuonHandle>        _muonHandleList;
   std::vector<ElectronHandle>    _elecHandleList;
   std::vector<GsfHandle>         _gsfHandleList;
   std::vector<TauHandle>         _tauHandleList;
   edm::Handle<reco::ConversionCollection> conversions_h;
   edm::Handle<edm::ValueMap<bool>>  veto_id_decisions   ;
   edm::Handle<edm::ValueMap<bool>>  loose_id_decisions  ;
   edm::Handle<edm::ValueMap<bool>>  medium_id_decisions ;
   edm::Handle<edm::ValueMap<bool>>  tight_id_decisions  ;
   edm::Handle<edm::ValueMap<bool>>  heep_id_decisions   ;

   //----- Photon related Handles  ------------------------------------------------
   std::vector<PhotonHandle>         _photonHandleList;
   edm::Handle<edm::ValueMap<bool>>  _photonLooseID;
   edm::Handle<edm::ValueMap<bool>>  _photonMediumID;
   edm::Handle<edm::ValueMap<bool>>  _photonTightID;
   edm::Handle<edm::ValueMap<float>> phoChargedIsolationMap;
   edm::Handle<edm::ValueMap<float>> phoNeutralHadronIsolationMap;
   edm::Handle<edm::ValueMap<float>> phoPhotonIsolationMap;
   edm::Handle<edm::ValueMap<float>> full5x5SigmaIEtaIEtaMap;
   
   reco::Vertex                         PrimVtx        ;
   reco::Vertex                         PrimVtx_BS     ;
   reco::BeamSpot                       beamSpot       ;
   const TransientTrackBuilder*         transientTrackBuilder;
   map<std::string,int>                 HLTmaplist;
   map<std::string,int>::iterator       HLTmaplist_pr;
   HLTConfigProvider                    hltConfig_;

   //----- Helper variables for muon-jet cleaning  --------------------------------
   std::vector<const pat::Muon*>  _mySelecMuons;
   FactorizedJetCorrector* JetCorrector;
   FactorizedJetCorrector* JetCorrectorAK8;
   
   //----- Configuration flags  -----------------------------------------------------------------------
   int  pairColl_      ;
   bool skipGenInfo_   ;
   bool includeL7_     ;
   int  debug_         ;
   bool runOnB2G       ;
   bool isData         ;
   bool runMuonJetClean;

   EGammaMvaEleEstimator* myMVATrig;
   std::vector<std::string> EIDMVAInputTags_;
};

#endif /* end of include guard: __BPRIMEKIT_H__ */
