/*******************************************************************************

 *  Filename    : bprimeKit.cc
 *  Description : The virtual and explicit functions for the bprimeKit
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

//---------------------  fBaseTree and standard template libraries  ----------------------
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
//#define Fill_DIJET_PAIRS 1      //Uncomment for jet pair processing
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
   fPhotonEffectiveArea_ChargeHadron  ((iConfig.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
   fPhotonEffectiveArea_NeutralHadron ((iConfig.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath()),
   fPhotonEffectiveArea_Photons    ((iConfig.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath()   )
{
   results = TFileDirectory( fs->mkdir( "results" ) );
   //----- Configuration flags  ---------------------------------------------------
   fPairCollectionType = iConfig.getUntrackedParameter<int> ( "PairCollection" , 0     ) ;
   fSkipfGenInfo       = iConfig.getUntrackedParameter<bool>( "SkipGenInfo"    , false ) ;
   fIncludeL7          = iConfig.getUntrackedParameter<bool>( "IncludeL7"      , true  ) ;
   fDebug              = iConfig.getUntrackedParameter<int> ( "Debug"          , 0     ) ;
   fRunOnB2G           = iConfig.getUntrackedParameter<bool>( "runOnB2G"       , false ) ;
   fRunMuonJetCleaning = iConfig.getParameter<bool>( "runMuonJetClean" );
   
   //----- Event related  -----------------------------------------------------------------------------
   fRhoLabel    = iConfig.getParameter<edm::InputTag> ( "rhoLabel"    ) ;
   fHLTLabel    = iConfig.getParameter<edm::InputTag> ( "hltLabel"    ) ; 
   fMETLabel    = iConfig.getParameter<edm::InputTag> ( "metLabel"    ) ;
   fPileupLabel = iConfig.getParameter<edm::InputTag> ( "puInfoLabel" ) ;
   
   //----- Vertex related  ----------------------------------------------------------------------------
   fPrimaryVertexLabel            = iConfig.getParameter<edm::InputTag>( "offlinePVLabel"   ) ;
   fPrimVertex_withBeamSpot_Label = iConfig.getParameter<edm::InputTag>( "offlinePVBSLabel" ) ;
   fBeamspotLabel                 = iConfig.getParameter<edm::InputTag>( "offlineBSLabel"   ) ;
   
   //----- fGenInfo related  ---------------------------------------------------------------------------
   fGenEventLabel    = iConfig.getParameter<edm::InputTag>( "genevtLabel" ) ;
   fGenParticleLabel = iConfig.getParameter<edm::InputTag>( "genLabel"    ) ;
   fGenDigiLabel     = iConfig.getParameter<edm::InputTag>( "gtdigiLabel" ) ;
   fLHELabel         = iConfig.getParameter<edm::InputTag>( "lheLabel"    ) ;

   //----- Jet related  -------------------------------------------------------------------------------
   fJetCollections      = iConfig.getParameter<StrList>( "JetCollections" ) ;
   fJetLabels           = iConfig.getParameter<TagList>( "jetLabel"       ) ;
   fQGLikelihoodToken   = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"));
   fQGAxis2Token        = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"));
   fQGMultiplicityToken = consumes<edm::ValueMap<int  >>(edm::InputTag("QGTagger", "mult"));
   fQGPtDToken          = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"));
   
   //----- Lepton related  ----------------------------------------------------------------------------
   fLeptonCollections     = iConfig.getParameter<StrList>( "LepCollections" ) ; //branch names
   fMuonLabels            = iConfig.getParameter<TagList>( "muonLabel"      ) ;
   fElectronLabels        = iConfig.getParameter<TagList>( "elecLabel"      ) ;
   fTauLabels             = iConfig.getParameter<TagList>( "tauLabel"       ) ;
   fElectronIDVetoToken   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleVetoIdMap"    )) ;
   fElectronIDLooseToken  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleLooseIdMap"   )) ;
   fElectronIDMediumToken = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleMediumIdMap"  )) ;
   fElectronIDTightToken  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleTightIdMap"   )) ;
   fElectronIDHEEPToken   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleHEEPIdMap"    )) ;
   fConversionsTag        = iConfig.getParameter<edm::InputTag>("conversionsLabel");
   
   //----- Photon related  ----------------------------------------------------------------------------
   fPhotonCollections             = iConfig.getParameter<StrList> ( "PhoCollections"     ) ; //branch names
   fPhotonLabels                  = iConfig.getParameter<TagList> ( "phoLabel"           ) ;
   fPhotonLooseIDToken            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoLooseIdMap"             )) ;
   fPhotonMediumIDToken           = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoMediumIdMap"            )) ;
   fPhotonTightIDToken            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoTightIdMap"             )) ;
   fPhotonIsolation_Charged_Token = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoChargedIsolation"       )) ;
   fPhotonIsolation_Neutral_Token = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoNeutralHadronIsolation" )) ;
   fPhotonIsolation_Photon_Token  = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoPhotonIsolation"        )) ;
   fPhotonSignaIEtaIEtaToken      = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "full5x5SigmaIEtaIEtaMap"   )) ;

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) { 
      fHighLevelTriggerMap.insert( pair<std::string,int>( TriggerBooking[i], i ) ) ; }
   
}

bprimeKit::~bprimeKit()
{
}


//------------------------------------------------------------------------------ 
//   bprimeKit begin and end processes methods
//------------------------------------------------------------------------------ 
void bprimeKit::beginJob()
{
  InitTree();
  InitJetEnergyCorrectors();
}
void bprimeKit::endJob()
{
   ClearTree();
   ClearJetEnergyCorrector();
}


//------------------------------------------------------------------------------ 
//   bprimeKit event based analysis methods
//------------------------------------------------------------------------------ 
void bprimeKit::beginRun( edm::Run const& iRun, edm::EventSetup const& iSetup )
{
   std::string processName_ = "HLT";
   bool changed =  true ;
   fHighLevelTriggerConfig.init( iRun, iSetup, processName_, changed );
}
void bprimeKit::endRun( edm::Run const&, edm::EventSetup const& )
{}


void bprimeKit::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   if( fDebug > 0 ) { cout << "[0] Begin Analysis!!" << endl; }

   GetEdmObjects( iEvent, iSetup );
   //----- Setting up common variables  ---------------------------------------------------------------
   fIsData = iEvent.isRealData();   // Add by Jacky

   if( fDebug > 0 ) { cout << "[0] Entering subroutines..." << endl; }

   //------------------------------------------------------------------------------ 
   //   Inserting Information
   //------------------------------------------------------------------------------
   memset( &fGenInfo, 0x00, sizeof( fGenInfo ) );
   memset( &fEvtInfo, 0x00, sizeof( fEvtInfo ) );
   FillfGenInfo( iEvent , iSetup );
   FillEvent  ( iEvent , iSetup );

   FillVertex( iEvent , iSetup ) ;
   FillLepton( iEvent , iSetup ) ;
   FillPhoton( iEvent , iSetup );
   FillJet( iEvent , iSetup ) ;
   
   if( fPairCollectionType >= 0 ) { FillLepPair( iEvent , iSetup ) ; } 
#ifdef Fill_DIJET_PAIRS
   FillJetPair( iEvent, iSetup ) ;
#endif


   //------------------------------------------------------------------------------ 
   //   Processing debugging messages and file writing
   //------------------------------------------------------------------------------ 
   if( fDebug > 0 ) { cout << "[0] Filling tree with all information" << endl; }
   fBaseTree->Fill();
   if( fDebug > 0 ) { cout << "[0] Filled event information: Run " << fEvtInfo.RunNo << " Event " << fEvtInfo.EvtNo << endl; }

   if( fDebug > 11 ) {
      fBaseTree->Show( -1, 999 );
      for( size_t i = 0; i < fLeptonCollections.size(); i++ ) {
         cout << "After Fill, Lepton Collection " << i << "(" << fLeptonCollections[i] << "): size " << fLepInfo[i].Size << endl;
         for( int j = 0; j < fLepInfo[i].Size; j++ ){ 
            cout << "  Lep " << j << " type,pt,eta,phi " 
               << fLepInfo[i].LeptonType[j] << "," 
               << fLepInfo[i].Pt[j] << "," 
               << fLepInfo[i].Eta[j] << "," 
               << fLepInfo[i].Phi[j] << endl;
         }
      }
   }
}

void bprimeKit::InitTree()
{
   fBaseTree = fs->make<TTree>( "root", "root" );
   fEvtInfo.RegisterTree( fBaseTree );
   fVertexInfo.RegisterTree( fBaseTree );
   if( !fSkipfGenInfo ) { fGenInfo.RegisterTree( fBaseTree ); } 
   for( unsigned i = 0; i < fLeptonCollections.size(); ++i ) {
      if( i >= MAX_LEPCOLLECTIONS ) { break; }
      fLepInfo[i].RegisterTree( fBaseTree, fLeptonCollections[i] );
   }
   for( unsigned i = 0; i < fPhotonCollections.size(); ++i ) {
      if( i >= MAX_PHOCOLLECTIONS ) { break; }
      fPhotonInfo[i].RegisterTree( fBaseTree, fPhotonCollections[i] );
   }
   for( unsigned i = 0; i < fJetCollections.size(); ++i ) {
      if( i >= MAX_JETCOLLECTIONS ) { break; }
      fJetInfo[i].RegisterTree( fBaseTree, fJetCollections[i] );
   }
   if( fPairCollectionType >= 0 ) { fPairInfo.RegisterTree( fBaseTree ); }
}

//------------------------------------------------------------------------------ 
//   Jet Energy correction
//------------------------------------------------------------------------------
static vector<JetCorrectorParameters> vPar;
static vector<JetCorrectorParameters> vParAK8;
static JetCorrectorParameters*    L3JetPar_;
static JetCorrectorParameters*    L2JetPar_;
static JetCorrectorParameters*    L1JetPar_;
static JetCorrectorParameters*    L3JetParAK8_;
static JetCorrectorParameters*    L2JetParAK8_;
static JetCorrectorParameters*    L1JetParAK8_;

void bprimeKit::InitJetEnergyCorrectors()
{
   L3JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L3Absolute_AK8PFchs.txt");
   L2JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L2Relative_AK4PFchs.txt");
   L1JetPar_    = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L1FastJet_AK4PFchs.txt");
   L3JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L3Absolute_AK8PFchs.txt");
   L2JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L2Relative_AK8PFchs.txt");
   L1JetParAK8_ = new JetCorrectorParameters("./JECs/PHYS14_25_V2_L1FastJet_AK8PFchs.txt");
   
   // IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vPar.push_back(*L1JetPar_);
   vPar.push_back(*L2JetPar_);
   vPar.push_back(*L3JetPar_);
   vParAK8.push_back(*L1JetParAK8_);
   vParAK8.push_back(*L2JetParAK8_);
   vParAK8.push_back(*L3JetParAK8_);

   fJetCorrector    = new FactorizedJetCorrector(vPar);
   fJetCorrectorAK8 = new FactorizedJetCorrector(vParAK8);
}

void bprimeKit::ClearTree()
{
   delete fBaseTree;
}

void bprimeKit::ClearJetEnergyCorrector()
{
   delete L3JetPar_;
   delete L2JetPar_;
   delete L1JetPar_;
   delete L3JetParAK8_;
   delete L2JetParAK8_;
   delete L1JetParAK8_;
   delete fJetCorrector;
   delete fJetCorrectorAK8;
}


void bprimeKit::GetEdmObjects( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( fDebug > 1 ){ std::cerr << "\t[1]Getting Event Wide Handles" << std::endl; }
   iEvent.getByLabel( fRhoLabel                                 , fRho_H                ) ;
   iEvent.getByLabel( fMETLabel                                 , fMET_H                ) ;
   iEvent.getByLabel( fBeamspotLabel                            , fBeamSpot_H           ) ;
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnUp"   , fMETTempPlus_H        ) ;
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnDown" , fMETTempDown_H        ) ;
   iEvent.getByLabel( fGenDigiLabel                             , fRecord_H             ) ;
   iEvent.getByLabel( fHLTLabel                                 , fTrigger_H            ) ;
   iEvent.getByLabel( fPrimaryVertexLabel                       , fVertex_H             ) ;
   iEvent.getByLabel( fPrimVertex_withBeamSpot_Label            , fVertexWithBeamSpot_H ) ;
   
   if( !iEvent.isRealData() ) { 
      iEvent.getByLabel( fPileupLabel, fPileup_H ); 
      if( !fSkipfGenInfo ) { 
         iEvent.getByLabel( fGenParticleLabel, fGenParticle_H ); 
         iEvent.getByLabel( fGenEventLabel, fGenEvent_H );
         iEvent.getByLabel( fLHELabel , fLHEInfo_H );
      }
   }
   
   for( unsigned i = 0; i < fJetLabels.size(); ++i ) {
      fJetList_Hs.push_back( JetHandle() );
      iEvent.getByLabel( fJetLabels[i], fJetList_Hs[i] );
      if( fDebug > 1 ) { 
         std::cerr << "\t[1]Getting Jet Collection" << i << std::endl
            << "\tjetLabel:" << fJetLabels[i] << " with " 
            << fJetList_Hs[i]->size() << " entries" << std::endl ; 
      }
   }

   if( fDebug > 1 ) { std::cerr <<"\t[1]Getting Q taggers" << endl;}
   iEvent.getByToken( fQGLikelihoodToken   , fQGLikelihood_H   ) ;
   iEvent.getByToken( fQGAxis2Token        , fQGAxis2_H        ) ;
   iEvent.getByToken( fQGMultiplicityToken , fQGMultiplicity_H ) ;
   iEvent.getByToken( fQGPtDToken          , fQGPtD_H          ) ;

   for( unsigned i = 0 ; i < fMuonLabels.size() ; ++i ){
      fMuonList_Hs.push_back( MuonHandle() );
      fElectronList_Hs.push_back( ElectronHandle() ) ;
      fGsfElectronList_Hs.push_back( GsfHandle() );
      fTauList_Hs.push_back( TauHandle() );
      iEvent.getByLabel( fMuonLabels[i] , fMuonList_Hs[i] );
      iEvent.getByLabel( fElectronLabels[i] , fElectronList_Hs[i] );
      iEvent.getByLabel( fElectronLabels[i] , fGsfElectronList_Hs[i] );
      iEvent.getByLabel( fTauLabels[i] , fTauList_Hs[i] );
      if( fDebug > 1 ) { 
         std::cerr << "\t[t]Getting Lepton Collection" << i << std::endl
            << "\tMuon Label:" << fMuonLabels[i] << " with " << fMuonList_Hs[i]->size() << " entries" << std::endl 
            << "\tElectron Label:" << fElectronLabels[i] << " with " << fElectronList_Hs[i]->size() << " entries" << std::endl 
            << "\tTau Label:" << fTauLabels[i] << " with " << fTauList_Hs[i]->size() << " entries" << std::endl ;
      }
   }
   if( fDebug > 1 ) { std::cerr <<"\t[1]Getting Electron ID maps" << std::endl;}
   iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", fTrackBuilder_H );
   iEvent.getByLabel( fConversionsTag        , fConversions_H       ) ;
   iEvent.getByToken( fElectronIDVetoToken   , fElectronIDVeto_H   ) ;
   iEvent.getByToken( fElectronIDLooseToken  , fElectronIDLoose_H  ) ;
   iEvent.getByToken( fElectronIDMediumToken , fElectronIDMedium_H ) ;
   iEvent.getByToken( fElectronIDTightToken  , fElectronIDTight_H  ) ;
   iEvent.getByToken( fElectronIDHEEPToken   , fElectronIDHEEP_H   ) ;

   
   for( unsigned il = 0; il < fPhotonLabels.size(); il++ ) {
      fPhotonList_Hs.push_back( PhotonHandle() );
      iEvent.getByLabel( fPhotonLabels[il], fPhotonList_Hs[il] );
      if( fDebug > 1 ) { 
         std::cerr << "\t[1]photons " << il << " phoLabel " << fPhotonLabels[il] 
            << " with " << fPhotonList_Hs[il]->size() << " entries\n"; }
   }
   if( fDebug > 1 ) { std::cerr <<"\t[1]Getting Photon ID maps" << std::endl;}
   iEvent.getByToken( fPhotonLooseIDToken            , fPhotonIDLoose             ) ;
   iEvent.getByToken( fPhotonMediumIDToken           , fPhotonIDMedium            ) ;
   iEvent.getByToken( fPhotonTightIDToken            , fPhotonIDTight             ) ;
   iEvent.getByToken( fPhotonIsolation_Charged_Token , fPhotonIsolation_Charged_H ) ;
   iEvent.getByToken( fPhotonIsolation_Neutral_Token , fPhotonIsolation_Neutral_H ) ;
   iEvent.getByToken( fPhotonIsolation_Photon_Token  , fPhotonIsolation_Photon_H  ) ;
   iEvent.getByToken( fPhotonSignaIEtaIEtaToken      , fPhotonSigmaIEtaIEta_H     ) ;
}

DEFINE_FWK_MODULE( bprimeKit );
