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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

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
typedef std::vector<edm::Handle<edm::ValueMap<reco::IsoDeposit>>>   IsoDepositMaps;
typedef std::vector<edm::Handle<edm::ValueMap<double>>>             IsoDepositVals;

typedef std::vector<pat::Electron>     ElectronList        ;
typedef edm::Handle<ElectronList>      ElectronHandler     ;
typedef ElectronList::const_iterator   ElectronIterator    ;
typedef edm::View<reco::GsfElectron>   GsfList             ;
typedef edm::Handle<GsfList>           GsfHandler          ;
typedef GsfList::const_iterator        GsfIterator         ;

typedef std::vector<pat::Muon>         MuonList;
typedef edm::Handle<MuonList>          MuonHandler;
typedef std::vector<MuonHandler>       MuonHandlerList;
typedef MuonList::const_iterator       MuonIterator ;

typedef std::vector<pat::Tau>          TauList;
typedef edm::Handle<TauList>           TauHandler;
typedef std::vector<TauHandler>        TauHandlerList;
typedef TauList::const_iterator        TauIterator;

typedef std::vector<pat::Photon>       PhotonList; 
typedef edm::Handle<PhotonList>        PhotonHandle;
typedef std::vector<PhotonHandle>      PhotonHandleList;
typedef PhotonList::const_iterator     PhotonIterator ;

typedef std::vector<pat::Jet>          JetList;
typedef edm::Handle<JetList>           JetHandler;
typedef std::vector<JetHandler>        JetHandlerList;
typedef JetList::const_iterator        JetIterator;

typedef edm::Handle<reco::TrackCollection>          TrackHandler        ;
typedef edm::Handle<reco::GsfElectronCollection>    GsfElectronHandle   ;
typedef edm::Handle<reco::ConversionCollection>     ConversionHandle    ;
typedef edm::Handle<DcsStatusCollection>            DcsStatusHandle     ;

typedef std::vector<reco::GenParticle>::const_iterator GenIterator ;

typedef std::vector<reco::Vertex>  VertexList;
typedef VertexList::const_iterator VertexListCIT;


#define MAX_LEPCOLLECTIONS 3
#define MAX_PHOCOLLECTIONS 3
#define MAX_JETCOLLECTIONS 3
#define TurnOffInCMSSW73x true
#define TurnOnInCMSSW_7_4_1 false
#define W_MASS          80.403
#define Z_MASS          91.1876
#define MUON_MASS       0.105658
#define ELECTRON_MASS   0.0005109989


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
   bool fillPairInfo  ( int , int );
   bool fillPairGen   ();

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

   //----------------------------  Photon isolation tokens  ----------------------------
   edm::EDGetTokenT<edm::ValueMap<bool> >  phoLooseIdMapToken_             ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  phoMediumIdMapToken_            ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  phoTightIdMapToken_             ;
   edm::EDGetTokenT<edm::ValueMap<float> > phoMVAValuesMapToken_           ;
   edm::EDGetTokenT<edm::ValueMap<float> > phoChargedIsolationToken_       ;
   edm::EDGetTokenT<edm::ValueMap<float> > phoNeutralHadronIsolationToken_ ;
   edm::EDGetTokenT<edm::ValueMap<float> > phoPhotonIsolationToken_        ;
   edm::EDGetTokenT<edm::ValueMap<float> > phoWorstChargedIsolationToken_  ;

   //---------------------------  Electron Isolation tokens  ---------------------------
   edm::EDGetTokenT<edm::ValueMap<bool> >  eleVetoIdMapToken_    ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  eleLooseIdMapToken_   ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  eleMediumIdMapToken_  ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  eleTightIdMapToken_   ;
   edm::EDGetTokenT<edm::ValueMap<bool> >  eleHEEPIdMapToken_    ;
   edm::EDGetTokenT<edm::ValueMap<float> > eleMVAValuesMapToken_ ;
   
   edm::EDGetTokenT<double>               rhoLabel_;
   //----------------  Information type independent helper variables  -----------------
   edm::Handle<reco::GenParticleCollection>     GenHandle;
   edm::Handle<edm::View<reco::Track> >         TrackHandle; //Dmitry
   std::vector<edm::Handle<double> >            sigmaHandle;
   TauHandlerList       TauHandle;
   TrackHandler          tracks_h       ; // Jacky
   GsfElectronHandle    els_h          ;
   ConversionHandle     conversions_h  ;
   DcsStatusHandle      dcsHandle      ; // Jacky
   edm::Handle<double> rhoIso_h;
   double rhoIso;
   double evt_bField;

   //--------------------------------  Vertex related  --------------------------------- 
   double                               Signal_Vz      ;
   reco::Vertex                         PrimVtx        ;
   reco::Vertex                         PrimVtx_BS     ;
   reco::BeamSpot                       beamSpot       ;
   edm::Handle<reco::BeamSpot>          beamSpotHandle ;
   edm::Handle<reco::VertexCollection>  VertexHandle   ;
   edm::Handle<reco::VertexCollection>  VertexHandleBS ; //Dmitry

   std::vector<edm::Handle<double> > rhoH;
  
   //----------------------------------  Electron ID  ----------------------------------
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
};

#endif /* end of include guard: __BPRIMEKIT_H__ */
