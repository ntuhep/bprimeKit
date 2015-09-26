Int_t      RunNo  ;
ULong64_t  EvtNo  ;
Int_t      BxNo   ;
Int_t      LumiNo ;
Int_t      Orbit  ;
//----- MC Information  ----------------------------------------------------------------------------
Bool_t  McIsTZTZ          ;
Bool_t  McIsTHTH          ;
Bool_t  McIsTZTH          ;
Bool_t  McIsTZBW          ;
Bool_t  McIsTHBW          ;
Bool_t  McIsBWBW          ;
Int_t   McFlag            ; // MC or not MC, that's the question
Int_t   McSigTag          ; // MC Signature tag    - 0: others, 1: 2L (opposite-sign), 2: 2L (same-sign), 3: 3L, 4: 4L
Int_t   McbprimeMode [2]  ; // b'(bar) decay mode   - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
Int_t   MctprimeMode [2]  ; // t'(bar) decay mode   - 0: others, 1: bW, 2: tZ, 3: tH, 4: tgamma
Int_t   McWMode      [4]  ; // W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar - 0: others, 1: enu, 2: munu, 3: taunu, 4: jj
Int_t   McZMode      [2]  ; // Z from b'(bar)/t'(t'bar)  - 0: others, 1: ee, 2: mumu, 3: tautau, 4: nunu, 5: bb, 6: jj
Float_t McbprimeMass [2]  ; // mass: b'(bar)
Float_t MctprimeMass [2]  ; // mass: t'(bar)
Float_t MctopMass    [2]  ; // mass: top(bar)
Float_t McWMass      [4]  ; // mass: W- from b'(t'bar)/W+ from t/W+ from b'bar(t')/W- from tbar
Float_t McZMass      [2]  ; // mass: Z from b'(bar) or t'(bar)
Float_t McDauPt      [14] ; // Generic MC daughter information
Float_t McDauEta     [14] ; // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters
Float_t McDauPhi     [14] ;
Int_t   McDauPdgID   [14] ;
//----- Isolation related information  -------------------------------------------------------------
Float_t Rho       ;
Float_t RhoPU  [2];       // [electron,muon]
Float_t SigmaPU[2];      // [electron,muon]
//----- Beamspot information  ----------------------------------------------------------------------
Float_t BeamSpotX;
Float_t BeamSpotY;
Float_t BeamSpotZ;
//----- PU information  ----------------------------------------------------------------------------
Int_t   nBX            ;
Int_t   nPU    [MAX_BX];
Int_t   BXPU   [MAX_BX];
Float_t TrueIT [MAX_BX];
//----- Parton distribution functions information  -------------------------------------------------
Int_t   PDFid1   ;
Int_t   PDFid2   ;
Float_t PDFx1    ;
Float_t PDFx2    ;
Float_t PDFscale ;
Float_t PDFv1    ;
Float_t PDFv2    ;
//----- Particle Flow information  -----------------------------------------------------------------
Float_t PFMET                                     ;
Float_t PFMETType1CorrectedPFMetUnclusteredEnUp   ;
Float_t PFMETType1CorrectedPFMetUnclusteredEnDown ;
Float_t PFMETPhi                                  ;
Float_t PFRawMET                                  ;
Float_t PFRawMETPhi                               ;
Float_t PFSumEt                                   ;
Float_t PFMETSig                                  ;
Float_t PFMETlongitudinal                         ;
Float_t PFMETRealSig                              ;
Float_t PFGenMET                                  ;
Float_t PFGenMETPhi                               ;
Float_t PFMETx                                    ; //Uly 2011-04-04
Float_t PFMETy                                    ; //Uly 2011-04-04
//----- High Level Trigger information  ------------------------------------------------------------
Int_t   TrgCount                    ; // No. of fired booking bits
Int_t   nTrgBook                    ;
Char_t  TrgBook[N_TRIGGER_BOOKINGS] ; // Trigger bits, reserved up to 120 entries
Int_t   nHLT                        ;
Float_t HighPurityFraction          ; //Added by Dmitry to help filter out bad events
Int_t   NofTracks                   ; //Added by Dmitry to help filter out bad events
Float_t ptHat                       ;
Int_t   HLTPrescaleFactor[512]      ;
Int_t   HLTName2enum[512]           ;
Bool_t  HLTbits[N_TRIGGER_BOOKINGS] ;
Int_t   L1[128]                     ; // L1 trigger bits
Int_t   TT[64]                      ; // Techical trigger bits

