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
bprimeKit::bprimeKit( const edm::ParameterSet& iConfig ) :
   effAreaChHadrons_  ((iConfig.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
   effAreaNeuHadrons_ ((iConfig.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath()),
   effAreaPhotons_    ((iConfig.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath()   )
{
   results = TFileDirectory( fs->mkdir( "results" ) );
   //----- Event related  -----------------------------------------------------------------------------
   rhoLabel_    = iConfig.getParameter<edm::InputTag> ( "rhoLabel"    ) ;
   hltLabel_    = iConfig.getParameter<edm::InputTag> ( "hltLabel"    ) ; 
   metLabel_    = iConfig.getParameter<edm::InputTag> ( "metLabel"    ) ;
   puInfoLabel_ = iConfig.getParameter<edm::InputTag> ( "puInfoLabel" ) ;
   
   //----- Vertex related  ----------------------------------------------------------------------------
   offlinePVLabel_   = iConfig.getParameter<edm::InputTag>( "offlinePVLabel"   ) ;
   offlinePVBSLabel_ = iConfig.getParameter<edm::InputTag>( "offlinePVBSLabel" ) ;
   offlineBSLabel_   = iConfig.getParameter<edm::InputTag>( "offlineBSLabel"   ) ;
   
   //----- GenInfo related  ---------------------------------------------------------------------------
   genevtLabel_ = iConfig.getParameter<edm::InputTag>( "genevtLabel" ) ;
   genLabel_    = iConfig.getParameter<edm::InputTag>( "genLabel"    ) ;
   gtdigiLabel_ = iConfig.getParameter<edm::InputTag>( "gtdigiLabel" ) ;

   //----- Jet related  -------------------------------------------------------------------------------
   jetcollections_    = iConfig.getParameter<StrList>( "JetCollections" ) ;
   jetLabel_          = iConfig.getParameter<TagList>( "jetLabel"       ) ;
   qgLikelihoodToken_ = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"));
   qgaxis2Token_      = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"));
   qgmultToken_       = consumes<edm::ValueMap<int  >>(edm::InputTag("QGTagger", "mult"));
   qgptDToken_        = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"));
   
   //----- Lepton related  ----------------------------------------------------------------------------
   lepcollections_      = iConfig.getParameter<StrList>( "LepCollections"     ) ; //branch names
   muonLabel_           = iConfig.getParameter<TagList>( "muonLabel"          ) ;
   elecLabel_           = iConfig.getParameter<TagList>( "elecLabel"          ) ;
   tauLabel_            = iConfig.getParameter<TagList>( "tauLabel"           ) ;
   eleVetoIdMapToken_   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleVetoIdMap"    )) ;
   eleLooseIdMapToken_  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleLooseIdMap"   )) ;
   eleMediumIdMapToken_ = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleMediumIdMap"  )) ;
   eleTightIdMapToken_  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleTightIdMap"   )) ;
   eleHEEPIdMapToken_   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleHEEPIdMap"    )) ;
   conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsLabel");
   
   //----- Photon related  ----------------------------------------------------------------------------
   phocollections_                 = iConfig.getParameter<StrList> ( "PhoCollections"     ) ; //branch names
   phoLabel_                       = iConfig.getParameter<TagList> ( "phoLabel"           ) ;
   phoLooseIdMapToken_             = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoLooseIdMap"             )) ;
   phoMediumIdMapToken_            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoMediumIdMap"            )) ;
   phoTightIdMapToken_             = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoTightIdMap"             )) ;
   phoChargedIsolationToken_       = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoChargedIsolation"       )) ;
   phoNeutralHadronIsolationToken_ = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoNeutralHadronIsolation" )) ;
   phoPhotonIsolationToken_        = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoPhotonIsolation"        )) ;
   full5x5SigmaIEtaIEtaMapToken_   = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "full5x5SigmaIEtaIEtaMap"   )) ;

   //----- Configuration flats  -----------------------------------------------------------------------
   pairColl_            = iConfig.getUntrackedParameter<int> ( "PairCollection" , 0     ) ;
   skipGenInfo_         = iConfig.getUntrackedParameter<bool>( "SkipGenInfo"    , false ) ;
   includeL7_           = iConfig.getUntrackedParameter<bool>( "IncludeL7"      , true  ) ;
   debug_               = iConfig.getUntrackedParameter<int> ( "Debug"          , 0     ) ;
   runOnB2G             = iConfig.getUntrackedParameter<bool>( "runOnB2G"       , false ) ;
   runMuonJetClean      = iConfig.getParameter<bool>( "runMuonJetClean" );
   //----- 2015 cut based electron ID  ----------------------------------------------------------------
   EIDMVAInputTags_     = iConfig.getParameter<StrList>       ( "EIDMVAInputTags"     ) ;

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) { 
      HLTmaplist.insert( pair<std::string,int>( TriggerBooking[i], i ) ) ; }
   
   initTree();
   initJetEnergyCorrectors();
}

bprimeKit::~bprimeKit()
{}


//------------------------------------------------------------------------------ 
//   bprimeKit begin and end processes methods
//------------------------------------------------------------------------------ 
void bprimeKit::beginJob()
{
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
   if( debug_ > 0 ) { cout << "[0] Begin Analysis!!" << endl; }

   GetEdmObjects( iEvent, iSetup );
   //----- Setting up common variables  ---------------------------------------------------------------
   edm::ESHandle<ParticleDataTable> pdt_;
   iSetup.getData( pdt_ );
   isData = iEvent.isRealData();   // Add by Jacky

   edm::ESHandle<TransientTrackBuilder> builder;
   iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", builder );
   TransientTrackBuilder thebuilder = *( builder.product() );
   transientTrackBuilder = builder.product();


   if( debug_ > 0 ) { cout << "[0] Entering subroutines..." << endl; }
   //------------------------------------------------------------------------------ 
   //   Generation and Event information
   //------------------------------------------------------------------------------  
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
   if( debug_ > 0 ) { cout << "[0] Filling tree with all information" << endl; }
   root->Fill();
   if( debug_ > 0 ) { cout << "[0] Filled event information: Run " << EvtInfo.RunNo << " Event " << EvtInfo.EvtNo << endl; }

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

void bprimeKit::initTree()
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

void bprimeKit::initJetEnergyCorrectors()
{
   vector<JetCorrectorParameters> vPar;
   vector<JetCorrectorParameters> vParAK8;
   L3JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L3Absolute_AK8PFchs.txt");
   L2JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L2Relative_AK4PFchs.txt");
   L1JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L1FastJet_AK4PFchs.txt");
   L3JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L3Absolute_AK8PFchs.txt");
   L2JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L2Relative_AK8PFchs.txt");
   L1JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L1FastJet_AK8PFchs.txt");
   // Load the JetCorrectorParameter objects into a vector,
   // IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vPar.push_back(*L1JetPar_);
   vPar.push_back(*L2JetPar_);
   vPar.push_back(*L3JetPar_);
   vParAK8.push_back(*L1JetParAK8_);
   vParAK8.push_back(*L2JetParAK8_);
   vParAK8.push_back(*L3JetParAK8_);

   JetCorrector    = new FactorizedJetCorrector(vPar);
   JetCorrectorAK8 = new FactorizedJetCorrector(vParAK8);
}

void bprimeKit::clearTree()
{
   delete root;
}

void bprimeKit::clearJetEnergyCorrector()
{
   delete L3JetPar_;
   delete L2JetPar_;
   delete L1JetPar_;
   delete L3JetParAK8_;
   delete L2JetParAK8_;
   delete L1JetParAK8_;
   delete JetCorrector;
   delete JetCorrectorAK8;
}


void bprimeKit::GetEdmObjects( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   iEvent.getByLabel( rhoLabel_ , _rhoHandle );
   iEvent.getByLabel( metLabel_,  _metHandle );
   iEvent.getByLabel( offlineBSLabel_, _beamSpotHandle );
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnUp",  _metHandle_TempPlus );
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnDown",  _metHandle_TempDown ); 
   iEvent.getByLabel( gtdigiLabel_, _gtRecord );
   iEvent.getByLabel( hltLabel_ , _triggerHandle );
   iEvent.getByLabel( offlinePVLabel_, _vertexHandle );
   iEvent.getByLabel( offlinePVBSLabel_ , _bsVertexHandle );
   
   if( !iEvent.isRealData() ) { 
      iEvent.getByLabel( puInfoLabel_, _pileupHandle ); 
      if( !skipGenInfo_ ) { 
         iEvent.getByLabel( genLabel_, _genHandle ); 
         iEvent.getByLabel( genevtLabel_, _genInfoHandle );
      }
   }
   
   for( unsigned i = 0; i < jetLabel_.size(); ++i ) {
      _jetHandleList.push_back( JetHandle() );
      iEvent.getByLabel( jetLabel_[i], _jetHandleList[i] );
      if( debug_ > 10 ) { cout << "jets " << i << " jetLabel " << jetLabel_[i] << " with " << _jetHandleList[i]->size() << " entries\n"; }
   }

   if( debug_ > 10 ) {cout <<"Getting Q taggers" << endl;}
   iEvent.getByToken( qgLikelihoodToken_ , qgLikelihoodHandle );
   iEvent.getByToken( qgaxis2Token_      , qgaxis2Handle      );
   iEvent.getByToken( qgmultToken_       , qgmultHandle       );
   iEvent.getByToken( qgptDToken_        , qgptDHandle        );

   for( unsigned i = 0 ; i < muonLabel_.size() ; ++i ){
      _muonHandleList.push_back( MuonHandle() );
      _elecHandleList.push_back( ElectronHandle() ) ;
      _gsfHandleList.push_back( GsfHandle() );
      _tauHandleList.push_back( TauHandle() );
      iEvent.getByLabel( muonLabel_[i] , _muonHandleList[i] );
      iEvent.getByLabel( elecLabel_[i] , _elecHandleList[i] );
      iEvent.getByLabel( elecLabel_[i] , _gsfHandleList[i] );
      iEvent.getByLabel( tauLabel_[i] , _tauHandleList[i] );
   }
   iEvent.getByLabel( conversionsInputTag_  , conversions_h       ) ;
   iEvent.getByToken( eleVetoIdMapToken_    , veto_id_decisions   ) ;
   iEvent.getByToken( eleLooseIdMapToken_   , loose_id_decisions  ) ;
   iEvent.getByToken( eleMediumIdMapToken_  , medium_id_decisions ) ;
   iEvent.getByToken( eleTightIdMapToken_   , tight_id_decisions  ) ;
   iEvent.getByToken( eleHEEPIdMapToken_    , heep_id_decisions   ) ;

   
   for( unsigned il = 0; il < phoLabel_.size(); il++ ) {
      _photonHandleList.push_back( PhotonHandle() );
      iEvent.getByLabel( phoLabel_[il], _photonHandleList[il] );
      if( debug_ > 10 ) { cout << "photons " << il << " phoLabel " << phoLabel_[il] << " with " << _photonHandleList[il]->size() << " entries\n"; }
   }
   iEvent.getByToken( phoLooseIdMapToken_             , _photonLooseID           ) ;
   iEvent.getByToken( phoMediumIdMapToken_            , _photonMediumID          ) ;
   iEvent.getByToken( phoTightIdMapToken_             , _photonTightID           ) ;
   iEvent.getByToken( phoChargedIsolationToken_       , phoChargedIsolationMap       ) ;
   iEvent.getByToken( phoNeutralHadronIsolationToken_ , phoNeutralHadronIsolationMap ) ;
   iEvent.getByToken( phoPhotonIsolationToken_        , phoPhotonIsolationMap        ) ;
   iEvent.getByToken( full5x5SigmaIEtaIEtaMapToken_   , full5x5SigmaIEtaIEtaMap      ) ;
}



DEFINE_FWK_MODULE( bprimeKit );

