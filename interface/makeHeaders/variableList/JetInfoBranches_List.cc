Int_t   Size;
Int_t   Index                   [MAX_JETS] ;
//----- Generic information  -----------------------------------------------------------------------
Int_t   NTracks                 [MAX_JETS] ;
Float_t Et                      [MAX_JETS] ;
Float_t Pt                      [MAX_JETS] ;
Float_t Unc                     [MAX_JETS] ;
Float_t Eta                     [MAX_JETS] ;
Float_t Phi                     [MAX_JETS] ;
Float_t Px                      [MAX_JETS] ; //Uly 2011-04-04
Float_t Py                      [MAX_JETS] ; //Uly 2011-04-04
Float_t Pz                      [MAX_JETS] ; //Uly 2011-04-04
Float_t Energy                  [MAX_JETS] ; //Uly 2011-04-04
Float_t Mass                    [MAX_JETS] ;
Float_t Area                    [MAX_JETS] ;
Int_t   JetIDLOOSE              [MAX_JETS] ; //Add by Chiyi
Float_t JetCharge               [MAX_JETS] ;
//----- QG tagger variables  -----------------------------------------------------------------------
Float_t QGTagsLikelihood        [MAX_JETS] ;
Float_t QGTagsAxis2             [MAX_JETS] ;
Float_t QGTagsMult              [MAX_JETS] ;
Float_t QGTagsPtD               [MAX_JETS] ; 
Int_t   NConstituents           [MAX_JETS] ;

Int_t   NCH                     [MAX_JETS] ;
Float_t CEF                     [MAX_JETS] ;
Float_t NHF                     [MAX_JETS] ;
Float_t NEF                     [MAX_JETS] ;
Float_t CHF                     [MAX_JETS] ;
Float_t JVAlpha                 [MAX_JETS] ;
Float_t JVBeta                  [MAX_JETS] ;

//----- Jet Corrections information  ---------------------------------------------------------------
Float_t PtCorrRaw                                    [MAX_JETS] ;
Float_t PtCorrL2                                     [MAX_JETS] ;
Float_t PtCorrL3                                     [MAX_JETS] ;
Float_t PtCorrL7g                                    [MAX_JETS] ;
Float_t PtCorrL7uds                                  [MAX_JETS] ;
Float_t PtCorrL7c                                    [MAX_JETS] ;
Float_t PtCorrL7b                                    [MAX_JETS] ;
//----- Btags  -------------------------------------------------------------------------------------
Float_t combinedSecondaryVertexBJetTags              [MAX_JETS] ;
Float_t pfJetBProbabilityBJetTags                    [MAX_JETS] ;
Float_t pfJetProbabilityBJetTags                     [MAX_JETS] ;
Float_t pfTrackCountingHighPurBJetTags               [MAX_JETS] ;
Float_t pfTrackCountingHighEffBJetTags               [MAX_JETS] ;
Float_t pfSimpleSecondaryVertexHighEffBJetTags       [MAX_JETS] ;
Float_t pfSimpleSecondaryVertexHighPurBJetTags       [MAX_JETS] ;
Float_t pfCombinedSecondaryVertexV2BJetTags          [MAX_JETS] ;
Float_t pfCombinedInclusiveSecondaryVertexV2BJetTags [MAX_JETS] ;
Float_t pfCombinedSecondaryVertexSoftLeptonBJetTags  [MAX_JETS] ;
Float_t pfCombinedMVABJetTags                        [MAX_JETS] ;

//----- Generation MC information  -----------------------------------------------------------------
Float_t GenJetPt                [MAX_JETS] ;
Float_t GenJetEta               [MAX_JETS] ;
Float_t GenJetPhi               [MAX_JETS] ;
Float_t GenPt                   [MAX_JETS] ;
Float_t GenEta                  [MAX_JETS] ;
Float_t GenPhi                  [MAX_JETS] ;
Int_t   GenPdgID                [MAX_JETS] ;
Int_t   GenFlavor               [MAX_JETS] ;
Int_t   GenHadronFlavor         [MAX_JETS] ;
Int_t   GenMCTag                [MAX_JETS] ; // 0: unknown, 1: decay from W, 2: decay from Z, (+10) from b', (+20) from t'

//----- SubjetInformation  -------------------------------------------------------------------------
Int_t NSubjets                    [MAX_JETS] ;
Int_t SubjetsIdxStart             [MAX_JETS] ;
Float_t NjettinessAK8tau1         [MAX_JETS] ;
Float_t NjettinessAK8tau2         [MAX_JETS] ;
Float_t NjettinessAK8tau3         [MAX_JETS] ;
Float_t ak8PFJetsCHSSoftDropMass  [MAX_JETS] ;
Float_t  ak8PFJetsCHSPrunedMass   [MAX_JETS] ;
Float_t  ak8PFJetsCHSTrimmedMass  [MAX_JETS] ;
Float_t  ak8PFJetsCHSFilteredMass [MAX_JETS] ;
Float_t topJetMass [MAX_JETS];
Float_t ca8TopMass [MAX_JETS];
Float_t ca8MinMass [MAX_JETS];
//----- Vector poInt_ter types used for reading  -----------------------------------------------------
std::vector<Float_t>* SubjetMass                ;
std::vector<Float_t>* SubjetPt                  ;
std::vector<Float_t>* SubjetEt                  ;
std::vector<Float_t>* SubjetEta                 ;
std::vector<Float_t>* SubjetPhi                 ;
std::vector<Float_t>* SubjetCombinedSVBJetTags  ;
std::vector<Float_t>* SubjetPtUncorr            ;
std::vector<Float_t>* SubjetArea                ;
//----- Vector type used for writing  --------------------------------------------------------------
std::vector<Float_t> SubjetMass_w               ;
std::vector<Float_t> SubjetPt_w                 ;
std::vector<Float_t> SubjetEt_w                 ;
std::vector<Float_t> SubjetEta_w                ;
std::vector<Float_t> SubjetPhi_w                ;
std::vector<Float_t> SubjetCombinedSVBJetTags_w ;
std::vector<Float_t> SubjetPtUncorr_w           ;
std::vector<Float_t> SubjetArea_w               ;

#ifdef __BPRIMEKIT_H__
   reco::Candidate* CandRef[MAX_JETS]; // backward poInt_ter to the PAT objects
#endif
