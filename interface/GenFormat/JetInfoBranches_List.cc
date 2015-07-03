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
//-------------------------  Particle flow jet information  -------------------------
int   NCH                     [MAX_JETS] ;
float CEF                     [MAX_JETS] ;
float NHF                     [MAX_JETS] ;
float NEF                     [MAX_JETS] ;
float CHF                     [MAX_JETS] ;
float JVAlpha                 [MAX_JETS] ;
float JVBeta                  [MAX_JETS] ;

//--------------------------  Jet Correction information  ---------------------------
float PtCorrRaw               [MAX_JETS] ;
float PtCorrL2                [MAX_JETS] ;
float PtCorrL3                [MAX_JETS] ;
float PtCorrL7g               [MAX_JETS] ;
float PtCorrL7uds             [MAX_JETS] ;
float PtCorrL7c               [MAX_JETS] ;
float PtCorrL7b               [MAX_JETS] ;
float JetBProbBJetTags        [MAX_JETS] ;
float CombinedSVBJetTags      [MAX_JETS] ;
float CombinedSVMVABJetTags   [MAX_JETS] ;

//----------------------------  Generation information  -----------------------------
float GenJetPt                [MAX_JETS] ;
float GenJetEta               [MAX_JETS] ;
float GenJetPhi               [MAX_JETS] ;
float GenPt                   [MAX_JETS] ;
float GenEta                  [MAX_JETS] ;
float GenPhi                  [MAX_JETS] ;
int   GenPdgID                [MAX_JETS] ;
int   GenFlavor               [MAX_JETS] ;
int   GenMCTag                [MAX_JETS] ; // 0: unknown, 1: decay from W, 2: decay from Z, (+10) from b', (+20) from t'

//------------------------------  Subjet information  -------------------------------
int NSubjets                  [MAX_JETS] ;
int SubjetsIdxStart           [MAX_JETS] ;
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

