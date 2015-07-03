int     RunNo  ;
size_t  EvtNo  ;
int     BxNo   ;
int     LumiNo ;
int     Orbit  ;
//--------------------------------  MC information  ---------------------------------
int   McFlag            ; // MC or not MC, that's the question
int   McSigTag          ; // MC Signature tag    - 0: others, 1: 2L (opposite-sign), 2: 2L (same-sign), 3: 3L, 4: 4L
int   McbprimeMode [2]  ; // b'(bar) decay mode   - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
int   MctprimeMode [2]  ; // t'(bar) decay mode   - 0: others, 1: bW, 2: tZ, 3: tH, 4: tgamma
int   McWMode      [4]  ; // W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar - 0: others, 1: enu, 2: munu, 3: taunu, 4: jj
int   McZMode      [2]  ; // Z from b'(bar)/t'(t'bar)  - 0: others, 1: ee, 2: mumu, 3: tautau, 4: nunu, 5: bb, 6: jj
float McbprimeMass [2]  ; // mass: b'(bar)
float MctprimeMass [2]  ; // mass: t'(bar)
float MctopMass    [2]  ; // mass: top(bar)
float McWMass      [4]  ; // mass: W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar
float McZMass      [2]  ; // mass: Z from b'(bar) or t'(bar)
float McDauPt      [14] ; // Generic MC daughter information
float McDauEta     [14] ; // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters
float McDauPhi     [14] ;
int   McDauPdgID   [14] ;
//-----------------------------  Isolation information  -----------------------------
float Rho       ;
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

