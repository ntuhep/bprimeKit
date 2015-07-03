//------------------------------------------------------------------------------ 
//   List of variables used by Lepton info branches
//------------------------------------------------------------------------------ 
   
int   Size;
int   Index                                 [MAX_LEPTONS] ;
int   LeptonType                            [MAX_LEPTONS] ;
int   Charge                                [MAX_LEPTONS] ;
float Pt                                    [MAX_LEPTONS] ;
float Et                                    [MAX_LEPTONS] ;
float Eta                                   [MAX_LEPTONS] ;
float Phi                                   [MAX_LEPTONS] ;
float Px                                    [MAX_LEPTONS] ; //Uly 2011-04-04
float Py                                    [MAX_LEPTONS] ; //Uly 2011-04-04
float Pz                                    [MAX_LEPTONS] ; //Uly 2011-04-04
float Energy                                [MAX_LEPTONS] ; //Uly 2011-04-04
float TrackIso                              [MAX_LEPTONS] ;
float EcalIso                               [MAX_LEPTONS] ;
float HcalIso                               [MAX_LEPTONS] ;
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

//------------------------------------------------------------------------------ 
//   Muon Information
//------------------------------------------------------------------------------
//------------------------------  General Information  ------------------------------
float  CaloEnergy                   [MAX_LEPTONS]  ;
bool   isGoodMuonTMOneStationTight  [MAX_LEPTONS]  ; //   For     Soft        Muon
bool   isPFMuon                     [MAX_LEPTONS]  ;
bool   MuIDGlobalMuonPromptTight    [MAX_LEPTONS]  ;
float  MuGlobalNormalizedChi2       [MAX_LEPTONS]  ; //   Dmitry
float  MuCaloCompat                 [MAX_LEPTONS]  ;
int    MuNChambers                  [MAX_LEPTONS]  ;
int    MuNChambersMatchesSegment    [MAX_LEPTONS]  ;
int    MuNMatchedStations           [MAX_LEPTONS]  ;
int    MuNLostOuterHits             [MAX_LEPTONS]  ;
int    MuNMuonhits                  [MAX_LEPTONS]  ;
int    MuDThits                     [MAX_LEPTONS]  ;
int    MuCSChits                    [MAX_LEPTONS]  ;
int    MuRPChits                    [MAX_LEPTONS]  ;
int    MuType                       [MAX_LEPTONS]  ; 
//--------------------------------  Cosmic Ray filter --------------------------
int    MuontimenDof                 [MAX_LEPTONS]  ;
float  MuontimeAtIpInOut            [MAX_LEPTONS]  ;
float  MuontimeAtIpOutIn            [MAX_LEPTONS]  ;
int    Muondirection                [MAX_LEPTONS]  ;
//----------------------------  Inner Muon Information  -----------------------------
float  innerTracknormalizedChi2     [MAX_LEPTONS]  ;   // For     Soft        Muon
float  MuInnerPtError               [MAX_LEPTONS]  ;
float  MuGlobalPtError              [MAX_LEPTONS]  ;
float  MuInnerTrackDz               [MAX_LEPTONS]  ;
float  MuInnerTrackD0               [MAX_LEPTONS]  ;
float  MuInnerTrackDxy_BS           [MAX_LEPTONS]  ;
float  MuInnerTrackDxy_PV           [MAX_LEPTONS]  ;
float  MuInnerTrackDxy_PVBS         [MAX_LEPTONS]  ;
int    MuInnerTrackNHits            [MAX_LEPTONS]  ;
int    MuNTrackerHits               [MAX_LEPTONS]  ;
int    MuNLostInnerHits             [MAX_LEPTONS]  ;
float  vertexZ                      [MAX_LEPTONS]  ; //   Uly     2011-04-04
int    MuNPixelLayers               [MAX_LEPTONS]  ;
int    MuNPixelLayersWMeasurement   [MAX_LEPTONS]  ; //   Uly     2011-04-04
int    MuNTrackLayersWMeasurement   [MAX_LEPTONS]  ;

//------------------------------------------------------------------------------ 
//   Electron Information
//------------------------------------------------------------------------------  
int   ChargeGsf                             [MAX_LEPTONS] ;
int   ChargeCtf                             [MAX_LEPTONS] ;
int   ChargeScPix                           [MAX_LEPTONS] ;
int   isEcalDriven                          [MAX_LEPTONS] ;
int   isTrackerDriven                       [MAX_LEPTONS] ;
float caloEta                               [MAX_LEPTONS] ;
float e1x5                                  [MAX_LEPTONS] ;
float e2x5Max                               [MAX_LEPTONS] ;
float e5x5                                  [MAX_LEPTONS] ;
float HcalDepth1Iso                         [MAX_LEPTONS] ;
float HcalDepth2Iso                         [MAX_LEPTONS] ;
float EgammaMVANonTrig                      [MAX_LEPTONS] ; // Add by Jacky
float EgammaMVATrig                         [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdTRIGGERTIGHT       [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdTRIGGERWP70        [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdVETO               [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdLOOSE              [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdMEDIUM             [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdTIGHT              [MAX_LEPTONS] ; // Add by Jacky
bool  EgammaCutBasedEleIdHEEP               [MAX_LEPTONS] ;
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
float ElSharedHitsFraction                  [MAX_LEPTONS] ; //Dmitry
float dR_gsf_ctfTrack                       [MAX_LEPTONS] ; //Dmitry
float dPt_gsf_ctfTrack                      [MAX_LEPTONS] ; //Dmitry
bool  ElhasConv                             [MAX_LEPTONS] ;
//-------------------------------  Track information  -------------------------------
int   ElTrackNHits                          [MAX_LEPTONS] ; //Dmitry
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

//-----------------------------  Conversion rejection  ------------------------------
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

//------------------------------------------------------------------------------ 
//   Tau information
//------------------------------------------------------------------------------  
int   isPFTau                                      [MAX_LEPTONS] ;    // YoungKyu 2012-10-16
bool   DiscriminationByDecayModeFinding            [MAX_LEPTONS] ;
bool   DiscriminationByDecayModeFindingNewDMs      [MAX_LEPTONS] ;
bool   ByLooseElectronRejection                    [MAX_LEPTONS] ;
bool   ByMediumElectronRejection                   [MAX_LEPTONS] ;
bool   ByTightElectronRejection                    [MAX_LEPTONS] ;
bool   ByMVA5LooseElectronRejection                [MAX_LEPTONS] ;
bool   ByMVA5MediumElectronRejection               [MAX_LEPTONS] ;
bool   ByMVA5TightElectronRejection                [MAX_LEPTONS] ;
bool   ByMVA5VTightElectronRejection               [MAX_LEPTONS] ;
bool   ByLooseMuonRejection3                       [MAX_LEPTONS] ;
bool   ByTightMuonRejection3                       [MAX_LEPTONS] ;
bool   ByMVALooseMuonRejection                     [MAX_LEPTONS] ;
bool   ByMVAMediumMuonRejection                    [MAX_LEPTONS] ;
bool   ByMVATightMuonRejection                     [MAX_LEPTONS] ;
bool   ByLooseCombinedIsolationDeltaBetaCorr3Hits  [MAX_LEPTONS] ;
bool   ByMediumCombinedIsolationDeltaBetaCorr3Hits [MAX_LEPTONS] ;
bool   ByTightCombinedIsolationDeltaBetaCorr3Hits  [MAX_LEPTONS] ;
float   CombinedIsolationDeltaBetaCorrRaw3Hits     [MAX_LEPTONS] ;
bool   ByVLooseIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
bool   ByLooseIsolationMVA3newDMwoLT               [MAX_LEPTONS] ;
bool   ByMediumIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
bool   ByTightIsolationMVA3newDMwoLT               [MAX_LEPTONS] ;
bool   ByVTightIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
bool   ByVVTightIsolationMVA3newDMwoLT             [MAX_LEPTONS] ;
float   ByIsolationMVA3newDMwoLTraw                [MAX_LEPTONS] ;
bool   ByVLooseIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
bool   ByLooseIsolationMVA3oldDMwLT                [MAX_LEPTONS] ;
bool   ByMediumIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
bool   ByTightIsolationMVA3oldDMwLT                [MAX_LEPTONS] ;
bool   ByVTightIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
bool   ByVVTightIsolationMVA3oldDMwLT              [MAX_LEPTONS] ;
float   ByIsolationMVA3oldDMwLTraw                 [MAX_LEPTONS] ;
bool   ByVLooseIsolationMVA3oldDMwoLT              [MAX_LEPTONS] ;
bool   ByLooseIsolationMVA3oldDMwoLT               [MAX_LEPTONS] ;
bool   ByTightIsolationMVA3oldDMwoLT               [MAX_LEPTONS] ;
bool   ByVTightIsolationMVA3oldDMwoLT              [MAX_LEPTONS] ;
bool   ByVVTightIsolationMVA3oldDMwoLT             [MAX_LEPTONS] ;
float   ByIsolationMVA3oldDMwoLTraw                [MAX_LEPTONS] ;
bool   ByVLooseIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
bool   ByLooseIsolationMVA3newDMwLT                [MAX_LEPTONS] ;
bool   ByMediumIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
bool   ByTightIsolationMVA3newDMwLT                [MAX_LEPTONS] ;
bool   ByVTightIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
bool   ByVVTightIsolationMVA3newDMwLT              [MAX_LEPTONS] ;
float   ByIsolationMVA3newDMwLTraw                 [MAX_LEPTONS] ;

//-------------------------------  GenMG information  -------------------------------
float GenPt                                 [MAX_LEPTONS] ;
float GenEta                                [MAX_LEPTONS] ;
float GenPhi                                [MAX_LEPTONS] ;
int   GenPdgID                              [MAX_LEPTONS] ;
int   GenMCTag                              [MAX_LEPTONS] ;
#ifdef __BPRIMEKIT__
   reco::Candidate* CandRef[MAX_LEPTONS]; // backward pointer to the PAT objects
#endif

