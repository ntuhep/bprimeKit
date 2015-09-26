/*******************************************************************************
 *
 *  Filename    : format.h
 *  Description : The branch format for the bprimeKit nTuple
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
#define N_TRIGGER_BOOKINGS 1690

class EvtInfoBranches {
public:
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

   void RegisterTree( TTree* root , std::string name = "EvtInfo" ) {
      root->Branch( ( name + ".RunNo" ).c_str(), &RunNo, ( name + ".RunNo/I" ).c_str() );
      root->Branch( ( name + ".EvtNo" ).c_str(), &EvtNo, ( name + ".EvtNo/l" ).c_str() );
      root->Branch( ( name + ".BxNo" ).c_str(), &BxNo, ( name + ".BxNo/I" ).c_str() );
      root->Branch( ( name + ".LumiNo" ).c_str(), &LumiNo, ( name + ".LumiNo/I" ).c_str() );
      root->Branch( ( name + ".Orbit" ).c_str(), &Orbit, ( name + ".Orbit/I" ).c_str() );
      root->Branch( ( name + ".McIsTZTZ" ).c_str(), &McIsTZTZ, ( name + ".McIsTZTZ/O" ).c_str() );
      root->Branch( ( name + ".McIsTHTH" ).c_str(), &McIsTHTH, ( name + ".McIsTHTH/O" ).c_str() );
      root->Branch( ( name + ".McIsTZTH" ).c_str(), &McIsTZTH, ( name + ".McIsTZTH/O" ).c_str() );
      root->Branch( ( name + ".McIsTZBW" ).c_str(), &McIsTZBW, ( name + ".McIsTZBW/O" ).c_str() );
      root->Branch( ( name + ".McIsTHBW" ).c_str(), &McIsTHBW, ( name + ".McIsTHBW/O" ).c_str() );
      root->Branch( ( name + ".McIsBWBW" ).c_str(), &McIsBWBW, ( name + ".McIsBWBW/O" ).c_str() );
      root->Branch( ( name + ".McFlag" ).c_str(), &McFlag, ( name + ".McFlag/I" ).c_str() );
      root->Branch( ( name + ".McSigTag" ).c_str(), &McSigTag, ( name + ".McSigTag/I" ).c_str() );
      root->Branch( ( name + ".McbprimeMode" ).c_str(), McbprimeMode, ( name + ".McbprimeMode[2]/I" ).c_str() );
      root->Branch( ( name + ".MctprimeMode" ).c_str(), MctprimeMode, ( name + ".MctprimeMode[2]/I" ).c_str() );
      root->Branch( ( name + ".McWMode" ).c_str(), McWMode, ( name + ".McWMode[4]/I" ).c_str() );
      root->Branch( ( name + ".McZMode" ).c_str(), McZMode, ( name + ".McZMode[2]/I" ).c_str() );
      root->Branch( ( name + ".McbprimeMass" ).c_str(), McbprimeMass, ( name + ".McbprimeMass[2]/F" ).c_str() );
      root->Branch( ( name + ".MctprimeMass" ).c_str(), MctprimeMass, ( name + ".MctprimeMass[2]/F" ).c_str() );
      root->Branch( ( name + ".MctopMass" ).c_str(), MctopMass, ( name + ".MctopMass[2]/F" ).c_str() );
      root->Branch( ( name + ".McWMass" ).c_str(), McWMass, ( name + ".McWMass[4]/F" ).c_str() );
      root->Branch( ( name + ".McZMass" ).c_str(), McZMass, ( name + ".McZMass[2]/F" ).c_str() );
      root->Branch( ( name + ".McDauPt" ).c_str(), McDauPt, ( name + ".McDauPt[14]/F" ).c_str() );
      root->Branch( ( name + ".McDauEta" ).c_str(), McDauEta, ( name + ".McDauEta[14]/F" ).c_str() );
      root->Branch( ( name + ".McDauPhi" ).c_str(), McDauPhi, ( name + ".McDauPhi[14]/F" ).c_str() );
      root->Branch( ( name + ".McDauPdgID" ).c_str(), McDauPdgID, ( name + ".McDauPdgID[14]/I" ).c_str() );
      root->Branch( ( name + ".Rho" ).c_str(), &Rho, ( name + ".Rho/F" ).c_str() );
      root->Branch( ( name + ".RhoPU" ).c_str(), RhoPU, ( name + ".RhoPU[2]/F" ).c_str() );
      root->Branch( ( name + ".SigmaPU" ).c_str(), SigmaPU, ( name + ".SigmaPU[2]/F" ).c_str() );
      root->Branch( ( name + ".BeamSpotX" ).c_str(), &BeamSpotX, ( name + ".BeamSpotX/F" ).c_str() );
      root->Branch( ( name + ".BeamSpotY" ).c_str(), &BeamSpotY, ( name + ".BeamSpotY/F" ).c_str() );
      root->Branch( ( name + ".BeamSpotZ" ).c_str(), &BeamSpotZ, ( name + ".BeamSpotZ/F" ).c_str() );
      root->Branch( ( name + ".nBX" ).c_str(), &nBX, ( name + ".nBX/I" ).c_str() );
      root->Branch( ( name + ".nPU" ).c_str(), nPU, ( name + ".nPU[" + name + ".nBX]/I" ).c_str() );
      root->Branch( ( name + ".BXPU" ).c_str(), BXPU, ( name + ".BXPU[" + name + ".nBX]/I" ).c_str() );
      root->Branch( ( name + ".TrueIT" ).c_str(), TrueIT, ( name + ".TrueIT[" + name + ".nBX]/F" ).c_str() );
      root->Branch( ( name + ".PDFid1" ).c_str(), &PDFid1, ( name + ".PDFid1/I" ).c_str() );
      root->Branch( ( name + ".PDFid2" ).c_str(), &PDFid2, ( name + ".PDFid2/I" ).c_str() );
      root->Branch( ( name + ".PDFx1" ).c_str(), &PDFx1, ( name + ".PDFx1/F" ).c_str() );
      root->Branch( ( name + ".PDFx2" ).c_str(), &PDFx2, ( name + ".PDFx2/F" ).c_str() );
      root->Branch( ( name + ".PDFscale" ).c_str(), &PDFscale, ( name + ".PDFscale/F" ).c_str() );
      root->Branch( ( name + ".PDFv1" ).c_str(), &PDFv1, ( name + ".PDFv1/F" ).c_str() );
      root->Branch( ( name + ".PDFv2" ).c_str(), &PDFv2, ( name + ".PDFv2/F" ).c_str() );
      root->Branch( ( name + ".PFMET" ).c_str(), &PFMET, ( name + ".PFMET/F" ).c_str() );
      root->Branch( ( name + ".PFMETType1CorrectedPFMetUnclusteredEnUp" ).c_str(), &PFMETType1CorrectedPFMetUnclusteredEnUp, ( name + ".PFMETType1CorrectedPFMetUnclusteredEnUp/F" ).c_str() );
      root->Branch( ( name + ".PFMETType1CorrectedPFMetUnclusteredEnDown" ).c_str(), &PFMETType1CorrectedPFMetUnclusteredEnDown, ( name + ".PFMETType1CorrectedPFMetUnclusteredEnDown/F" ).c_str() );
      root->Branch( ( name + ".PFMETPhi" ).c_str(), &PFMETPhi, ( name + ".PFMETPhi/F" ).c_str() );
      root->Branch( ( name + ".PFRawMET" ).c_str(), &PFRawMET, ( name + ".PFRawMET/F" ).c_str() );
      root->Branch( ( name + ".PFRawMETPhi" ).c_str(), &PFRawMETPhi, ( name + ".PFRawMETPhi/F" ).c_str() );
      root->Branch( ( name + ".PFSumEt" ).c_str(), &PFSumEt, ( name + ".PFSumEt/F" ).c_str() );
      root->Branch( ( name + ".PFMETSig" ).c_str(), &PFMETSig, ( name + ".PFMETSig/F" ).c_str() );
      root->Branch( ( name + ".PFMETlongitudinal" ).c_str(), &PFMETlongitudinal, ( name + ".PFMETlongitudinal/F" ).c_str() );
      root->Branch( ( name + ".PFMETRealSig" ).c_str(), &PFMETRealSig, ( name + ".PFMETRealSig/F" ).c_str() );
      root->Branch( ( name + ".PFGenMET" ).c_str(), &PFGenMET, ( name + ".PFGenMET/F" ).c_str() );
      root->Branch( ( name + ".PFGenMETPhi" ).c_str(), &PFGenMETPhi, ( name + ".PFGenMETPhi/F" ).c_str() );
      root->Branch( ( name + ".PFMETx" ).c_str(), &PFMETx, ( name + ".PFMETx/F" ).c_str() );
      root->Branch( ( name + ".PFMETy" ).c_str(), &PFMETy, ( name + ".PFMETy/F" ).c_str() );
      root->Branch( ( name + ".TrgCount" ).c_str(), &TrgCount, ( name + ".TrgCount/I" ).c_str() );
      root->Branch( ( name + ".nTrgBook" ).c_str(), &nTrgBook, ( name + ".nTrgBook/I" ).c_str() );
      root->Branch( ( name + ".TrgBook" ).c_str(), TrgBook, ( name + ".TrgBook[" + name + ".nTrgBook]/C" ).c_str() );
      root->Branch( ( name + ".nHLT" ).c_str(), &nHLT, ( name + ".nHLT/I" ).c_str() );
      root->Branch( ( name + ".HighPurityFraction" ).c_str(), &HighPurityFraction, ( name + ".HighPurityFraction/F" ).c_str() );
      root->Branch( ( name + ".NofTracks" ).c_str(), &NofTracks, ( name + ".NofTracks/I" ).c_str() );
      root->Branch( ( name + ".ptHat" ).c_str(), &ptHat, ( name + ".ptHat/F" ).c_str() );
      root->Branch( ( name + ".HLTPrescaleFactor" ).c_str(), HLTPrescaleFactor, ( name + ".HLTPrescaleFactor[" + name + ".nHLT]/I" ).c_str() );
      root->Branch( ( name + ".HLTName2enum" ).c_str(), HLTName2enum, ( name + ".HLTName2enum[" + name + ".nHLT]/I" ).c_str() );
      root->Branch( ( name + ".HLTbits" ).c_str(), HLTbits, ( name + ".HLTbits[" + name + ".nHLT]/O" ).c_str() );
      root->Branch( ( name + ".L1" ).c_str(), L1, ( name + ".L1[128]/I" ).c_str() );
      root->Branch( ( name + ".TT" ).c_str(), TT, ( name + ".TT[64]/I" ).c_str() );
   }
   void Register( TTree* root , std::string name = "EvtInfo" ) {
      root->SetBranchAddress( ( name + ".RunNo" ).c_str() , &RunNo ) ;
      root->SetBranchAddress( ( name + ".EvtNo" ).c_str() , &EvtNo ) ;
      root->SetBranchAddress( ( name + ".BxNo" ).c_str() , &BxNo ) ;
      root->SetBranchAddress( ( name + ".LumiNo" ).c_str() , &LumiNo ) ;
      root->SetBranchAddress( ( name + ".Orbit" ).c_str() , &Orbit ) ;
      root->SetBranchAddress( ( name + ".McIsTZTZ" ).c_str() , &McIsTZTZ ) ;
      root->SetBranchAddress( ( name + ".McIsTHTH" ).c_str() , &McIsTHTH ) ;
      root->SetBranchAddress( ( name + ".McIsTZTH" ).c_str() , &McIsTZTH ) ;
      root->SetBranchAddress( ( name + ".McIsTZBW" ).c_str() , &McIsTZBW ) ;
      root->SetBranchAddress( ( name + ".McIsTHBW" ).c_str() , &McIsTHBW ) ;
      root->SetBranchAddress( ( name + ".McIsBWBW" ).c_str() , &McIsBWBW ) ;
      root->SetBranchAddress( ( name + ".McFlag" ).c_str() , &McFlag ) ;
      root->SetBranchAddress( ( name + ".McSigTag" ).c_str() , &McSigTag ) ;
      root->SetBranchAddress( ( name + ".McbprimeMode" ).c_str() , McbprimeMode ) ;
      root->SetBranchAddress( ( name + ".MctprimeMode" ).c_str() , MctprimeMode ) ;
      root->SetBranchAddress( ( name + ".McWMode" ).c_str() , McWMode ) ;
      root->SetBranchAddress( ( name + ".McZMode" ).c_str() , McZMode ) ;
      root->SetBranchAddress( ( name + ".McbprimeMass" ).c_str() , McbprimeMass ) ;
      root->SetBranchAddress( ( name + ".MctprimeMass" ).c_str() , MctprimeMass ) ;
      root->SetBranchAddress( ( name + ".MctopMass" ).c_str() , MctopMass ) ;
      root->SetBranchAddress( ( name + ".McWMass" ).c_str() , McWMass ) ;
      root->SetBranchAddress( ( name + ".McZMass" ).c_str() , McZMass ) ;
      root->SetBranchAddress( ( name + ".McDauPt" ).c_str() , McDauPt ) ;
      root->SetBranchAddress( ( name + ".McDauEta" ).c_str() , McDauEta ) ;
      root->SetBranchAddress( ( name + ".McDauPhi" ).c_str() , McDauPhi ) ;
      root->SetBranchAddress( ( name + ".McDauPdgID" ).c_str() , McDauPdgID ) ;
      root->SetBranchAddress( ( name + ".Rho" ).c_str() , &Rho ) ;
      root->SetBranchAddress( ( name + ".RhoPU" ).c_str() , RhoPU ) ;
      root->SetBranchAddress( ( name + ".SigmaPU" ).c_str() , SigmaPU ) ;
      root->SetBranchAddress( ( name + ".BeamSpotX" ).c_str() , &BeamSpotX ) ;
      root->SetBranchAddress( ( name + ".BeamSpotY" ).c_str() , &BeamSpotY ) ;
      root->SetBranchAddress( ( name + ".BeamSpotZ" ).c_str() , &BeamSpotZ ) ;
      root->SetBranchAddress( ( name + ".nBX" ).c_str() , &nBX ) ;
      root->SetBranchAddress( ( name + ".nPU" ).c_str() , nPU ) ;
      root->SetBranchAddress( ( name + ".BXPU" ).c_str() , BXPU ) ;
      root->SetBranchAddress( ( name + ".TrueIT" ).c_str() , TrueIT ) ;
      root->SetBranchAddress( ( name + ".PDFid1" ).c_str() , &PDFid1 ) ;
      root->SetBranchAddress( ( name + ".PDFid2" ).c_str() , &PDFid2 ) ;
      root->SetBranchAddress( ( name + ".PDFx1" ).c_str() , &PDFx1 ) ;
      root->SetBranchAddress( ( name + ".PDFx2" ).c_str() , &PDFx2 ) ;
      root->SetBranchAddress( ( name + ".PDFscale" ).c_str() , &PDFscale ) ;
      root->SetBranchAddress( ( name + ".PDFv1" ).c_str() , &PDFv1 ) ;
      root->SetBranchAddress( ( name + ".PDFv2" ).c_str() , &PDFv2 ) ;
      root->SetBranchAddress( ( name + ".PFMET" ).c_str() , &PFMET ) ;
      root->SetBranchAddress( ( name + ".PFMETType1CorrectedPFMetUnclusteredEnUp" ).c_str() , &PFMETType1CorrectedPFMetUnclusteredEnUp ) ;
      root->SetBranchAddress( ( name + ".PFMETType1CorrectedPFMetUnclusteredEnDown" ).c_str() , &PFMETType1CorrectedPFMetUnclusteredEnDown ) ;
      root->SetBranchAddress( ( name + ".PFMETPhi" ).c_str() , &PFMETPhi ) ;
      root->SetBranchAddress( ( name + ".PFRawMET" ).c_str() , &PFRawMET ) ;
      root->SetBranchAddress( ( name + ".PFRawMETPhi" ).c_str() , &PFRawMETPhi ) ;
      root->SetBranchAddress( ( name + ".PFSumEt" ).c_str() , &PFSumEt ) ;
      root->SetBranchAddress( ( name + ".PFMETSig" ).c_str() , &PFMETSig ) ;
      root->SetBranchAddress( ( name + ".PFMETlongitudinal" ).c_str() , &PFMETlongitudinal ) ;
      root->SetBranchAddress( ( name + ".PFMETRealSig" ).c_str() , &PFMETRealSig ) ;
      root->SetBranchAddress( ( name + ".PFGenMET" ).c_str() , &PFGenMET ) ;
      root->SetBranchAddress( ( name + ".PFGenMETPhi" ).c_str() , &PFGenMETPhi ) ;
      root->SetBranchAddress( ( name + ".PFMETx" ).c_str() , &PFMETx ) ;
      root->SetBranchAddress( ( name + ".PFMETy" ).c_str() , &PFMETy ) ;
      root->SetBranchAddress( ( name + ".TrgCount" ).c_str() , &TrgCount ) ;
      root->SetBranchAddress( ( name + ".nTrgBook" ).c_str() , &nTrgBook ) ;
      root->SetBranchAddress( ( name + ".TrgBook" ).c_str() , TrgBook ) ;
      root->SetBranchAddress( ( name + ".nHLT" ).c_str() , &nHLT ) ;
      root->SetBranchAddress( ( name + ".HighPurityFraction" ).c_str() , &HighPurityFraction ) ;
      root->SetBranchAddress( ( name + ".NofTracks" ).c_str() , &NofTracks ) ;
      root->SetBranchAddress( ( name + ".ptHat" ).c_str() , &ptHat ) ;
      root->SetBranchAddress( ( name + ".HLTPrescaleFactor" ).c_str() , HLTPrescaleFactor ) ;
      root->SetBranchAddress( ( name + ".HLTName2enum" ).c_str() , HLTName2enum ) ;
      root->SetBranchAddress( ( name + ".HLTbits" ).c_str() , HLTbits ) ;
      root->SetBranchAddress( ( name + ".L1" ).c_str() , L1 ) ;
      root->SetBranchAddress( ( name + ".TT" ).c_str() , TT ) ;
   }
};
class LepInfoBranches {
public:
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

   void RegisterTree( TTree* root , std::string name = "LepInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".Index" ).c_str(), Index, ( name + ".Index[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".LeptonType" ).c_str(), LeptonType, ( name + ".LeptonType[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Charge" ).c_str(), Charge, ( name + ".Charge[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Pt" ).c_str(), Pt, ( name + ".Pt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Et" ).c_str(), Et, ( name + ".Et[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eta" ).c_str(), Eta, ( name + ".Eta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Phi" ).c_str(), Phi, ( name + ".Phi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Px" ).c_str(), Px, ( name + ".Px[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Py" ).c_str(), Py, ( name + ".Py[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pz" ).c_str(), Pz, ( name + ".Pz[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Energy" ).c_str(), Energy, ( name + ".Energy[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".TrackIso" ).c_str(), TrackIso, ( name + ".TrackIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EcalIso" ).c_str(), EcalIso, ( name + ".EcalIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".HcalIso" ).c_str(), HcalIso, ( name + ".HcalIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ChargedHadronIso" ).c_str(), ChargedHadronIso, ( name + ".ChargedHadronIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NeutralHadronIso" ).c_str(), NeutralHadronIso, ( name + ".NeutralHadronIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PhotonIso" ).c_str(), PhotonIso, ( name + ".PhotonIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".SumPUPt" ).c_str(), SumPUPt, ( name + ".SumPUPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ChargedHadronIsoR03" ).c_str(), ChargedHadronIsoR03, ( name + ".ChargedHadronIsoR03[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NeutralHadronIsoR03" ).c_str(), NeutralHadronIsoR03, ( name + ".NeutralHadronIsoR03[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PhotonIsoR03" ).c_str(), PhotonIsoR03, ( name + ".PhotonIsoR03[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".sumPUPtR03" ).c_str(), sumPUPtR03, ( name + ".sumPUPtR03[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".IsoRhoCorrR03" ).c_str(), IsoRhoCorrR03, ( name + ".IsoRhoCorrR03[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ChargedHadronIsoR04" ).c_str(), ChargedHadronIsoR04, ( name + ".ChargedHadronIsoR04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NeutralHadronIsoR04" ).c_str(), NeutralHadronIsoR04, ( name + ".NeutralHadronIsoR04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PhotonIsoR04" ).c_str(), PhotonIsoR04, ( name + ".PhotonIsoR04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".sumPUPtR04" ).c_str(), sumPUPtR04, ( name + ".sumPUPtR04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".IsoRhoCorrR04" ).c_str(), IsoRhoCorrR04, ( name + ".IsoRhoCorrR04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Ip3dPV" ).c_str(), Ip3dPV, ( name + ".Ip3dPV[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Ip3dPVErr" ).c_str(), Ip3dPVErr, ( name + ".Ip3dPVErr[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Ip3dPVSignificance" ).c_str(), Ip3dPVSignificance, ( name + ".Ip3dPVSignificance[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".CaloEnergy" ).c_str(), CaloEnergy, ( name + ".CaloEnergy[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".isGoodMuonTMOneStationTight" ).c_str(), isGoodMuonTMOneStationTight, ( name + ".isGoodMuonTMOneStationTight[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".isPFMuon" ).c_str(), isPFMuon, ( name + ".isPFMuon[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".MuIDGlobalMuonPromptTight" ).c_str(), MuIDGlobalMuonPromptTight, ( name + ".MuIDGlobalMuonPromptTight[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".MuGlobalNormalizedChi2" ).c_str(), MuGlobalNormalizedChi2, ( name + ".MuGlobalNormalizedChi2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuCaloCompat" ).c_str(), MuCaloCompat, ( name + ".MuCaloCompat[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuNChambers" ).c_str(), MuNChambers, ( name + ".MuNChambers[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNChambersMatchesSegment" ).c_str(), MuNChambersMatchesSegment, ( name + ".MuNChambersMatchesSegment[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNMatchedStations" ).c_str(), MuNMatchedStations, ( name + ".MuNMatchedStations[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNLostOuterHits" ).c_str(), MuNLostOuterHits, ( name + ".MuNLostOuterHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNMuonhits" ).c_str(), MuNMuonhits, ( name + ".MuNMuonhits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuDThits" ).c_str(), MuDThits, ( name + ".MuDThits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuCSChits" ).c_str(), MuCSChits, ( name + ".MuCSChits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuRPChits" ).c_str(), MuRPChits, ( name + ".MuRPChits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuType" ).c_str(), MuType, ( name + ".MuType[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuontimenDof" ).c_str(), MuontimenDof, ( name + ".MuontimenDof[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuontimeAtIpInOut" ).c_str(), MuontimeAtIpInOut, ( name + ".MuontimeAtIpInOut[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuontimeAtIpOutIn" ).c_str(), MuontimeAtIpOutIn, ( name + ".MuontimeAtIpOutIn[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Muondirection" ).c_str(), Muondirection, ( name + ".Muondirection[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".innerTracknormalizedChi2" ).c_str(), innerTracknormalizedChi2, ( name + ".innerTracknormalizedChi2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerPtError" ).c_str(), MuInnerPtError, ( name + ".MuInnerPtError[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuGlobalPtError" ).c_str(), MuGlobalPtError, ( name + ".MuGlobalPtError[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackDz" ).c_str(), MuInnerTrackDz, ( name + ".MuInnerTrackDz[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackD0" ).c_str(), MuInnerTrackD0, ( name + ".MuInnerTrackD0[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackDxy_BS" ).c_str(), MuInnerTrackDxy_BS, ( name + ".MuInnerTrackDxy_BS[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackDxy_PV" ).c_str(), MuInnerTrackDxy_PV, ( name + ".MuInnerTrackDxy_PV[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackDxy_PVBS" ).c_str(), MuInnerTrackDxy_PVBS, ( name + ".MuInnerTrackDxy_PVBS[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuInnerTrackNHits" ).c_str(), MuInnerTrackNHits, ( name + ".MuInnerTrackNHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNTrackerHits" ).c_str(), MuNTrackerHits, ( name + ".MuNTrackerHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNLostInnerHits" ).c_str(), MuNLostInnerHits, ( name + ".MuNLostInnerHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".vertexZ" ).c_str(), vertexZ, ( name + ".vertexZ[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".MuNPixelLayers" ).c_str(), MuNPixelLayers, ( name + ".MuNPixelLayers[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNPixelLayersWMeasurement" ).c_str(), MuNPixelLayersWMeasurement, ( name + ".MuNPixelLayersWMeasurement[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".MuNTrackLayersWMeasurement" ).c_str(), MuNTrackLayersWMeasurement, ( name + ".MuNTrackLayersWMeasurement[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ChargeGsf" ).c_str(), ChargeGsf, ( name + ".ChargeGsf[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ChargeCtf" ).c_str(), ChargeCtf, ( name + ".ChargeCtf[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ChargeScPix" ).c_str(), ChargeScPix, ( name + ".ChargeScPix[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".isEcalDriven" ).c_str(), isEcalDriven, ( name + ".isEcalDriven[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".isTrackerDriven" ).c_str(), isTrackerDriven, ( name + ".isTrackerDriven[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".caloEta" ).c_str(), caloEta, ( name + ".caloEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".e1x5" ).c_str(), e1x5, ( name + ".e1x5[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".e2x5Max" ).c_str(), e2x5Max, ( name + ".e2x5Max[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".e5x5" ).c_str(), e5x5, ( name + ".e5x5[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".HcalDepth1Iso" ).c_str(), HcalDepth1Iso, ( name + ".HcalDepth1Iso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".HcalDepth2Iso" ).c_str(), HcalDepth2Iso, ( name + ".HcalDepth2Iso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EgammaMVANonTrig" ).c_str(), EgammaMVANonTrig, ( name + ".EgammaMVANonTrig[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EgammaMVATrig" ).c_str(), EgammaMVATrig, ( name + ".EgammaMVATrig[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdTRIGGERTIGHT" ).c_str(), EgammaCutBasedEleIdTRIGGERTIGHT, ( name + ".EgammaCutBasedEleIdTRIGGERTIGHT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdTRIGGERWP70" ).c_str(), EgammaCutBasedEleIdTRIGGERWP70, ( name + ".EgammaCutBasedEleIdTRIGGERWP70[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdVETO" ).c_str(), EgammaCutBasedEleIdVETO, ( name + ".EgammaCutBasedEleIdVETO[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdLOOSE" ).c_str(), EgammaCutBasedEleIdLOOSE, ( name + ".EgammaCutBasedEleIdLOOSE[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdMEDIUM" ).c_str(), EgammaCutBasedEleIdMEDIUM, ( name + ".EgammaCutBasedEleIdMEDIUM[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdTIGHT" ).c_str(), EgammaCutBasedEleIdTIGHT, ( name + ".EgammaCutBasedEleIdTIGHT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EgammaCutBasedEleIdHEEP" ).c_str(), EgammaCutBasedEleIdHEEP, ( name + ".EgammaCutBasedEleIdHEEP[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".Eldr03HcalDepth1TowerSumEtBc" ).c_str(), Eldr03HcalDepth1TowerSumEtBc, ( name + ".Eldr03HcalDepth1TowerSumEtBc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eldr03HcalDepth2TowerSumEtBc" ).c_str(), Eldr03HcalDepth2TowerSumEtBc, ( name + ".Eldr03HcalDepth2TowerSumEtBc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eldr04HcalDepth1TowerSumEtBc" ).c_str(), Eldr04HcalDepth1TowerSumEtBc, ( name + ".Eldr04HcalDepth1TowerSumEtBc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eldr04HcalDepth2TowerSumEtBc" ).c_str(), Eldr04HcalDepth2TowerSumEtBc, ( name + ".Eldr04HcalDepth2TowerSumEtBc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElhcalOverEcalBc" ).c_str(), ElhcalOverEcalBc, ( name + ".ElhcalOverEcalBc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElEcalE" ).c_str(), ElEcalE, ( name + ".ElEcalE[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElEoverP" ).c_str(), ElEoverP, ( name + ".ElEoverP[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EldeltaEta" ).c_str(), EldeltaEta, ( name + ".EldeltaEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".EldeltaPhi" ).c_str(), EldeltaPhi, ( name + ".EldeltaPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElHadoverEm" ).c_str(), ElHadoverEm, ( name + ".ElHadoverEm[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElsigmaIetaIeta" ).c_str(), ElsigmaIetaIeta, ( name + ".ElsigmaIetaIeta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElscSigmaIetaIeta" ).c_str(), ElscSigmaIetaIeta, ( name + ".ElscSigmaIetaIeta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElEnergyErr" ).c_str(), ElEnergyErr, ( name + ".ElEnergyErr[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElMomentumErr" ).c_str(), ElMomentumErr, ( name + ".ElMomentumErr[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElSharedHitsFraction" ).c_str(), ElSharedHitsFraction, ( name + ".ElSharedHitsFraction[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".dR_gsf_ctfTrack" ).c_str(), dR_gsf_ctfTrack, ( name + ".dR_gsf_ctfTrack[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".dPt_gsf_ctfTrack" ).c_str(), dPt_gsf_ctfTrack, ( name + ".dPt_gsf_ctfTrack[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElhasConv" ).c_str(), ElhasConv, ( name + ".ElhasConv[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ElTrackNHits" ).c_str(), ElTrackNHits, ( name + ".ElTrackNHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ElTrackNLostHits" ).c_str(), ElTrackNLostHits, ( name + ".ElTrackNLostHits[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackDz" ).c_str(), ElTrackDz, ( name + ".ElTrackDz[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackDz_BS" ).c_str(), ElTrackDz_BS, ( name + ".ElTrackDz_BS[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackD0" ).c_str(), ElTrackD0, ( name + ".ElTrackD0[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackDxy_BS" ).c_str(), ElTrackDxy_BS, ( name + ".ElTrackDxy_BS[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackDxy_PV" ).c_str(), ElTrackDxy_PV, ( name + ".ElTrackDxy_PV[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElTrackDxy_PVBS" ).c_str(), ElTrackDxy_PVBS, ( name + ".ElTrackDxy_PVBS[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElNClusters" ).c_str(), ElNClusters, ( name + ".ElNClusters[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ElClassification" ).c_str(), ElClassification, ( name + ".ElClassification[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".ElFBrem" ).c_str(), ElFBrem, ( name + ".ElFBrem[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NumberOfExpectedInnerHits" ).c_str(), NumberOfExpectedInnerHits, ( name + ".NumberOfExpectedInnerHits[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Eldist" ).c_str(), Eldist, ( name + ".Eldist[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eldcot" ).c_str(), Eldcot, ( name + ".Eldcot[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Elconvradius" ).c_str(), Elconvradius, ( name + ".Elconvradius[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElConvPoint_x" ).c_str(), ElConvPoint_x, ( name + ".ElConvPoint_x[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElConvPoint_y" ).c_str(), ElConvPoint_y, ( name + ".ElConvPoint_y[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElConvPoint_z" ).c_str(), ElConvPoint_z, ( name + ".ElConvPoint_z[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".dcotdist" ).c_str(), dcotdist, ( name + ".dcotdist[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElseedEoverP" ).c_str(), ElseedEoverP, ( name + ".ElseedEoverP[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElEcalIso04" ).c_str(), ElEcalIso04, ( name + ".ElEcalIso04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElHcalIso04" ).c_str(), ElHcalIso04, ( name + ".ElHcalIso04[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ElNumberOfBrems" ).c_str(), ElNumberOfBrems, ( name + ".ElNumberOfBrems[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".TrgPt" ).c_str(), TrgPt, ( name + ".TrgPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".TrgEta" ).c_str(), TrgEta, ( name + ".TrgEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".TrgPhi" ).c_str(), TrgPhi, ( name + ".TrgPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".TrgID" ).c_str(), TrgID, ( name + ".TrgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".isPFTau" ).c_str(), isPFTau, ( name + ".isPFTau[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".DiscriminationByDecayModeFinding" ).c_str(), DiscriminationByDecayModeFinding, ( name + ".DiscriminationByDecayModeFinding[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".DiscriminationByDecayModeFindingNewDMs" ).c_str(), DiscriminationByDecayModeFindingNewDMs, ( name + ".DiscriminationByDecayModeFindingNewDMs[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseElectronRejection" ).c_str(), ByLooseElectronRejection, ( name + ".ByLooseElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMediumElectronRejection" ).c_str(), ByMediumElectronRejection, ( name + ".ByMediumElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightElectronRejection" ).c_str(), ByTightElectronRejection, ( name + ".ByTightElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVA5LooseElectronRejection" ).c_str(), ByMVA5LooseElectronRejection, ( name + ".ByMVA5LooseElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVA5MediumElectronRejection" ).c_str(), ByMVA5MediumElectronRejection, ( name + ".ByMVA5MediumElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVA5TightElectronRejection" ).c_str(), ByMVA5TightElectronRejection, ( name + ".ByMVA5TightElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVA5VTightElectronRejection" ).c_str(), ByMVA5VTightElectronRejection, ( name + ".ByMVA5VTightElectronRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseMuonRejection3" ).c_str(), ByLooseMuonRejection3, ( name + ".ByLooseMuonRejection3[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightMuonRejection3" ).c_str(), ByTightMuonRejection3, ( name + ".ByTightMuonRejection3[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVALooseMuonRejection" ).c_str(), ByMVALooseMuonRejection, ( name + ".ByMVALooseMuonRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVAMediumMuonRejection" ).c_str(), ByMVAMediumMuonRejection, ( name + ".ByMVAMediumMuonRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMVATightMuonRejection" ).c_str(), ByMVATightMuonRejection, ( name + ".ByMVATightMuonRejection[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseCombinedIsolationDeltaBetaCorr3Hits" ).c_str(), ByLooseCombinedIsolationDeltaBetaCorr3Hits, ( name + ".ByLooseCombinedIsolationDeltaBetaCorr3Hits[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMediumCombinedIsolationDeltaBetaCorr3Hits" ).c_str(), ByMediumCombinedIsolationDeltaBetaCorr3Hits, ( name + ".ByMediumCombinedIsolationDeltaBetaCorr3Hits[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightCombinedIsolationDeltaBetaCorr3Hits" ).c_str(), ByTightCombinedIsolationDeltaBetaCorr3Hits, ( name + ".ByTightCombinedIsolationDeltaBetaCorr3Hits[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".CombinedIsolationDeltaBetaCorrRaw3Hits" ).c_str(), CombinedIsolationDeltaBetaCorrRaw3Hits, ( name + ".CombinedIsolationDeltaBetaCorrRaw3Hits[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ByVLooseIsolationMVA3newDMwoLT" ).c_str(), ByVLooseIsolationMVA3newDMwoLT, ( name + ".ByVLooseIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseIsolationMVA3newDMwoLT" ).c_str(), ByLooseIsolationMVA3newDMwoLT, ( name + ".ByLooseIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMediumIsolationMVA3newDMwoLT" ).c_str(), ByMediumIsolationMVA3newDMwoLT, ( name + ".ByMediumIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightIsolationMVA3newDMwoLT" ).c_str(), ByTightIsolationMVA3newDMwoLT, ( name + ".ByTightIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVTightIsolationMVA3newDMwoLT" ).c_str(), ByVTightIsolationMVA3newDMwoLT, ( name + ".ByVTightIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVVTightIsolationMVA3newDMwoLT" ).c_str(), ByVVTightIsolationMVA3newDMwoLT, ( name + ".ByVVTightIsolationMVA3newDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByIsolationMVA3newDMwoLTraw" ).c_str(), ByIsolationMVA3newDMwoLTraw, ( name + ".ByIsolationMVA3newDMwoLTraw[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ByVLooseIsolationMVA3oldDMwLT" ).c_str(), ByVLooseIsolationMVA3oldDMwLT, ( name + ".ByVLooseIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseIsolationMVA3oldDMwLT" ).c_str(), ByLooseIsolationMVA3oldDMwLT, ( name + ".ByLooseIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMediumIsolationMVA3oldDMwLT" ).c_str(), ByMediumIsolationMVA3oldDMwLT, ( name + ".ByMediumIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightIsolationMVA3oldDMwLT" ).c_str(), ByTightIsolationMVA3oldDMwLT, ( name + ".ByTightIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVTightIsolationMVA3oldDMwLT" ).c_str(), ByVTightIsolationMVA3oldDMwLT, ( name + ".ByVTightIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVVTightIsolationMVA3oldDMwLT" ).c_str(), ByVVTightIsolationMVA3oldDMwLT, ( name + ".ByVVTightIsolationMVA3oldDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByIsolationMVA3oldDMwLTraw" ).c_str(), ByIsolationMVA3oldDMwLTraw, ( name + ".ByIsolationMVA3oldDMwLTraw[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ByVLooseIsolationMVA3oldDMwoLT" ).c_str(), ByVLooseIsolationMVA3oldDMwoLT, ( name + ".ByVLooseIsolationMVA3oldDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseIsolationMVA3oldDMwoLT" ).c_str(), ByLooseIsolationMVA3oldDMwoLT, ( name + ".ByLooseIsolationMVA3oldDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightIsolationMVA3oldDMwoLT" ).c_str(), ByTightIsolationMVA3oldDMwoLT, ( name + ".ByTightIsolationMVA3oldDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVTightIsolationMVA3oldDMwoLT" ).c_str(), ByVTightIsolationMVA3oldDMwoLT, ( name + ".ByVTightIsolationMVA3oldDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVVTightIsolationMVA3oldDMwoLT" ).c_str(), ByVVTightIsolationMVA3oldDMwoLT, ( name + ".ByVVTightIsolationMVA3oldDMwoLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByIsolationMVA3oldDMwoLTraw" ).c_str(), ByIsolationMVA3oldDMwoLTraw, ( name + ".ByIsolationMVA3oldDMwoLTraw[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ByVLooseIsolationMVA3newDMwLT" ).c_str(), ByVLooseIsolationMVA3newDMwLT, ( name + ".ByVLooseIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByLooseIsolationMVA3newDMwLT" ).c_str(), ByLooseIsolationMVA3newDMwLT, ( name + ".ByLooseIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByMediumIsolationMVA3newDMwLT" ).c_str(), ByMediumIsolationMVA3newDMwLT, ( name + ".ByMediumIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByTightIsolationMVA3newDMwLT" ).c_str(), ByTightIsolationMVA3newDMwLT, ( name + ".ByTightIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVTightIsolationMVA3newDMwLT" ).c_str(), ByVTightIsolationMVA3newDMwLT, ( name + ".ByVTightIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByVVTightIsolationMVA3newDMwLT" ).c_str(), ByVVTightIsolationMVA3newDMwLT, ( name + ".ByVVTightIsolationMVA3newDMwLT[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".ByIsolationMVA3newDMwLTraw" ).c_str(), ByIsolationMVA3newDMwLTraw, ( name + ".ByIsolationMVA3newDMwLTraw[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPt" ).c_str(), GenPt, ( name + ".GenPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenEta" ).c_str(), GenEta, ( name + ".GenEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPhi" ).c_str(), GenPhi, ( name + ".GenPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPdgID" ).c_str(), GenPdgID, ( name + ".GenPdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GenMCTag" ).c_str(), GenMCTag, ( name + ".GenMCTag[" + name + ".Size]/I" ).c_str() );
   }
   void Register( TTree* root , std::string name = "LepInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".Index" ).c_str() , Index ) ;
      root->SetBranchAddress( ( name + ".LeptonType" ).c_str() , LeptonType ) ;
      root->SetBranchAddress( ( name + ".Charge" ).c_str() , Charge ) ;
      root->SetBranchAddress( ( name + ".Pt" ).c_str() , Pt ) ;
      root->SetBranchAddress( ( name + ".Et" ).c_str() , Et ) ;
      root->SetBranchAddress( ( name + ".Eta" ).c_str() , Eta ) ;
      root->SetBranchAddress( ( name + ".Phi" ).c_str() , Phi ) ;
      root->SetBranchAddress( ( name + ".Px" ).c_str() , Px ) ;
      root->SetBranchAddress( ( name + ".Py" ).c_str() , Py ) ;
      root->SetBranchAddress( ( name + ".Pz" ).c_str() , Pz ) ;
      root->SetBranchAddress( ( name + ".Energy" ).c_str() , Energy ) ;
      root->SetBranchAddress( ( name + ".TrackIso" ).c_str() , TrackIso ) ;
      root->SetBranchAddress( ( name + ".EcalIso" ).c_str() , EcalIso ) ;
      root->SetBranchAddress( ( name + ".HcalIso" ).c_str() , HcalIso ) ;
      root->SetBranchAddress( ( name + ".ChargedHadronIso" ).c_str() , ChargedHadronIso ) ;
      root->SetBranchAddress( ( name + ".NeutralHadronIso" ).c_str() , NeutralHadronIso ) ;
      root->SetBranchAddress( ( name + ".PhotonIso" ).c_str() , PhotonIso ) ;
      root->SetBranchAddress( ( name + ".SumPUPt" ).c_str() , SumPUPt ) ;
      root->SetBranchAddress( ( name + ".ChargedHadronIsoR03" ).c_str() , ChargedHadronIsoR03 ) ;
      root->SetBranchAddress( ( name + ".NeutralHadronIsoR03" ).c_str() , NeutralHadronIsoR03 ) ;
      root->SetBranchAddress( ( name + ".PhotonIsoR03" ).c_str() , PhotonIsoR03 ) ;
      root->SetBranchAddress( ( name + ".sumPUPtR03" ).c_str() , sumPUPtR03 ) ;
      root->SetBranchAddress( ( name + ".IsoRhoCorrR03" ).c_str() , IsoRhoCorrR03 ) ;
      root->SetBranchAddress( ( name + ".ChargedHadronIsoR04" ).c_str() , ChargedHadronIsoR04 ) ;
      root->SetBranchAddress( ( name + ".NeutralHadronIsoR04" ).c_str() , NeutralHadronIsoR04 ) ;
      root->SetBranchAddress( ( name + ".PhotonIsoR04" ).c_str() , PhotonIsoR04 ) ;
      root->SetBranchAddress( ( name + ".sumPUPtR04" ).c_str() , sumPUPtR04 ) ;
      root->SetBranchAddress( ( name + ".IsoRhoCorrR04" ).c_str() , IsoRhoCorrR04 ) ;
      root->SetBranchAddress( ( name + ".Ip3dPV" ).c_str() , Ip3dPV ) ;
      root->SetBranchAddress( ( name + ".Ip3dPVErr" ).c_str() , Ip3dPVErr ) ;
      root->SetBranchAddress( ( name + ".Ip3dPVSignificance" ).c_str() , Ip3dPVSignificance ) ;
      root->SetBranchAddress( ( name + ".CaloEnergy" ).c_str() , CaloEnergy ) ;
      root->SetBranchAddress( ( name + ".isGoodMuonTMOneStationTight" ).c_str() , isGoodMuonTMOneStationTight ) ;
      root->SetBranchAddress( ( name + ".isPFMuon" ).c_str() , isPFMuon ) ;
      root->SetBranchAddress( ( name + ".MuIDGlobalMuonPromptTight" ).c_str() , MuIDGlobalMuonPromptTight ) ;
      root->SetBranchAddress( ( name + ".MuGlobalNormalizedChi2" ).c_str() , MuGlobalNormalizedChi2 ) ;
      root->SetBranchAddress( ( name + ".MuCaloCompat" ).c_str() , MuCaloCompat ) ;
      root->SetBranchAddress( ( name + ".MuNChambers" ).c_str() , MuNChambers ) ;
      root->SetBranchAddress( ( name + ".MuNChambersMatchesSegment" ).c_str() , MuNChambersMatchesSegment ) ;
      root->SetBranchAddress( ( name + ".MuNMatchedStations" ).c_str() , MuNMatchedStations ) ;
      root->SetBranchAddress( ( name + ".MuNLostOuterHits" ).c_str() , MuNLostOuterHits ) ;
      root->SetBranchAddress( ( name + ".MuNMuonhits" ).c_str() , MuNMuonhits ) ;
      root->SetBranchAddress( ( name + ".MuDThits" ).c_str() , MuDThits ) ;
      root->SetBranchAddress( ( name + ".MuCSChits" ).c_str() , MuCSChits ) ;
      root->SetBranchAddress( ( name + ".MuRPChits" ).c_str() , MuRPChits ) ;
      root->SetBranchAddress( ( name + ".MuType" ).c_str() , MuType ) ;
      root->SetBranchAddress( ( name + ".MuontimenDof" ).c_str() , MuontimenDof ) ;
      root->SetBranchAddress( ( name + ".MuontimeAtIpInOut" ).c_str() , MuontimeAtIpInOut ) ;
      root->SetBranchAddress( ( name + ".MuontimeAtIpOutIn" ).c_str() , MuontimeAtIpOutIn ) ;
      root->SetBranchAddress( ( name + ".Muondirection" ).c_str() , Muondirection ) ;
      root->SetBranchAddress( ( name + ".innerTracknormalizedChi2" ).c_str() , innerTracknormalizedChi2 ) ;
      root->SetBranchAddress( ( name + ".MuInnerPtError" ).c_str() , MuInnerPtError ) ;
      root->SetBranchAddress( ( name + ".MuGlobalPtError" ).c_str() , MuGlobalPtError ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackDz" ).c_str() , MuInnerTrackDz ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackD0" ).c_str() , MuInnerTrackD0 ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackDxy_BS" ).c_str() , MuInnerTrackDxy_BS ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackDxy_PV" ).c_str() , MuInnerTrackDxy_PV ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackDxy_PVBS" ).c_str() , MuInnerTrackDxy_PVBS ) ;
      root->SetBranchAddress( ( name + ".MuInnerTrackNHits" ).c_str() , MuInnerTrackNHits ) ;
      root->SetBranchAddress( ( name + ".MuNTrackerHits" ).c_str() , MuNTrackerHits ) ;
      root->SetBranchAddress( ( name + ".MuNLostInnerHits" ).c_str() , MuNLostInnerHits ) ;
      root->SetBranchAddress( ( name + ".vertexZ" ).c_str() , vertexZ ) ;
      root->SetBranchAddress( ( name + ".MuNPixelLayers" ).c_str() , MuNPixelLayers ) ;
      root->SetBranchAddress( ( name + ".MuNPixelLayersWMeasurement" ).c_str() , MuNPixelLayersWMeasurement ) ;
      root->SetBranchAddress( ( name + ".MuNTrackLayersWMeasurement" ).c_str() , MuNTrackLayersWMeasurement ) ;
      root->SetBranchAddress( ( name + ".ChargeGsf" ).c_str() , ChargeGsf ) ;
      root->SetBranchAddress( ( name + ".ChargeCtf" ).c_str() , ChargeCtf ) ;
      root->SetBranchAddress( ( name + ".ChargeScPix" ).c_str() , ChargeScPix ) ;
      root->SetBranchAddress( ( name + ".isEcalDriven" ).c_str() , isEcalDriven ) ;
      root->SetBranchAddress( ( name + ".isTrackerDriven" ).c_str() , isTrackerDriven ) ;
      root->SetBranchAddress( ( name + ".caloEta" ).c_str() , caloEta ) ;
      root->SetBranchAddress( ( name + ".e1x5" ).c_str() , e1x5 ) ;
      root->SetBranchAddress( ( name + ".e2x5Max" ).c_str() , e2x5Max ) ;
      root->SetBranchAddress( ( name + ".e5x5" ).c_str() , e5x5 ) ;
      root->SetBranchAddress( ( name + ".HcalDepth1Iso" ).c_str() , HcalDepth1Iso ) ;
      root->SetBranchAddress( ( name + ".HcalDepth2Iso" ).c_str() , HcalDepth2Iso ) ;
      root->SetBranchAddress( ( name + ".EgammaMVANonTrig" ).c_str() , EgammaMVANonTrig ) ;
      root->SetBranchAddress( ( name + ".EgammaMVATrig" ).c_str() , EgammaMVATrig ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdTRIGGERTIGHT" ).c_str() , EgammaCutBasedEleIdTRIGGERTIGHT ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdTRIGGERWP70" ).c_str() , EgammaCutBasedEleIdTRIGGERWP70 ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdVETO" ).c_str() , EgammaCutBasedEleIdVETO ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdLOOSE" ).c_str() , EgammaCutBasedEleIdLOOSE ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdMEDIUM" ).c_str() , EgammaCutBasedEleIdMEDIUM ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdTIGHT" ).c_str() , EgammaCutBasedEleIdTIGHT ) ;
      root->SetBranchAddress( ( name + ".EgammaCutBasedEleIdHEEP" ).c_str() , EgammaCutBasedEleIdHEEP ) ;
      root->SetBranchAddress( ( name + ".Eldr03HcalDepth1TowerSumEtBc" ).c_str() , Eldr03HcalDepth1TowerSumEtBc ) ;
      root->SetBranchAddress( ( name + ".Eldr03HcalDepth2TowerSumEtBc" ).c_str() , Eldr03HcalDepth2TowerSumEtBc ) ;
      root->SetBranchAddress( ( name + ".Eldr04HcalDepth1TowerSumEtBc" ).c_str() , Eldr04HcalDepth1TowerSumEtBc ) ;
      root->SetBranchAddress( ( name + ".Eldr04HcalDepth2TowerSumEtBc" ).c_str() , Eldr04HcalDepth2TowerSumEtBc ) ;
      root->SetBranchAddress( ( name + ".ElhcalOverEcalBc" ).c_str() , ElhcalOverEcalBc ) ;
      root->SetBranchAddress( ( name + ".ElEcalE" ).c_str() , ElEcalE ) ;
      root->SetBranchAddress( ( name + ".ElEoverP" ).c_str() , ElEoverP ) ;
      root->SetBranchAddress( ( name + ".EldeltaEta" ).c_str() , EldeltaEta ) ;
      root->SetBranchAddress( ( name + ".EldeltaPhi" ).c_str() , EldeltaPhi ) ;
      root->SetBranchAddress( ( name + ".ElHadoverEm" ).c_str() , ElHadoverEm ) ;
      root->SetBranchAddress( ( name + ".ElsigmaIetaIeta" ).c_str() , ElsigmaIetaIeta ) ;
      root->SetBranchAddress( ( name + ".ElscSigmaIetaIeta" ).c_str() , ElscSigmaIetaIeta ) ;
      root->SetBranchAddress( ( name + ".ElEnergyErr" ).c_str() , ElEnergyErr ) ;
      root->SetBranchAddress( ( name + ".ElMomentumErr" ).c_str() , ElMomentumErr ) ;
      root->SetBranchAddress( ( name + ".ElSharedHitsFraction" ).c_str() , ElSharedHitsFraction ) ;
      root->SetBranchAddress( ( name + ".dR_gsf_ctfTrack" ).c_str() , dR_gsf_ctfTrack ) ;
      root->SetBranchAddress( ( name + ".dPt_gsf_ctfTrack" ).c_str() , dPt_gsf_ctfTrack ) ;
      root->SetBranchAddress( ( name + ".ElhasConv" ).c_str() , ElhasConv ) ;
      root->SetBranchAddress( ( name + ".ElTrackNHits" ).c_str() , ElTrackNHits ) ;
      root->SetBranchAddress( ( name + ".ElTrackNLostHits" ).c_str() , ElTrackNLostHits ) ;
      root->SetBranchAddress( ( name + ".ElTrackDz" ).c_str() , ElTrackDz ) ;
      root->SetBranchAddress( ( name + ".ElTrackDz_BS" ).c_str() , ElTrackDz_BS ) ;
      root->SetBranchAddress( ( name + ".ElTrackD0" ).c_str() , ElTrackD0 ) ;
      root->SetBranchAddress( ( name + ".ElTrackDxy_BS" ).c_str() , ElTrackDxy_BS ) ;
      root->SetBranchAddress( ( name + ".ElTrackDxy_PV" ).c_str() , ElTrackDxy_PV ) ;
      root->SetBranchAddress( ( name + ".ElTrackDxy_PVBS" ).c_str() , ElTrackDxy_PVBS ) ;
      root->SetBranchAddress( ( name + ".ElNClusters" ).c_str() , ElNClusters ) ;
      root->SetBranchAddress( ( name + ".ElClassification" ).c_str() , ElClassification ) ;
      root->SetBranchAddress( ( name + ".ElFBrem" ).c_str() , ElFBrem ) ;
      root->SetBranchAddress( ( name + ".NumberOfExpectedInnerHits" ).c_str() , NumberOfExpectedInnerHits ) ;
      root->SetBranchAddress( ( name + ".Eldist" ).c_str() , Eldist ) ;
      root->SetBranchAddress( ( name + ".Eldcot" ).c_str() , Eldcot ) ;
      root->SetBranchAddress( ( name + ".Elconvradius" ).c_str() , Elconvradius ) ;
      root->SetBranchAddress( ( name + ".ElConvPoint_x" ).c_str() , ElConvPoint_x ) ;
      root->SetBranchAddress( ( name + ".ElConvPoint_y" ).c_str() , ElConvPoint_y ) ;
      root->SetBranchAddress( ( name + ".ElConvPoint_z" ).c_str() , ElConvPoint_z ) ;
      root->SetBranchAddress( ( name + ".dcotdist" ).c_str() , dcotdist ) ;
      root->SetBranchAddress( ( name + ".ElseedEoverP" ).c_str() , ElseedEoverP ) ;
      root->SetBranchAddress( ( name + ".ElEcalIso04" ).c_str() , ElEcalIso04 ) ;
      root->SetBranchAddress( ( name + ".ElHcalIso04" ).c_str() , ElHcalIso04 ) ;
      root->SetBranchAddress( ( name + ".ElNumberOfBrems" ).c_str() , ElNumberOfBrems ) ;
      root->SetBranchAddress( ( name + ".TrgPt" ).c_str() , TrgPt ) ;
      root->SetBranchAddress( ( name + ".TrgEta" ).c_str() , TrgEta ) ;
      root->SetBranchAddress( ( name + ".TrgPhi" ).c_str() , TrgPhi ) ;
      root->SetBranchAddress( ( name + ".TrgID" ).c_str() , TrgID ) ;
      root->SetBranchAddress( ( name + ".isPFTau" ).c_str() , isPFTau ) ;
      root->SetBranchAddress( ( name + ".DiscriminationByDecayModeFinding" ).c_str() , DiscriminationByDecayModeFinding ) ;
      root->SetBranchAddress( ( name + ".DiscriminationByDecayModeFindingNewDMs" ).c_str() , DiscriminationByDecayModeFindingNewDMs ) ;
      root->SetBranchAddress( ( name + ".ByLooseElectronRejection" ).c_str() , ByLooseElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByMediumElectronRejection" ).c_str() , ByMediumElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByTightElectronRejection" ).c_str() , ByTightElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVA5LooseElectronRejection" ).c_str() , ByMVA5LooseElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVA5MediumElectronRejection" ).c_str() , ByMVA5MediumElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVA5TightElectronRejection" ).c_str() , ByMVA5TightElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVA5VTightElectronRejection" ).c_str() , ByMVA5VTightElectronRejection ) ;
      root->SetBranchAddress( ( name + ".ByLooseMuonRejection3" ).c_str() , ByLooseMuonRejection3 ) ;
      root->SetBranchAddress( ( name + ".ByTightMuonRejection3" ).c_str() , ByTightMuonRejection3 ) ;
      root->SetBranchAddress( ( name + ".ByMVALooseMuonRejection" ).c_str() , ByMVALooseMuonRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVAMediumMuonRejection" ).c_str() , ByMVAMediumMuonRejection ) ;
      root->SetBranchAddress( ( name + ".ByMVATightMuonRejection" ).c_str() , ByMVATightMuonRejection ) ;
      root->SetBranchAddress( ( name + ".ByLooseCombinedIsolationDeltaBetaCorr3Hits" ).c_str() , ByLooseCombinedIsolationDeltaBetaCorr3Hits ) ;
      root->SetBranchAddress( ( name + ".ByMediumCombinedIsolationDeltaBetaCorr3Hits" ).c_str() , ByMediumCombinedIsolationDeltaBetaCorr3Hits ) ;
      root->SetBranchAddress( ( name + ".ByTightCombinedIsolationDeltaBetaCorr3Hits" ).c_str() , ByTightCombinedIsolationDeltaBetaCorr3Hits ) ;
      root->SetBranchAddress( ( name + ".CombinedIsolationDeltaBetaCorrRaw3Hits" ).c_str() , CombinedIsolationDeltaBetaCorrRaw3Hits ) ;
      root->SetBranchAddress( ( name + ".ByVLooseIsolationMVA3newDMwoLT" ).c_str() , ByVLooseIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByLooseIsolationMVA3newDMwoLT" ).c_str() , ByLooseIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByMediumIsolationMVA3newDMwoLT" ).c_str() , ByMediumIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByTightIsolationMVA3newDMwoLT" ).c_str() , ByTightIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByVTightIsolationMVA3newDMwoLT" ).c_str() , ByVTightIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByVVTightIsolationMVA3newDMwoLT" ).c_str() , ByVVTightIsolationMVA3newDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByIsolationMVA3newDMwoLTraw" ).c_str() , ByIsolationMVA3newDMwoLTraw ) ;
      root->SetBranchAddress( ( name + ".ByVLooseIsolationMVA3oldDMwLT" ).c_str() , ByVLooseIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByLooseIsolationMVA3oldDMwLT" ).c_str() , ByLooseIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByMediumIsolationMVA3oldDMwLT" ).c_str() , ByMediumIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByTightIsolationMVA3oldDMwLT" ).c_str() , ByTightIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByVTightIsolationMVA3oldDMwLT" ).c_str() , ByVTightIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByVVTightIsolationMVA3oldDMwLT" ).c_str() , ByVVTightIsolationMVA3oldDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByIsolationMVA3oldDMwLTraw" ).c_str() , ByIsolationMVA3oldDMwLTraw ) ;
      root->SetBranchAddress( ( name + ".ByVLooseIsolationMVA3oldDMwoLT" ).c_str() , ByVLooseIsolationMVA3oldDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByLooseIsolationMVA3oldDMwoLT" ).c_str() , ByLooseIsolationMVA3oldDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByTightIsolationMVA3oldDMwoLT" ).c_str() , ByTightIsolationMVA3oldDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByVTightIsolationMVA3oldDMwoLT" ).c_str() , ByVTightIsolationMVA3oldDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByVVTightIsolationMVA3oldDMwoLT" ).c_str() , ByVVTightIsolationMVA3oldDMwoLT ) ;
      root->SetBranchAddress( ( name + ".ByIsolationMVA3oldDMwoLTraw" ).c_str() , ByIsolationMVA3oldDMwoLTraw ) ;
      root->SetBranchAddress( ( name + ".ByVLooseIsolationMVA3newDMwLT" ).c_str() , ByVLooseIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByLooseIsolationMVA3newDMwLT" ).c_str() , ByLooseIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByMediumIsolationMVA3newDMwLT" ).c_str() , ByMediumIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByTightIsolationMVA3newDMwLT" ).c_str() , ByTightIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByVTightIsolationMVA3newDMwLT" ).c_str() , ByVTightIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByVVTightIsolationMVA3newDMwLT" ).c_str() , ByVVTightIsolationMVA3newDMwLT ) ;
      root->SetBranchAddress( ( name + ".ByIsolationMVA3newDMwLTraw" ).c_str() , ByIsolationMVA3newDMwLTraw ) ;
      root->SetBranchAddress( ( name + ".GenPt" ).c_str() , GenPt ) ;
      root->SetBranchAddress( ( name + ".GenEta" ).c_str() , GenEta ) ;
      root->SetBranchAddress( ( name + ".GenPhi" ).c_str() , GenPhi ) ;
      root->SetBranchAddress( ( name + ".GenPdgID" ).c_str() , GenPdgID ) ;
      root->SetBranchAddress( ( name + ".GenMCTag" ).c_str() , GenMCTag ) ;
   }
};
class JetInfoBranches {
public:
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
   void RegisterTree( TTree* root , std::string name = "JetInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".Index" ).c_str(), Index, ( name + ".Index[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".NTracks" ).c_str(), NTracks, ( name + ".NTracks[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Et" ).c_str(), Et, ( name + ".Et[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pt" ).c_str(), Pt, ( name + ".Pt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Unc" ).c_str(), Unc, ( name + ".Unc[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eta" ).c_str(), Eta, ( name + ".Eta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Phi" ).c_str(), Phi, ( name + ".Phi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Px" ).c_str(), Px, ( name + ".Px[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Py" ).c_str(), Py, ( name + ".Py[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pz" ).c_str(), Pz, ( name + ".Pz[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Energy" ).c_str(), Energy, ( name + ".Energy[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Mass" ).c_str(), Mass, ( name + ".Mass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Area" ).c_str(), Area, ( name + ".Area[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".JetIDLOOSE" ).c_str(), JetIDLOOSE, ( name + ".JetIDLOOSE[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".JetCharge" ).c_str(), JetCharge, ( name + ".JetCharge[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".QGTagsLikelihood" ).c_str(), QGTagsLikelihood, ( name + ".QGTagsLikelihood[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".QGTagsAxis2" ).c_str(), QGTagsAxis2, ( name + ".QGTagsAxis2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".QGTagsMult" ).c_str(), QGTagsMult, ( name + ".QGTagsMult[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".QGTagsPtD" ).c_str(), QGTagsPtD, ( name + ".QGTagsPtD[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NConstituents" ).c_str(), NConstituents, ( name + ".NConstituents[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".NCH" ).c_str(), NCH, ( name + ".NCH[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".CEF" ).c_str(), CEF, ( name + ".CEF[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NHF" ).c_str(), NHF, ( name + ".NHF[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NEF" ).c_str(), NEF, ( name + ".NEF[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".CHF" ).c_str(), CHF, ( name + ".CHF[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".JVAlpha" ).c_str(), JVAlpha, ( name + ".JVAlpha[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".JVBeta" ).c_str(), JVBeta, ( name + ".JVBeta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrRaw" ).c_str(), PtCorrRaw, ( name + ".PtCorrRaw[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL2" ).c_str(), PtCorrL2, ( name + ".PtCorrL2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL3" ).c_str(), PtCorrL3, ( name + ".PtCorrL3[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL7g" ).c_str(), PtCorrL7g, ( name + ".PtCorrL7g[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL7uds" ).c_str(), PtCorrL7uds, ( name + ".PtCorrL7uds[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL7c" ).c_str(), PtCorrL7c, ( name + ".PtCorrL7c[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PtCorrL7b" ).c_str(), PtCorrL7b, ( name + ".PtCorrL7b[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".combinedSecondaryVertexBJetTags" ).c_str(), combinedSecondaryVertexBJetTags, ( name + ".combinedSecondaryVertexBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfJetBProbabilityBJetTags" ).c_str(), pfJetBProbabilityBJetTags, ( name + ".pfJetBProbabilityBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfJetProbabilityBJetTags" ).c_str(), pfJetProbabilityBJetTags, ( name + ".pfJetProbabilityBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfTrackCountingHighPurBJetTags" ).c_str(), pfTrackCountingHighPurBJetTags, ( name + ".pfTrackCountingHighPurBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfTrackCountingHighEffBJetTags" ).c_str(), pfTrackCountingHighEffBJetTags, ( name + ".pfTrackCountingHighEffBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfSimpleSecondaryVertexHighEffBJetTags" ).c_str(), pfSimpleSecondaryVertexHighEffBJetTags, ( name + ".pfSimpleSecondaryVertexHighEffBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfSimpleSecondaryVertexHighPurBJetTags" ).c_str(), pfSimpleSecondaryVertexHighPurBJetTags, ( name + ".pfSimpleSecondaryVertexHighPurBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfCombinedSecondaryVertexV2BJetTags" ).c_str(), pfCombinedSecondaryVertexV2BJetTags, ( name + ".pfCombinedSecondaryVertexV2BJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfCombinedInclusiveSecondaryVertexV2BJetTags" ).c_str(), pfCombinedInclusiveSecondaryVertexV2BJetTags, ( name + ".pfCombinedInclusiveSecondaryVertexV2BJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfCombinedSecondaryVertexSoftLeptonBJetTags" ).c_str(), pfCombinedSecondaryVertexSoftLeptonBJetTags, ( name + ".pfCombinedSecondaryVertexSoftLeptonBJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".pfCombinedMVABJetTags" ).c_str(), pfCombinedMVABJetTags, ( name + ".pfCombinedMVABJetTags[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenJetPt" ).c_str(), GenJetPt, ( name + ".GenJetPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenJetEta" ).c_str(), GenJetEta, ( name + ".GenJetEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenJetPhi" ).c_str(), GenJetPhi, ( name + ".GenJetPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPt" ).c_str(), GenPt, ( name + ".GenPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenEta" ).c_str(), GenEta, ( name + ".GenEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPhi" ).c_str(), GenPhi, ( name + ".GenPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPdgID" ).c_str(), GenPdgID, ( name + ".GenPdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GenFlavor" ).c_str(), GenFlavor, ( name + ".GenFlavor[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GenHadronFlavor" ).c_str(), GenHadronFlavor, ( name + ".GenHadronFlavor[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GenMCTag" ).c_str(), GenMCTag, ( name + ".GenMCTag[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".NSubjets" ).c_str(), NSubjets, ( name + ".NSubjets[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".SubjetsIdxStart" ).c_str(), SubjetsIdxStart, ( name + ".SubjetsIdxStart[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".NjettinessAK8tau1" ).c_str(), NjettinessAK8tau1, ( name + ".NjettinessAK8tau1[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NjettinessAK8tau2" ).c_str(), NjettinessAK8tau2, ( name + ".NjettinessAK8tau2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NjettinessAK8tau3" ).c_str(), NjettinessAK8tau3, ( name + ".NjettinessAK8tau3[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ak8PFJetsCHSSoftDropMass" ).c_str(), ak8PFJetsCHSSoftDropMass, ( name + ".ak8PFJetsCHSSoftDropMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ak8PFJetsCHSPrunedMass" ).c_str(), ak8PFJetsCHSPrunedMass, ( name + ".ak8PFJetsCHSPrunedMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ak8PFJetsCHSTrimmedMass" ).c_str(), ak8PFJetsCHSTrimmedMass, ( name + ".ak8PFJetsCHSTrimmedMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ak8PFJetsCHSFilteredMass" ).c_str(), ak8PFJetsCHSFilteredMass, ( name + ".ak8PFJetsCHSFilteredMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".topJetMass" ).c_str(), topJetMass, ( name + ".topJetMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ca8TopMass" ).c_str(), ca8TopMass, ( name + ".ca8TopMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".ca8MinMass" ).c_str(), ca8MinMass, ( name + ".ca8MinMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".SubjetMass" ).c_str(), &SubjetMass_w );
      root->Branch( ( name + ".SubjetPt" ).c_str(), &SubjetPt_w );
      root->Branch( ( name + ".SubjetEt" ).c_str(), &SubjetEt_w );
      root->Branch( ( name + ".SubjetEta" ).c_str(), &SubjetEta_w );
      root->Branch( ( name + ".SubjetPhi" ).c_str(), &SubjetPhi_w );
      root->Branch( ( name + ".SubjetCombinedSVBJetTags" ).c_str(), &SubjetCombinedSVBJetTags_w );
      root->Branch( ( name + ".SubjetPtUncorr" ).c_str(), &SubjetPtUncorr_w );
      root->Branch( ( name + ".SubjetArea" ).c_str(), &SubjetArea_w );
   }
   void Register( TTree* root , std::string name = "JetInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".Index" ).c_str() , Index ) ;
      root->SetBranchAddress( ( name + ".NTracks" ).c_str() , NTracks ) ;
      root->SetBranchAddress( ( name + ".Et" ).c_str() , Et ) ;
      root->SetBranchAddress( ( name + ".Pt" ).c_str() , Pt ) ;
      root->SetBranchAddress( ( name + ".Unc" ).c_str() , Unc ) ;
      root->SetBranchAddress( ( name + ".Eta" ).c_str() , Eta ) ;
      root->SetBranchAddress( ( name + ".Phi" ).c_str() , Phi ) ;
      root->SetBranchAddress( ( name + ".Px" ).c_str() , Px ) ;
      root->SetBranchAddress( ( name + ".Py" ).c_str() , Py ) ;
      root->SetBranchAddress( ( name + ".Pz" ).c_str() , Pz ) ;
      root->SetBranchAddress( ( name + ".Energy" ).c_str() , Energy ) ;
      root->SetBranchAddress( ( name + ".Mass" ).c_str() , Mass ) ;
      root->SetBranchAddress( ( name + ".Area" ).c_str() , Area ) ;
      root->SetBranchAddress( ( name + ".JetIDLOOSE" ).c_str() , JetIDLOOSE ) ;
      root->SetBranchAddress( ( name + ".JetCharge" ).c_str() , JetCharge ) ;
      root->SetBranchAddress( ( name + ".QGTagsLikelihood" ).c_str() , QGTagsLikelihood ) ;
      root->SetBranchAddress( ( name + ".QGTagsAxis2" ).c_str() , QGTagsAxis2 ) ;
      root->SetBranchAddress( ( name + ".QGTagsMult" ).c_str() , QGTagsMult ) ;
      root->SetBranchAddress( ( name + ".QGTagsPtD" ).c_str() , QGTagsPtD ) ;
      root->SetBranchAddress( ( name + ".NConstituents" ).c_str() , NConstituents ) ;
      root->SetBranchAddress( ( name + ".NCH" ).c_str() , NCH ) ;
      root->SetBranchAddress( ( name + ".CEF" ).c_str() , CEF ) ;
      root->SetBranchAddress( ( name + ".NHF" ).c_str() , NHF ) ;
      root->SetBranchAddress( ( name + ".NEF" ).c_str() , NEF ) ;
      root->SetBranchAddress( ( name + ".CHF" ).c_str() , CHF ) ;
      root->SetBranchAddress( ( name + ".JVAlpha" ).c_str() , JVAlpha ) ;
      root->SetBranchAddress( ( name + ".JVBeta" ).c_str() , JVBeta ) ;
      root->SetBranchAddress( ( name + ".PtCorrRaw" ).c_str() , PtCorrRaw ) ;
      root->SetBranchAddress( ( name + ".PtCorrL2" ).c_str() , PtCorrL2 ) ;
      root->SetBranchAddress( ( name + ".PtCorrL3" ).c_str() , PtCorrL3 ) ;
      root->SetBranchAddress( ( name + ".PtCorrL7g" ).c_str() , PtCorrL7g ) ;
      root->SetBranchAddress( ( name + ".PtCorrL7uds" ).c_str() , PtCorrL7uds ) ;
      root->SetBranchAddress( ( name + ".PtCorrL7c" ).c_str() , PtCorrL7c ) ;
      root->SetBranchAddress( ( name + ".PtCorrL7b" ).c_str() , PtCorrL7b ) ;
      root->SetBranchAddress( ( name + ".combinedSecondaryVertexBJetTags" ).c_str() , combinedSecondaryVertexBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfJetBProbabilityBJetTags" ).c_str() , pfJetBProbabilityBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfJetProbabilityBJetTags" ).c_str() , pfJetProbabilityBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfTrackCountingHighPurBJetTags" ).c_str() , pfTrackCountingHighPurBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfTrackCountingHighEffBJetTags" ).c_str() , pfTrackCountingHighEffBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfSimpleSecondaryVertexHighEffBJetTags" ).c_str() , pfSimpleSecondaryVertexHighEffBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfSimpleSecondaryVertexHighPurBJetTags" ).c_str() , pfSimpleSecondaryVertexHighPurBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfCombinedSecondaryVertexV2BJetTags" ).c_str() , pfCombinedSecondaryVertexV2BJetTags ) ;
      root->SetBranchAddress( ( name + ".pfCombinedInclusiveSecondaryVertexV2BJetTags" ).c_str() , pfCombinedInclusiveSecondaryVertexV2BJetTags ) ;
      root->SetBranchAddress( ( name + ".pfCombinedSecondaryVertexSoftLeptonBJetTags" ).c_str() , pfCombinedSecondaryVertexSoftLeptonBJetTags ) ;
      root->SetBranchAddress( ( name + ".pfCombinedMVABJetTags" ).c_str() , pfCombinedMVABJetTags ) ;
      root->SetBranchAddress( ( name + ".GenJetPt" ).c_str() , GenJetPt ) ;
      root->SetBranchAddress( ( name + ".GenJetEta" ).c_str() , GenJetEta ) ;
      root->SetBranchAddress( ( name + ".GenJetPhi" ).c_str() , GenJetPhi ) ;
      root->SetBranchAddress( ( name + ".GenPt" ).c_str() , GenPt ) ;
      root->SetBranchAddress( ( name + ".GenEta" ).c_str() , GenEta ) ;
      root->SetBranchAddress( ( name + ".GenPhi" ).c_str() , GenPhi ) ;
      root->SetBranchAddress( ( name + ".GenPdgID" ).c_str() , GenPdgID ) ;
      root->SetBranchAddress( ( name + ".GenFlavor" ).c_str() , GenFlavor ) ;
      root->SetBranchAddress( ( name + ".GenHadronFlavor" ).c_str() , GenHadronFlavor ) ;
      root->SetBranchAddress( ( name + ".GenMCTag" ).c_str() , GenMCTag ) ;
      root->SetBranchAddress( ( name + ".NSubjets" ).c_str() , NSubjets ) ;
      root->SetBranchAddress( ( name + ".SubjetsIdxStart" ).c_str() , SubjetsIdxStart ) ;
      root->SetBranchAddress( ( name + ".NjettinessAK8tau1" ).c_str() , NjettinessAK8tau1 ) ;
      root->SetBranchAddress( ( name + ".NjettinessAK8tau2" ).c_str() , NjettinessAK8tau2 ) ;
      root->SetBranchAddress( ( name + ".NjettinessAK8tau3" ).c_str() , NjettinessAK8tau3 ) ;
      root->SetBranchAddress( ( name + ".ak8PFJetsCHSSoftDropMass" ).c_str() , ak8PFJetsCHSSoftDropMass ) ;
      root->SetBranchAddress( ( name + ".ak8PFJetsCHSPrunedMass" ).c_str() , ak8PFJetsCHSPrunedMass ) ;
      root->SetBranchAddress( ( name + ".ak8PFJetsCHSTrimmedMass" ).c_str() , ak8PFJetsCHSTrimmedMass ) ;
      root->SetBranchAddress( ( name + ".ak8PFJetsCHSFilteredMass" ).c_str() , ak8PFJetsCHSFilteredMass ) ;
      root->SetBranchAddress( ( name + ".topJetMass" ).c_str() , topJetMass ) ;
      root->SetBranchAddress( ( name + ".ca8TopMass" ).c_str() , ca8TopMass ) ;
      root->SetBranchAddress( ( name + ".ca8MinMass" ).c_str() , ca8MinMass ) ;
      SubjetMass = 0 ;
      root->SetBranchAddress( ( name + ".SubjetMass" ).c_str() , &SubjetMass ) ;
      SubjetPt = 0 ;
      root->SetBranchAddress( ( name + ".SubjetPt" ).c_str() , &SubjetPt ) ;
      SubjetEt = 0 ;
      root->SetBranchAddress( ( name + ".SubjetEt" ).c_str() , &SubjetEt ) ;
      SubjetEta = 0 ;
      root->SetBranchAddress( ( name + ".SubjetEta" ).c_str() , &SubjetEta ) ;
      SubjetPhi = 0 ;
      root->SetBranchAddress( ( name + ".SubjetPhi" ).c_str() , &SubjetPhi ) ;
      SubjetCombinedSVBJetTags = 0 ;
      root->SetBranchAddress( ( name + ".SubjetCombinedSVBJetTags" ).c_str() , &SubjetCombinedSVBJetTags ) ;
      SubjetPtUncorr = 0 ;
      root->SetBranchAddress( ( name + ".SubjetPtUncorr" ).c_str() , &SubjetPtUncorr ) ;
      SubjetArea = 0 ;
      root->SetBranchAddress( ( name + ".SubjetArea" ).c_str() , &SubjetArea ) ;
   }
};
class PairInfoBranches {
public:
   Int_t   Size;
   Int_t   Index     [MAX_PAIRS] ;
   Int_t   Type      [MAX_PAIRS] ; // type of pairing - 1: ll (regardless of charge and flavor!), 2: jj
   Int_t   Obj1Index [MAX_PAIRS] ;
   Int_t   Obj2Index [MAX_PAIRS] ;
   Int_t   GenPdgID  [MAX_PAIRS] ;
   Float_t Mass      [MAX_PAIRS] ;
   Float_t Pt        [MAX_PAIRS] ;
   Float_t Eta       [MAX_PAIRS] ;
   Float_t Phi       [MAX_PAIRS] ;
   Float_t GenMass   [MAX_PAIRS] ;
   Float_t GenPt     [MAX_PAIRS] ;
   Float_t GenEta    [MAX_PAIRS] ;
   Float_t GenPhi    [MAX_PAIRS] ;

   void RegisterTree( TTree* root , std::string name = "PairInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".Index" ).c_str(), Index, ( name + ".Index[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Type" ).c_str(), Type, ( name + ".Type[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Obj1Index" ).c_str(), Obj1Index, ( name + ".Obj1Index[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Obj2Index" ).c_str(), Obj2Index, ( name + ".Obj2Index[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GenPdgID" ).c_str(), GenPdgID, ( name + ".GenPdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mass" ).c_str(), Mass, ( name + ".Mass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pt" ).c_str(), Pt, ( name + ".Pt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eta" ).c_str(), Eta, ( name + ".Eta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Phi" ).c_str(), Phi, ( name + ".Phi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenMass" ).c_str(), GenMass, ( name + ".GenMass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPt" ).c_str(), GenPt, ( name + ".GenPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenEta" ).c_str(), GenEta, ( name + ".GenEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPhi" ).c_str(), GenPhi, ( name + ".GenPhi[" + name + ".Size]/F" ).c_str() );
   }
   void Register( TTree* root , std::string name = "PairInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".Index" ).c_str() , Index ) ;
      root->SetBranchAddress( ( name + ".Type" ).c_str() , Type ) ;
      root->SetBranchAddress( ( name + ".Obj1Index" ).c_str() , Obj1Index ) ;
      root->SetBranchAddress( ( name + ".Obj2Index" ).c_str() , Obj2Index ) ;
      root->SetBranchAddress( ( name + ".GenPdgID" ).c_str() , GenPdgID ) ;
      root->SetBranchAddress( ( name + ".Mass" ).c_str() , Mass ) ;
      root->SetBranchAddress( ( name + ".Pt" ).c_str() , Pt ) ;
      root->SetBranchAddress( ( name + ".Eta" ).c_str() , Eta ) ;
      root->SetBranchAddress( ( name + ".Phi" ).c_str() , Phi ) ;
      root->SetBranchAddress( ( name + ".GenMass" ).c_str() , GenMass ) ;
      root->SetBranchAddress( ( name + ".GenPt" ).c_str() , GenPt ) ;
      root->SetBranchAddress( ( name + ".GenEta" ).c_str() , GenEta ) ;
      root->SetBranchAddress( ( name + ".GenPhi" ).c_str() , GenPhi ) ;
   }
};
class GenInfoBranches {
public:
   Int_t Size;
   Float_t Weight;
   Float_t Pt           [MAX_GENS] ;
   Float_t Eta          [MAX_GENS] ;
   Float_t Phi          [MAX_GENS] ;
   Float_t Mass         [MAX_GENS] ;
   Int_t PdgID          [MAX_GENS] ;
   Int_t PhotonFlag     [MAX_GENS] ;   // -1 : unknown or not photon, 0 : prompt photon, 1 : decay in flight, 2 : ISR, 3 : FSR
   Int_t Status         [MAX_GENS] ;
   Int_t nMo            [MAX_GENS] ;
   Int_t nDa            [MAX_GENS] ;
   Int_t Mo1            [MAX_GENS] ;
   Int_t Mo2            [MAX_GENS] ;
   Int_t Da1            [MAX_GENS] ;
   Int_t Da2            [MAX_GENS] ;
   Int_t Mo1PdgID       [MAX_GENS] ;
   Int_t Mo2PdgID       [MAX_GENS] ;
   Int_t Mo1Status      [MAX_GENS] ;
   Int_t Mo2Status      [MAX_GENS] ;
   Int_t Da1PdgID       [MAX_GENS] ;
   Int_t Da2PdgID       [MAX_GENS] ;
   Int_t GrandMo1PdgID  [MAX_GENS] ;
   Int_t GrandMo2PdgID  [MAX_GENS] ;
   Int_t GrandMo1Status [MAX_GENS] ;
   Int_t GrandMo2Status [MAX_GENS] ;

   void RegisterTree( TTree* root , std::string name = "GenInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".Weight" ).c_str(), &Weight, ( name + ".Weight/F" ).c_str() );
      root->Branch( ( name + ".Pt" ).c_str(), Pt, ( name + ".Pt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eta" ).c_str(), Eta, ( name + ".Eta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Phi" ).c_str(), Phi, ( name + ".Phi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Mass" ).c_str(), Mass, ( name + ".Mass[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".PdgID" ).c_str(), PdgID, ( name + ".PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".PhotonFlag" ).c_str(), PhotonFlag, ( name + ".PhotonFlag[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Status" ).c_str(), Status, ( name + ".Status[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".nMo" ).c_str(), nMo, ( name + ".nMo[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".nDa" ).c_str(), nDa, ( name + ".nDa[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo1" ).c_str(), Mo1, ( name + ".Mo1[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo2" ).c_str(), Mo2, ( name + ".Mo2[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Da1" ).c_str(), Da1, ( name + ".Da1[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Da2" ).c_str(), Da2, ( name + ".Da2[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo1PdgID" ).c_str(), Mo1PdgID, ( name + ".Mo1PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo2PdgID" ).c_str(), Mo2PdgID, ( name + ".Mo2PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo1Status" ).c_str(), Mo1Status, ( name + ".Mo1Status[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Mo2Status" ).c_str(), Mo2Status, ( name + ".Mo2Status[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Da1PdgID" ).c_str(), Da1PdgID, ( name + ".Da1PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Da2PdgID" ).c_str(), Da2PdgID, ( name + ".Da2PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GrandMo1PdgID" ).c_str(), GrandMo1PdgID, ( name + ".GrandMo1PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GrandMo2PdgID" ).c_str(), GrandMo2PdgID, ( name + ".GrandMo2PdgID[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GrandMo1Status" ).c_str(), GrandMo1Status, ( name + ".GrandMo1Status[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".GrandMo2Status" ).c_str(), GrandMo2Status, ( name + ".GrandMo2Status[" + name + ".Size]/I" ).c_str() );
   }
   void Register( TTree* root , std::string name = "GenInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".Weight" ).c_str() , &Weight ) ;
      root->SetBranchAddress( ( name + ".Pt" ).c_str() , Pt ) ;
      root->SetBranchAddress( ( name + ".Eta" ).c_str() , Eta ) ;
      root->SetBranchAddress( ( name + ".Phi" ).c_str() , Phi ) ;
      root->SetBranchAddress( ( name + ".Mass" ).c_str() , Mass ) ;
      root->SetBranchAddress( ( name + ".PdgID" ).c_str() , PdgID ) ;
      root->SetBranchAddress( ( name + ".PhotonFlag" ).c_str() , PhotonFlag ) ;
      root->SetBranchAddress( ( name + ".Status" ).c_str() , Status ) ;
      root->SetBranchAddress( ( name + ".nMo" ).c_str() , nMo ) ;
      root->SetBranchAddress( ( name + ".nDa" ).c_str() , nDa ) ;
      root->SetBranchAddress( ( name + ".Mo1" ).c_str() , Mo1 ) ;
      root->SetBranchAddress( ( name + ".Mo2" ).c_str() , Mo2 ) ;
      root->SetBranchAddress( ( name + ".Da1" ).c_str() , Da1 ) ;
      root->SetBranchAddress( ( name + ".Da2" ).c_str() , Da2 ) ;
      root->SetBranchAddress( ( name + ".Mo1PdgID" ).c_str() , Mo1PdgID ) ;
      root->SetBranchAddress( ( name + ".Mo2PdgID" ).c_str() , Mo2PdgID ) ;
      root->SetBranchAddress( ( name + ".Mo1Status" ).c_str() , Mo1Status ) ;
      root->SetBranchAddress( ( name + ".Mo2Status" ).c_str() , Mo2Status ) ;
      root->SetBranchAddress( ( name + ".Da1PdgID" ).c_str() , Da1PdgID ) ;
      root->SetBranchAddress( ( name + ".Da2PdgID" ).c_str() , Da2PdgID ) ;
      root->SetBranchAddress( ( name + ".GrandMo1PdgID" ).c_str() , GrandMo1PdgID ) ;
      root->SetBranchAddress( ( name + ".GrandMo2PdgID" ).c_str() , GrandMo2PdgID ) ;
      root->SetBranchAddress( ( name + ".GrandMo1Status" ).c_str() , GrandMo1Status ) ;
      root->SetBranchAddress( ( name + ".GrandMo2Status" ).c_str() , GrandMo2Status ) ;
   }
};
class PhotonInfoBranches {
public:
   Int_t   Size;
   Float_t Pt                   [MAX_PHOTONS] ;
   Float_t Eta                  [MAX_PHOTONS] ;
   Float_t Phi                  [MAX_PHOTONS] ;
   Float_t HoverE               [MAX_PHOTONS] ;
   Float_t SigmaIetaIeta        [MAX_PHOTONS] ;
   Float_t hadTowOverEm         [MAX_PHOTONS] ;
   Float_t hcalIsoConeDR04_2012 [MAX_PHOTONS] ;
   Float_t phoPFChIso           [MAX_PHOTONS] ;
   Float_t phoPFNeuIso          [MAX_PHOTONS] ;
   Float_t phoPFPhoIso          [MAX_PHOTONS] ;
   Float_t phoIDMVA             [MAX_PHOTONS] ;
   Bool_t  phoPassLoose         [MAX_PHOTONS] ;
   Bool_t  phoPassMedium        [MAX_PHOTONS] ;
   Bool_t  phoPassTight         [MAX_PHOTONS] ;
   Float_t r9                   [MAX_PHOTONS] ;
   Bool_t  passelectronveto     [MAX_PHOTONS] ;
   Float_t  EcalIso             [MAX_PHOTONS] ;
   Float_t  HcalIso             [MAX_PHOTONS] ;
   Float_t  TrackIso            [MAX_PHOTONS] ;
   Float_t GenPt                [MAX_PHOTONS] ;
   Float_t GenEta               [MAX_PHOTONS] ;
   Float_t GenPhi               [MAX_PHOTONS] ;
   Int_t   GenPdgID             [MAX_PHOTONS] ;

   void RegisterTree( TTree* root , std::string name = "PhotonInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".Pt" ).c_str(), Pt, ( name + ".Pt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Eta" ).c_str(), Eta, ( name + ".Eta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Phi" ).c_str(), Phi, ( name + ".Phi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".HoverE" ).c_str(), HoverE, ( name + ".HoverE[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".SigmaIetaIeta" ).c_str(), SigmaIetaIeta, ( name + ".SigmaIetaIeta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".hadTowOverEm" ).c_str(), hadTowOverEm, ( name + ".hadTowOverEm[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".hcalIsoConeDR04_2012" ).c_str(), hcalIsoConeDR04_2012, ( name + ".hcalIsoConeDR04_2012[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".phoPFChIso" ).c_str(), phoPFChIso, ( name + ".phoPFChIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".phoPFNeuIso" ).c_str(), phoPFNeuIso, ( name + ".phoPFNeuIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".phoPFPhoIso" ).c_str(), phoPFPhoIso, ( name + ".phoPFPhoIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".phoIDMVA" ).c_str(), phoIDMVA, ( name + ".phoIDMVA[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".phoPassLoose" ).c_str(), phoPassLoose, ( name + ".phoPassLoose[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".phoPassMedium" ).c_str(), phoPassMedium, ( name + ".phoPassMedium[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".phoPassTight" ).c_str(), phoPassTight, ( name + ".phoPassTight[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".r9" ).c_str(), r9, ( name + ".r9[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".passelectronveto" ).c_str(), passelectronveto, ( name + ".passelectronveto[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".EcalIso" ).c_str(), EcalIso, ( name + ".EcalIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".HcalIso" ).c_str(), HcalIso, ( name + ".HcalIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".TrackIso" ).c_str(), TrackIso, ( name + ".TrackIso[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPt" ).c_str(), GenPt, ( name + ".GenPt[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenEta" ).c_str(), GenEta, ( name + ".GenEta[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPhi" ).c_str(), GenPhi, ( name + ".GenPhi[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".GenPdgID" ).c_str(), GenPdgID, ( name + ".GenPdgID[" + name + ".Size]/I" ).c_str() );
   }
   void Register( TTree* root , std::string name = "PhotonInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".Pt" ).c_str() , Pt ) ;
      root->SetBranchAddress( ( name + ".Eta" ).c_str() , Eta ) ;
      root->SetBranchAddress( ( name + ".Phi" ).c_str() , Phi ) ;
      root->SetBranchAddress( ( name + ".HoverE" ).c_str() , HoverE ) ;
      root->SetBranchAddress( ( name + ".SigmaIetaIeta" ).c_str() , SigmaIetaIeta ) ;
      root->SetBranchAddress( ( name + ".hadTowOverEm" ).c_str() , hadTowOverEm ) ;
      root->SetBranchAddress( ( name + ".hcalIsoConeDR04_2012" ).c_str() , hcalIsoConeDR04_2012 ) ;
      root->SetBranchAddress( ( name + ".phoPFChIso" ).c_str() , phoPFChIso ) ;
      root->SetBranchAddress( ( name + ".phoPFNeuIso" ).c_str() , phoPFNeuIso ) ;
      root->SetBranchAddress( ( name + ".phoPFPhoIso" ).c_str() , phoPFPhoIso ) ;
      root->SetBranchAddress( ( name + ".phoIDMVA" ).c_str() , phoIDMVA ) ;
      root->SetBranchAddress( ( name + ".phoPassLoose" ).c_str() , phoPassLoose ) ;
      root->SetBranchAddress( ( name + ".phoPassMedium" ).c_str() , phoPassMedium ) ;
      root->SetBranchAddress( ( name + ".phoPassTight" ).c_str() , phoPassTight ) ;
      root->SetBranchAddress( ( name + ".r9" ).c_str() , r9 ) ;
      root->SetBranchAddress( ( name + ".passelectronveto" ).c_str() , passelectronveto ) ;
      root->SetBranchAddress( ( name + ".EcalIso" ).c_str() , EcalIso ) ;
      root->SetBranchAddress( ( name + ".HcalIso" ).c_str() , HcalIso ) ;
      root->SetBranchAddress( ( name + ".TrackIso" ).c_str() , TrackIso ) ;
      root->SetBranchAddress( ( name + ".GenPt" ).c_str() , GenPt ) ;
      root->SetBranchAddress( ( name + ".GenEta" ).c_str() , GenEta ) ;
      root->SetBranchAddress( ( name + ".GenPhi" ).c_str() , GenPhi ) ;
      root->SetBranchAddress( ( name + ".GenPdgID" ).c_str() , GenPdgID ) ;
   }
};
class VertexInfoBranches {
public:
   Int_t     Size;
   Int_t     isValid        [MAX_Vertices] ;
   Bool_t    isFake         [MAX_Vertices] ; //Uly 2011-04-04
   Int_t     Type           [MAX_Vertices] ;   //0 - Offline Primary Vertices, 1 - Offline Primary Vertices with beam spot constraint, 2 - Pixel Vertices
   Float_t   Ndof           [MAX_Vertices] ;
   Float_t   NormalizedChi2 [MAX_Vertices] ;
   Float_t   Pt_Sum         [MAX_Vertices] ;
   Float_t   Pt_Sum2        [MAX_Vertices] ;
   Float_t   x              [MAX_Vertices] ;
   Float_t   y              [MAX_Vertices] ;
   Float_t   z              [MAX_Vertices] ;
   Float_t   Rho            [MAX_Vertices] ;
   void RegisterTree( TTree* root , std::string name = "VertexInfo" ) {
      root->Branch( ( name + ".Size" ).c_str(), &Size, ( name + ".Size/I" ).c_str() );
      root->Branch( ( name + ".isValid" ).c_str(), isValid, ( name + ".isValid[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".isFake" ).c_str(), isFake, ( name + ".isFake[" + name + ".Size]/O" ).c_str() );
      root->Branch( ( name + ".Type" ).c_str(), Type, ( name + ".Type[" + name + ".Size]/I" ).c_str() );
      root->Branch( ( name + ".Ndof" ).c_str(), Ndof, ( name + ".Ndof[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".NormalizedChi2" ).c_str(), NormalizedChi2, ( name + ".NormalizedChi2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pt_Sum" ).c_str(), Pt_Sum, ( name + ".Pt_Sum[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Pt_Sum2" ).c_str(), Pt_Sum2, ( name + ".Pt_Sum2[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".x" ).c_str(), x, ( name + ".x[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".y" ).c_str(), y, ( name + ".y[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".z" ).c_str(), z, ( name + ".z[" + name + ".Size]/F" ).c_str() );
      root->Branch( ( name + ".Rho" ).c_str(), Rho, ( name + ".Rho[" + name + ".Size]/F" ).c_str() );
   }
   void Register( TTree* root , std::string name = "VertexInfo" ) {
      root->SetBranchAddress( ( name + ".Size" ).c_str() , &Size ) ;
      root->SetBranchAddress( ( name + ".isValid" ).c_str() , isValid ) ;
      root->SetBranchAddress( ( name + ".isFake" ).c_str() , isFake ) ;
      root->SetBranchAddress( ( name + ".Type" ).c_str() , Type ) ;
      root->SetBranchAddress( ( name + ".Ndof" ).c_str() , Ndof ) ;
      root->SetBranchAddress( ( name + ".NormalizedChi2" ).c_str() , NormalizedChi2 ) ;
      root->SetBranchAddress( ( name + ".Pt_Sum" ).c_str() , Pt_Sum ) ;
      root->SetBranchAddress( ( name + ".Pt_Sum2" ).c_str() , Pt_Sum2 ) ;
      root->SetBranchAddress( ( name + ".x" ).c_str() , x ) ;
      root->SetBranchAddress( ( name + ".y" ).c_str() , y ) ;
      root->SetBranchAddress( ( name + ".z" ).c_str() , z ) ;
      root->SetBranchAddress( ( name + ".Rho" ).c_str() , Rho ) ;
   }
};
#endif // __BPRIMEKIT_FORMAT_H__

