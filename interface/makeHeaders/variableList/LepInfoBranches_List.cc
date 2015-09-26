Int_t   Size;
Int_t   Index                                 [MAX_LEPTONS] ;
//----- Generic information  -----------------------------------------------------------------------
Int_t   LeptonType                            [MAX_LEPTONS] ;
Int_t   Charge                                [MAX_LEPTONS] ;
Float_t Pt                                    [MAX_LEPTONS] ;
Float_t Et                                    [MAX_LEPTONS] ;
Float_t Eta                                   [MAX_LEPTONS] ;
Float_t Phi                                   [MAX_LEPTONS] ;
Float_t Px                                    [MAX_LEPTONS] ; //Uly 2011-04-04
Float_t Py                                    [MAX_LEPTONS] ; //Uly 2011-04-04
Float_t Pz                                    [MAX_LEPTONS] ; //Uly 2011-04-04
Float_t Energy                                [MAX_LEPTONS] ; //Uly 2011-04-04
Float_t TrackIso                              [MAX_LEPTONS] ;
Float_t EcalIso                               [MAX_LEPTONS] ;
Float_t HcalIso                               [MAX_LEPTONS] ;
Float_t ChargedHadronIso                      [MAX_LEPTONS] ;
Float_t NeutralHadronIso                      [MAX_LEPTONS] ;
Float_t PhotonIso                             [MAX_LEPTONS] ;
Float_t SumPUPt                               [MAX_LEPTONS] ;
Float_t ChargedHadronIsoR03                   [MAX_LEPTONS] ;
Float_t NeutralHadronIsoR03                   [MAX_LEPTONS] ;
Float_t PhotonIsoR03                          [MAX_LEPTONS] ;
Float_t sumPUPtR03                            [MAX_LEPTONS] ;
Float_t IsoRhoCorrR03                         [MAX_LEPTONS] ;
Float_t ChargedHadronIsoR04                   [MAX_LEPTONS] ;
Float_t NeutralHadronIsoR04                   [MAX_LEPTONS] ;
Float_t PhotonIsoR04                          [MAX_LEPTONS] ;
Float_t sumPUPtR04                            [MAX_LEPTONS] ;
Float_t IsoRhoCorrR04                         [MAX_LEPTONS] ;
Float_t Ip3dPV                                [MAX_LEPTONS] ;
Float_t Ip3dPVErr                             [MAX_LEPTONS] ;
Float_t Ip3dPVSignificance                    [MAX_LEPTONS] ;

//------------------------------------------------------------------------------ 
//   Muon Information
//------------------------------------------------------------------------------

//----- General Muon information  ------------------------------------------------------------------
Float_t  CaloEnergy                   [MAX_LEPTONS]  ;
Bool_t   isGoodMuonTMOneStationTight  [MAX_LEPTONS]  ; //   For     Soft        Muon
Bool_t   isPFMuon                     [MAX_LEPTONS]  ;
Bool_t   MuIDGlobalMuonPromptTight    [MAX_LEPTONS]  ;
Float_t  MuGlobalNormalizedChi2       [MAX_LEPTONS]  ; //   Dmitry
Float_t  MuCaloCompat                 [MAX_LEPTONS]  ;
Int_t    MuNChambers                  [MAX_LEPTONS]  ;
Int_t    MuNChambersMatchesSegment    [MAX_LEPTONS]  ;
Int_t    MuNMatchedStations           [MAX_LEPTONS]  ;
Int_t    MuNLostOuterHits             [MAX_LEPTONS]  ;
Int_t    MuNMuonhits                  [MAX_LEPTONS]  ;
Int_t    MuDThits                     [MAX_LEPTONS]  ;
Int_t    MuCSChits                    [MAX_LEPTONS]  ;
Int_t    MuRPChits                    [MAX_LEPTONS]  ;
Int_t    MuType                       [MAX_LEPTONS]  ; 

//----- Cosmic Ray filters  ------------------------------------------------------------------------
Int_t    MuontimenDof                 [MAX_LEPTONS]  ;
Float_t  MuontimeAtIpInOut            [MAX_LEPTONS]  ;
Float_t  MuontimeAtIpOutIn            [MAX_LEPTONS]  ;
Int_t    Muondirection                [MAX_LEPTONS]  ;

//----- Inner Muon information  --------------------------------------------------------------------
Float_t  innerTracknormalizedChi2     [MAX_LEPTONS]  ;   // For     Soft        Muon
Float_t  MuInnerPtError               [MAX_LEPTONS]  ;
Float_t  MuGlobalPtError              [MAX_LEPTONS]  ;
Float_t  MuInnerTrackDz               [MAX_LEPTONS]  ;
Float_t  MuInnerTrackD0               [MAX_LEPTONS]  ;
Float_t  MuInnerTrackDxy_BS           [MAX_LEPTONS]  ;
Float_t  MuInnerTrackDxy_PV           [MAX_LEPTONS]  ;
Float_t  MuInnerTrackDxy_PVBS         [MAX_LEPTONS]  ;
Int_t    MuInnerTrackNHits            [MAX_LEPTONS]  ;
Int_t    MuNTrackerHits               [MAX_LEPTONS]  ;
Int_t    MuNLostInnerHits             [MAX_LEPTONS]  ;
Float_t  vertexZ                      [MAX_LEPTONS]  ; //   Uly     2011-04-04
Int_t    MuNPixelLayers               [MAX_LEPTONS]  ;
Int_t    MuNPixelLayersWMeasurement   [MAX_LEPTONS]  ; //   Uly     2011-04-04
Int_t    MuNTrackLayersWMeasurement   [MAX_LEPTONS]  ;

//------------------------------------------------------------------------------ 
//   Electron Information
//------------------------------------------------------------------------------  
Int_t   ChargeGsf                             [MAX_LEPTONS] ;
Int_t   ChargeCtf                             [MAX_LEPTONS] ;
Int_t   ChargeScPix                           [MAX_LEPTONS] ;
Int_t   isEcalDriven                          [MAX_LEPTONS] ;
Int_t   isTrackerDriven                       [MAX_LEPTONS] ;
Float_t caloEta                               [MAX_LEPTONS] ;
Float_t e1x5                                  [MAX_LEPTONS] ;
Float_t e2x5Max                               [MAX_LEPTONS] ;
Float_t e5x5                                  [MAX_LEPTONS] ;
Float_t HcalDepth1Iso                         [MAX_LEPTONS] ;
Float_t HcalDepth2Iso                         [MAX_LEPTONS] ;
Float_t EgammaMVANonTrig                      [MAX_LEPTONS] ; // Add by Jacky
Float_t EgammaMVATrig                         [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdTRIGGERTIGHT       [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdTRIGGERWP70        [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdVETO               [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdLOOSE              [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdMEDIUM             [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdTIGHT              [MAX_LEPTONS] ; // Add by Jacky
Bool_t  EgammaCutBasedEleIdHEEP               [MAX_LEPTONS] ;
Float_t Eldr03HcalDepth1TowerSumEtBc          [MAX_LEPTONS] ;
Float_t Eldr03HcalDepth2TowerSumEtBc          [MAX_LEPTONS] ;
Float_t Eldr04HcalDepth1TowerSumEtBc          [MAX_LEPTONS] ;
Float_t Eldr04HcalDepth2TowerSumEtBc          [MAX_LEPTONS] ;
Float_t ElhcalOverEcalBc                      [MAX_LEPTONS] ;
Float_t ElEcalE                               [MAX_LEPTONS] ;
Float_t ElEoverP                              [MAX_LEPTONS] ;
Float_t EldeltaEta                            [MAX_LEPTONS] ;
Float_t EldeltaPhi                            [MAX_LEPTONS] ;
Float_t ElHadoverEm                           [MAX_LEPTONS] ;
Float_t ElsigmaIetaIeta                       [MAX_LEPTONS] ; // Jacky
Float_t ElscSigmaIetaIeta                     [MAX_LEPTONS] ; // Jacky
Float_t ElEnergyErr                           [MAX_LEPTONS] ;
Float_t ElMomentumErr                         [MAX_LEPTONS] ;
Float_t ElSharedHitsFraction                  [MAX_LEPTONS] ; //Dmitry
Float_t dR_gsf_ctfTrack                       [MAX_LEPTONS] ; //Dmitry
Float_t dPt_gsf_ctfTrack                      [MAX_LEPTONS] ; //Dmitry
Bool_t  ElhasConv                             [MAX_LEPTONS] ;

//----- Track related information  -----------------------------------------------------------------
Int_t   ElTrackNHits                          [MAX_LEPTONS] ; //Dmitry
Float_t ElTrackNLostHits                      [MAX_LEPTONS] ;  //yjlei
Float_t ElTrackDz                             [MAX_LEPTONS] ;
Float_t ElTrackDz_BS                          [MAX_LEPTONS] ;
Float_t ElTrackD0                             [MAX_LEPTONS] ;
Float_t ElTrackDxy_BS                         [MAX_LEPTONS] ;
Float_t ElTrackDxy_PV                         [MAX_LEPTONS] ;
Float_t ElTrackDxy_PVBS                       [MAX_LEPTONS] ; //yjlei
Int_t   ElNClusters                           [MAX_LEPTONS] ;
Int_t   ElClassification                      [MAX_LEPTONS] ;
Float_t ElFBrem                               [MAX_LEPTONS] ;
Int_t NumberOfExpectedInnerHits               [MAX_LEPTONS] ; // Add by Jacky

//----- Conversion rejections  ---------------------------------------------------------------------
Float_t Eldist                                [MAX_LEPTONS] ; // Add by Jacky
Float_t Eldcot                                [MAX_LEPTONS] ; // Add by Jacky
Float_t Elconvradius                          [MAX_LEPTONS] ; // Add by Jacky
Float_t ElConvPoint_x                         [MAX_LEPTONS] ; // Add by Jacky
Float_t ElConvPoint_y                         [MAX_LEPTONS] ; // Add by Jacky
Float_t ElConvPoint_z                         [MAX_LEPTONS] ; // Add by Jacky
Float_t dcotdist                              [MAX_LEPTONS] ;
Float_t ElseedEoverP                          [MAX_LEPTONS] ;
Float_t ElEcalIso04                           [MAX_LEPTONS] ;
Float_t ElHcalIso04                           [MAX_LEPTONS] ;
Int_t   ElNumberOfBrems                       [MAX_LEPTONS] ;
Float_t TrgPt                                 [MAX_LEPTONS] ;
Float_t TrgEta                                [MAX_LEPTONS] ;
Float_t TrgPhi                                [MAX_LEPTONS] ;
Int_t TrgID                                   [MAX_LEPTONS] ;

//------------------------------------------------------------------------------ 
//   Tau information
//------------------------------------------------------------------------------  
Int_t   isPFTau                                      [MAX_LEPTONS] ;    // YoungKyu 2012-10-16
Bool_t   DiscriminationByDecayModeFinding            [MAX_LEPTONS] ;
Bool_t   DiscriminationByDecayModeFindingNewDMs      [MAX_LEPTONS] ;
Bool_t   ByLooseElectronRejection                    [MAX_LEPTONS] ;
Bool_t   ByMediumElectronRejection                   [MAX_LEPTONS] ;
Bool_t   ByTightElectronRejection                    [MAX_LEPTONS] ;
Bool_t   ByMVA5LooseElectronRejection                [MAX_LEPTONS] ;
Bool_t   ByMVA5MediumElectronRejection               [MAX_LEPTONS] ;
Bool_t   ByMVA5TightElectronRejection                [MAX_LEPTONS] ;
Bool_t   ByMVA5VTightElectronRejection               [MAX_LEPTONS] ;
Bool_t   ByLooseMuonRejection3                       [MAX_LEPTONS] ;
Bool_t   ByTightMuonRejection3                       [MAX_LEPTONS] ;
Bool_t   ByMVALooseMuonRejection                     [MAX_LEPTONS] ;
Bool_t   ByMVAMediumMuonRejection                    [MAX_LEPTONS] ;
Bool_t   ByMVATightMuonRejection                     [MAX_LEPTONS] ;
Bool_t   ByLooseCombinedIsolationDeltaBetaCorr3Hits  [MAX_LEPTONS] ;
Bool_t   ByMediumCombinedIsolationDeltaBetaCorr3Hits [MAX_LEPTONS] ;
Bool_t   ByTightCombinedIsolationDeltaBetaCorr3Hits  [MAX_LEPTONS] ;
Float_t   CombinedIsolationDeltaBetaCorrRaw3Hits     [MAX_LEPTONS] ;
Bool_t   ByVLooseIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
Bool_t   ByLooseIsolationMVA3newDMwoLT               [MAX_LEPTONS] ;
Bool_t   ByMediumIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
Bool_t   ByTightIsolationMVA3newDMwoLT               [MAX_LEPTONS] ;
Bool_t   ByVTightIsolationMVA3newDMwoLT              [MAX_LEPTONS] ;
Bool_t   ByVVTightIsolationMVA3newDMwoLT             [MAX_LEPTONS] ;
Float_t   ByIsolationMVA3newDMwoLTraw                [MAX_LEPTONS] ;
Bool_t   ByVLooseIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
Bool_t   ByLooseIsolationMVA3oldDMwLT                [MAX_LEPTONS] ;
Bool_t   ByMediumIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
Bool_t   ByTightIsolationMVA3oldDMwLT                [MAX_LEPTONS] ;
Bool_t   ByVTightIsolationMVA3oldDMwLT               [MAX_LEPTONS] ;
Bool_t   ByVVTightIsolationMVA3oldDMwLT              [MAX_LEPTONS] ;
Float_t   ByIsolationMVA3oldDMwLTraw                 [MAX_LEPTONS] ;
Bool_t   ByVLooseIsolationMVA3oldDMwoLT              [MAX_LEPTONS] ;
Bool_t   ByLooseIsolationMVA3oldDMwoLT               [MAX_LEPTONS] ;
Bool_t   ByTightIsolationMVA3oldDMwoLT               [MAX_LEPTONS] ;
Bool_t   ByVTightIsolationMVA3oldDMwoLT              [MAX_LEPTONS] ;
Bool_t   ByVVTightIsolationMVA3oldDMwoLT             [MAX_LEPTONS] ;
Float_t   ByIsolationMVA3oldDMwoLTraw                [MAX_LEPTONS] ;
Bool_t   ByVLooseIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
Bool_t   ByLooseIsolationMVA3newDMwLT                [MAX_LEPTONS] ;
Bool_t   ByMediumIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
Bool_t   ByTightIsolationMVA3newDMwLT                [MAX_LEPTONS] ;
Bool_t   ByVTightIsolationMVA3newDMwLT               [MAX_LEPTONS] ;
Bool_t   ByVVTightIsolationMVA3newDMwLT              [MAX_LEPTONS] ;
Float_t   ByIsolationMVA3newDMwLTraw                 [MAX_LEPTONS] ;

//----- Generation Monte Carlo information  --------------------------------------------------------
Float_t GenPt                                 [MAX_LEPTONS] ;
Float_t GenEta                                [MAX_LEPTONS] ;
Float_t GenPhi                                [MAX_LEPTONS] ;
Int_t   GenPdgID                              [MAX_LEPTONS] ;
Int_t   GenMCTag                              [MAX_LEPTONS] ;
#ifdef __BPRIMEKIT_H__
   reco::Candidate* CandRef[MAX_LEPTONS]; // backward poInt_ter to the PAT objects
#endif

