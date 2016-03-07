/*******************************************************************************

 *  Filename    : bprimeKit.cc
 *  Description : The virtual and explicit functions for the bprimeKit
 *
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

#include <string>
#include <TFile.h>
#include <TTree.h>

#include "FWCore/Framework/interface/MakerMacros.h"          // For plugin definition
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
//------------------------------------------------------------------------------
//   Custom enums, typedefs and macros
//------------------------------------------------------------------------------
//#define Fill_DIJET_PAIRS 1      //Uncomment for jet pair processing
typedef std::vector<edm::InputTag> TagList;
typedef std::vector<std::string>   StrList;


//------------------------------------------------------------------------------
//   bprimeKit methods: constructor and destructor
//------------------------------------------------------------------------------
bprimeKit::bprimeKit( const edm::ParameterSet& iConfig ) :
   fPhotonEffectiveArea_ChargeHadron  ((iConfig.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
   fPhotonEffectiveArea_NeutralHadron ((iConfig.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath()),
   fPhotonEffectiveArea_Photons    ((iConfig.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath()   )
{
   //----- Configuration flags  ---------------------------------------------------
   fPairCollectionType = iConfig.getParameter<int> ( "PairCollection"  ) ;
   fSkipfGenInfo       = iConfig.getParameter<bool>( "SkipGenInfo"     ) ;
   fIncludeL7          = iConfig.getParameter<bool>( "IncludeL7"       ) ;
   fDebug              = iConfig.getParameter<int> ( "Debug"           ) ;
   fRunOnB2G           = iConfig.getParameter<bool>( "runOnB2G"        ) ;
   fRunMuonJetCleaning = iConfig.getParameter<bool>( "runMuonJetClean" ) ;

   //----- Event related  -----------------------------------------------------------------------------
   fRhoToken      = consumes<double>(iConfig.getParameter<edm::InputTag>("rhoLabel"));
   fHLTToken      = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag> ( "hltLabel"    ) );
   fMETToken      = consumes<METList>(iConfig.getParameter<edm::InputTag> ( "metLabel"    ) );
   fPuppiMETToken = consumes<METList>(iConfig.getParameter<edm::InputTag> ("puppimetLabel"));
   fPileupToken   = consumes<PileupList>(iConfig.getParameter<edm::InputTag> ( "puInfoLabel" ) );
   fPackedCandToken = consumes<pat::PackedCandidateCollection>( iConfig.getParameter<edm::InputTag>("packedCand") );

   //----- Vertex related  ----------------------------------------------------------------------------
   fPrimaryVertexToken            = consumes<VertexList> (iConfig.getParameter<edm::InputTag>( "offlinePVLabel"   ) );
   fPrimVertex_withBeamSpot_Token = consumes<VertexList> (iConfig.getParameter<edm::InputTag>( "offlinePVBSLabel" ) );
   fBeamspotToken                 = consumes<reco::BeamSpot> (iConfig.getParameter<edm::InputTag>( "offlineBSLabel"   ) );

   //----- fGenInfo related  ---------------------------------------------------------------------------
   fGenEventToken    = consumes<GenEventInfoProduct>          ( iConfig.getParameter<edm::InputTag> ( "genevtLabel" )  ) ;
   fGenParticleToken = consumes<GenList>                      ( iConfig.getParameter<edm::InputTag> ( "genLabel"    )  ) ;
   fGenDigiToken     = consumes<L1GlobalTriggerReadoutRecord> ( iConfig.getParameter<edm::InputTag> ( "gtdigiLabel" )  ) ;
   fLHEToken         = consumes<LHEEventProduct>              ( iConfig.getParameter<edm::InputTag> ( "lheLabel"    )  ) ;
   fLHERunToken      = consumes<LHERunInfoProduct,edm::InRun>   ( iConfig.getParameter<edm::InputTag> ( "lheRunLabel"    )  ) ;

   //----- Jet related  -------------------------------------------------------------------------------
   for( const auto& jetsetting : iConfig.getParameter<std::vector<edm::ParameterSet>>( "JetSettings" ) ){
      fJetCollections.push_back( jetsetting.getParameter<std::string>( "jetCollection" ) ) ;
      fJetTokens.push_back( consumes<JetList>( jetsetting.getParameter<edm::InputTag>("jetLabel") ) );
      fSubjetTokens.push_back( consumes<JetList>( jetsetting.getParameter<edm::InputTag>("subjetLabel") ) );
   }

   //----- Lepton related  ----------------------------------------------------------------------------
   fLeptonCollections      = iConfig.getParameter<StrList>( "LepCollections" ) ; //branch names
   const auto muontag_list = iConfig.getParameter<TagList>( "muonLabel"      ) ;
   const auto electag_list = iConfig.getParameter<TagList>( "elecLabel"      ) ;
   const auto tautag_list  = iConfig.getParameter<TagList>( "tauLabel"       ) ;
   for( unsigned i = 0 ; i < muontag_list.size() ; ++i ) {
      fMuonTokens.push_back( consumes<MuonList>(muontag_list[i]) );
      fElectronTokens.push_back( consumes<ElectronList>(electag_list[i]) );
      fTauTokens.push_back( consumes<TauList>(tautag_list[i]) );
   }
   fElectronIDVetoToken    = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleVetoIdMap"    )) ;
   fElectronIDLooseToken   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleLooseIdMap"   )) ;
   fElectronIDMediumToken  = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleMediumIdMap"  )) ;
   fElectronIDTightToken   = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleTightIdMap"   )) ;
   fElectronIDHEEPToken    = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "eleHEEPIdMap"    )) ;
   fConversionsTag         = consumes<reco::ConversionCollection>( iConfig.getParameter<edm::InputTag>("conversionsLabel") );

   //----- Photon related  ----------------------------------------------------------------------------
   fPhotonCollections     = iConfig.getParameter<StrList> ( "PhoCollections" ) ; //branch names
   const auto photag_list = iConfig.getParameter<TagList> ( "phoLabel"       ) ;
   for( const auto& photag : photag_list ){
      fPhotonTokens.push_back( consumes<PhotonList>( photag ) ); }
   fPhotonLooseIDToken            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoLooseIdMap"             )) ;
   fPhotonMediumIDToken           = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoMediumIdMap"            )) ;
   fPhotonTightIDToken            = consumes<edm::ValueMap<bool>> (iConfig.getParameter<edm::InputTag>( "phoTightIdMap"             )) ;
   fPhotonIsolation_Charged_Token = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoChargedIsolation"       )) ;
   fPhotonIsolation_Neutral_Token = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoNeutralHadronIsolation" )) ;
   fPhotonIsolation_Photon_Token  = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "phoPhotonIsolation"        )) ;
   fPhotonSignaIEtaIEtaToken      = consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>( "full5x5SigmaIEtaIEtaMap"   )) ;

   for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) {
      fHighLevelTriggerMap.insert( pair<std::string,int>( TriggerBooking[i], i ) ) ; }

   /***** MADITORY!! DO NOTE REMOVE  *********************************************/
   edm::Service<TFileService> fs;
   TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
   /******************************************************************************/

   InitJetEnergyCorrectors();
   if( fDebug ) { cout << "[0] Finished creating bprimeKit class" << endl; }
}

bprimeKit::~bprimeKit()
{
   ClearJetEnergyCorrector();
}


/*******************************************************************************
 *
 *  Note :
 *    1. All writing objects (TTrees, TH1Fs) MUST be created here and
 *       not in the analyzer constructor!!
 *    2. Do not use the WRITE functions and delete operator!! These are
 *       automatically handled by the TFileService instance.
 *
*******************************************************************************/
void bprimeKit::beginJob()
{
   fBaseTree = new TTree( "root", "root" );
   fRunTree  = new TTree( "run" , "run"  );
   InitTree();
}
void bprimeKit::endJob()
{
   // fBaseTree->Write();
   // delete fBaseTree;
   ClearTree();
}


//------------------------------------------------------------------------------
//   bprimeKit event based analysis methods
//------------------------------------------------------------------------------
void bprimeKit::beginRun( const edm::Run& iRun, const edm::EventSetup& iSetup )
{
}
void bprimeKit::endRun( const edm::Run& iRun, const edm::EventSetup& iSetup)
{
   GetRunObjects( iRun, iSetup );
   FillRunInfo();
   fRunTree->Fill();
}


void bprimeKit::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   if( fDebug > 0 ) { cout << "[0] Begin Analysis!!" << endl; }

   GetEventObjects( iEvent, iSetup );
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

//------------------------------------------------------------------------------
//   Tree set-up options
//------------------------------------------------------------------------------
void bprimeKit::InitTree()
{
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

   //----- Setting the Run Information tree  --------------------------------------
   fRunInfo.RegisterTree( fRunTree );
}

void bprimeKit::ClearTree()
{
   /***** DO NOT DELETE TREES!  **************************************************/
}

//------------------------------------------------------------------------------
//   Jet Energy correction
//------------------------------------------------------------------------------
void bprimeKit::InitJetEnergyCorrectors()
{
   if( fDebug ) { cout << "[0] Initializating Jet Enery corrections" << endl ; }
   JetCorrectorParameters L3JetPar_    ("./JECs/PHYS14_25_V2_L3Absolute_AK4PFchs.txt" );
   JetCorrectorParameters L2JetPar_    ("./JECs/PHYS14_25_V2_L2Relative_AK4PFchs.txt" );
   JetCorrectorParameters L1JetPar_    ("./JECs/PHYS14_25_V2_L1FastJet_AK4PFchs.txt"  );
   JetCorrectorParameters L3JetParAK8_ ("./JECs/PHYS14_25_V2_L3Absolute_AK8PFchs.txt" );
   JetCorrectorParameters L2JetParAK8_ ("./JECs/PHYS14_25_V2_L2Relative_AK8PFchs.txt" );
   JetCorrectorParameters L1JetParAK8_ ("./JECs/PHYS14_25_V2_L1FastJet_AK8PFchs.txt"  );

   if( fDebug ) { cout << "[0] Finished Making Jet Corrector Parameters" << endl; }
   // IMPORTANT: THE ORDER MATTERS HERE !!!!

   vector<JetCorrectorParameters> vPar;
   vector<JetCorrectorParameters> vParAK8;

   vPar.clear();
   vParAK8.clear();
   vPar.push_back(L1JetPar_);
   vPar.push_back(L2JetPar_);
   vPar.push_back(L3JetPar_);
   vParAK8.push_back(L1JetParAK8_);
   vParAK8.push_back(L2JetParAK8_);
   vParAK8.push_back(L3JetParAK8_);

   // fJetCorrector    = new FactorizedJetCorrector(vPar);
   // fJetCorrectorAK8 = new FactorizedJetCorrector(vParAK8);
   if( fDebug ) { cout << "[0] Finished Making Jet Correctors" << endl;}

}


void bprimeKit::ClearJetEnergyCorrector()
{
   // delete fJetCorrector;
   // delete fJetCorrectorAK8;
}


void bprimeKit::GetEventObjects( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( fDebug > 1 ){ std::cerr << "\t[1]Getting Event Wide Handles" << std::endl; }
   iEvent.getByToken( fRhoToken                                 , fRho_H                ) ;
   iEvent.getByToken( fMETToken                                 , fMET_H                ) ;
   iEvent.getByToken( fPuppiMETToken                            , fPuppiMET_H           ) ;
   iEvent.getByToken( fBeamspotToken                            , fBeamSpot_H           ) ;
   // iEvent.getByToken( "patType1CorrectedPFMetUnclusteredEnUp"   , fMETTempPlus_H        ) ;
   // iEvent.getByToken( "patType1CorrectedPFMetUnclusteredEnDown" , fMETTempDown_H        ) ;
   iEvent.getByToken( fGenDigiToken                             , fRecord_H             ) ;
   iEvent.getByToken( fHLTToken                                 , fTrigger_H            ) ;
   iEvent.getByToken( fPrimaryVertexToken                       , fVertex_H             ) ;
   iEvent.getByToken( fPrimVertex_withBeamSpot_Token            , fVertexWithBeamSpot_H ) ;

   bool changed = true;
   fHighLevelTriggerConfig.init( iEvent.getRun(), iSetup, "HLT", changed );

   if( !iEvent.isRealData() ) {
      iEvent.getByToken( fPileupToken, fPileup_H );
      if( !fSkipfGenInfo ) {
         iEvent.getByToken( fGenParticleToken, fGenParticle_H );
         iEvent.getByToken( fGenEventToken, fGenEvent_H );
         iEvent.getByToken( fLHEToken , fLHEInfo_H );
      }
   }

   for( unsigned i = 0; i < fJetTokens.size(); ++i ) {
      fJetList_Hs.push_back( JetHandle() );
      fSubjetList_Hs.push_back( JetHandle() );
      iEvent.getByToken( fJetTokens[i], fJetList_Hs[i] );
      iEvent.getByToken( fSubjetTokens[i] , fSubjetList_Hs[i] );
      if( fDebug > 1 ) {
         std::cout << "\t[1]Getting Jet Collection " << i << " : "
            << fJetList_Hs[i]->size() << " entries" << std::endl ;
         if( fSubjetList_Hs[i].isValid() ){
            std::cout << "\t[1]Getting Subhet Collection " << i << " : "
               << fSubjetList_Hs[i]->size() << " entries" << std::endl ;
         }
      }
   }

   for( unsigned i = 0 ; i < fMuonTokens.size() ; ++i ){
      fMuonList_Hs.push_back( MuonHandle() );
      fElectronList_Hs.push_back( ElectronHandle() ) ;
      fTauList_Hs.push_back( TauHandle() );
      iEvent.getByToken( fMuonTokens[i] , fMuonList_Hs[i] );
      iEvent.getByToken( fElectronTokens[i] , fElectronList_Hs[i] );
      iEvent.getByToken( fTauTokens[i] , fTauList_Hs[i] );
      if( fDebug > 1 ) {
         std::cerr << "\t[1]Getting Lepton Collection" << i << std::endl
            << "\tMuon      Handle:" << fMuonList_Hs[      i]->size() << "  entries" <<  std::endl
            << "\tElectron  Handle"  << fElectronList_Hs[  i]->size() << "  entries" <<  std::endl
            << "\tTau       Handle:" << fTauList_Hs[       i]->size() << "  entries" <<  std::endl ;
      }
   }
   iEvent.getByToken( fPackedCandToken                          , fPackedCand_H         ) ;
   iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", fTrackBuilder_H );
   if( fDebug > 1 ) { std::cerr <<"\t[1]Getting Electron ID maps" << std::endl;}
   iEvent.getByToken( fConversionsTag        , fConversions_H      ) ;
   iEvent.getByToken( fElectronIDVetoToken   , fElectronIDVeto_H   ) ;
   iEvent.getByToken( fElectronIDLooseToken  , fElectronIDLoose_H  ) ;
   iEvent.getByToken( fElectronIDMediumToken , fElectronIDMedium_H ) ;
   iEvent.getByToken( fElectronIDTightToken  , fElectronIDTight_H  ) ;
   iEvent.getByToken( fElectronIDHEEPToken   , fElectronIDHEEP_H   ) ;


   for( unsigned il = 0; il < fPhotonTokens.size(); il++ ) {
      fPhotonList_Hs.push_back( PhotonHandle() );
      iEvent.getByToken( fPhotonTokens[il], fPhotonList_Hs[il] );
      if( fDebug > 1 ) {
         std::cout << "\t[1]photons " << il
            << " with " << fPhotonList_Hs[il]->size() << " entries" << endl; }
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
