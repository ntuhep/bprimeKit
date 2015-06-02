/*******************************************************************************
 *
 *  Filename    : format.h
 *  Description : The branch format for the bprimeKit nTuple
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __BPRIMEKIT_FORMAT_H__
#define __BPRIMEKIT_FORMAT_H__

//------------------------------  Required libraries  -------------------------------
#include <TTree.h>
#include <vector>

//-------------------------------  Size limitations  --------------------------------
#define MAX_LEPTONS        256
#define MAX_TRACKS         256
#define MAX_JETS           128
#define MAX_PAIRS          512
#define MAX_PHOTONS        128
#define MAX_GENS           128
#define MAX_Vertices       256
#define MAX_BX             128
#define N_TRIGGER_BOOKINGS 5842 

//------------------------------------------------------------------------------ 
//   Macros functions for analysis cleaner code 
//------------------------------------------------------------------------------
static char branchName[1024];
static char leafList[1024];
#define  BRANCH_OBJ( DATA , TYPE )  \
   sprintf( branchName , "%s.%s" , name.c_str() , #DATA ) ; \
   sprintf( leafList   , "%s.%s/%s" , name.c_str() , #DATA , #TYPE ); \
   root->Branch( branchName , (void*)&DATA  , leafList );

#define BRANCH_ARRAY( DATA , TYPE ) \
   sprintf( branchName , "%s.%s" , name.c_str() , #DATA ) ; \
   sprintf( leafList   , "%s.%s[%s.Size]/%s" , name.c_str() , #DATA , name.c_str() , #TYPE ); \
   root->Branch( branchName , (void*)DATA  , leafList );

#define BRANCH_FIX_ARRAY( DATA , SIZE , TYPE ) \
   sprintf( branchName , "%s.%s" , name.c_str() , #DATA ) ; \
   sprintf( leafList   , "%s.%s[%d]/%s" , name.c_str() , #DATA , SIZE , #TYPE ); \
   root->Branch( branchName , (void*)DATA  , leafList );

#define SET_BRANCH_ARRAY( DATA ) \
   sprintf( branchName , "%s.%s" , name.c_str() , #DATA ) ; \
   root->SetBranchAddress( branchName ,  DATA );

#define SET_BRANCH_OBJ( DATA ) \
   sprintf( branchName , "%s.%s" , name.c_str() , #DATA ) ; \
   root->SetBranchAddress( branchName , & DATA );


//------------------------------------------------------------------------------ 
//   Begin class definition
//------------------------------------------------------------------------------ 
class EvtInfoBranches {
public:
   int   RunNo;
   long long int   EvtNo;
   int   BxNo;
   int   LumiNo;
   int   Orbit;
   //--------------------------------  MC information  ---------------------------------
   int   McFlag            ; // MC or not MC, that's the question
   int   McSigTag          ; // MC Signature tag    - 0: others, 1: 2L (opposite-sign), 2: 2L (same-sign), 3: 3L, 4: 4L
   int   McbprimeMode [2 ] ; // b'(bar) decay mode   - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
   int   MctprimeMode [2 ] ; // t'(bar) decay mode   - 0: others, 1: bW, 2: tZ, 3: tH, 4: tgamma
   int   McWMode      [4 ] ; // W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar - 0: others, 1: enu, 2: munu, 3: taunu, 4: jj
   int   McZMode      [2 ] ; // Z from b'(bar)/t'(t'bar)  - 0: others, 1: ee, 2: mumu, 3: tautau, 4: nunu, 5: bb, 6: jj
   float McbprimeMass [2 ] ; // mass: b'(bar)
   float MctprimeMass [2 ] ; // mass: t'(bar)
   float MctopMass    [2 ] ; // mass: top(bar)
   float McWMass      [4 ] ; // mass: W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar
   float McZMass      [2 ] ; // mass: Z from b'(bar) or t'(bar)
   float McDauPt      [14] ; // Generic MC daughter information
   float McDauEta     [14] ; // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters
   float McDauPhi     [14] ;
   int   McDauPdgID   [14] ;
   //-----------------------------  Isolation information  -----------------------------
   float RhoPU  [2];       // [electron,muon]
   float SigmaPU[2];      // [electron,muon]
   //-----------------------------------  Beam spot  -----------------------------------
   float BeamSpotX;
   float BeamSpotY;
   float BeamSpotZ;
   //--------------------------------  PU information  ---------------------------------
   int   nBX            ;
   int   nPU    [MAX_BX];
   int   BXPU   [MAX_BX];
   float TrueIT [MAX_BX];
   //--------------------------------  PDF information  --------------------------------
   int   PDFid1   ;
   int   PDFid2   ;
   float PDFx1    ;
   float PDFx2    ;
   float PDFscale ;
   float PDFv1    ;
   float PDFv2    ;
   //---------------------------  partilce flow information  ---------------------------
   float PFMET                                     ;
   float PFMETType1CorrectedPFMetUnclusteredEnUp   ;
   float PFMETType1CorrectedPFMetUnclusteredEnDown ;
   float PFMETPhi                                  ;
   float PFRawMET                                  ;
   float PFRawMETPhi                               ;
   float PFSumEt                                   ;
   float PFMETSig                                  ;
   float PFMETlongitudinal                         ;
   float PFMETRealSig                              ;
   float PFGenMET                                  ;
   float PFGenMETPhi                               ;
   float PFMETx                                    ; //Uly 2011-04-04
   float PFMETy                                    ; //Uly 2011-04-04
   //------------------------------  Tirgger information  ------------------------------
   int   TrgCount                    ; // No. of fired booking bits
   int   nTrgBook                    ;
   char  TrgBook[N_TRIGGER_BOOKINGS] ; // Trigger bits, reserved up to 120 entries
   int   nHLT                        ;
   float HighPurityFraction          ; //Added by Dmitry to help filter out bad events
   int   NofTracks                   ; //Added by Dmitry to help filter out bad events
   float ptHat                       ;
   int   HLTPrescaleFactor[512]      ;
   int   HLTName2enum[512]           ;
   bool  HLTbits[N_TRIGGER_BOOKINGS] ;
   int   L1[128]                     ; // L1 trigger bits
   int   TT[64]                      ; // Techical trigger bits


   void RegisterTree( TTree* root , std::string name = "EvtInfo" ) {
      BRANCH_OBJ( RunNo                                     , F );
      BRANCH_OBJ( EvtNo                                     , L );
      BRANCH_OBJ( BxNo                                      , F );
      BRANCH_OBJ( LumiNo                                    , F );
      BRANCH_OBJ( Orbit                                     , F );
      BRANCH_OBJ( McFlag                                    , F );
      BRANCH_OBJ( McSigTag                                  , F );
      BRANCH_OBJ( PDFid1                                    , F );
      BRANCH_OBJ( PDFid2                                    , F );
      BRANCH_OBJ( PDFx1                                     , F );
      BRANCH_OBJ( PDFx2                                     , F );
      BRANCH_OBJ( PDFscale                                  , F );
      BRANCH_OBJ( PDFv1                                     , F );
      BRANCH_OBJ( PDFv2                                     , F );
      BRANCH_OBJ( BeamSpotX                                 , F );
      BRANCH_OBJ( BeamSpotY                                 , F );
      BRANCH_OBJ( BeamSpotZ                                 , F );
      BRANCH_OBJ( nBX                                       , F );
      BRANCH_OBJ( PFMET                                     , F );
      BRANCH_OBJ( PFMETType1CorrectedPFMetUnclusteredEnUp   , F );
      BRANCH_OBJ( PFMETType1CorrectedPFMetUnclusteredEnDown , F );
      BRANCH_OBJ( PFMETPhi                                  , F );
      BRANCH_OBJ( PFRawMET                                  , F );
      BRANCH_OBJ( PFRawMETPhi                               , F );
      BRANCH_OBJ( PFSumEt                                   , F );
      BRANCH_OBJ( PFMETSig                                  , F );
      BRANCH_OBJ( PFMETlongitudinal                         , F );
      BRANCH_OBJ( PFMETRealSig                              , F );
      BRANCH_OBJ( PFGenMET                                  , F );
      BRANCH_OBJ( PFGenMETPhi                               , F );
      BRANCH_OBJ( PFMETx                                    , F );
      BRANCH_OBJ( PFMETy                                    , F );
      BRANCH_OBJ( TrgCount                                  , F );
      BRANCH_OBJ( nTrgBook                                  , F );
      BRANCH_OBJ( nHLT                                      , F );
      BRANCH_OBJ( HighPurityFraction                        , F );
      BRANCH_OBJ( NofTracks                                 , F );
      BRANCH_OBJ( ptHat                                     , F );
      BRANCH_FIX_ARRAY( nPU                                 , MAX_BX             , I ) ;
      BRANCH_FIX_ARRAY( BXPU                                , MAX_BX             , I ) ;
      BRANCH_FIX_ARRAY( TrueIT                              , MAX_BX             , F ) ;
      BRANCH_FIX_ARRAY( TrgBook                             , N_TRIGGER_BOOKINGS , B ) ;
      BRANCH_FIX_ARRAY( HLTPrescaleFactor                   , 512                , I ) ;
      BRANCH_FIX_ARRAY( HLTName2enum                        , 512                , I ) ;
      BRANCH_FIX_ARRAY( HLTbits                             , N_TRIGGER_BOOKINGS , O ) ;
      BRANCH_FIX_ARRAY( L1                                  , 128                , I ) ;
      BRANCH_FIX_ARRAY( TT                                  , 64                 , I ) ;
      BRANCH_FIX_ARRAY( McbprimeMode                        , 2                  , I ) ;
      BRANCH_FIX_ARRAY( MctprimeMode                        , 2                  , I ) ;
      BRANCH_FIX_ARRAY( McWMode                             , 4                  , I ) ;
      BRANCH_FIX_ARRAY( McZMode                             , 2                  , I ) ;
      BRANCH_FIX_ARRAY( McbprimeMass                        , 2                  , F ) ;
      BRANCH_FIX_ARRAY( MctprimeMass                        , 2                  , F ) ;
      BRANCH_FIX_ARRAY( MctopMass                           , 2                  , F ) ;
      BRANCH_FIX_ARRAY( McWMass                             , 4                  , F ) ;
      BRANCH_FIX_ARRAY( McZMass                             , 2                  , F ) ;
      BRANCH_FIX_ARRAY( McDauPt                             , 14                 , F ) ;
      BRANCH_FIX_ARRAY( McDauEta                            , 14                 , F ) ;
      BRANCH_FIX_ARRAY( McDauPhi                            , 14                 , F ) ;
      BRANCH_FIX_ARRAY( RhoPU                               , 2                  , F ) ;
      BRANCH_FIX_ARRAY( SigmaPU                             , 2                  , F ) ;
      BRANCH_FIX_ARRAY( McDauPdgID                          , 14                 , I ) ;
   }

   void Register( TTree* root , std::string name="EvtInfo") {
      SET_BRANCH_OBJ( RunNo                                     );
      SET_BRANCH_OBJ( EvtNo                                     );
      SET_BRANCH_OBJ( BxNo                                      );
      SET_BRANCH_OBJ( LumiNo                                    );
      SET_BRANCH_OBJ( Orbit                                     );
      SET_BRANCH_OBJ( McFlag                                    );
      SET_BRANCH_OBJ( McSigTag                                  );
      SET_BRANCH_OBJ( PDFid1                                    );
      SET_BRANCH_OBJ( PDFid2                                    );
      SET_BRANCH_OBJ( PDFx1                                     );
      SET_BRANCH_OBJ( PDFx2                                     );
      SET_BRANCH_OBJ( PDFscale                                  );
      SET_BRANCH_OBJ( PDFv1                                     );
      SET_BRANCH_OBJ( PDFv2                                     );
      SET_BRANCH_OBJ( BeamSpotX                                 );
      SET_BRANCH_OBJ( BeamSpotY                                 );
      SET_BRANCH_OBJ( BeamSpotZ                                 );
      SET_BRANCH_OBJ( PFMET                                     );
      SET_BRANCH_OBJ( PFMETType1CorrectedPFMetUnclusteredEnUp   );
      SET_BRANCH_OBJ( PFMETType1CorrectedPFMetUnclusteredEnDown );
      SET_BRANCH_OBJ( PFMETPhi                                  );
      SET_BRANCH_OBJ( PFRawMET                                  );
      SET_BRANCH_OBJ( PFRawMETPhi                               );
      SET_BRANCH_OBJ( PFSumEt                                   );
      SET_BRANCH_OBJ( PFMETSig                                  );
      SET_BRANCH_OBJ( PFMETlongitudinal                         );
      SET_BRANCH_OBJ( PFMETRealSig                              );
      SET_BRANCH_OBJ( PFGenMET                                  );
      SET_BRANCH_OBJ( PFGenMETPhi                               );
      SET_BRANCH_OBJ( PFMETx                                    ); //Uly 2011-04-04
      SET_BRANCH_OBJ( PFMETy                                    ); //Uly 2011-04-04
      SET_BRANCH_OBJ( TrgCount                                  );
      SET_BRANCH_OBJ( nTrgBook                                  );
      SET_BRANCH_OBJ( HighPurityFraction                        );
      SET_BRANCH_OBJ( NofTracks                                 );
      SET_BRANCH_OBJ( nHLT                                      );
      SET_BRANCH_OBJ( nBX                                       );
      SET_BRANCH_OBJ( ptHat                                     );
      SET_BRANCH_ARRAY( HLTbits                                 );
      SET_BRANCH_ARRAY( TrgBook                                 );
      SET_BRANCH_ARRAY( HLTPrescaleFactor                       );
      SET_BRANCH_ARRAY( HLTName2enum                            );
      SET_BRANCH_ARRAY( RhoPU                                   );
      SET_BRANCH_ARRAY( SigmaPU                                 );
      SET_BRANCH_ARRAY( L1                                      );
      SET_BRANCH_ARRAY( TT                                      );
      SET_BRANCH_ARRAY( nPU                                     );
      SET_BRANCH_ARRAY( BXPU                                    );
      SET_BRANCH_ARRAY( TrueIT                                  );
      SET_BRANCH_ARRAY( McbprimeMode                            );
      SET_BRANCH_ARRAY( MctprimeMode                            );
      SET_BRANCH_ARRAY( McWMode                                 );
      SET_BRANCH_ARRAY( McZMode                                 );
      SET_BRANCH_ARRAY( McbprimeMass                            );
      SET_BRANCH_ARRAY( MctprimeMass                            );
      SET_BRANCH_ARRAY( MctopMass                               );
      SET_BRANCH_ARRAY( McWMass                                 );
      SET_BRANCH_ARRAY( McZMass                                 );
      SET_BRANCH_ARRAY( McDauPt                                 );
      SET_BRANCH_ARRAY( McDauEta                                );
      SET_BRANCH_ARRAY( McDauPhi                                );
      SET_BRANCH_ARRAY( McDauPdgID                              );
   }
};


class LepInfoBranches {
public:
   int   Size;
   int   Index                                 [MAX_LEPTONS] ;
   int   isEcalDriven                          [MAX_LEPTONS] ;
   int   isTrackerDriven                       [MAX_LEPTONS] ;
   int   LeptonType                            [MAX_LEPTONS] ;
   int   Charge                                [MAX_LEPTONS] ;
   int   ChargeGsf                             [MAX_LEPTONS] ;
   int   ChargeCtf                             [MAX_LEPTONS] ;
   int   ChargeScPix                           [MAX_LEPTONS] ;
   float Pt                                    [MAX_LEPTONS] ;
   float Et                                    [MAX_LEPTONS] ;
   float Eta                                   [MAX_LEPTONS] ;
   float caloEta                               [MAX_LEPTONS] ;
   float Phi                                   [MAX_LEPTONS] ;
   float e1x5                                  [MAX_LEPTONS] ;
   float e2x5Max                               [MAX_LEPTONS] ;
   float e5x5                                  [MAX_LEPTONS] ;
   float Px                                    [MAX_LEPTONS] ; //Uly 2011-04-04
   float Py                                    [MAX_LEPTONS] ; //Uly 2011-04-04
   float Pz                                    [MAX_LEPTONS] ; //Uly 2011-04-04
   float Energy                                [MAX_LEPTONS] ; //Uly 2011-04-04
   float TrackIso                              [MAX_LEPTONS] ;
   float EcalIso                               [MAX_LEPTONS] ;
   float HcalIso                               [MAX_LEPTONS] ;
   float HcalDepth1Iso                         [MAX_LEPTONS] ;
   float HcalDepth2Iso                         [MAX_LEPTONS] ;
   float ChargedHadronIso                      [MAX_LEPTONS] ;
   float NeutralHadronIso                      [MAX_LEPTONS] ;
   float PhotonIso                             [MAX_LEPTONS] ;
   float ChargedHadronIsoR03                   [MAX_LEPTONS] ;
   float NeutralHadronIsoR03                   [MAX_LEPTONS] ;
   float PhotonIsoR03                          [MAX_LEPTONS] ;
   float sumPUPtR03                            [MAX_LEPTONS] ;
   float IsoRhoCorrR03                         [MAX_LEPTONS] ;
   float ChargedHadronIsoR04                   [MAX_LEPTONS] ;
   float NeutralHadronIsoR04                   [MAX_LEPTONS] ;
   float PhotonIsoR04                          [MAX_LEPTONS] ;
   float sumPUPtR04                            [MAX_LEPTONS] ;
   float IsoRhoCorrR04                         [MAX_LEPTONS] ;
   float Ip3dPV                                [MAX_LEPTONS] ;
   float Ip3dPVErr                             [MAX_LEPTONS] ;
   float Ip3dPVSignificance                    [MAX_LEPTONS] ;
   //-------------------------------  Muon information  --------------------------------
   int   MuontimenDof                          [MAX_LEPTONS] ;
   float MuontimeAtIpInOut                     [MAX_LEPTONS] ;
   float MuontimeAtIpOutIn                     [MAX_LEPTONS] ;
   int   Muondirection                         [MAX_LEPTONS] ;
   float CaloEnergy                            [MAX_LEPTONS] ;
   bool isGoodMuonTMOneStationTight            [MAX_LEPTONS] ; // For Soft Muon
   float innerTracknormalizedChi2              [MAX_LEPTONS] ;   // For Soft Muon
   float vertexZ                               [MAX_LEPTONS] ; //Uly 2011-04-04
   bool  isPFMuon                              [MAX_LEPTONS] ;
   bool  MuIDGlobalMuonPromptTight             [MAX_LEPTONS] ;
   float MuInnerPtError                        [MAX_LEPTONS] ;
   float MuGlobalPtError                       [MAX_LEPTONS] ;
   float MuInnerTrackDz                        [MAX_LEPTONS] ;
   float MuInnerTrackD0                        [MAX_LEPTONS] ;
   float MuInnerTrackDxy_BS                    [MAX_LEPTONS] ;
   float MuInnerTrackDxy_PV                    [MAX_LEPTONS] ;
   float MuInnerTrackDxy_PVBS                  [MAX_LEPTONS] ;
   int   MuInnerTrackNHits                     [MAX_LEPTONS] ;
   int   MuNTrackerHits                        [MAX_LEPTONS] ;
   float MuGlobalNormalizedChi2                [MAX_LEPTONS] ; // Dmitry
   float MuCaloCompat                          [MAX_LEPTONS] ;
   int   MuNChambers                           [MAX_LEPTONS] ;
   int   MuNChambersMatchesSegment             [MAX_LEPTONS] ;
   int   MuNMatchedStations                    [MAX_LEPTONS] ;
   int   MuNPixelLayers                        [MAX_LEPTONS] ;
   int   MuNPixelLayersWMeasurement            [MAX_LEPTONS] ; //Uly 2011-04-04
   int   MuNTrackLayersWMeasurement            [MAX_LEPTONS] ;
   int   MuNLostInnerHits                      [MAX_LEPTONS] ;
   int   MuNLostOuterHits                      [MAX_LEPTONS] ;
   int   MuNMuonhits                           [MAX_LEPTONS] ;
   int   MuDThits                              [MAX_LEPTONS] ;
   int   MuCSChits                             [MAX_LEPTONS] ;
   int   MuRPChits                             [MAX_LEPTONS] ;
   int   MuType                                [MAX_LEPTONS] ;
   //-----------------------------  Electron Information  ------------------------------
   float EgammaMVANonTrig                      [MAX_LEPTONS] ; // Add by Jacky
   float EgammaMVATrig                         [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdTRIGGERTIGHT        [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdTRIGGERWP70         [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdVETO                [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdLOOSE               [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdMEDIUM              [MAX_LEPTONS] ; // Add by Jacky
   bool EgammaCutBasedEleIdTIGHT               [MAX_LEPTONS] ; // Add by Jacky
   float Eldr03HcalDepth1TowerSumEtBc          [MAX_LEPTONS] ;
   float Eldr03HcalDepth2TowerSumEtBc          [MAX_LEPTONS] ;
   float Eldr04HcalDepth1TowerSumEtBc          [MAX_LEPTONS] ;
   float Eldr04HcalDepth2TowerSumEtBc          [MAX_LEPTONS] ;
   float ElhcalOverEcalBc                      [MAX_LEPTONS] ;
   float ElEcalE                               [MAX_LEPTONS] ;
   float ElEoverP                              [MAX_LEPTONS] ;
   float EldeltaEta                            [MAX_LEPTONS] ;
   float EldeltaPhi                            [MAX_LEPTONS] ;
   float ElHadoverEm                           [MAX_LEPTONS] ;
   float ElsigmaIetaIeta                       [MAX_LEPTONS] ; // Jacky
   float ElscSigmaIetaIeta                     [MAX_LEPTONS] ;  // Jacky
   float ElEnergyErr                           [MAX_LEPTONS] ;
   float ElMomentumErr                         [MAX_LEPTONS] ;
   int   ElTrackNHits                          [MAX_LEPTONS] ; //Dmitry
   float ElSharedHitsFraction                  [MAX_LEPTONS] ; //Dmitry
   float dR_gsf_ctfTrack                       [MAX_LEPTONS] ; //Dmitry
   float dPt_gsf_ctfTrack                      [MAX_LEPTONS] ; //Dmitry
   bool  ElhasConv                             [MAX_LEPTONS] ;
   float ElTrackNLostHits                      [MAX_LEPTONS] ;  //yjlei
   float ElTrackDz                             [MAX_LEPTONS] ;
   float ElTrackDz_BS                          [MAX_LEPTONS] ;
   float ElTrackD0                             [MAX_LEPTONS] ;
   float ElTrackDxy_BS                         [MAX_LEPTONS] ;
   float ElTrackDxy_PV                         [MAX_LEPTONS] ;
   float ElTrackDxy_PVBS                       [MAX_LEPTONS] ; //yjlei
   int   ElNClusters                           [MAX_LEPTONS] ;
   int   ElClassification                      [MAX_LEPTONS] ;
   float ElFBrem                               [MAX_LEPTONS] ;
   int NumberOfExpectedInnerHits               [MAX_LEPTONS] ; // Add by Jacky
   float Eldist                                [MAX_LEPTONS] ; // Add by Jacky
   float Eldcot                                [MAX_LEPTONS] ; // Add by Jacky
   float Elconvradius                          [MAX_LEPTONS] ; // Add by Jacky
   float ElConvPoint_x                         [MAX_LEPTONS] ; // Add by Jacky
   float ElConvPoint_y                         [MAX_LEPTONS] ; // Add by Jacky
   float ElConvPoint_z                         [MAX_LEPTONS] ; // Add by Jacky
   float dcotdist                              [MAX_LEPTONS] ;
   float ElseedEoverP                          [MAX_LEPTONS] ;
   float ElEcalIso04                           [MAX_LEPTONS] ;
   float ElHcalIso04                           [MAX_LEPTONS] ;
   int   ElNumberOfBrems                       [MAX_LEPTONS] ;
   float TrgPt                                 [MAX_LEPTONS] ;
   float TrgEta                                [MAX_LEPTONS] ;
   float TrgPhi                                [MAX_LEPTONS] ;
   int TrgID                                   [MAX_LEPTONS] ;

   //--------------------------------  Tau information  --------------------------------
   int isPFTau                                 [MAX_LEPTONS] ;    // YoungKyu 2012-10-16
   float decayModeFinding                      [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float byVLooseCombinedIsolationDeltaBetaCorr[MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float byLooseCombinedIsolationDeltaBetaCorr [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float byMediumCombinedIsolationDeltaBetaCorr[MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float byTightCombinedIsolationDeltaBetaCorr [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstElectronLoose                  [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstElectronMedium                 [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstElectronTight                  [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstElectronMVA                    [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstMuonLoose                      [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstMuonMedium                     [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   float againstMuonTight                      [MAX_LEPTONS] ; // YoungKyu 2012-10-31
   //-------------------------------  GenMG information  -------------------------------
   float GenPt                                 [MAX_LEPTONS] ;
   float GenEta                                [MAX_LEPTONS] ;
   float GenPhi                                [MAX_LEPTONS] ;
   int   GenPdgID                              [MAX_LEPTONS] ;
   int   GenMCTag                              [MAX_LEPTONS] ;
#ifdef __BPRIMEKIT__
   reco::Candidate* CandRef[MAX_LEPTONS]; // backward pointer to the PAT objects
#endif

   void RegisterTree( TTree* root, std::string name = "LepInfo" ) {
      BRANCH_OBJ( Size                                      , I ) ;
      BRANCH_ARRAY ( Index                                  , I ) ;
      BRANCH_ARRAY ( isEcalDriven                           , I ) ;
      BRANCH_ARRAY ( isTrackerDriven                        , I ) ;
      BRANCH_ARRAY ( LeptonType                             , I ) ;
      BRANCH_ARRAY ( Charge                                 , I ) ;
      BRANCH_ARRAY ( ChargeGsf                              , I ) ;
      BRANCH_ARRAY ( ChargeCtf                              , I ) ;
      BRANCH_ARRAY ( ChargeScPix                            , I ) ;
      BRANCH_ARRAY ( Pt                                     , F ) ;
      BRANCH_ARRAY ( Et                                     , F ) ;
      BRANCH_ARRAY ( Eta                                    , F ) ;
      BRANCH_ARRAY ( caloEta                                , F ) ;
      //BRANCH_ARRAY ( r9                                   , //F ) ;
      BRANCH_ARRAY ( Phi                                    , F ) ;
      BRANCH_ARRAY ( TrackIso                               , F ) ;
      BRANCH_ARRAY ( EcalIso                                , F ) ;
      BRANCH_ARRAY ( HcalIso                                , F ) ;
      BRANCH_ARRAY ( HcalDepth1Iso                          , F ) ;
      BRANCH_ARRAY ( HcalDepth2Iso                          , F ) ;
      BRANCH_ARRAY ( ChargedHadronIso                       , F ) ;
      BRANCH_ARRAY ( NeutralHadronIso                       , F ) ;
      BRANCH_ARRAY ( PhotonIso                              , F ) ;
      BRANCH_ARRAY ( ChargedHadronIsoR03                    , F ) ;
      BRANCH_ARRAY ( NeutralHadronIsoR03                    , F ) ;
      BRANCH_ARRAY ( PhotonIsoR03                           , F ) ;
      BRANCH_ARRAY ( sumPUPtR03                             , F ) ;
      BRANCH_ARRAY ( IsoRhoCorrR03                          , F ) ;
      BRANCH_ARRAY ( ChargedHadronIsoR04                    , F ) ;
      BRANCH_ARRAY ( NeutralHadronIsoR04                    , F ) ;
      BRANCH_ARRAY ( PhotonIsoR04                           , F ) ;
      BRANCH_ARRAY ( sumPUPtR04                             , F ) ;
      BRANCH_ARRAY ( IsoRhoCorrR04                          , F ) ;
      BRANCH_ARRAY ( Ip3dPV                                 , F ) ;
      BRANCH_ARRAY ( Ip3dPVErr                              , F ) ;
      BRANCH_ARRAY ( Ip3dPVSignificance                     , F ) ;
      BRANCH_ARRAY ( MuontimenDof                           , I ) ;
      BRANCH_ARRAY ( MuontimeAtIpInOut                      , F ) ;
      BRANCH_ARRAY ( MuontimeAtIpOutIn                      , F ) ;
      BRANCH_ARRAY ( Muondirection                          , I ) ;
      BRANCH_ARRAY ( CaloEnergy                             , F ) ;
      BRANCH_ARRAY ( e1x5                                   , F ) ;
      BRANCH_ARRAY ( e2x5Max                                , F ) ;
      BRANCH_ARRAY ( e5x5                                   , F ) ;
      BRANCH_ARRAY ( Px                                     , F ) ;
      BRANCH_ARRAY ( Py                                     , F ) ;
      BRANCH_ARRAY ( Pz                                     , F ) ;
      BRANCH_ARRAY ( Energy                                 , F ) ;
      BRANCH_ARRAY ( isGoodMuonTMOneStationTight            , B ) ;
      BRANCH_ARRAY ( innerTracknormalizedChi2               , F ) ;
      BRANCH_ARRAY ( vertexZ                                , F ) ;
      BRANCH_ARRAY ( isPFMuon                               , B ) ;
      BRANCH_ARRAY ( MuIDGlobalMuonPromptTight              , B ) ;
      BRANCH_ARRAY ( MuInnerPtError                         , F ) ;
      BRANCH_ARRAY ( MuGlobalPtError                        , F ) ;
      BRANCH_ARRAY ( MuInnerTrackDz                         , F ) ;
      BRANCH_ARRAY ( MuInnerTrackD0                         , F ) ;
      BRANCH_ARRAY ( MuInnerTrackDxy_BS                     , F ) ;
      BRANCH_ARRAY ( MuInnerTrackDxy_PV                     , F ) ;
      BRANCH_ARRAY ( MuInnerTrackDxy_PVBS                   , F ) ;
      BRANCH_ARRAY ( MuInnerTrackNHits                      , I ) ;
      BRANCH_ARRAY ( MuNTrackerHits                         , I ) ;
      BRANCH_ARRAY ( MuGlobalNormalizedChi2                 , F ) ;
      BRANCH_ARRAY ( MuCaloCompat                           , F ) ;
      BRANCH_ARRAY ( MuNChambers                            , I ) ;
      BRANCH_ARRAY ( MuNChambersMatchesSegment              , I ) ;
      BRANCH_ARRAY ( MuNMatchedStations                     , I ) ;
      BRANCH_ARRAY ( MuNPixelLayers                         , I ) ;
      BRANCH_ARRAY ( MuNPixelLayersWMeasurement             , I ) ;
      BRANCH_ARRAY ( MuNTrackLayersWMeasurement             , I ) ;
      BRANCH_ARRAY ( MuNLostInnerHits                       , I ) ;
      BRANCH_ARRAY ( MuNLostOuterHits                       , I ) ;
      BRANCH_ARRAY ( MuNMuonhits                            , I ) ;
      BRANCH_ARRAY ( MuDThits                               , I ) ;
      BRANCH_ARRAY ( MuCSChits                              , I ) ;
      BRANCH_ARRAY ( MuRPChits                              , I ) ;
      BRANCH_ARRAY ( MuType                                 , I ) ;
      BRANCH_ARRAY ( EgammaMVANonTrig                       , F ) ;
      BRANCH_ARRAY ( EgammaMVATrig                          , F ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdTRIGGERTIGHT        , B ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdTRIGGERWP70         , B ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdVETO                , B ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdLOOSE               , B ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdMEDIUM              , B ) ;
      BRANCH_ARRAY ( EgammaCutBasedEleIdTIGHT               , B ) ;
      BRANCH_ARRAY ( Eldr03HcalDepth1TowerSumEtBc           , F ) ;
      BRANCH_ARRAY ( Eldr03HcalDepth2TowerSumEtBc           , F ) ;
      BRANCH_ARRAY ( Eldr04HcalDepth1TowerSumEtBc           , F ) ;
      BRANCH_ARRAY ( Eldr04HcalDepth2TowerSumEtBc           , F ) ;
      BRANCH_ARRAY ( ElhcalOverEcalBc                       , F ) ;
      BRANCH_ARRAY ( ElEcalE                                , F ) ;
      BRANCH_ARRAY ( ElEoverP                               , F ) ;
      BRANCH_ARRAY ( EldeltaEta                             , F ) ;
      BRANCH_ARRAY ( EldeltaPhi                             , F ) ;
      BRANCH_ARRAY ( ElHadoverEm                            , F ) ;
      BRANCH_ARRAY ( ElsigmaIetaIeta                        , F ) ;
      BRANCH_ARRAY ( ElscSigmaIetaIeta                      , F ) ;
      BRANCH_ARRAY ( ElEnergyErr                            , F ) ;
      BRANCH_ARRAY ( ElMomentumErr                          , F ) ;
      BRANCH_ARRAY ( ElTrackNHits                           , I ) ;
      BRANCH_ARRAY ( ElSharedHitsFraction                   , F ) ;
      BRANCH_ARRAY ( dR_gsf_ctfTrack                        , F ) ;
      BRANCH_ARRAY ( dPt_gsf_ctfTrack                       , F ) ;
      BRANCH_ARRAY ( ElhasConv                              , B ) ;
      BRANCH_ARRAY ( ElTrackNLostHits                       , F ) ;
      BRANCH_ARRAY ( ElTrackDz                              , F ) ;
      BRANCH_ARRAY ( ElTrackDz_BS                           , F ) ;
      BRANCH_ARRAY ( ElTrackD0                              , F ) ;
      BRANCH_ARRAY ( ElTrackDxy_BS                          , F ) ;
      BRANCH_ARRAY ( ElTrackDxy_PV                          , F ) ;
      BRANCH_ARRAY ( ElTrackDxy_PVBS                        , F ) ;
      BRANCH_ARRAY ( ElNClusters                            , I ) ;
      BRANCH_ARRAY ( ElClassification                       , I ) ;
      BRANCH_ARRAY ( ElFBrem                                , F ) ;
      BRANCH_ARRAY ( NumberOfExpectedInnerHits              , I ) ;
      BRANCH_ARRAY ( Eldist                                 , F ) ;
      BRANCH_ARRAY ( Eldcot                                 , F ) ;
      BRANCH_ARRAY ( Elconvradius                           , F ) ;
      BRANCH_ARRAY ( ElConvPoint_x                          , F ) ;
      BRANCH_ARRAY ( ElConvPoint_y                          , F ) ;
      BRANCH_ARRAY ( ElConvPoint_z                          , F ) ;
      BRANCH_ARRAY ( dcotdist                               , F ) ;
      BRANCH_ARRAY ( ElseedEoverP                           , F ) ;
      BRANCH_ARRAY ( ElEcalIso04                            , F ) ;
      BRANCH_ARRAY ( ElHcalIso04                            , F ) ;
      BRANCH_ARRAY ( ElNumberOfBrems                        , I ) ;
      BRANCH_ARRAY ( GenPt                                  , F ) ;
      BRANCH_ARRAY ( GenEta                                 , F ) ;
      BRANCH_ARRAY ( GenPhi                                 , F ) ;
      BRANCH_ARRAY ( GenPdgID                               , I ) ;
      BRANCH_ARRAY ( GenMCTag                               , I ) ;
      BRANCH_ARRAY ( TrgPt                                  , F ) ;
      BRANCH_ARRAY ( TrgEta                                 , F ) ;
      BRANCH_ARRAY ( TrgPhi                                 , F ) ;
      BRANCH_ARRAY ( TrgID                                  , I ) ;
      BRANCH_ARRAY ( isPFTau                                , I ) ;
      BRANCH_ARRAY ( decayModeFinding                       , F ) ;
      BRANCH_ARRAY ( byVLooseCombinedIsolationDeltaBetaCorr , F ) ;
      BRANCH_ARRAY ( byLooseCombinedIsolationDeltaBetaCorr  , F ) ;
      BRANCH_ARRAY ( byMediumCombinedIsolationDeltaBetaCorr , F ) ;
      BRANCH_ARRAY ( byTightCombinedIsolationDeltaBetaCorr  , F ) ;
      BRANCH_ARRAY ( againstElectronLoose                   , F ) ;
      BRANCH_ARRAY ( againstElectronMedium                  , F ) ;
      BRANCH_ARRAY ( againstElectronTight                   , F ) ;
      BRANCH_ARRAY ( againstElectronMVA                     , F ) ;
      BRANCH_ARRAY ( againstMuonLoose                       , F ) ;
      BRANCH_ARRAY ( againstMuonMedium                      , F ) ;
      BRANCH_ARRAY ( againstMuonTight                       , F ) ;
   }


   void Register( TTree* root, std::string name = "LepInfo" ) {
      SET_BRANCH_OBJ  ( Size                                   ) ; 
      SET_BRANCH_ARRAY( Index                                  ) ;
      SET_BRANCH_ARRAY( isEcalDriven                           ) ;
      SET_BRANCH_ARRAY( isTrackerDriven                        ) ;
      SET_BRANCH_ARRAY( LeptonType                             ) ;
      SET_BRANCH_ARRAY( Charge                                 ) ;
      SET_BRANCH_ARRAY( ChargeGsf                              ) ;
      SET_BRANCH_ARRAY( ChargeCtf                              ) ;
      SET_BRANCH_ARRAY( ChargeScPix                            ) ;
      SET_BRANCH_ARRAY( Pt                                     ) ;
      SET_BRANCH_ARRAY( Et                                     ) ;
      SET_BRANCH_ARRAY( Eta                                    ) ;
      SET_BRANCH_ARRAY( caloEta                                ) ;
      //SET_BRANCH_ARRAY( r9                                   ) ;
      SET_BRANCH_ARRAY( Phi                                    ) ;
      SET_BRANCH_ARRAY( TrackIso                               ) ;
      SET_BRANCH_ARRAY( EcalIso                                ) ;
      SET_BRANCH_ARRAY( HcalIso                                ) ;
      SET_BRANCH_ARRAY( HcalDepth1Iso                          ) ;
      SET_BRANCH_ARRAY( HcalDepth2Iso                          ) ;
      SET_BRANCH_ARRAY( ChargedHadronIso                       ) ;
      SET_BRANCH_ARRAY( NeutralHadronIso                       ) ;
      SET_BRANCH_ARRAY( PhotonIso                              ) ;
      SET_BRANCH_ARRAY( ChargedHadronIsoR03                    ) ;
      SET_BRANCH_ARRAY( NeutralHadronIsoR03                    ) ;
      SET_BRANCH_ARRAY( PhotonIsoR03                           ) ;
      SET_BRANCH_ARRAY( sumPUPtR03                             ) ;
      SET_BRANCH_ARRAY( IsoRhoCorrR03                          ) ;
      SET_BRANCH_ARRAY( ChargedHadronIsoR04                    ) ;
      SET_BRANCH_ARRAY( NeutralHadronIsoR04                    ) ;
      SET_BRANCH_ARRAY( PhotonIsoR04                           ) ;
      SET_BRANCH_ARRAY( sumPUPtR04                             ) ;
      SET_BRANCH_ARRAY( IsoRhoCorrR04                          ) ;
      SET_BRANCH_ARRAY( Ip3dPV                                 ) ;
      SET_BRANCH_ARRAY( Ip3dPVErr                              ) ;
      SET_BRANCH_ARRAY( Ip3dPVSignificance                     ) ;
      SET_BRANCH_ARRAY( MuontimenDof                           ) ;
      SET_BRANCH_ARRAY( MuontimeAtIpInOut                      ) ;
      SET_BRANCH_ARRAY( MuontimeAtIpOutIn                      ) ;
      SET_BRANCH_ARRAY( Muondirection                          ) ;
      SET_BRANCH_ARRAY( CaloEnergy                             ) ;
      SET_BRANCH_ARRAY( e1x5                                   ) ;
      SET_BRANCH_ARRAY( e2x5Max                                ) ;
      SET_BRANCH_ARRAY( e5x5                                   ) ;
      SET_BRANCH_ARRAY( Px                                     ) ;
      SET_BRANCH_ARRAY( Py                                     ) ;
      SET_BRANCH_ARRAY( Pz                                     ) ;
      SET_BRANCH_ARRAY( Energy                                 ) ;
      SET_BRANCH_ARRAY( isGoodMuonTMOneStationTight            ) ;
      SET_BRANCH_ARRAY( innerTracknormalizedChi2               ) ;
      SET_BRANCH_ARRAY( vertexZ                                ) ;
      SET_BRANCH_ARRAY( isPFMuon                               ) ;
      SET_BRANCH_ARRAY( MuIDGlobalMuonPromptTight              ) ;
      SET_BRANCH_ARRAY( MuInnerPtError                         ) ;
      SET_BRANCH_ARRAY( MuGlobalPtError                        ) ;
      SET_BRANCH_ARRAY( MuInnerTrackDz                         ) ;
      SET_BRANCH_ARRAY( MuInnerTrackD0                         ) ;
      SET_BRANCH_ARRAY( MuInnerTrackDxy_BS                     ) ;
      SET_BRANCH_ARRAY( MuInnerTrackDxy_PV                     ) ;
      SET_BRANCH_ARRAY( MuInnerTrackDxy_PVBS                   ) ;
      SET_BRANCH_ARRAY( MuInnerTrackNHits                      ) ;
      SET_BRANCH_ARRAY( MuNTrackerHits                         ) ;
      SET_BRANCH_ARRAY( MuGlobalNormalizedChi2                 ) ;
      SET_BRANCH_ARRAY( MuCaloCompat                           ) ;
      SET_BRANCH_ARRAY( MuNChambers                            ) ;
      SET_BRANCH_ARRAY( MuNChambersMatchesSegment              ) ;
      SET_BRANCH_ARRAY( MuNMatchedStations                     ) ;
      SET_BRANCH_ARRAY( MuNPixelLayers                         ) ;
      SET_BRANCH_ARRAY( MuNPixelLayersWMeasurement             ) ;
      SET_BRANCH_ARRAY( MuNTrackLayersWMeasurement             ) ;
      SET_BRANCH_ARRAY( MuNLostInnerHits                       ) ;
      SET_BRANCH_ARRAY( MuNLostOuterHits                       ) ;
      SET_BRANCH_ARRAY( MuNMuonhits                            ) ;
      SET_BRANCH_ARRAY( MuDThits                               ) ;
      SET_BRANCH_ARRAY( MuCSChits                              ) ;
      SET_BRANCH_ARRAY( MuRPChits                              ) ;
      SET_BRANCH_ARRAY( MuType                                 ) ;
      SET_BRANCH_ARRAY( EgammaMVANonTrig                       ) ;
      SET_BRANCH_ARRAY( EgammaMVATrig                          ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdTRIGGERTIGHT        ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdTRIGGERWP70         ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdVETO                ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdLOOSE               ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdMEDIUM              ) ;
      SET_BRANCH_ARRAY( EgammaCutBasedEleIdTIGHT               ) ;
      SET_BRANCH_ARRAY( Eldr03HcalDepth1TowerSumEtBc           ) ;
      SET_BRANCH_ARRAY( Eldr03HcalDepth2TowerSumEtBc           ) ;
      SET_BRANCH_ARRAY( Eldr04HcalDepth1TowerSumEtBc           ) ;
      SET_BRANCH_ARRAY( Eldr04HcalDepth2TowerSumEtBc           ) ;
      SET_BRANCH_ARRAY( ElhcalOverEcalBc                       ) ;
      SET_BRANCH_ARRAY( ElEcalE                                ) ;
      SET_BRANCH_ARRAY( ElEoverP                               ) ;
      SET_BRANCH_ARRAY( EldeltaEta                             ) ;
      SET_BRANCH_ARRAY( EldeltaPhi                             ) ;
      SET_BRANCH_ARRAY( ElHadoverEm                            ) ;
      SET_BRANCH_ARRAY( ElsigmaIetaIeta                        ) ;
      SET_BRANCH_ARRAY( ElscSigmaIetaIeta                      ) ;
      SET_BRANCH_ARRAY( ElEnergyErr                            ) ;
      SET_BRANCH_ARRAY( ElMomentumErr                          ) ;
      SET_BRANCH_ARRAY( ElTrackNHits                           ) ;
      SET_BRANCH_ARRAY( ElSharedHitsFraction                   ) ;
      SET_BRANCH_ARRAY( dR_gsf_ctfTrack                        ) ;
      SET_BRANCH_ARRAY( dPt_gsf_ctfTrack                       ) ;
      SET_BRANCH_ARRAY( ElhasConv                              ) ;
      SET_BRANCH_ARRAY( ElTrackNLostHits                       ) ;
      SET_BRANCH_ARRAY( ElTrackDz                              ) ;
      SET_BRANCH_ARRAY( ElTrackDz_BS                           ) ;
      SET_BRANCH_ARRAY( ElTrackD0                              ) ;
      SET_BRANCH_ARRAY( ElTrackDxy_BS                          ) ;
      SET_BRANCH_ARRAY( ElTrackDxy_PV                          ) ;
      SET_BRANCH_ARRAY( ElTrackDxy_PVBS                        ) ;
      SET_BRANCH_ARRAY( ElNClusters                            ) ;
      SET_BRANCH_ARRAY( ElClassification                       ) ;
      SET_BRANCH_ARRAY( ElFBrem                                ) ;
      SET_BRANCH_ARRAY( NumberOfExpectedInnerHits              ) ;
      SET_BRANCH_ARRAY( Eldist                                 ) ;
      SET_BRANCH_ARRAY( Eldcot                                 ) ;
      SET_BRANCH_ARRAY( Elconvradius                           ) ;
      SET_BRANCH_ARRAY( ElConvPoint_x                          ) ;
      SET_BRANCH_ARRAY( ElConvPoint_y                          ) ;
      SET_BRANCH_ARRAY( ElConvPoint_z                          ) ;
      SET_BRANCH_ARRAY( dcotdist                               ) ;
      SET_BRANCH_ARRAY( ElseedEoverP                           ) ;
      SET_BRANCH_ARRAY( ElEcalIso04                            ) ;
      SET_BRANCH_ARRAY( ElHcalIso04                            ) ;
      SET_BRANCH_ARRAY( ElNumberOfBrems                        ) ;
      SET_BRANCH_ARRAY( GenPt                                  ) ;
      SET_BRANCH_ARRAY( GenEta                                 ) ;
      SET_BRANCH_ARRAY( GenPhi                                 ) ;
      SET_BRANCH_ARRAY( GenPdgID                               ) ;
      SET_BRANCH_ARRAY( GenMCTag                               ) ;
      SET_BRANCH_ARRAY( TrgPt                                  ) ;
      SET_BRANCH_ARRAY( TrgEta                                 ) ;
      SET_BRANCH_ARRAY( TrgPhi                                 ) ;
      SET_BRANCH_ARRAY( TrgID                                  ) ;
      SET_BRANCH_ARRAY( isPFTau                                ) ;
      SET_BRANCH_ARRAY( decayModeFinding                       ) ;
      SET_BRANCH_ARRAY( byVLooseCombinedIsolationDeltaBetaCorr ) ;
      SET_BRANCH_ARRAY( byLooseCombinedIsolationDeltaBetaCorr  ) ;
      SET_BRANCH_ARRAY( byMediumCombinedIsolationDeltaBetaCorr ) ;
      SET_BRANCH_ARRAY( byTightCombinedIsolationDeltaBetaCorr  ) ;
      SET_BRANCH_ARRAY( againstElectronLoose                   ) ;
      SET_BRANCH_ARRAY( againstElectronMedium                  ) ;
      SET_BRANCH_ARRAY( againstElectronTight                   ) ;
      SET_BRANCH_ARRAY( againstElectronMVA                     ) ;
      SET_BRANCH_ARRAY( againstMuonLoose                       ) ;
      SET_BRANCH_ARRAY( againstMuonMedium                      ) ;
      SET_BRANCH_ARRAY( againstMuonTight                       ) ;
   }
};


class JetInfoBranches {
public:
   int   Size;
   int   Index                   [MAX_JETS] ;
   int   NTracks                 [MAX_JETS] ;
   float Et                      [MAX_JETS] ;
   float Pt                      [MAX_JETS] ;
   float Unc                     [MAX_JETS] ;
   float Eta                     [MAX_JETS] ;
   float Phi                     [MAX_JETS] ;
   float Px                      [MAX_JETS] ; //Uly 2011-04-04
   float Py                      [MAX_JETS] ; //Uly 2011-04-04
   float Pz                      [MAX_JETS] ; //Uly 2011-04-04
   float Energy                  [MAX_JETS] ; //Uly 2011-04-04
   float Mass                    [MAX_JETS] ;
   float Area                    [MAX_JETS] ;
   int   JetIDLOOSE              [MAX_JETS] ; //Add by Chiyi
   float JetCharge               [MAX_JETS] ;
   float QGTagsMLP               [MAX_JETS] ;
   float QGTagsLikelihood        [MAX_JETS] ;
   int   NConstituents           [MAX_JETS] ;
   int   NCH                     [MAX_JETS] ;
   float CEF                     [MAX_JETS] ;
   float NHF                     [MAX_JETS] ;
   float NEF                     [MAX_JETS] ;
   float CHF                     [MAX_JETS] ;
   float JVAlpha                 [MAX_JETS] ;
   float JVBeta                  [MAX_JETS] ;
   float PtCorrRaw               [MAX_JETS] ;
   float PtCorrL2                [MAX_JETS] ;
   float PtCorrL3                [MAX_JETS] ;
   float PtCorrL7g               [MAX_JETS] ;
   float PtCorrL7uds             [MAX_JETS] ;
   float PtCorrL7c               [MAX_JETS] ;
   float PtCorrL7b               [MAX_JETS] ;
   float JetBProbBJetTags        [MAX_JETS] ;
   float JetProbBJetTags         [MAX_JETS] ;
   float TrackCountHiPurBJetTags [MAX_JETS] ;
   float TrackCountHiEffBJetTags [MAX_JETS] ;
   float SimpleSVBJetTags        [MAX_JETS] ;  //for 35X sample //Add by Chiyi
   float SimpleSVHEBJetTags      [MAX_JETS] ;  //for 36X sample //Add by Chiyi
   float SimpleSVHPBJetTags      [MAX_JETS] ;  //for 36X sample //Add by Chiyi
   float CombinedSVBJetTags      [MAX_JETS] ;
   float CombinedSVMVABJetTags   [MAX_JETS] ;
   float SoftElecByIP3dBJetTags  [MAX_JETS] ;
   float SoftElecByPtBJetTags    [MAX_JETS] ;
   float SoftMuonBJetTags        [MAX_JETS] ;
   float SoftMuonByIP3dBJetTags  [MAX_JETS] ;
   float SoftMuonByPtBJetTags    [MAX_JETS] ;
   float DoubleSVHighEffBJetTags [MAX_JETS] ; //// Added by DM
   float GenJetPt                [MAX_JETS] ;
   float GenJetEta               [MAX_JETS] ;
   float GenJetPhi               [MAX_JETS] ;
   float GenPt                   [MAX_JETS] ;
   float GenEta                  [MAX_JETS] ;
   float GenPhi                  [MAX_JETS] ;
   int   GenPdgID                [MAX_JETS] ;
   int   GenFlavor               [MAX_JETS] ;
   int   GenMCTag                [MAX_JETS] ; // 0: unknown, 1: decay from W, 2: decay from Z, (+10) from b', (+20) from t'
   int NSubjets                  [MAX_JETS] ;
   int SubjetsIdxStart           [MAX_JETS] ;
   // Vector for writing
   std::vector<float> SubjetMass_w               ;
   std::vector<float> SubjetPt_w                 ;
   std::vector<float> SubjetEt_w                 ;
   std::vector<float> SubjetEta_w                ;
   std::vector<float> SubjetPhi_w                ;
   std::vector<float> SubjetCombinedSVBJetTags_w ;
   std::vector<float> SubjetPtUncorr_w           ;
   std::vector<float> SubjetArea_w               ;
   // Vector for reading
   std::vector<float>* SubjetMass                ;
   std::vector<float>* SubjetPt                  ;
   std::vector<float>* SubjetEt                  ;
   std::vector<float>* SubjetEta                 ;
   std::vector<float>* SubjetPhi                 ;
   std::vector<float>* SubjetCombinedSVBJetTags  ;
   std::vector<float>* SubjetPtUncorr            ;
   std::vector<float>* SubjetArea                ;
#ifdef __BPRIMEKIT__
   reco::Candidate* CandRef[MAX_JETS]; // backward pointer to the PAT objects
#endif

   void RegisterTree( TTree* root, std::string name = "JetInfo" ) {
      BRANCH_OBJ  ( Size                    , I   ) ;
      BRANCH_ARRAY( Index                   , I   ) ;
      BRANCH_ARRAY( NTracks                 , I   ) ;
      BRANCH_ARRAY( Et                      , F   ) ;
      BRANCH_ARRAY( Pt                      , F   ) ;
      BRANCH_ARRAY( Unc                     , F   ) ;
      BRANCH_ARRAY( Eta                     , F   ) ;
      BRANCH_ARRAY( Phi                     , F   ) ;
      BRANCH_ARRAY( JetIDLOOSE              , I   ) ;
      BRANCH_ARRAY( JetCharge               , F   ) ;
      BRANCH_ARRAY( QGTagsMLP               , F   ) ;
      BRANCH_ARRAY( QGTagsLikelihood        , F   ) ;
      BRANCH_ARRAY( NConstituents           , I   ) ;
      BRANCH_ARRAY( NCH                     , I   ) ;
      BRANCH_ARRAY( CEF                     , F   ) ;
      BRANCH_ARRAY( NHF                     , F   ) ;
      BRANCH_ARRAY( NEF                     , F   ) ;
      BRANCH_ARRAY( CHF                     , F   ) ;
      BRANCH_ARRAY( JVAlpha                 , F   ) ;
      BRANCH_ARRAY( JVBeta                  , F   ) ;
      BRANCH_ARRAY( PtCorrRaw               , F   ) ;
      BRANCH_ARRAY( PtCorrL2                , F   ) ;
      BRANCH_ARRAY( PtCorrL3                , F   ) ;
      BRANCH_ARRAY( PtCorrL7g               , F   ) ;
      BRANCH_ARRAY( PtCorrL7uds             , F   ) ;
      BRANCH_ARRAY( PtCorrL7c               , F   ) ;
      BRANCH_ARRAY( PtCorrL7b               , F   ) ;
      BRANCH_ARRAY( JetBProbBJetTags        , F   ) ;
      BRANCH_ARRAY( JetProbBJetTags         , F   ) ;
      BRANCH_ARRAY( TrackCountHiPurBJetTags , F   ) ;
      BRANCH_ARRAY( TrackCountHiEffBJetTags , F   ) ;
      BRANCH_ARRAY( SimpleSVBJetTags        , F   ) ;
      BRANCH_ARRAY( SimpleSVHEBJetTags      , F   ) ;
      BRANCH_ARRAY( SimpleSVHPBJetTags      , F   ) ;
      BRANCH_ARRAY( CombinedSVBJetTags      , F   ) ;
      BRANCH_ARRAY( CombinedSVMVABJetTags   , F   ) ;
      BRANCH_ARRAY( SoftElecByIP3dBJetTags  , F   ) ;
      BRANCH_ARRAY( SoftElecByPtBJetTags    , F   ) ;
      BRANCH_ARRAY( SoftMuonBJetTags        , F   ) ;
      BRANCH_ARRAY( SoftMuonByIP3dBJetTags  , F   ) ;
      BRANCH_ARRAY( SoftMuonByPtBJetTags    , F   ) ;
      BRANCH_ARRAY( DoubleSVHighEffBJetTags , F   ) ;
      BRANCH_ARRAY( GenJetPt                , F   ) ;
      BRANCH_ARRAY( GenJetEta               , F   ) ;
      BRANCH_ARRAY( GenJetPhi               , F   ) ;
      BRANCH_ARRAY( GenPt                   , F   ) ;
      BRANCH_ARRAY( GenEta                  , F   ) ;
      BRANCH_ARRAY( GenPhi                  , F   ) ;
      BRANCH_ARRAY( GenPdgID                , I   ) ;
      BRANCH_ARRAY( GenFlavor               , I   ) ;
      BRANCH_ARRAY( GenMCTag                , I   ) ;
      BRANCH_ARRAY( Px                      , F   ) ;
      BRANCH_ARRAY( Py                      , F   ) ;
      BRANCH_ARRAY( Pz                      , F   ) ;
      BRANCH_ARRAY( Energy                  , F   ) ;
      BRANCH_ARRAY( Mass                    , F   ) ;
      BRANCH_ARRAY( Area                    , F   ) ;
      BRANCH_ARRAY( NSubjets                , I   ) ;
      BRANCH_ARRAY( SubjetsIdxStart         , I   ) ;
      
      root->Branch( ( name + ".SubjetMass" ).c_str()               , &SubjetMass_w );
      root->Branch( ( name + ".SubjetPt" ).c_str()                 , &SubjetPt_w );
      root->Branch( ( name + ".SubjetEt" ).c_str()                 , &SubjetEt_w );
      root->Branch( ( name + ".SubjetEta" ).c_str()                , &SubjetEta_w );
      root->Branch( ( name + ".SubjetPhi" ).c_str()                , &SubjetPhi_w );
      root->Branch( ( name + ".SubjetCombinedSVBJetTags" ).c_str() , &SubjetCombinedSVBJetTags_w );
      root->Branch( ( name + ".SubjetPtUncorr" ).c_str()           , &SubjetPtUncorr_w );
      root->Branch( ( name + ".SubjetArea" ).c_str()               , &SubjetArea_w );
   }

   void Register( TTree* root, std::string name = "JetInfo" ) {
      SET_BRANCH_OBJ  ( Size                     ) ;
      SET_BRANCH_ARRAY( Index                    ) ;
      SET_BRANCH_ARRAY( NTracks                  ) ;
      SET_BRANCH_ARRAY( Et                       ) ;
      SET_BRANCH_ARRAY( Pt                       ) ;
      SET_BRANCH_ARRAY( Unc                      ) ;
      SET_BRANCH_ARRAY( Eta                      ) ;
      SET_BRANCH_ARRAY( Phi                      ) ;
      SET_BRANCH_ARRAY( JetIDLOOSE               ) ;
      SET_BRANCH_ARRAY( JetCharge                ) ;
      SET_BRANCH_ARRAY( QGTagsMLP                ) ;
      SET_BRANCH_ARRAY( QGTagsLikelihood         ) ;
      SET_BRANCH_ARRAY( NConstituents            ) ;
      SET_BRANCH_ARRAY( NCH                      ) ;
      SET_BRANCH_ARRAY( CEF                      ) ;
      SET_BRANCH_ARRAY( NHF                      ) ;
      SET_BRANCH_ARRAY( NEF                      ) ;
      SET_BRANCH_ARRAY( CHF                      ) ;
      SET_BRANCH_ARRAY( JVAlpha                  ) ;
      SET_BRANCH_ARRAY( JVBeta                   ) ;
      SET_BRANCH_ARRAY( PtCorrRaw                ) ;
      SET_BRANCH_ARRAY( PtCorrL2                 ) ;
      SET_BRANCH_ARRAY( PtCorrL3                 ) ;
      SET_BRANCH_ARRAY( PtCorrL7g                ) ;
      SET_BRANCH_ARRAY( PtCorrL7uds              ) ;
      SET_BRANCH_ARRAY( PtCorrL7c                ) ;
      SET_BRANCH_ARRAY( PtCorrL7b                ) ;
      SET_BRANCH_ARRAY( JetBProbBJetTags         ) ;
      SET_BRANCH_ARRAY( JetProbBJetTags          ) ;
      SET_BRANCH_ARRAY( TrackCountHiPurBJetTags  ) ;
      SET_BRANCH_ARRAY( TrackCountHiEffBJetTags  ) ;
      // SET_BRANCH_ARRAY( ImpactParaMVABJetTags ) ;
      SET_BRANCH_ARRAY( SimpleSVBJetTags         ) ;
      SET_BRANCH_ARRAY( SimpleSVHEBJetTags       ) ;
      SET_BRANCH_ARRAY( SimpleSVHPBJetTags       ) ;
      SET_BRANCH_ARRAY( CombinedSVBJetTags       ) ;
      SET_BRANCH_ARRAY( CombinedSVMVABJetTags    ) ;
      SET_BRANCH_ARRAY( SoftElecByIP3dBJetTags   ) ;
      SET_BRANCH_ARRAY( SoftElecByPtBJetTags     ) ;
      SET_BRANCH_ARRAY( SoftMuonBJetTags         ) ;
      SET_BRANCH_ARRAY( SoftMuonByIP3dBJetTags   ) ;
      SET_BRANCH_ARRAY( SoftMuonByPtBJetTags     ) ;
      SET_BRANCH_ARRAY( DoubleSVHighEffBJetTags  ) ;
      // SET_BRANCH_ARRAY( JetLRval              ) ;
      // SET_BRANCH_ARRAY( JetProb               ) ;
      SET_BRANCH_ARRAY( GenJetPt                 ) ;
      SET_BRANCH_ARRAY( GenJetEta                ) ;
      SET_BRANCH_ARRAY( GenJetPhi                ) ;
      SET_BRANCH_ARRAY( GenPt                    ) ;
      SET_BRANCH_ARRAY( GenEta                   ) ;
      SET_BRANCH_ARRAY( GenPhi                   ) ;
      SET_BRANCH_ARRAY( GenPdgID                 ) ;
      SET_BRANCH_ARRAY( GenFlavor                ) ;
      SET_BRANCH_ARRAY( GenMCTag                 ) ;
      SET_BRANCH_ARRAY( Px                       ) ;
      SET_BRANCH_ARRAY( Py                       ) ;
      SET_BRANCH_ARRAY( Pz                       ) ;
      SET_BRANCH_ARRAY( Energy                   ) ;
      SET_BRANCH_ARRAY( Mass                     ) ;
      SET_BRANCH_ARRAY( Area                     ) ;
      SET_BRANCH_ARRAY( NSubjets                 ) ;
      SET_BRANCH_ARRAY( SubjetsIdxStart          ) ;
      SET_BRANCH_OBJ( SubjetMass                 ) ;
      SET_BRANCH_OBJ( SubjetPt                   ) ;
      SET_BRANCH_OBJ( SubjetEt                   ) ;
      SET_BRANCH_OBJ( SubjetEta                  ) ;
      SET_BRANCH_OBJ( SubjetPhi                  ) ;
      SET_BRANCH_OBJ( SubjetCombinedSVBJetTags   ) ;
      SET_BRANCH_OBJ( SubjetPtUncorr             ) ;
      SET_BRANCH_OBJ( SubjetArea                 ) ;
      SubjetMass               = 0;
      SubjetPt                 = 0;
      SubjetEt                 = 0;
      SubjetEta                = 0;
      SubjetPhi                = 0;
      SubjetCombinedSVBJetTags = 0;
      SubjetPtUncorr           = 0;
      SubjetArea               = 0;
   }
};


class PairInfoBranches {
public:
   int   Size;
   int   Index     [MAX_PAIRS] ;
   int   Type      [MAX_PAIRS] ; // type of pairing - 1: ll (regardless of charge and flavor!), 2: jj
   int   Obj1Index [MAX_PAIRS] ;
   int   Obj2Index [MAX_PAIRS] ;
   int   GenPdgID  [MAX_PAIRS] ;
   float Mass      [MAX_PAIRS] ;
   float Pt        [MAX_PAIRS] ;
   float Eta       [MAX_PAIRS] ;
   float Phi       [MAX_PAIRS] ;
   float GenMass   [MAX_PAIRS] ;
   float GenPt     [MAX_PAIRS] ;
   float GenEta    [MAX_PAIRS] ;
   float GenPhi    [MAX_PAIRS] ;

   void RegisterTree ( TTree* root , std::string name = "PairInfo" ) {
      BRANCH_OBJ     ( Size      , I ) ;
      BRANCH_ARRAY   ( Index     , I ) ;
      BRANCH_ARRAY   ( Type      , I ) ;
      BRANCH_ARRAY   ( Obj1Index , I ) ;
      BRANCH_ARRAY   ( Obj2Index , I ) ;
      BRANCH_ARRAY   ( GenPdgID  , I ) ;
      BRANCH_ARRAY   ( Mass      , F ) ;
      BRANCH_ARRAY   ( Pt        , F ) ;
      BRANCH_ARRAY   ( Eta       , F ) ;
      BRANCH_ARRAY   ( Phi       , F ) ;
      BRANCH_ARRAY   ( GenMass   , F ) ;
      BRANCH_ARRAY   ( GenPt     , F ) ;
      BRANCH_ARRAY   ( GenEta    , F ) ;
      BRANCH_ARRAY   ( GenPhi    , F ) ;
   }

   void Register( TTree* root , std::string name = "PairInfo" ) {
      SET_BRANCH_OBJ( Size ) ;
      SET_BRANCH_ARRAY( Index       ) ;  
      SET_BRANCH_ARRAY( Type        ) ;  
      SET_BRANCH_ARRAY( Obj1Index   ) ;  
      SET_BRANCH_ARRAY( Obj2Index   ) ;  
      SET_BRANCH_ARRAY( Mass        ) ;  
      SET_BRANCH_ARRAY( Pt          ) ;  
      SET_BRANCH_ARRAY( Eta         ) ;  
      SET_BRANCH_ARRAY( Phi         ) ;  
      SET_BRANCH_ARRAY( GenMass     ) ;  
      SET_BRANCH_ARRAY( GenPt       ) ;  
      SET_BRANCH_ARRAY( GenEta      ) ;  
      SET_BRANCH_ARRAY( GenPhi      ) ;  
      SET_BRANCH_ARRAY( GenPdgID    ) ;  
   }
};

class PhotonInfoBranches {
public:
   int   Size;
   float Pt                   [MAX_PHOTONS] ;
   float Eta                  [MAX_PHOTONS] ;
   float Phi                  [MAX_PHOTONS] ;
   float HoverE               [MAX_PHOTONS] ;
   float SigmaIetaIeta        [MAX_PHOTONS] ;
   float hadTowOverEm         [MAX_PHOTONS] ;
   float hcalIsoConeDR04_2012 [MAX_PHOTONS] ;
   float phoPFChIsoDR03       [MAX_PHOTONS] ;
   float phoPFNeuIsoDR03      [MAX_PHOTONS] ;
   float phoPFPhoIsoDR03      [MAX_PHOTONS] ;
   float phoPFChIsoDR04       [MAX_PHOTONS] ;
   float phoPFNeuIsoDR04      [MAX_PHOTONS] ;
   float phoPFPhoIsoDR04      [MAX_PHOTONS] ;
   float r9                   [MAX_PHOTONS] ;
   bool  passelectronveto     [MAX_PHOTONS] ;
   float  EcalIso             [MAX_PHOTONS] ;
   float  HcalIso             [MAX_PHOTONS] ;
   float  TrackIso            [MAX_PHOTONS] ;
   float GenPt                [MAX_PHOTONS] ;
   float GenEta               [MAX_PHOTONS] ;
   float GenPhi               [MAX_PHOTONS] ;
   int   GenPdgID             [MAX_PHOTONS] ;

   void RegisterTree( TTree* root, std::string name = "PhotonInfo" ) {
      BRANCH_OBJ( Size , I  ) ;
      BRANCH_ARRAY( Pt                   , F ) ;
      BRANCH_ARRAY( Eta                  , F ) ;
      BRANCH_ARRAY( Phi                  , F ) ;
      BRANCH_ARRAY( HoverE               , F ) ;
      BRANCH_ARRAY( SigmaIetaIeta        , F ) ;
      BRANCH_ARRAY( hadTowOverEm         , F ) ;
      BRANCH_ARRAY( hcalIsoConeDR04_2012 , F ) ;
      BRANCH_ARRAY( phoPFChIsoDR03       , F ) ;
      BRANCH_ARRAY( phoPFNeuIsoDR03      , F ) ;
      BRANCH_ARRAY( phoPFPhoIsoDR03      , F ) ;
      BRANCH_ARRAY( phoPFChIsoDR04       , F ) ;
      BRANCH_ARRAY( phoPFNeuIsoDR04      , F ) ;
      BRANCH_ARRAY( phoPFPhoIsoDR04      , F ) ;
      BRANCH_ARRAY( r9                   , F ) ;
      BRANCH_ARRAY( passelectronveto     , B ) ;
      BRANCH_ARRAY( EcalIso              , F ) ;
      BRANCH_ARRAY( HcalIso              , F ) ;
      BRANCH_ARRAY( TrackIso             , F ) ;
      BRANCH_ARRAY( GenPt                , F ) ;
      BRANCH_ARRAY( GenEta               , F ) ;
      BRANCH_ARRAY( GenPhi               , F ) ;
      BRANCH_ARRAY( GenPdgID             , I ) ;
   }

   void Register( TTree* root, std::string name = "PhotonInfo" ) {
      SET_BRANCH_OBJ    ( Size                 ) ;
      SET_BRANCH_ARRAY( Pt                   ) ;
      SET_BRANCH_ARRAY( Eta                  ) ;
      SET_BRANCH_ARRAY( Phi                  ) ;
      SET_BRANCH_ARRAY( HoverE               ) ;
      SET_BRANCH_ARRAY( SigmaIetaIeta        ) ;
      SET_BRANCH_ARRAY( hadTowOverEm         ) ;
      SET_BRANCH_ARRAY( hcalIsoConeDR04_2012 ) ;
      SET_BRANCH_ARRAY( phoPFChIsoDR03       ) ;
      SET_BRANCH_ARRAY( phoPFNeuIsoDR03      ) ;
      SET_BRANCH_ARRAY( phoPFPhoIsoDR03      ) ;
      SET_BRANCH_ARRAY( phoPFChIsoDR04       ) ;
      SET_BRANCH_ARRAY( phoPFNeuIsoDR04      ) ;
      SET_BRANCH_ARRAY( phoPFPhoIsoDR04      ) ;
      SET_BRANCH_ARRAY( r9                   ) ;
      SET_BRANCH_ARRAY( passelectronveto     ) ;
      SET_BRANCH_ARRAY( EcalIso              ) ;
      SET_BRANCH_ARRAY( HcalIso              ) ;
      SET_BRANCH_ARRAY( TrackIso             ) ;
      SET_BRANCH_ARRAY( GenPt                ) ;
      SET_BRANCH_ARRAY( GenEta               ) ;
      SET_BRANCH_ARRAY( GenPhi               ) ;
      SET_BRANCH_ARRAY( GenPdgID             ) ;
   }
};

class VertexInfoBranches {
public:
   int     Size;
   int     isValid        [MAX_Vertices] ;
   bool    isFake         [MAX_Vertices] ; //Uly 2011-04-04
   int     Type           [MAX_Vertices] ;   //0 - Offline Primary Vertices, 1 - Offline Primary Vertices with beam spot constraint, 2 - Pixel Vertices
   float   Ndof           [MAX_Vertices] ;
   float   NormalizedChi2 [MAX_Vertices] ;
   float   Pt_Sum         [MAX_Vertices] ;
   float   Pt_Sum2        [MAX_Vertices] ;
   float   x              [MAX_Vertices] ;
   float   y              [MAX_Vertices] ;
   float   z              [MAX_Vertices] ;
   float   Rho            [MAX_Vertices] ;

   void RegisterTree( TTree* root , std::string name = "VertexInfo" ) {
      BRANCH_OBJ( Size , I );
      BRANCH_ARRAY(   isValid        , I  ) ;  
      BRANCH_ARRAY(   isFake         , B ) ;  
      BRANCH_ARRAY(   Type           , I  ) ; 
      BRANCH_ARRAY(   Ndof           , F ) ;
      BRANCH_ARRAY(   NormalizedChi2 , F ) ;
      BRANCH_ARRAY(   Pt_Sum         , F ) ;
      BRANCH_ARRAY(   Pt_Sum2        , F ) ;
      BRANCH_ARRAY(   x              , F ) ;
      BRANCH_ARRAY(   y              , F ) ;
      BRANCH_ARRAY(   z              , F ) ;
      BRANCH_ARRAY(   Rho            , F ) ;
   }
   void Register( TTree* root , std::string name="VertexInfo") {
      SET_BRANCH_OBJ( Size );
      SET_BRANCH_ARRAY( isValid        ) ;
      SET_BRANCH_ARRAY( isFake         ) ; //Uly 2011-04-04
      SET_BRANCH_ARRAY( Type           ) ;
      SET_BRANCH_ARRAY( Ndof           ) ;
      SET_BRANCH_ARRAY( NormalizedChi2 ) ;
      SET_BRANCH_ARRAY( Pt_Sum         ) ;
      SET_BRANCH_ARRAY( Pt_Sum2        ) ;
      SET_BRANCH_ARRAY( x              ) ;
      SET_BRANCH_ARRAY( y              ) ;
      SET_BRANCH_ARRAY( z              ) ;
      SET_BRANCH_ARRAY( Rho            ) ;
   }
};



class GenInfoBranches {
public:
   int Size;
   float Weight;
   float Pt           [MAX_GENS] ;
   float Eta          [MAX_GENS] ;
   float Phi          [MAX_GENS] ;
   float Mass         [MAX_GENS] ;
   int PdgID          [MAX_GENS] ;
   int PhotonFlag     [MAX_GENS] ;   // -1 : unknown or not photon, 0 : prompt photon, 1 : decay in flight, 2 : ISR, 3 : FSR
   int Status         [MAX_GENS] ;
   int nMo            [MAX_GENS] ;
   int nDa            [MAX_GENS] ;
   int Mo1            [MAX_GENS] ;
   int Mo2            [MAX_GENS] ;
   int Da1            [MAX_GENS] ;
   int Da2            [MAX_GENS] ;
   int Mo1PdgID       [MAX_GENS] ;
   int Mo2PdgID       [MAX_GENS] ;
   int Mo1Status      [MAX_GENS] ;
   int Mo2Status      [MAX_GENS] ;
   int Da1PdgID       [MAX_GENS] ;
   int Da2PdgID       [MAX_GENS] ;
   int GrandMo1PdgID  [MAX_GENS] ;
   int GrandMo2PdgID  [MAX_GENS] ;
   int GrandMo1Status [MAX_GENS] ;
   int GrandMo2Status [MAX_GENS] ;

   void RegisterTree( TTree* root    , std::string name = "GenInfo" ) {
      BRANCH_OBJ( Size               , I );
      BRANCH_OBJ( Weight             , F );
      BRANCH_ARRAY(    Pt            , F ) ;
      BRANCH_ARRAY(   Eta            , F ) ;
      BRANCH_ARRAY(   Phi            , F ) ;
      BRANCH_ARRAY(   Mass           , F ) ;
      BRANCH_ARRAY(   PdgID          , I ) ;
      BRANCH_ARRAY(   PhotonFlag     , I ) ;
      BRANCH_ARRAY(   Status         , I ) ;
      BRANCH_ARRAY(   nMo            , I ) ;
      BRANCH_ARRAY(   nDa            , I ) ;
      BRANCH_ARRAY(   Mo1            , I ) ;
      BRANCH_ARRAY(   Mo2            , I ) ;
      BRANCH_ARRAY(   Da1            , I ) ;
      BRANCH_ARRAY(   Da2            , I ) ;
      BRANCH_ARRAY(   Mo1PdgID       , I ) ;
      BRANCH_ARRAY(   Mo2PdgID       , I ) ;
      BRANCH_ARRAY(   Mo1Status      , I ) ;
      BRANCH_ARRAY(   Mo2Status      , I ) ;
      BRANCH_ARRAY(   Da1PdgID       , I ) ;
      BRANCH_ARRAY(   Da2PdgID       , I ) ;
      BRANCH_ARRAY(   GrandMo1PdgID  , I ) ;
      BRANCH_ARRAY(   GrandMo2PdgID  , I ) ;
      BRANCH_ARRAY(   GrandMo1Status , I ) ;
      BRANCH_ARRAY(   GrandMo2Status , I ) ;
   }

   void Register( TTree* root , std::string name = "GenInfo" ) {
      SET_BRANCH_OBJ( Size );
      SET_BRANCH_OBJ( Weight );
      SET_BRANCH_ARRAY( Pt             ) ;
      SET_BRANCH_ARRAY( Eta            ) ;
      SET_BRANCH_ARRAY( Phi            ) ;
      SET_BRANCH_ARRAY( Mass           ) ;
      SET_BRANCH_ARRAY( PdgID          ) ;
      SET_BRANCH_ARRAY( PhotonFlag     ) ;
      SET_BRANCH_ARRAY( Status         ) ;
      SET_BRANCH_ARRAY( nMo            ) ;
      SET_BRANCH_ARRAY( nDa            ) ;
      SET_BRANCH_ARRAY( Mo1            ) ;
      SET_BRANCH_ARRAY( Mo2            ) ;
      SET_BRANCH_ARRAY( Da1            ) ;
      SET_BRANCH_ARRAY( Da2            ) ;
      SET_BRANCH_ARRAY( Mo1PdgID       ) ;
      SET_BRANCH_ARRAY( Mo2PdgID       ) ;
      SET_BRANCH_ARRAY( Mo1Status      ) ;
      SET_BRANCH_ARRAY( Mo2Status      ) ;
      SET_BRANCH_ARRAY( Da1PdgID       ) ;
      SET_BRANCH_ARRAY( Da1PdgID       ) ;
      SET_BRANCH_ARRAY( Da2PdgID       ) ;
      SET_BRANCH_ARRAY( GrandMo1PdgID  ) ;
      SET_BRANCH_ARRAY( GrandMo2PdgID  ) ;
      SET_BRANCH_ARRAY( GrandMo1Status ) ;
      SET_BRANCH_ARRAY( GrandMo2Status ) ;
   }

};

//------------------------------------------------------------------------------ 
//   Un-defining MACROS functions 
//------------------------------------------------------------------------------ 
#undef BRANCH_OBJ 
#undef BRANCH_ARRAY
#undef BRANCH_FIX_ARRAY
#undef SET_BRANCH_OBJ 
#undef SET_BRANCH_ARRAY 

#endif
