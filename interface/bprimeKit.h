/*******************************************************************************
 *
 *  Filename    : bprimeKit.h
 *
*******************************************************************************/

#ifndef __BPRIMEKIT_H__
#define __BPRIMEKIT_H__


#ifndef __BPRIMEKIT__
#define __BPRIMEKIT__
#endif

//------------------------------------------------------------------------------ 
//   Libraries 
//------------------------------------------------------------------------------ 

//--------------------------  Standard Template Libraries  --------------------------
#include <map>

//--------------------------------  ROOT Libraries  ---------------------------------
#include "TTree.h"

//----------------------------  CMS Software Libraries  -----------------------------
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//--------------------------------  Custom Classes  ---------------------------------
#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/TriggerBooking.h"
#include "MyAna/bprimeKit/interface/objectSelector.h"



//------------------------------------------------------------------------------ 
//   Global typedefs, enums and macros 
//------------------------------------------------------------------------------ 
typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;
#define MAX_LEPCOLLECTIONS 3
#define MAX_PHOCOLLECTIONS 3
#define MAX_JETCOLLECTIONS 3
#define TurnOffInCMSSW73x true
#define TurnOnInCMSSW_7_4_1 false


//------------------------------------------------------------------------------ 
//   BprimeKit definitions 
//------------------------------------------------------------------------------ 
class bprimeKit : public edm::EDAnalyzer {
public:
   explicit bprimeKit( const edm::ParameterSet& ) ;
   ~bprimeKit();

private:
   //--------------------------  Methods required by plugin  ---------------------------
   virtual void beginJob() ;
   virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
   virtual void endJob() ;
   virtual void beginRun( edm::Run const& iRun, edm::EventSetup const& iSetup );
   virtual void endRun( edm::Run const&, edm::EventSetup const& );

   //-----------------------------  Custom helper methods  -----------------------------
   bool fillVertex    ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillPhoton    ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillMuon      ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillElectron  ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillTau       ( const edm::Event&, const edm::EventSetup& , const size_t ) ;
   bool fillJet       ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillLepPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillJetPair   ( const edm::Event&, const edm::EventSetup& ) ;
   bool fillPairInfo  ( int , int );
   bool fillPairGen   ();
   bool fillGenInfo   ( const edm::Event& , const edm::EventSetup& ) ;
   bool fiilGenGeneric();
 
   //-------------------------  NTuple interaction variables  --------------------------
   TTree*                   root                           ;
   EvtInfoBranches          EvtInfo                        ;
   GenInfoBranches          GenInfo                        ;
   LepInfoBranches          LepInfo   [MAX_LEPCOLLECTIONS] ;
   PhotonInfoBranches       PhotonInfo[MAX_PHOCOLLECTIONS] ;
   JetInfoBranches          JetInfo   [MAX_JETCOLLECTIONS] ;
   VertexInfoBranches       VertexInfo                     ;
   PairInfoBranches         PairInfo                       ;
   std::vector<std::string> lepcollections_                ;
   std::vector<std::string> phocollections_                ;
   std::vector<std::string> jetcollections_                ;
   std::vector<int>         jettype_                       ; //true for pf jets, false for calo
   
   //-------------------------  Plugin interaction variables  --------------------------
   std::vector<edm::InputTag>  muonlabel_           ;
   std::vector<edm::InputTag>  eleclabel_           ;
   std::vector<edm::InputTag>  taulabel_            ;
   std::vector<edm::InputTag>  pholabel_            ;
   std::vector<edm::InputTag>  jetlabel_            ;
   std::vector<edm::InputTag>  metlabel_            ;
   std::vector<edm::InputTag>  pfmetlabel_          ;
   std::vector<edm::InputTag>  pfjetlabel_          ;
   std::vector<edm::InputTag>  genlabel_            ;
   std::vector<edm::InputTag>  hltlabel_            ;
   std::vector<edm::InputTag>  pathltlabel_         ;
   std::vector<edm::InputTag>  offlinePVlabel_      ;
   std::vector<edm::InputTag>  offlinePVBSlabel_    ;
   std::vector<edm::InputTag>  offlineBSlabel_      ;
   std::vector<edm::InputTag>  tracklabel_          ;
   std::vector<edm::InputTag>  dcslabel_            ;
   std::vector<edm::InputTag>  genevtlabel_         ;
   std::vector<edm::InputTag>  gtdigilabel_         ;
   std::vector<edm::InputTag>  rhocorrectionlabel_  ;
   std::vector<edm::InputTag>  sigmaLabel_          ;
   std::vector<edm::InputTag>  puInfoLabel_         ;
   edm::InputTag               conversionsInputTag_ ;
   edm::InputTag               rhoIsoInputTag       ;
   std::vector<edm::InputTag>  isoValInputTags_     ;


   //----------------  Information type independent helper variables  -----------------
   edm::Handle<reco::GenParticleCollection>     GenHandle;

   //--------------------------------  Vertex related  --------------------------------- 
   double                               Signal_Vz      ;
   reco::Vertex                         PrimVtx        ;
   reco::Vertex                         PrimVtx_BS     ;
   reco::BeamSpot                       beamSpot       ;
   edm::Handle<reco::BeamSpot>          beamSpotHandle ;
   edm::Handle<reco::VertexCollection>  VertexHandle   ;
   edm::Handle<reco::VertexCollection>  VertexHandleBS ; //Dmitry

   std::vector<edm::Handle<double> > rhoH;
  
   //----------------------------  Electron ID, added 2012  ----------------------------
   PFIsolationEstimator isolatorR03       ;
   PFIsolationEstimator isolatorR04       ;
   PFIsolationEstimator PhotonisolatorR03 ;
   PFIsolationEstimator PhotonisolatorR04 ;

   EGammaMvaEleEstimator* myMVANonTrig;
   EGammaMvaEleEstimator* myMVATrig;
   std::vector<std::string> EIDMVAInputTags_;

   // update for CMSSW_7_2_0
   edm::EDGetTokenT<EcalRecHitCollection> reducedEBRecHitCollectionToken_;
   edm::EDGetTokenT<EcalRecHitCollection> reducedEERecHitCollectionToken_;

   edm::EDGetTokenT<pat::PackedCandidateCollection> pfToken_;
   
   
   int pairColl_;//which lepton collection to use for pairs
   bool getElectronID_;
   bool skipGenInfo_;
   bool includeL7_;

   edm::ParameterSet SelectionParameters_;

   objectSelector* Selector_;

   int debug_;
   map < std::string, int > HLTmaplist;
   map < std::string, int >::iterator HLTmaplist_pr;
   HLTConfigProvider hltConfig_;

   bool isData;
   
   PFCandidateCollection thePfColl;
};

#endif /* end of include guard: __BPRIMEKIT_H__ */
