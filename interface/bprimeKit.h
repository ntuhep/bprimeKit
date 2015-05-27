
#ifndef __BPRIMEKIT_H__
#define __BPRIMEKIT_H__

// ROOT Libraries 
#include "TTree.h"

// CMSSW libraries 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// Custom Classes 
#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/TriggerBooking.h"
#include "MyAna/bprimeKit/interface/objectSelector.h"

// Standard template library
#include <map>

#define MAX_LEPCOLLECTIONS 3
#define MAX_PHOCOLLECTIONS 3
#define MAX_JETCOLLECTIONS 3

class bprimeKit : public edm::EDAnalyzer {
public:
   explicit bprimeKit( const edm::ParameterSet& ) ;
   ~bprimeKit();

private:
   virtual void beginJob() ;
   virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
   virtual void endJob() ;
   /// Private methods 
   void fillPhoton( const edm::Event&, const edm::EventSetup& ) ;

   virtual void beginRun( edm::Run const& iRun, edm::EventSetup const& iSetup );
   virtual void endRun( edm::Run const&, edm::EventSetup const& );


   TTree* root;
   EvtInfoBranches EvtInfo;
   GenInfoBranches GenInfo;
   LepInfoBranches LepInfo[MAX_LEPCOLLECTIONS];
   PhotonInfoBranches PhotonInfo[MAX_PHOCOLLECTIONS];
   VertexInfoBranches VertexInfo;
   JetInfoBranches JetInfo[MAX_JETCOLLECTIONS];
   PairInfoBranches PairInfo;
   
   std::vector<edm::InputTag> muonlabel_;
   std::vector<edm::InputTag> eleclabel_;
   std::vector<edm::InputTag> taulabel_;
   std::vector<edm::InputTag> pholabel_;
   std::vector<edm::InputTag> jetlabel_;
   std::vector<edm::InputTag> metlabel_;
   std::vector<edm::InputTag> pfmetlabel_;
   std::vector<edm::InputTag> pfjetlabel_;
   std::vector<edm::InputTag> genlabel_;
   std::vector<edm::InputTag> hltlabel_;
   std::vector<edm::InputTag> pathltlabel_;
   std::vector<edm::InputTag> offlinePVlabel_;
   std::vector<edm::InputTag> offlinePVBSlabel_;
   std::vector<edm::InputTag> offlineBSlabel_;
   std::vector<edm::InputTag> tracklabel_;
   std::vector<edm::InputTag> dcslabel_;
   std::vector<edm::InputTag> genevtlabel_;
   std::vector<edm::InputTag> gtdigilabel_;
   std::vector<edm::InputTag> rhocorrectionlabel_;
   std::vector<edm::InputTag> sigmaLabel_;
   std::vector<edm::InputTag> puInfoLabel_;

   // Add 2012 EID simple-cut-based
   edm::InputTag               conversionsInputTag_;
   edm::InputTag               rhoIsoInputTag;
   std::vector<edm::InputTag>  isoValInputTags_;
   PFIsolationEstimator isolatorR03;
   PFIsolationEstimator isolatorR04;
   PFIsolationEstimator PhotonisolatorR03;
   PFIsolationEstimator PhotonisolatorR04;

   EGammaMvaEleEstimator* myMVANonTrig;
   EGammaMvaEleEstimator* myMVATrig;
   std::vector<std::string> EIDMVAInputTags_;

   // update for CMSSW_7_2_0
   edm::EDGetTokenT<EcalRecHitCollection> reducedEBRecHitCollectionToken_;
   edm::EDGetTokenT<EcalRecHitCollection> reducedEERecHitCollectionToken_;

   edm::EDGetTokenT<pat::PackedCandidateCollection> pfToken_;

   std::vector<std::string> lepcollections_;
   std::vector<std::string> phocollections_;
   std::vector<std::string> jetcollections_;
   std::vector<int> jettype_;//true for pf jets, false for calo

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
   edm::Handle<reco::VertexCollection>          VertexHandle;
   PFCandidateCollection thePfColl;

};

// constants, enums and typedefs for 2012 EID
typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;



#endif /* end of include guard: __BPRIMEKIT_H__ */
