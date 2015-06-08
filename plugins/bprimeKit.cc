/*******************************************************************************

 *  Filename    : bprimeKit.cc
 *  Description : The virtual and explicit functions for the bprimeKit
 *
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit.h"

//---------------------  ROOT and standard template libraries  ----------------------
#include <string>
#include <TFile.h>
#include <TTree.h>

//---------------------------  Specific CMSSW libraries  ----------------------------
#include "FWCore/Framework/interface/MakerMacros.h"          // For plugin definition
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"


//------------------------------------------------------------------------------ 
//   Custom enums, typedefs and macros
//------------------------------------------------------------------------------ 
//#define FILL_DIJET_PAIRS 1      //Uncomment for jet pair processing
using namespace reco;
using namespace pat;
typedef std::vector<edm::InputTag> TagList;
typedef std::vector<std::string>   StrList;

//------------------------------------------------------------------------------ 
//   Helper variables
//------------------------------------------------------------------------------ 
edm::Service<TFileService> fs;
TFileDirectory results ;


//------------------------------------------------------------------------------ 
//   bprimeKit methods: constructor and destructor
//------------------------------------------------------------------------------ 
bprimeKit::bprimeKit( const edm::ParameterSet& iConfig )
{
   results = TFileDirectory( fs->mkdir( "results" ) );
   muonlabel_          = iConfig.getParameter<TagList>( "muonlabel"          ) ; //"cleanPatMuons"
   eleclabel_          = iConfig.getParameter<TagList>( "eleclabel"          ) ; // "cleanPatElectrons"
   taulabel_           = iConfig.getParameter<TagList>( "taulabel"           ) ; // "selectedPatTausPFlow"
   pholabel_           = iConfig.getParameter<TagList>( "pholabel"           ) ;
// jetlabel_           = iConfig.getParameter<TagList>( "jetlabel"           ) ; // "cleanPatJets"
   metlabel_           = iConfig.getParameter<TagList>( "metlabel"           ) ; //"patMETs"
   pfmetlabel_         = iConfig.getParameter<TagList>( "pfmetlabel"         ) ; //"pfpatMETs"
   genlabel_           = iConfig.getParameter<TagList>( "genlabel"           ) ; // "genParticles"
   hltlabel_           = iConfig.getParameter<TagList>( "hltlabel"           ) ; // "TriggerResults::HLT"
   pathltlabel_        = iConfig.getParameter<TagList>( "pathltlabel"        ) ; // patTriggerEvent
   offlinePVlabel_     = iConfig.getParameter<TagList>( "offlinePVlabel"     ) ; //offlinePrimaryVertices
   offlinePVBSlabel_   = iConfig.getParameter<TagList>( "offlinePVBSlabel"   ) ; //offlinePrimaryVerticesWithBS
   offlineBSlabel_     = iConfig.getParameter<TagList>( "offlineBSlabel"     ) ; //offlineBeamSpot
   tracklabel_         = iConfig.getParameter<TagList>( "tracklabel"         ) ; //generalTracks
   dcslabel_           = iConfig.getParameter<TagList>( "dcslabel"           ) ; //scalersRawToDigi
   genevtlabel_        = iConfig.getParameter<TagList>( "genevtlabel"        ) ; //generator
   gtdigilabel_        = iConfig.getParameter<TagList>( "gtdigilabel"        ) ; //gtDigis
   rhocorrectionlabel_ = iConfig.getParameter<TagList>( "rhocorrectionlabel" ) ; // For PU correction
   sigmaLabel_         = iConfig.getParameter<TagList>( "sigmaLabel"         ) ; // For PU correction
   puInfoLabel_        = iConfig.getParameter<TagList>( "puInfoLabel"        ) ;

   //-----------------------  2012 Election simple-cut-based ID  ----------------------- 
   conversionsInputTag_ = iConfig.getParameter<edm::InputTag> ( "conversionsInputTag" ) ;
   rhoIsoInputTag       = iConfig.getParameter<edm::InputTag> ( "rhoIsoInputTag"      ) ;
   isoValInputTags_     = iConfig.getParameter<TagList>       ( "isoValInputTags"     ) ;
   EIDMVAInputTags_     = iConfig.getParameter<StrList>       ( "EIDMVAInputTags"     ) ;

   //-------------------------------  Collection naming  -------------------------------
   lepcollections_      = iConfig.getParameter<StrList>           ( "LepCollections" ) ; //branch names
   phocollections_      = iConfig.getParameter<StrList>           ( "PhoCollections" ) ; //branch names
   jetcollections_      = iConfig.getParameter<StrList>           ( "JetCollections" ) ; //branch names
   jettype_             = iConfig.getParameter<std::vector<int> > ( "JetType"        ) ;

   //---------------------------------  Settings flag  ---------------------------------
   pairColl_            = iConfig.getUntrackedParameter<int>  ( "PairCollection" , 0     ) ;
   getElectronID_       = iConfig.getUntrackedParameter<bool> ( "ElectronID"     , true  ) ;
   skipGenInfo_         = iConfig.getUntrackedParameter<bool> ( "SkipGenInfo"    , false ) ;
   includeL7_           = iConfig.getUntrackedParameter<bool> ( "IncludeL7"      , true  ) ;
   SelectionParameters_ = iConfig.getParameter<edm::ParameterSet>( "SelectionParameters" );
   debug_               = iConfig.getUntrackedParameter<int>  ( "Debug"          , 0      );
   
   //----------------------------------  Electron ID  ----------------------------------
   eleVetoIdMapToken_       = consumes<edm::ValueMap<bool> >( iConfig.getParameter<edm::InputTag>( "eleVetoIdMap"   )  ) ;
   eleLooseIdMapToken_      = consumes<edm::ValueMap<bool> >( iConfig.getParameter<edm::InputTag>( "eleLooseIdMap"  )  ) ;
   eleMediumIdMapToken_     = consumes<edm::ValueMap<bool> >( iConfig.getParameter<edm::InputTag>( "eleMediumIdMap" )  ) ;
   eleTightIdMapToken_      = consumes<edm::ValueMap<bool> >( iConfig.getParameter<edm::InputTag>( "eleTightIdMap"  )  ) ;
   eleHEEPIdMapToken_       = consumes<edm::ValueMap<bool> >( iConfig.getParameter<edm::InputTag>( "eleHEEPIdMap"   )  ) ;

   phoLooseIdMapToken_        = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_       = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_        = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoTightIdMap"));
   //-------------------------------  CMSSW_7X updates  --------------------------------
   // update for CMSSW_7_2_0
   reducedEBRecHitCollectionToken_    = consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedEBRecHitCollection" ) );
   reducedEERecHitCollectionToken_    = consumes<EcalRecHitCollection>( iConfig.getParameter<edm::InputTag>( "reducedEERecHitCollection" ) );
   // update for CMSSW_7_3_1
   // reducedEBRecHitCollectionToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEgamma","reducedEBRecHits"));
   // reducedEERecHitCollectionToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEgamma","reducedEERecHits"));
   

   //----------------------------  Common variables set-up  ----------------------------
   isolatorR03.initializeElectronIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   isolatorR04.initializeElectronIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   isolatorR03.setConeSize( 0.3 );
   isolatorR04.setConeSize( 0.4 );

   PhotonisolatorR03.initializePhotonIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   PhotonisolatorR04.initializePhotonIsolation( kTRUE ); //NOTE: this automatically set all the correct defaul veto values
   PhotonisolatorR03.setConeSize( 0.3 );
   PhotonisolatorR04.setConeSize( 0.4 );
   // NOTE: It is safer and crab-compliant to get the files locally, i.e in EgammaAnalysis/ElectronTools/data
   // (see the downloard.url file in that directory)
   // Alternatively (for tests), they can be read from AFS:
   //*
   myMVANonTrig = new EGammaMvaEleEstimator();
   StrList  myManualCatWeigths;
   if( EIDMVAInputTags_.size() != 12 ) { cout << "EIDMVAInputTags array size (12) is not correct" << endl; }
   for( int ie = 0; ie < 6; ie++ ) { myManualCatWeigths.push_back( EIDMVAInputTags_[ie].c_str() ); }

   bool manualCat = true;

   myMVANonTrig->initialize( "BDT",
                             EGammaMvaEleEstimator::kNonTrig,
                             manualCat,
                             myManualCatWeigths );
   StrList myManualCatWeigthsTrig;
   for( int ie = 0; ie < 6; ie++ ) { 
      myManualCatWeigthsTrig.push_back( EIDMVAInputTags_[ie + 6].c_str() ); }

   myMVATrig = new EGammaMvaEleEstimator();
   myMVATrig->initialize( "BDT",
                          EGammaMvaEleEstimator::kTrig,
                          manualCat,
                          myManualCatWeigthsTrig );

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) { 
      HLTmaplist.insert( pair< std::string, int > ( TriggerBooking[i], i ) ); }
}
bprimeKit::~bprimeKit()
{}


//------------------------------------------------------------------------------ 
//   bprimeKit begin and end processes methods
//------------------------------------------------------------------------------ 
void bprimeKit::beginJob()
{
   root = new TTree( "root", "root" );
   EvtInfo.RegisterTree( root );
   if( !skipGenInfo_ ) { GenInfo.RegisterTree( root ); }
   if( lepcollections_.size() > MAX_LEPCOLLECTIONS ) { cout << "WARNING: Too many lep collections, using first " << MAX_LEPCOLLECTIONS << endl; }
   for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
      if( i >= MAX_LEPCOLLECTIONS ) { break; }
      LepInfo[i].RegisterTree( root, lepcollections_[i] );
   }
   for( unsigned i = 0; i < phocollections_.size(); i++ ) {
      if( i >= MAX_PHOCOLLECTIONS ) { break; }
      PhotonInfo[i].RegisterTree( root, phocollections_[i] );
   }
   VertexInfo.RegisterTree( root );
   if( jetcollections_.size() > MAX_JETCOLLECTIONS ) { cout << "WARNING: Too many jet collections, using first " << MAX_JETCOLLECTIONS << endl; }
   for( unsigned i = 0; i < jetcollections_.size(); i++ ) {
      if( i >= MAX_JETCOLLECTIONS ) { break; }
      JetInfo[i].RegisterTree( root, jetcollections_[i] );
   }
   if( pairColl_ >= 0 ) { PairInfo.RegisterTree( root ); }

}
void bprimeKit::endJob()
{}


//------------------------------------------------------------------------------ 
//   bprimeKit event based analysis methods
//------------------------------------------------------------------------------ 
void bprimeKit::beginRun( edm::Run const& iRun, edm::EventSetup const& iSetup )
{
   std::string processName_ = "HLT";
   bool changed( false );
   hltConfig_.init( iRun, iSetup, processName_, changed );
}
void bprimeKit::endRun( edm::Run const&, edm::EventSetup const& )
{}


void bprimeKit::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   edm::ESHandle<ParticleDataTable> pdt_;
   iSetup.getData( pdt_ );
   if( debug_ > 0 ) { cout << "Begin Analyze" << endl; }
   isData = iEvent.isRealData();   // Add by Jacky

   if( !TurnOffInCMSSW73x )
      for( unsigned il = 0; il < rhocorrectionlabel_.size(); il++ ) {
         rhoH.push_back( edm::Handle<double> () );
         iEvent.getByLabel( rhocorrectionlabel_[il], rhoH[il] );
         sigmaHandle.push_back( edm::Handle<double> () );
         iEvent.getByLabel( sigmaLabel_[il], sigmaHandle[il] );
      }
   if(!isData && genlabel_.size() > 0 ) iEvent.getByLabel( genlabel_[0], GenHandle);
   if( offlinePVlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVlabel_[0], VertexHandle ); }
   if( offlinePVBSlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVBSlabel_[0], VertexHandleBS ); } //Offline primary vertices with Beam Spot constraint //Dmitry
   if( !TurnOffInCMSSW73x )
      if( tracklabel_.size() > 0 ) { iEvent.getByLabel( tracklabel_[0], TrackHandle ); }         //get tracks for calculating dRmin (Dmitry)

   printf( "[Test] tracklabel is ok?\n" );

   // All PF Candidate for alternate isolation
   // edm::Handle<reco::PFCandidateCollection> pfCandidatesH;
   // if( !TurnOffInCMSSW73x )
   // { iEvent.getByLabel( "particleFlow", pfCandidatesH ); }
   // if( !TurnOffInCMSSW73x )
   // { thePfColl = *( pfCandidatesH.product() ); }

   // development for CMSSW_73X
   //edm::Handle<pat::PackedCandidateCollection> thePfColl;
   //iEvent.getByToken(pfToken_, thePfColl);
   //const  PackedCandidateCollection thePfColl = pfCandidatesH;


   // update for CMSSW_7_2_0
   // reference to (https://cmssdt.cern.ch/SDT/lxr/source//EgammaAnalysis/ElectronTools/plugins/ElectronIdMVAProducer.cc)
   //EcalClusterLazyTools lazyTools( iEvent, iSetup, reducedEBRecHitCollectionToken_, reducedEERecHitCollectionToken_ );

   if( TurnOnInCMSSW_7_4_1 ){
   //    edm::ESHandle<TransientTrackBuilder> builder;
   //    iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", builder );
   //    TransientTrackBuilder thebuilder = *( builder.product() );
   //    const TransientTrackBuilder* transientTrackBuilder = builder.product();
   }

   //  get ConditionsInRunBlock (Only for RECO)
   /*
   edm::Handle<edm::ConditionsInRunBlock> condInRunBlock;
   iEvent.getByLabel("conditionsInEdm", condInRunBlock);

   const boost::uint16_t beamMomentumVal = condInRunBlock->beamMomentum;
   const boost::uint32_t lhcFillNumberVal = condInRunBlock->lhcFillNumber;

   std::cout <<"  Beam Momentum = " << beamMomentumVal << " GeV" << std::endl;
   std::cout <<"  LHC Fill Number = " << lhcFillNumberVal<< std::endl;
   */

   //  iEvent.getByLabel("electronGsfTracks",      gsfTrackHandle);  //get gsftracks (Dmitry)
   //  const edm::View<reco::GsfTrack> & gsfTracks = *gsfTrackHandle;


   // Start to fill the main root branches
   //------------------------------------------------------------------------------ 
   //   VertexInfo added by Dmitry to help filter out pile up and bad events
   //------------------------------------------------------------------------------ 
   fillVertex( iEvent , iSetup ) ;

   //------------------------------------------------------------------------------ 
   //   Leptons
   //------------------------------------------------------------------------------ 
   fillLepton( iEvent , iSetup ) ;
   
   //------------------------------------------------------------------------------ 
   //   Photons
   //------------------------------------------------------------------------------ 
   fillPhoton( iEvent , iSetup );

   //------------------------------------------------------------------------------ 
   //   Jets
   //------------------------------------------------------------------------------  
   fillJet( iEvent , iSetup ) ;

   //------------------------------------------------------------------------------ 
   //   Pairs
   //------------------------------------------------------------------------------  
   if( pairColl_ >= 0 ) { fillLepPair( iEvent , iSetup ) ; } 
#ifdef FILL_DIJET_PAIRS
   fillJetPair( iEvent, iSetup ) ;
#endif

   //------------------------------------------------------------------------------ 
   //   Generation and Event information
   //------------------------------------------------------------------------------  
   if( debug_ > 5 ) { cout << "\tFill Gen and Event Info.\n"; }
   memset( &GenInfo, 0x00, sizeof( GenInfo ) );
   memset( &EvtInfo, 0x00, sizeof( EvtInfo ) );
   fillGenInfo( iEvent , iSetup );
   fillEvent  ( iEvent , iSetup );
   
   //------------------------------------------------------------------------------ 
   //   Processing debugging messages and file writing
   //------------------------------------------------------------------------------ 
   if( debug_ > 11 ) {
      for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
         cout << "Lepton Collection " << i << "(" << lepcollections_[i] << "): size " << LepInfo[i].Size << endl;
         for( int j = 0; j < LepInfo[i].Size; j++ ){ 
            cout << "  Lep " << j << " type,pt,eta,phi " 
                 << LepInfo[i].LeptonType[j] << "," 
                 << LepInfo[i].Pt[j] << "," 
                 << LepInfo[i].Eta[j] << "," 
                 << LepInfo[i].Phi[j] << endl; 
         }
      }
   }
   if( debug_ > 5 ) { cout << "Fill tree with all information\n"; }
   root->Fill();
   if( debug_ > 10 ) { cout << "Filled event information: Run " << EvtInfo.RunNo << " Event " << EvtInfo.EvtNo << endl; }
   if( debug_ > 11 ) {
      root->Show( -1, 999 );
      for( size_t i = 0; i < lepcollections_.size(); i++ ) {
         cout << "After fill, Lepton Collection " << i << "(" << lepcollections_[i] << "): size " << LepInfo[i].Size << endl;
         for( int j = 0; j < LepInfo[i].Size; j++ ){ 
            cout << "  Lep " << j << " type,pt,eta,phi " 
                 << LepInfo[i].LeptonType[j] << "," 
                 << LepInfo[i].Pt[j] << "," 
                 << LepInfo[i].Eta[j] << "," 
                 << LepInfo[i].Phi[j] << endl;
         }
      }
   }
}
//define this as a plug-in
DEFINE_FWK_MODULE( bprimeKit );

