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
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

//------------------------------------------------------------------------------ 
//   Custom enums, typedefs and macros
//------------------------------------------------------------------------------ 
//#define FILL_DIJET_PAIRS 1      //Uncomment for jet pair processing
typedef std::vector<edm::InputTag> TagList;
typedef std::vector<std::string>   StrList;

//------------------------------------------------------------------------------ 
//   Helper variables
//------------------------------------------------------------------------------ 
static edm::Service<TFileService> fs;
static TFileDirectory results ;

//------------------------------------------------------------------------------ 
//   bprimeKit methods: constructor and destructor
//------------------------------------------------------------------------------ 
bprimeKit::bprimeKit( const edm::ParameterSet& iConfig )
{
   results = TFileDirectory( fs->mkdir( "results" ) );
   //----- Event related  -----------------------------------------------------------------------------
   metlabel_           = iConfig.getParameter<TagList>( "metlabel"           ) ; //"patMETs"
   rhoLabel_ = consumes<double> (iConfig.getParameter<InputTag>("rhoLabel"));
   hltlabel_           = iConfig.getParameter<TagList>( "hltlabel"           ) ; // "TriggerResults::HLT"
   if( ! isData ){
      puInfoLabel_ = iConfig.getParameter<TagList>( "puInfoLabel" ) ; }
   
   //----- Vertex related  ----------------------------------------------------------------------------
   offlinePVlabel_     = iConfig.getParameter<TagList>( "offlinePVlabel"     ) ; //offlinePrimaryVertices
   offlinePVBSlabel_   = iConfig.getParameter<TagList>( "offlinePVBSlabel"   ) ; //offlinePrimaryVerticesWithBS
   offlineBSlabel_     = iConfig.getParameter<TagList>( "offlineBSlabel"     ) ; //offlineBeamSpot
   
   //----- GenInfo related  ---------------------------------------------------------------------------
   genevtlabel_        = iConfig.getParameter<TagList>( "genevtlabel"        ) ; //generator
   gtdigilabel_        = iConfig.getParameter<TagList>( "gtdigilabel"        ) ; //gtDigis

   //----- Jet related  -------------------------------------------------------------------------------
   jetcollections_  = iConfig.getParameter<StrList>( "JetCollections" ) ; //branch names
   jetlabel_        = iConfig.getParameter<TagList>( "jetlabel"       ) ; 
   //qgToken_ = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"));
   
   //----- Lepton related  ----------------------------------------------------------------------------
   lepcollections_       = iConfig.getParameter<StrList>( "LepCollections"     ) ; //branch names
   muonlabel_            = iConfig.getParameter<TagList>( "muonlabel"          ) ;
   eleclabel_            = iConfig.getParameter<TagList>( "eleclabel"          ) ;
   taulabel_             = iConfig.getParameter<TagList>( "taulabel"           ) ;
   eleVetoIdMapToken_    = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleVetoIdMap"    )) ;
   eleLooseIdMapToken_   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleLooseIdMap"   )) ;
   eleMediumIdMapToken_  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleMediumIdMap"  )) ;
   eleTightIdMapToken_   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleTightIdMap"   )) ;
   eleHEEPIdMapToken_    = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleHEEPIdMap"    )) ;
   genlabel_           = iConfig.getParameter<TagList>( "genlabel"           ) ; // "genParticles"
   eleMVAValuesMapToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "eleMVAValuesMap" )) ;
   
   //----- Photon related  ----------------------------------------------------------------------------
   phocollections_                 = iConfig.getParameter<StrList> ( "PhoCollections"     ) ; //branch names
   pholabel_                       = iConfig.getParameter<TagList> ( "pholabel"           ) ;
   phoLooseIdMapToken_             = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoLooseIdMap"             )) ;
   phoMediumIdMapToken_            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoMediumIdMap"            )) ;
   phoTightIdMapToken_             = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoTightIdMap"             )) ;
   phoMVAValuesMapToken_           = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoMVAValuesMap"           )) ;
   phoChargedIsolationToken_       = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoChargedIsolation"       )) ;
   phoNeutralHadronIsolationToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoNeutralHadronIsolation" )) ;
   phoPhotonIsolationToken_        = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoPhotonIsolation"        )) ;
   
   
   //----- Configuration flats  -----------------------------------------------------------------------
   pairColl_            = iConfig.getUntrackedParameter<int>      ( "PairCollection" , 0     ) ;
   getElectronID_       = iConfig.getUntrackedParameter<bool>     ( "ElectronID"     , true  ) ;
   getPhotonID_         = iConfig.getUntrackedParameter<bool>     ( "PhotonID"       , true  ) ;
   skipGenInfo_         = iConfig.getUntrackedParameter<bool>     ( "SkipGenInfo"    , false ) ;
   includeL7_           = iConfig.getUntrackedParameter<bool>     ( "IncludeL7"      , true  ) ;
   SelectionParameters_ = iConfig.getParameter<edm::ParameterSet> ( "SelectionParameters"    ) ;
   debug_               = iConfig.getUntrackedParameter<int>      ( "Debug"          , 0     ) ;
   
   //----- 2015 cut based electron ID  ----------------------------------------------------------------
   EIDMVAInputTags_     = iConfig.getParameter<StrList>       ( "EIDMVAInputTags"     ) ;

   
   if( EIDMVAInputTags_.size() != 12 ) { cout << "EIDMVAInputTags array size (12) is not correct" << endl; }
   StrList myManualCatWeigthsTrig;
   for( int ie = 0; ie < 6; ie++ ) { 
      myManualCatWeigthsTrig.push_back( EIDMVAInputTags_[ie + 6].c_str() ); }
   myMVATrig = new EGammaMvaEleEstimator();
   myMVATrig->initialize( "BDT", EGammaMvaEleEstimator::kTrig, true, myManualCatWeigthsTrig );

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) { 
      HLTmaplist.insert( pair<std::string,int>( TriggerBooking[i], i ) ) ; }
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
   VertexInfo.RegisterTree( root );
   if( !skipGenInfo_ ) { GenInfo.RegisterTree( root ); } 
   for( unsigned i = 0; i < lepcollections_.size(); i++ ) {
      if( i >= MAX_LEPCOLLECTIONS ) { break; }
      LepInfo[i].RegisterTree( root, lepcollections_[i] );
   }
   for( unsigned i = 0; i < phocollections_.size(); i++ ) {
      if( i >= MAX_PHOCOLLECTIONS ) { break; }
      PhotonInfo[i].RegisterTree( root, phocollections_[i] );
   }
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
   bool changed =  true ;
   hltConfig_.init( iRun, iSetup, processName_, changed );
}
void bprimeKit::endRun( edm::Run const&, edm::EventSetup const& )
{}


void bprimeKit::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   //----- Setting up common variables  ---------------------------------------------------------------
   edm::ESHandle<ParticleDataTable> pdt_;
   iSetup.getData( pdt_ );
   if( debug_ > 0 ) { cout << "Begin Analyze" << endl; }
   isData = iEvent.isRealData();   // Add by Jacky

   if(!isData && genlabel_.size() > 0 ) iEvent.getByLabel( genlabel_[0], GenHandle);
   if( offlinePVlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVlabel_[0], VertexHandle ); }
   if( offlinePVBSlabel_.size() > 0 ) { iEvent.getByLabel( offlinePVBSlabel_[0], VertexHandleBS ); } //Offline primary vertices with Beam Spot constraint //Dmitry
   pvCol = VertexHandle.product();
   
   edm::ESHandle<TransientTrackBuilder> builder;
   iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", builder );
   TransientTrackBuilder thebuilder = *( builder.product() );
   transientTrackBuilder = builder.product();


   //------------------------------------------------------------------------------ 
   //   Generation and Event information
   //------------------------------------------------------------------------------  
   if( debug_ > 5 ) { cout << "\tFill Gen and Event Info.\n"; }
   memset( &GenInfo, 0x00, sizeof( GenInfo ) );
   memset( &EvtInfo, 0x00, sizeof( EvtInfo ) );
   fillGenInfo( iEvent , iSetup );
   fillEvent  ( iEvent , iSetup );

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

DEFINE_FWK_MODULE( bprimeKit );

