/*******************************************************************************
*
*  Filename    : format.h
*  Description : The branch format for the bprimeKit nTuple
*
*******************************************************************************/
#ifndef __BPRIMEKIT_FORMAT_H__
#define __BPRIMEKIT_FORMAT_H__

// ------------------------------  Required libraries  -------------------------------
#include "TriggerBooking.h"
#include <TTree.h>
#include <vector>

// -------------------------------  Size limitations  --------------------------------
#define MAX_LEPTONS        256
#define MAX_TRACKS         256
#define MAX_JETS           128
#define MAX_PHOTONS        128
#define MAX_GENS           128
#define MAX_LHE            256
#define MAX_Vertices       256
#define MAX_BX             128
#define MAX_TRGOBJS        64


class EvtInfoBranches
{
public:
  Int_t RunNo;
  ULong64_t EvtNo;
  Int_t BxNo;
  Int_t LumiNo;
  Int_t Orbit;
  Int_t McFlag;
  Bool_t Flag_METFilter;
  Float_t Rho;
  Float_t RhoPU [2];
  Float_t SigmaPU [2];
  Float_t BeamSpotX;
  Float_t BeamSpotY;
  Float_t BeamSpotZ;
  Int_t nBX;
  Int_t nPU [MAX_BX];
  Int_t BXPU [MAX_BX];
  Float_t TrueIT [MAX_BX];
  Float_t PFMET;
  Float_t PFMETType1CorrectedPFMetUnclusteredEnUp;
  Float_t PFMETType1CorrectedPFMetUnclusteredEnDown;
  Float_t PFMETPhi;
  Float_t PFRawMET;
  Float_t PFRawMETPhi;
  Float_t PFSumEt;
  Float_t PFMETSig;
  Float_t PFMETlongitudinal;
  Float_t PFMETRealSig;
  Float_t PFGenMET;
  Float_t PFGenMETPhi;
  Float_t PFMETx;
  Float_t PFMETy;
  Float_t PuppiMET;
  Float_t PuppiMETPhi;
  Float_t PuppiRawMET;
  Float_t PuppiRawMETPhi;
  Float_t PuppiSumEt;
  Float_t PuppiMETSig;
  Float_t PuppiMETlongitudinal;
  Float_t PuppiMETRealSig;
  Float_t PuppiGenMET;
  Float_t PuppiGenMETPhi;

  void
  RegisterTree( TTree* root, const std::string& name = "EvtInfo" )
  {
    root->Branch( ( name+".RunNo" ).c_str(),                                     &RunNo,                                     ( name+"RunNo/I" ).c_str() );
    root->Branch( ( name+".EvtNo" ).c_str(),                                     &EvtNo,                                     ( name+"EvtNo/l" ).c_str() );
    root->Branch( ( name+".BxNo" ).c_str(),                                      &BxNo,                                      ( name+"BxNo/I" ).c_str() );
    root->Branch( ( name+".LumiNo" ).c_str(),                                    &LumiNo,                                    ( name+"LumiNo/I" ).c_str() );
    root->Branch( ( name+".Orbit" ).c_str(),                                     &Orbit,                                     ( name+"Orbit/I" ).c_str() );
    root->Branch( ( name+".McFlag" ).c_str(),                                    &McFlag,                                    ( name+"McFlag/I" ).c_str() );
    root->Branch( ( name+".Flag_METFilter" ).c_str(),                            &Flag_METFilter,                            ( name+"Flag_METFilter/O" ).c_str() );
    root->Branch( ( name+".Rho" ).c_str(),                                       &Rho,                                       ( name+"Rho/F" ).c_str() );
    root->Branch( ( name+".RhoPU" ).c_str(),                                     RhoPU,                                      ( name+".RhoPU[2]/F" ).c_str() );
    root->Branch( ( name+".SigmaPU" ).c_str(),                                   SigmaPU,                                    ( name+".SigmaPU[2]/F" ).c_str() );
    root->Branch( ( name+".BeamSpotX" ).c_str(),                                 &BeamSpotX,                                 ( name+"BeamSpotX/F" ).c_str() );
    root->Branch( ( name+".BeamSpotY" ).c_str(),                                 &BeamSpotY,                                 ( name+"BeamSpotY/F" ).c_str() );
    root->Branch( ( name+".BeamSpotZ" ).c_str(),                                 &BeamSpotZ,                                 ( name+"BeamSpotZ/F" ).c_str() );
    root->Branch( ( name+".nBX" ).c_str(),                                       &nBX,                                       ( name+"nBX/I" ).c_str() );
    root->Branch( ( name+".nPU" ).c_str(),                                       nPU,                                        ( name+".nPU["+name+".nBX]/I" ).c_str() );
    root->Branch( ( name+".BXPU" ).c_str(),                                      BXPU,                                       ( name+".BXPU["+name+".nBX]/I" ).c_str() );
    root->Branch( ( name+".TrueIT" ).c_str(),                                    TrueIT,                                     ( name+".TrueIT["+name+".nBX]/F" ).c_str() );
    root->Branch( ( name+".PFMET" ).c_str(),                                     &PFMET,                                     ( name+"PFMET/F" ).c_str() );
    root->Branch( ( name+".PFMETType1CorrectedPFMetUnclusteredEnUp" ).c_str(),   &PFMETType1CorrectedPFMetUnclusteredEnUp,   ( name+"PFMETType1CorrectedPFMetUnclusteredEnUp/F" ).c_str() );
    root->Branch( ( name+".PFMETType1CorrectedPFMetUnclusteredEnDown" ).c_str(), &PFMETType1CorrectedPFMetUnclusteredEnDown, ( name+"PFMETType1CorrectedPFMetUnclusteredEnDown/F" ).c_str() );
    root->Branch( ( name+".PFMETPhi" ).c_str(),                                  &PFMETPhi,                                  ( name+"PFMETPhi/F" ).c_str() );
    root->Branch( ( name+".PFRawMET" ).c_str(),                                  &PFRawMET,                                  ( name+"PFRawMET/F" ).c_str() );
    root->Branch( ( name+".PFRawMETPhi" ).c_str(),                               &PFRawMETPhi,                               ( name+"PFRawMETPhi/F" ).c_str() );
    root->Branch( ( name+".PFSumEt" ).c_str(),                                   &PFSumEt,                                   ( name+"PFSumEt/F" ).c_str() );
    root->Branch( ( name+".PFMETSig" ).c_str(),                                  &PFMETSig,                                  ( name+"PFMETSig/F" ).c_str() );
    root->Branch( ( name+".PFMETlongitudinal" ).c_str(),                         &PFMETlongitudinal,                         ( name+"PFMETlongitudinal/F" ).c_str() );
    root->Branch( ( name+".PFMETRealSig" ).c_str(),                              &PFMETRealSig,                              ( name+"PFMETRealSig/F" ).c_str() );
    root->Branch( ( name+".PFGenMET" ).c_str(),                                  &PFGenMET,                                  ( name+"PFGenMET/F" ).c_str() );
    root->Branch( ( name+".PFGenMETPhi" ).c_str(),                               &PFGenMETPhi,                               ( name+"PFGenMETPhi/F" ).c_str() );
    root->Branch( ( name+".PFMETx" ).c_str(),                                    &PFMETx,                                    ( name+"PFMETx/F" ).c_str() );
    root->Branch( ( name+".PFMETy" ).c_str(),                                    &PFMETy,                                    ( name+"PFMETy/F" ).c_str() );
    root->Branch( ( name+".PuppiMET" ).c_str(),                                  &PuppiMET,                                  ( name+"PuppiMET/F" ).c_str() );
    root->Branch( ( name+".PuppiMETPhi" ).c_str(),                               &PuppiMETPhi,                               ( name+"PuppiMETPhi/F" ).c_str() );
    root->Branch( ( name+".PuppiRawMET" ).c_str(),                               &PuppiRawMET,                               ( name+"PuppiRawMET/F" ).c_str() );
    root->Branch( ( name+".PuppiRawMETPhi" ).c_str(),                            &PuppiRawMETPhi,                            ( name+"PuppiRawMETPhi/F" ).c_str() );
    root->Branch( ( name+".PuppiSumEt" ).c_str(),                                &PuppiSumEt,                                ( name+"PuppiSumEt/F" ).c_str() );
    root->Branch( ( name+".PuppiMETSig" ).c_str(),                               &PuppiMETSig,                               ( name+"PuppiMETSig/F" ).c_str() );
    root->Branch( ( name+".PuppiMETlongitudinal" ).c_str(),                      &PuppiMETlongitudinal,                      ( name+"PuppiMETlongitudinal/F" ).c_str() );
    root->Branch( ( name+".PuppiMETRealSig" ).c_str(),                           &PuppiMETRealSig,                           ( name+"PuppiMETRealSig/F" ).c_str() );
    root->Branch( ( name+".PuppiGenMET" ).c_str(),                               &PuppiGenMET,                               ( name+"PuppiGenMET/F" ).c_str() );
    root->Branch( ( name+".PuppiGenMETPhi" ).c_str(),                            &PuppiGenMETPhi,                            ( name+"PuppiGenMETPhi/F" ).c_str() );
  } 
    
  void
  Register( TTree* root, const std::string& name = "EvtInfo" )
  { 
    root->SetBranchAddress( ( name+".RunNo" ).c_str(),                                     &RunNo );
    root->SetBranchAddress( ( name+".EvtNo" ).c_str(),                                     &EvtNo );
    root->SetBranchAddress( ( name+".BxNo" ).c_str(),                                      &BxNo );
    root->SetBranchAddress( ( name+".LumiNo" ).c_str(),                                    &LumiNo );
    root->SetBranchAddress( ( name+".Orbit" ).c_str(),                                     &Orbit );
    root->SetBranchAddress( ( name+".McFlag" ).c_str(),                                    &McFlag );
    root->SetBranchAddress( ( name+".Flag_METFilter" ).c_str(),                            &Flag_METFilter );
    root->SetBranchAddress( ( name+".Rho" ).c_str(),                                       &Rho );
    root->SetBranchAddress( ( name+".RhoPU" ).c_str(),                                     RhoPU );
    root->SetBranchAddress( ( name+".SigmaPU" ).c_str(),                                   SigmaPU );
    root->SetBranchAddress( ( name+".BeamSpotX" ).c_str(),                                 &BeamSpotX );
    root->SetBranchAddress( ( name+".BeamSpotY" ).c_str(),                                 &BeamSpotY );
    root->SetBranchAddress( ( name+".BeamSpotZ" ).c_str(),                                 &BeamSpotZ );
    root->SetBranchAddress( ( name+".nBX" ).c_str(),                                       &nBX );
    root->SetBranchAddress( ( name+".nPU" ).c_str(),                                       nPU );
    root->SetBranchAddress( ( name+".BXPU" ).c_str(),                                      BXPU );
    root->SetBranchAddress( ( name+".TrueIT" ).c_str(),                                    TrueIT );
    root->SetBranchAddress( ( name+".PFMET" ).c_str(),                                     &PFMET );
    root->SetBranchAddress( ( name+".PFMETType1CorrectedPFMetUnclusteredEnUp" ).c_str(),   &PFMETType1CorrectedPFMetUnclusteredEnUp );
    root->SetBranchAddress( ( name+".PFMETType1CorrectedPFMetUnclusteredEnDown" ).c_str(), &PFMETType1CorrectedPFMetUnclusteredEnDown );
    root->SetBranchAddress( ( name+".PFMETPhi" ).c_str(),                                  &PFMETPhi );
    root->SetBranchAddress( ( name+".PFRawMET" ).c_str(),                                  &PFRawMET );
    root->SetBranchAddress( ( name+".PFRawMETPhi" ).c_str(),                               &PFRawMETPhi );
    root->SetBranchAddress( ( name+".PFSumEt" ).c_str(),                                   &PFSumEt );
    root->SetBranchAddress( ( name+".PFMETSig" ).c_str(),                                  &PFMETSig );
    root->SetBranchAddress( ( name+".PFMETlongitudinal" ).c_str(),                         &PFMETlongitudinal );
    root->SetBranchAddress( ( name+".PFMETRealSig" ).c_str(),                              &PFMETRealSig );
    root->SetBranchAddress( ( name+".PFGenMET" ).c_str(),                                  &PFGenMET );
    root->SetBranchAddress( ( name+".PFGenMETPhi" ).c_str(),                               &PFGenMETPhi );
    root->SetBranchAddress( ( name+".PFMETx" ).c_str(),                                    &PFMETx );
    root->SetBranchAddress( ( name+".PFMETy" ).c_str(),                                    &PFMETy );
    root->SetBranchAddress( ( name+".PuppiMET" ).c_str(),                                  &PuppiMET );
    root->SetBranchAddress( ( name+".PuppiMETPhi" ).c_str(),                               &PuppiMETPhi );
    root->SetBranchAddress( ( name+".PuppiRawMET" ).c_str(),                               &PuppiRawMET );
    root->SetBranchAddress( ( name+".PuppiRawMETPhi" ).c_str(),                            &PuppiRawMETPhi );
    root->SetBranchAddress( ( name+".PuppiSumEt" ).c_str(),                                &PuppiSumEt );
    root->SetBranchAddress( ( name+".PuppiMETSig" ).c_str(),                               &PuppiMETSig );
    root->SetBranchAddress( ( name+".PuppiMETlongitudinal" ).c_str(),                      &PuppiMETlongitudinal );
    root->SetBranchAddress( ( name+".PuppiMETRealSig" ).c_str(),                           &PuppiMETRealSig );
    root->SetBranchAddress( ( name+".PuppiGenMET" ).c_str(),                               &PuppiGenMET );
    root->SetBranchAddress( ( name+".PuppiGenMETPhi" ).c_str(),                            &PuppiGenMETPhi );
  }
};

class GenInfoBranches
{
public:
  Int_t Size;
  Float_t Pt [MAX_GENS];
  Float_t Eta [MAX_GENS];
  Float_t Phi [MAX_GENS];
  Float_t Mass [MAX_GENS];
  Int_t PdgID [MAX_GENS];
  Int_t PhotonFlag [MAX_GENS];
  Int_t Status [MAX_GENS];
  Int_t nMo [MAX_GENS];
  Int_t nDa [MAX_GENS];
  Int_t Mo1 [MAX_GENS];
  Int_t Mo2 [MAX_GENS];
  Int_t Da1 [MAX_GENS];
  Int_t Da2 [MAX_GENS];
  Int_t Mo1PdgID [MAX_GENS];
  Int_t Mo2PdgID [MAX_GENS];
  Int_t Mo1Status [MAX_GENS];
  Int_t Mo2Status [MAX_GENS];
  Int_t Da1PdgID [MAX_GENS];
  Int_t Da2PdgID [MAX_GENS];
  Int_t GrandMo1PdgID [MAX_GENS];
  Int_t GrandMo2PdgID [MAX_GENS];
  Int_t GrandMo1Status [MAX_GENS];
  Int_t GrandMo2Status [MAX_GENS];
  Bool_t McIsTZTZ;
  Bool_t McIsTHTH;
  Bool_t McIsTZTH;
  Bool_t McIsTZBW;
  Bool_t McIsTHBW;
  Bool_t McIsBWBW;
  Float_t Weight;
  Float_t ptHat;
  Int_t PDFid1;
  Int_t PDFid2;
  Float_t PDFx1;
  Float_t PDFx2;
  Float_t PDFscale;
  Float_t PDFv1;
  Float_t PDFv2;
  Float_t LHENominalWeight;
  Float_t LHEOriginalWeight;
  Int_t LHESize;
  Int_t LHESystematicId [MAX_LHE];
  Float_t LHESystematicWeights [MAX_LHE];

  void
  RegisterTree( TTree* root, const std::string& name = "GenInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),                 &Size,                ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".Pt" ).c_str(),                   Pt,                   ( name+".Pt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta" ).c_str(),                  Eta,                  ( name+".Eta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi" ).c_str(),                  Phi,                  ( name+".Phi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Mass" ).c_str(),                 Mass,                 ( name+".Mass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PdgID" ).c_str(),                PdgID,                ( name+".PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".PhotonFlag" ).c_str(),           PhotonFlag,           ( name+".PhotonFlag["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Status" ).c_str(),               Status,               ( name+".Status["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".nMo" ).c_str(),                  nMo,                  ( name+".nMo["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".nDa" ).c_str(),                  nDa,                  ( name+".nDa["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo1" ).c_str(),                  Mo1,                  ( name+".Mo1["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo2" ).c_str(),                  Mo2,                  ( name+".Mo2["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Da1" ).c_str(),                  Da1,                  ( name+".Da1["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Da2" ).c_str(),                  Da2,                  ( name+".Da2["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo1PdgID" ).c_str(),             Mo1PdgID,             ( name+".Mo1PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo2PdgID" ).c_str(),             Mo2PdgID,             ( name+".Mo2PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo1Status" ).c_str(),            Mo1Status,            ( name+".Mo1Status["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Mo2Status" ).c_str(),            Mo2Status,            ( name+".Mo2Status["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Da1PdgID" ).c_str(),             Da1PdgID,             ( name+".Da1PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Da2PdgID" ).c_str(),             Da2PdgID,             ( name+".Da2PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GrandMo1PdgID" ).c_str(),        GrandMo1PdgID,        ( name+".GrandMo1PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GrandMo2PdgID" ).c_str(),        GrandMo2PdgID,        ( name+".GrandMo2PdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GrandMo1Status" ).c_str(),       GrandMo1Status,       ( name+".GrandMo1Status["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GrandMo2Status" ).c_str(),       GrandMo2Status,       ( name+".GrandMo2Status["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".McIsTZTZ" ).c_str(),             &McIsTZTZ,            ( name+"McIsTZTZ/O" ).c_str() );
    root->Branch( ( name+".McIsTHTH" ).c_str(),             &McIsTHTH,            ( name+"McIsTHTH/O" ).c_str() );
    root->Branch( ( name+".McIsTZTH" ).c_str(),             &McIsTZTH,            ( name+"McIsTZTH/O" ).c_str() );
    root->Branch( ( name+".McIsTZBW" ).c_str(),             &McIsTZBW,            ( name+"McIsTZBW/O" ).c_str() );
    root->Branch( ( name+".McIsTHBW" ).c_str(),             &McIsTHBW,            ( name+"McIsTHBW/O" ).c_str() );
    root->Branch( ( name+".McIsBWBW" ).c_str(),             &McIsBWBW,            ( name+"McIsBWBW/O" ).c_str() );
    root->Branch( ( name+".Weight" ).c_str(),               &Weight,              ( name+"Weight/F" ).c_str() );
    root->Branch( ( name+".ptHat" ).c_str(),                &ptHat,               ( name+"ptHat/F" ).c_str() );
    root->Branch( ( name+".PDFid1" ).c_str(),               &PDFid1,              ( name+"PDFid1/I" ).c_str() );
    root->Branch( ( name+".PDFid2" ).c_str(),               &PDFid2,              ( name+"PDFid2/I" ).c_str() );
    root->Branch( ( name+".PDFx1" ).c_str(),                &PDFx1,               ( name+"PDFx1/F" ).c_str() );
    root->Branch( ( name+".PDFx2" ).c_str(),                &PDFx2,               ( name+"PDFx2/F" ).c_str() );
    root->Branch( ( name+".PDFscale" ).c_str(),             &PDFscale,            ( name+"PDFscale/F" ).c_str() );
    root->Branch( ( name+".PDFv1" ).c_str(),                &PDFv1,               ( name+"PDFv1/F" ).c_str() );
    root->Branch( ( name+".PDFv2" ).c_str(),                &PDFv2,               ( name+"PDFv2/F" ).c_str() );
    root->Branch( ( name+".LHENominalWeight" ).c_str(),     &LHENominalWeight,    ( name+"LHENominalWeight/F" ).c_str() );
    root->Branch( ( name+".LHEOriginalWeight" ).c_str(),    &LHEOriginalWeight,   ( name+"LHEOriginalWeight/F" ).c_str() );
    root->Branch( ( name+".LHESize" ).c_str(),              &LHESize,             ( name+"LHESize/I" ).c_str() );
    root->Branch( ( name+".LHESystematicId" ).c_str(),      LHESystematicId,      ( name+".LHESystematicId["+name+".LHESize]/I" ).c_str() );
    root->Branch( ( name+".LHESystematicWeights" ).c_str(), LHESystematicWeights, ( name+".LHESystematicWeights["+name+".LHESize]/F" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "GenInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),                 &Size );
    root->SetBranchAddress( ( name+".Pt" ).c_str(),                   Pt );
    root->SetBranchAddress( ( name+".Eta" ).c_str(),                  Eta );
    root->SetBranchAddress( ( name+".Phi" ).c_str(),                  Phi );
    root->SetBranchAddress( ( name+".Mass" ).c_str(),                 Mass );
    root->SetBranchAddress( ( name+".PdgID" ).c_str(),                PdgID );
    root->SetBranchAddress( ( name+".PhotonFlag" ).c_str(),           PhotonFlag );
    root->SetBranchAddress( ( name+".Status" ).c_str(),               Status );
    root->SetBranchAddress( ( name+".nMo" ).c_str(),                  nMo );
    root->SetBranchAddress( ( name+".nDa" ).c_str(),                  nDa );
    root->SetBranchAddress( ( name+".Mo1" ).c_str(),                  Mo1 );
    root->SetBranchAddress( ( name+".Mo2" ).c_str(),                  Mo2 );
    root->SetBranchAddress( ( name+".Da1" ).c_str(),                  Da1 );
    root->SetBranchAddress( ( name+".Da2" ).c_str(),                  Da2 );
    root->SetBranchAddress( ( name+".Mo1PdgID" ).c_str(),             Mo1PdgID );
    root->SetBranchAddress( ( name+".Mo2PdgID" ).c_str(),             Mo2PdgID );
    root->SetBranchAddress( ( name+".Mo1Status" ).c_str(),            Mo1Status );
    root->SetBranchAddress( ( name+".Mo2Status" ).c_str(),            Mo2Status );
    root->SetBranchAddress( ( name+".Da1PdgID" ).c_str(),             Da1PdgID );
    root->SetBranchAddress( ( name+".Da2PdgID" ).c_str(),             Da2PdgID );
    root->SetBranchAddress( ( name+".GrandMo1PdgID" ).c_str(),        GrandMo1PdgID );
    root->SetBranchAddress( ( name+".GrandMo2PdgID" ).c_str(),        GrandMo2PdgID );
    root->SetBranchAddress( ( name+".GrandMo1Status" ).c_str(),       GrandMo1Status );
    root->SetBranchAddress( ( name+".GrandMo2Status" ).c_str(),       GrandMo2Status );
    root->SetBranchAddress( ( name+".McIsTZTZ" ).c_str(),             &McIsTZTZ );
    root->SetBranchAddress( ( name+".McIsTHTH" ).c_str(),             &McIsTHTH );
    root->SetBranchAddress( ( name+".McIsTZTH" ).c_str(),             &McIsTZTH );
    root->SetBranchAddress( ( name+".McIsTZBW" ).c_str(),             &McIsTZBW );
    root->SetBranchAddress( ( name+".McIsTHBW" ).c_str(),             &McIsTHBW );
    root->SetBranchAddress( ( name+".McIsBWBW" ).c_str(),             &McIsBWBW );
    root->SetBranchAddress( ( name+".Weight" ).c_str(),               &Weight );
    root->SetBranchAddress( ( name+".ptHat" ).c_str(),                &ptHat );
    root->SetBranchAddress( ( name+".PDFid1" ).c_str(),               &PDFid1 );
    root->SetBranchAddress( ( name+".PDFid2" ).c_str(),               &PDFid2 );
    root->SetBranchAddress( ( name+".PDFx1" ).c_str(),                &PDFx1 );
    root->SetBranchAddress( ( name+".PDFx2" ).c_str(),                &PDFx2 );
    root->SetBranchAddress( ( name+".PDFscale" ).c_str(),             &PDFscale );
    root->SetBranchAddress( ( name+".PDFv1" ).c_str(),                &PDFv1 );
    root->SetBranchAddress( ( name+".PDFv2" ).c_str(),                &PDFv2 );
    root->SetBranchAddress( ( name+".LHENominalWeight" ).c_str(),     &LHENominalWeight );
    root->SetBranchAddress( ( name+".LHEOriginalWeight" ).c_str(),    &LHEOriginalWeight );
    root->SetBranchAddress( ( name+".LHESize" ).c_str(),              &LHESize );
    root->SetBranchAddress( ( name+".LHESystematicId" ).c_str(),      LHESystematicId );
    root->SetBranchAddress( ( name+".LHESystematicWeights" ).c_str(), LHESystematicWeights );
  }
};

class JetInfoBranches
{
public:
  Int_t Size;
  Int_t Index [MAX_JETS];
  Int_t NTracks [MAX_JETS];
  Float_t Et [MAX_JETS];
  Float_t Pt [MAX_JETS];
  Float_t Eta [MAX_JETS];
  Float_t Phi [MAX_JETS];
  Float_t Px [MAX_JETS];
  Float_t Py [MAX_JETS];
  Float_t Pz [MAX_JETS];
  Float_t Energy [MAX_JETS];
  Float_t Mass [MAX_JETS];
  Float_t Area [MAX_JETS];
  Int_t JetIDLOOSE [MAX_JETS];
  Float_t JetCharge [MAX_JETS];
  Int_t NConstituents [MAX_JETS];
  Float_t Pt_MuonCleaned [MAX_JETS];
  Float_t Eta_MuonCleaned [MAX_JETS];
  Float_t Phi_MuonCleaned [MAX_JETS];
  Float_t Energy_MuonCleaned [MAX_JETS];
  Float_t Unc [MAX_JETS];
  Float_t JesUnc [MAX_JETS];
  Float_t JERPt [MAX_JETS];
  Float_t JERPhi [MAX_JETS];
  Float_t JERScale [MAX_JETS];
  Float_t QGTagsLikelihood [MAX_JETS];
  Float_t PUJetIDfullDiscriminant [MAX_JETS];
  Int_t PUJetIDcutbased [MAX_JETS];
  Int_t NCH [MAX_JETS];
  Float_t CEF [MAX_JETS];
  Float_t NHF [MAX_JETS];
  Float_t NEF [MAX_JETS];
  Float_t CHF [MAX_JETS];
  Float_t PtCorrRaw [MAX_JETS];
  Float_t PtCorrL2 [MAX_JETS];
  Float_t PtCorrL3 [MAX_JETS];
  Float_t pfCombinedInclusiveSecondaryVertexV2BJetTags [MAX_JETS];
  Float_t pfCombinedMVAV2BJetTags [MAX_JETS];
  Float_t pfDeepCSVJetTags_probb [MAX_JETS];
  Float_t pfDeepCSVJetTags_probbb [MAX_JETS];
  Float_t pfDeepCSVJetTags_probc [MAX_JETS];
  Float_t pfDeepCSVJetTags_probudsg [MAX_JETS];
  Float_t pfBoostedDoubleSecondaryVertexAK8BJetTags [MAX_JETS];
  Float_t GenJetPt [MAX_JETS];
  Float_t GenJetEta [MAX_JETS];
  Float_t GenJetPhi [MAX_JETS];
  Float_t GenPt [MAX_JETS];
  Float_t GenEta [MAX_JETS];
  Float_t GenPhi [MAX_JETS];
  Int_t GenPdgID [MAX_JETS];
  Int_t GenFlavor [MAX_JETS];
  Int_t GenHadronFlavor [MAX_JETS];
  Int_t GenMCTag [MAX_JETS];
  Int_t NSubjets [MAX_JETS];
  Int_t SubjetsIdxStart [MAX_JETS];
  Float_t NjettinessAK8tau1 [MAX_JETS];
  Float_t NjettinessAK8tau2 [MAX_JETS];
  Float_t NjettinessAK8tau3 [MAX_JETS];
  Float_t ak8PFJetsCHSSoftDropMass [MAX_JETS];
  Float_t ak8PFJetsCHSPrunedMass [MAX_JETS];
  Float_t ak8PFJetsCHSTrimmedMass [MAX_JETS];
  Float_t ak8PFJetsCHSFilteredMass [MAX_JETS];
  Float_t topJetMass [MAX_JETS];
  Float_t ca8TopMass [MAX_JETS];
  Float_t ca8MinMass [MAX_JETS];
  std::vector<Float_t>* SubjetMass;
  std::vector<Float_t>* SubjetPt;
  std::vector<Float_t>* SubjetEt;
  std::vector<Float_t>* SubjetEta;
  std::vector<Float_t>* SubjetPhi;
  std::vector<Float_t>* SubjetArea;
  std::vector<Float_t>* SubjetPtUncorr;
  std::vector<Float_t>* SubjetCombinedSVBJetTags;
  std::vector<Float_t>* SubjetDeepCSVJetTags_probb;
  std::vector<Float_t>* SubjetDeepCSVJetTags_probbb;
  std::vector<Float_t>* SubjetDeepCSVJetTags_probc;
  std::vector<Float_t>* SubjetDeepCSVJetTags_probudsg;
  std::vector<Float_t>* SubjetGenPdgId;
  std::vector<Float_t>* SubjetGenFlavour;
  std::vector<Float_t>* SubjetHadronFlavour;
  std::vector<Float_t> SubjetMass_w;
  std::vector<Float_t> SubjetPt_w;
  std::vector<Float_t> SubjetEt_w;
  std::vector<Float_t> SubjetEta_w;
  std::vector<Float_t> SubjetPhi_w;
  std::vector<Float_t> SubjetArea_w;
  std::vector<Float_t> SubjetPtUncorr_w;
  std::vector<Float_t> SubjetCombinedSVBJetTags_w;
  std::vector<Float_t> SubjetDeepCSVJetTags_probb_w;
  std::vector<Float_t> SubjetDeepCSVJetTags_probbb_w;
  std::vector<Float_t> SubjetDeepCSVJetTags_probc_w;
  std::vector<Float_t> SubjetDeepCSVJetTags_probudsg_w;
  std::vector<Float_t> SubjetGenPdgId_w;
  std::vector<Float_t> SubjetGenFlavour_w;
  std::vector<Float_t> SubjetHadronFlavour_w;
  Float_t TempPt1 [MAX_JETS];
  Float_t TempEta1 [MAX_JETS];
  Float_t TempPhi1 [MAX_JETS];
  Float_t TempEnergy1 [MAX_JETS];
  Float_t TempPt2 [MAX_JETS];
  Float_t TempEta2 [MAX_JETS];
  Float_t TempPhi2 [MAX_JETS];
  Float_t TempEnergy2 [MAX_JETS];
  Float_t TempFloat1 [MAX_JETS];
  Float_t TempFloat2 [MAX_JETS];
  Float_t TempFloat3 [MAX_JETS];
  Float_t TempFloat4 [MAX_JETS];

  void
  RegisterTree( TTree* root, const std::string& name = "JetInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),                                         &Size,                                        ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".Index" ).c_str(),                                        Index,                                        ( name+".Index["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".NTracks" ).c_str(),                                      NTracks,                                      ( name+".NTracks["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Et" ).c_str(),                                           Et,                                           ( name+".Et["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Pt" ).c_str(),                                           Pt,                                           ( name+".Pt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta" ).c_str(),                                          Eta,                                          ( name+".Eta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi" ).c_str(),                                          Phi,                                          ( name+".Phi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Px" ).c_str(),                                           Px,                                           ( name+".Px["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Py" ).c_str(),                                           Py,                                           ( name+".Py["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Pz" ).c_str(),                                           Pz,                                           ( name+".Pz["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Energy" ).c_str(),                                       Energy,                                       ( name+".Energy["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Mass" ).c_str(),                                         Mass,                                         ( name+".Mass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Area" ).c_str(),                                         Area,                                         ( name+".Area["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".JetIDLOOSE" ).c_str(),                                   JetIDLOOSE,                                   ( name+".JetIDLOOSE["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".JetCharge" ).c_str(),                                    JetCharge,                                    ( name+".JetCharge["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NConstituents" ).c_str(),                                NConstituents,                                ( name+".NConstituents["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Pt_MuonCleaned" ).c_str(),                               Pt_MuonCleaned,                               ( name+".Pt_MuonCleaned["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta_MuonCleaned" ).c_str(),                              Eta_MuonCleaned,                              ( name+".Eta_MuonCleaned["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi_MuonCleaned" ).c_str(),                              Phi_MuonCleaned,                              ( name+".Phi_MuonCleaned["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Energy_MuonCleaned" ).c_str(),                           Energy_MuonCleaned,                           ( name+".Energy_MuonCleaned["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Unc" ).c_str(),                                          Unc,                                          ( name+".Unc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".JesUnc" ).c_str(),                                       JesUnc,                                       ( name+".JesUnc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".JERPt" ).c_str(),                                        JERPt,                                        ( name+".JERPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".JERPhi" ).c_str(),                                       JERPhi,                                       ( name+".JERPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".JERScale" ).c_str(),                                     JERScale,                                     ( name+".JERScale["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".QGTagsLikelihood" ).c_str(),                             QGTagsLikelihood,                             ( name+".QGTagsLikelihood["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PUJetIDfullDiscriminant" ).c_str(),                      PUJetIDfullDiscriminant,                      ( name+".PUJetIDfullDiscriminant["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PUJetIDcutbased" ).c_str(),                              PUJetIDcutbased,                              ( name+".PUJetIDcutbased["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".NCH" ).c_str(),                                          NCH,                                          ( name+".NCH["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".CEF" ).c_str(),                                          CEF,                                          ( name+".CEF["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NHF" ).c_str(),                                          NHF,                                          ( name+".NHF["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NEF" ).c_str(),                                          NEF,                                          ( name+".NEF["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".CHF" ).c_str(),                                          CHF,                                          ( name+".CHF["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PtCorrRaw" ).c_str(),                                    PtCorrRaw,                                    ( name+".PtCorrRaw["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PtCorrL2" ).c_str(),                                     PtCorrL2,                                     ( name+".PtCorrL2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PtCorrL3" ).c_str(),                                     PtCorrL3,                                     ( name+".PtCorrL3["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfCombinedInclusiveSecondaryVertexV2BJetTags" ).c_str(), pfCombinedInclusiveSecondaryVertexV2BJetTags, ( name+".pfCombinedInclusiveSecondaryVertexV2BJetTags["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfCombinedMVAV2BJetTags" ).c_str(),                      pfCombinedMVAV2BJetTags,                      ( name+".pfCombinedMVAV2BJetTags["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfDeepCSVJetTags_probb" ).c_str(),                       pfDeepCSVJetTags_probb,                       ( name+".pfDeepCSVJetTags_probb["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfDeepCSVJetTags_probbb" ).c_str(),                      pfDeepCSVJetTags_probbb,                      ( name+".pfDeepCSVJetTags_probbb["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfDeepCSVJetTags_probc" ).c_str(),                       pfDeepCSVJetTags_probc,                       ( name+".pfDeepCSVJetTags_probc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfDeepCSVJetTags_probudsg" ).c_str(),                    pfDeepCSVJetTags_probudsg,                    ( name+".pfDeepCSVJetTags_probudsg["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".pfBoostedDoubleSecondaryVertexAK8BJetTags" ).c_str(),    pfBoostedDoubleSecondaryVertexAK8BJetTags,    ( name+".pfBoostedDoubleSecondaryVertexAK8BJetTags["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenJetPt" ).c_str(),                                     GenJetPt,                                     ( name+".GenJetPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenJetEta" ).c_str(),                                    GenJetEta,                                    ( name+".GenJetEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenJetPhi" ).c_str(),                                    GenJetPhi,                                    ( name+".GenJetPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPt" ).c_str(),                                        GenPt,                                        ( name+".GenPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenEta" ).c_str(),                                       GenEta,                                       ( name+".GenEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPhi" ).c_str(),                                       GenPhi,                                       ( name+".GenPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPdgID" ).c_str(),                                     GenPdgID,                                     ( name+".GenPdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GenFlavor" ).c_str(),                                    GenFlavor,                                    ( name+".GenFlavor["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GenHadronFlavor" ).c_str(),                              GenHadronFlavor,                              ( name+".GenHadronFlavor["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GenMCTag" ).c_str(),                                     GenMCTag,                                     ( name+".GenMCTag["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".NSubjets" ).c_str(),                                     NSubjets,                                     ( name+".NSubjets["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".SubjetsIdxStart" ).c_str(),                              SubjetsIdxStart,                              ( name+".SubjetsIdxStart["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".NjettinessAK8tau1" ).c_str(),                            NjettinessAK8tau1,                            ( name+".NjettinessAK8tau1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NjettinessAK8tau2" ).c_str(),                            NjettinessAK8tau2,                            ( name+".NjettinessAK8tau2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NjettinessAK8tau3" ).c_str(),                            NjettinessAK8tau3,                            ( name+".NjettinessAK8tau3["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ak8PFJetsCHSSoftDropMass" ).c_str(),                     ak8PFJetsCHSSoftDropMass,                     ( name+".ak8PFJetsCHSSoftDropMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ak8PFJetsCHSPrunedMass" ).c_str(),                       ak8PFJetsCHSPrunedMass,                       ( name+".ak8PFJetsCHSPrunedMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ak8PFJetsCHSTrimmedMass" ).c_str(),                      ak8PFJetsCHSTrimmedMass,                      ( name+".ak8PFJetsCHSTrimmedMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ak8PFJetsCHSFilteredMass" ).c_str(),                     ak8PFJetsCHSFilteredMass,                     ( name+".ak8PFJetsCHSFilteredMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".topJetMass" ).c_str(),                                   topJetMass,                                   ( name+".topJetMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ca8TopMass" ).c_str(),                                   ca8TopMass,                                   ( name+".ca8TopMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ca8MinMass" ).c_str(),                                   ca8MinMass,                                   ( name+".ca8MinMass["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".SubjetMass" ).c_str(),                                   &SubjetMass_w );
    root->Branch( ( name+".SubjetPt" ).c_str(),                                     &SubjetPt_w );
    root->Branch( ( name+".SubjetEt" ).c_str(),                                     &SubjetEt_w );
    root->Branch( ( name+".SubjetEta" ).c_str(),                                    &SubjetEta_w );
    root->Branch( ( name+".SubjetPhi" ).c_str(),                                    &SubjetPhi_w );
    root->Branch( ( name+".SubjetArea" ).c_str(),                                   &SubjetArea_w );
    root->Branch( ( name+".SubjetPtUncorr" ).c_str(),                               &SubjetPtUncorr_w );
    root->Branch( ( name+".SubjetCombinedSVBJetTags" ).c_str(),                     &SubjetCombinedSVBJetTags_w );
    root->Branch( ( name+".SubjetDeepCSVJetTags_probb" ).c_str(),                   &SubjetDeepCSVJetTags_probb_w );
    root->Branch( ( name+".SubjetDeepCSVJetTags_probbb" ).c_str(),                  &SubjetDeepCSVJetTags_probbb_w );
    root->Branch( ( name+".SubjetDeepCSVJetTags_probc" ).c_str(),                   &SubjetDeepCSVJetTags_probc_w );
    root->Branch( ( name+".SubjetDeepCSVJetTags_probudsg" ).c_str(),                &SubjetDeepCSVJetTags_probudsg_w );
    root->Branch( ( name+".SubjetGenPdgId" ).c_str(),                               &SubjetGenPdgId_w );
    root->Branch( ( name+".SubjetGenFlavour" ).c_str(),                             &SubjetGenFlavour_w );
    root->Branch( ( name+".SubjetHadronFlavour" ).c_str(),                          &SubjetHadronFlavour_w );
    root->Branch( ( name+".TempPt1" ).c_str(),                                      TempPt1,     ( name+".TempPt1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta1" ).c_str(),                                     TempEta1,    ( name+".TempEta1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi1" ).c_str(),                                     TempPhi1,    ( name+".TempPhi1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy1" ).c_str(),                                  TempEnergy1, ( name+".TempEnergy1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPt2" ).c_str(),                                      TempPt2,     ( name+".TempPt2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta2" ).c_str(),                                     TempEta2,    ( name+".TempEta2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi2" ).c_str(),                                     TempPhi2,    ( name+".TempPhi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy2" ).c_str(),                                  TempEnergy2, ( name+".TempEnergy2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat1" ).c_str(),                                   TempFloat1,  ( name+".TempFloat1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat2" ).c_str(),                                   TempFloat2,  ( name+".TempFloat2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat3" ).c_str(),                                   TempFloat3,  ( name+".TempFloat3["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat4" ).c_str(),                                   TempFloat4,  ( name+".TempFloat4["+name+".Size]/F" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "JetInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),                                         &Size );
    root->SetBranchAddress( ( name+".Index" ).c_str(),                                        Index );
    root->SetBranchAddress( ( name+".NTracks" ).c_str(),                                      NTracks );
    root->SetBranchAddress( ( name+".Et" ).c_str(),                                           Et );
    root->SetBranchAddress( ( name+".Pt" ).c_str(),                                           Pt );
    root->SetBranchAddress( ( name+".Eta" ).c_str(),                                          Eta );
    root->SetBranchAddress( ( name+".Phi" ).c_str(),                                          Phi );
    root->SetBranchAddress( ( name+".Px" ).c_str(),                                           Px );
    root->SetBranchAddress( ( name+".Py" ).c_str(),                                           Py );
    root->SetBranchAddress( ( name+".Pz" ).c_str(),                                           Pz );
    root->SetBranchAddress( ( name+".Energy" ).c_str(),                                       Energy );
    root->SetBranchAddress( ( name+".Mass" ).c_str(),                                         Mass );
    root->SetBranchAddress( ( name+".Area" ).c_str(),                                         Area );
    root->SetBranchAddress( ( name+".JetIDLOOSE" ).c_str(),                                   JetIDLOOSE );
    root->SetBranchAddress( ( name+".JetCharge" ).c_str(),                                    JetCharge );
    root->SetBranchAddress( ( name+".NConstituents" ).c_str(),                                NConstituents );
    root->SetBranchAddress( ( name+".Pt_MuonCleaned" ).c_str(),                               Pt_MuonCleaned );
    root->SetBranchAddress( ( name+".Eta_MuonCleaned" ).c_str(),                              Eta_MuonCleaned );
    root->SetBranchAddress( ( name+".Phi_MuonCleaned" ).c_str(),                              Phi_MuonCleaned );
    root->SetBranchAddress( ( name+".Energy_MuonCleaned" ).c_str(),                           Energy_MuonCleaned );
    root->SetBranchAddress( ( name+".Unc" ).c_str(),                                          Unc );
    root->SetBranchAddress( ( name+".JesUnc" ).c_str(),                                       JesUnc );
    root->SetBranchAddress( ( name+".JERPt" ).c_str(),                                        JERPt );
    root->SetBranchAddress( ( name+".JERPhi" ).c_str(),                                       JERPhi );
    root->SetBranchAddress( ( name+".JERScale" ).c_str(),                                     JERScale );
    root->SetBranchAddress( ( name+".QGTagsLikelihood" ).c_str(),                             QGTagsLikelihood );
    root->SetBranchAddress( ( name+".PUJetIDfullDiscriminant" ).c_str(),                      PUJetIDfullDiscriminant );
    root->SetBranchAddress( ( name+".PUJetIDcutbased" ).c_str(),                              PUJetIDcutbased );
    root->SetBranchAddress( ( name+".NCH" ).c_str(),                                          NCH );
    root->SetBranchAddress( ( name+".CEF" ).c_str(),                                          CEF );
    root->SetBranchAddress( ( name+".NHF" ).c_str(),                                          NHF );
    root->SetBranchAddress( ( name+".NEF" ).c_str(),                                          NEF );
    root->SetBranchAddress( ( name+".CHF" ).c_str(),                                          CHF );
    root->SetBranchAddress( ( name+".PtCorrRaw" ).c_str(),                                    PtCorrRaw );
    root->SetBranchAddress( ( name+".PtCorrL2" ).c_str(),                                     PtCorrL2 );
    root->SetBranchAddress( ( name+".PtCorrL3" ).c_str(),                                     PtCorrL3 );
    root->SetBranchAddress( ( name+".pfCombinedInclusiveSecondaryVertexV2BJetTags" ).c_str(), pfCombinedInclusiveSecondaryVertexV2BJetTags );
    root->SetBranchAddress( ( name+".pfCombinedMVAV2BJetTags" ).c_str(),                      pfCombinedMVAV2BJetTags );
    root->SetBranchAddress( ( name+".pfDeepCSVJetTags_probb" ).c_str(),                       pfDeepCSVJetTags_probb );
    root->SetBranchAddress( ( name+".pfDeepCSVJetTags_probbb" ).c_str(),                      pfDeepCSVJetTags_probbb );
    root->SetBranchAddress( ( name+".pfDeepCSVJetTags_probc" ).c_str(),                       pfDeepCSVJetTags_probc );
    root->SetBranchAddress( ( name+".pfDeepCSVJetTags_probudsg" ).c_str(),                    pfDeepCSVJetTags_probudsg );
    root->SetBranchAddress( ( name+".pfBoostedDoubleSecondaryVertexAK8BJetTags" ).c_str(),    pfBoostedDoubleSecondaryVertexAK8BJetTags );
    root->SetBranchAddress( ( name+".GenJetPt" ).c_str(),                                     GenJetPt );
    root->SetBranchAddress( ( name+".GenJetEta" ).c_str(),                                    GenJetEta );
    root->SetBranchAddress( ( name+".GenJetPhi" ).c_str(),                                    GenJetPhi );
    root->SetBranchAddress( ( name+".GenPt" ).c_str(),                                        GenPt );
    root->SetBranchAddress( ( name+".GenEta" ).c_str(),                                       GenEta );
    root->SetBranchAddress( ( name+".GenPhi" ).c_str(),                                       GenPhi );
    root->SetBranchAddress( ( name+".GenPdgID" ).c_str(),                                     GenPdgID );
    root->SetBranchAddress( ( name+".GenFlavor" ).c_str(),                                    GenFlavor );
    root->SetBranchAddress( ( name+".GenHadronFlavor" ).c_str(),                              GenHadronFlavor );
    root->SetBranchAddress( ( name+".GenMCTag" ).c_str(),                                     GenMCTag );
    root->SetBranchAddress( ( name+".NSubjets" ).c_str(),                                     NSubjets );
    root->SetBranchAddress( ( name+".SubjetsIdxStart" ).c_str(),                              SubjetsIdxStart );
    root->SetBranchAddress( ( name+".NjettinessAK8tau1" ).c_str(),                            NjettinessAK8tau1 );
    root->SetBranchAddress( ( name+".NjettinessAK8tau2" ).c_str(),                            NjettinessAK8tau2 );
    root->SetBranchAddress( ( name+".NjettinessAK8tau3" ).c_str(),                            NjettinessAK8tau3 );
    root->SetBranchAddress( ( name+".ak8PFJetsCHSSoftDropMass" ).c_str(),                     ak8PFJetsCHSSoftDropMass );
    root->SetBranchAddress( ( name+".ak8PFJetsCHSPrunedMass" ).c_str(),                       ak8PFJetsCHSPrunedMass );
    root->SetBranchAddress( ( name+".ak8PFJetsCHSTrimmedMass" ).c_str(),                      ak8PFJetsCHSTrimmedMass );
    root->SetBranchAddress( ( name+".ak8PFJetsCHSFilteredMass" ).c_str(),                     ak8PFJetsCHSFilteredMass );
    root->SetBranchAddress( ( name+".topJetMass" ).c_str(),                                   topJetMass );
    root->SetBranchAddress( ( name+".ca8TopMass" ).c_str(),                                   ca8TopMass );
    root->SetBranchAddress( ( name+".ca8MinMass" ).c_str(),                                   ca8MinMass );
    SubjetMass = 0;
    root->SetBranchAddress( ( name+".SubjetMass" ).c_str(),                                   &SubjetMass );
    SubjetPt = 0;
    root->SetBranchAddress( ( name+".SubjetPt" ).c_str(),                                     &SubjetPt );
    SubjetEt = 0;
    root->SetBranchAddress( ( name+".SubjetEt" ).c_str(),                                     &SubjetEt );
    SubjetEta = 0;
    root->SetBranchAddress( ( name+".SubjetEta" ).c_str(),                                    &SubjetEta );
    SubjetPhi = 0;
    root->SetBranchAddress( ( name+".SubjetPhi" ).c_str(),                                    &SubjetPhi );
    SubjetArea = 0;
    root->SetBranchAddress( ( name+".SubjetArea" ).c_str(),                                   &SubjetArea );
    SubjetPtUncorr = 0;
    root->SetBranchAddress( ( name+".SubjetPtUncorr" ).c_str(),                               &SubjetPtUncorr );
    SubjetCombinedSVBJetTags = 0;
    root->SetBranchAddress( ( name+".SubjetCombinedSVBJetTags" ).c_str(),                     &SubjetCombinedSVBJetTags );
    SubjetDeepCSVJetTags_probb = 0;
    root->SetBranchAddress( ( name+".SubjetDeepCSVJetTags_probb" ).c_str(),                   &SubjetDeepCSVJetTags_probb );
    SubjetDeepCSVJetTags_probbb = 0;
    root->SetBranchAddress( ( name+".SubjetDeepCSVJetTags_probbb" ).c_str(),                  &SubjetDeepCSVJetTags_probbb );
    SubjetDeepCSVJetTags_probc = 0;
    root->SetBranchAddress( ( name+".SubjetDeepCSVJetTags_probc" ).c_str(),                   &SubjetDeepCSVJetTags_probc );
    SubjetDeepCSVJetTags_probudsg = 0;
    root->SetBranchAddress( ( name+".SubjetDeepCSVJetTags_probudsg" ).c_str(),                &SubjetDeepCSVJetTags_probudsg );
    SubjetGenPdgId = 0;
    root->SetBranchAddress( ( name+".SubjetGenPdgId" ).c_str(),                               &SubjetGenPdgId );
    SubjetGenFlavour = 0;
    root->SetBranchAddress( ( name+".SubjetGenFlavour" ).c_str(),                             &SubjetGenFlavour );
    SubjetHadronFlavour = 0;
    root->SetBranchAddress( ( name+".SubjetHadronFlavour" ).c_str(),                          &SubjetHadronFlavour );
    root->SetBranchAddress( ( name+".TempPt1" ).c_str(),                                      TempPt1 );
    root->SetBranchAddress( ( name+".TempEta1" ).c_str(),                                     TempEta1 );
    root->SetBranchAddress( ( name+".TempPhi1" ).c_str(),                                     TempPhi1 );
    root->SetBranchAddress( ( name+".TempEnergy1" ).c_str(),                                  TempEnergy1 );
    root->SetBranchAddress( ( name+".TempPt2" ).c_str(),                                      TempPt2 );
    root->SetBranchAddress( ( name+".TempEta2" ).c_str(),                                     TempEta2 );
    root->SetBranchAddress( ( name+".TempPhi2" ).c_str(),                                     TempPhi2 );
    root->SetBranchAddress( ( name+".TempEnergy2" ).c_str(),                                  TempEnergy2 );
    root->SetBranchAddress( ( name+".TempFloat1" ).c_str(),                                   TempFloat1 );
    root->SetBranchAddress( ( name+".TempFloat2" ).c_str(),                                   TempFloat2 );
    root->SetBranchAddress( ( name+".TempFloat3" ).c_str(),                                   TempFloat3 );
    root->SetBranchAddress( ( name+".TempFloat4" ).c_str(),                                   TempFloat4 );
  }
};

class LepInfoBranches
{
public:
  Int_t Size;
  Int_t Index [MAX_LEPTONS];
  Int_t LeptonType [MAX_LEPTONS];
  Int_t Charge [MAX_LEPTONS];
  Float_t Pt [MAX_LEPTONS];
  Float_t Et [MAX_LEPTONS];
  Float_t Eta [MAX_LEPTONS];
  Float_t Phi [MAX_LEPTONS];
  Float_t Px [MAX_LEPTONS];
  Float_t Py [MAX_LEPTONS];
  Float_t Pz [MAX_LEPTONS];
  Float_t Energy [MAX_LEPTONS];
  Float_t TrackIso [MAX_LEPTONS];
  Float_t EcalIso [MAX_LEPTONS];
  Float_t HcalIso [MAX_LEPTONS];
  Float_t ChargedHadronIso [MAX_LEPTONS];
  Float_t NeutralHadronIso [MAX_LEPTONS];
  Float_t PhotonIso [MAX_LEPTONS];
  Float_t SumPUPt [MAX_LEPTONS];
  Float_t ChargedHadronIsoR03 [MAX_LEPTONS];
  Float_t NeutralHadronIsoR03 [MAX_LEPTONS];
  Float_t PhotonIsoR03 [MAX_LEPTONS];
  Float_t sumPUPtR03 [MAX_LEPTONS];
  Float_t IsoRhoCorrR03 [MAX_LEPTONS];
  Float_t ChargedHadronIsoR04 [MAX_LEPTONS];
  Float_t NeutralHadronIsoR04 [MAX_LEPTONS];
  Float_t PhotonIsoR04 [MAX_LEPTONS];
  Float_t sumPUPtR04 [MAX_LEPTONS];
  Float_t IsoRhoCorrR04 [MAX_LEPTONS];
  Float_t Ip3dPV [MAX_LEPTONS];
  Float_t Ip3dPVErr [MAX_LEPTONS];
  Float_t Ip3dPVSignificance [MAX_LEPTONS];
  Float_t MiniIso [MAX_LEPTONS];
  Float_t CaloEnergy [MAX_LEPTONS];
  Bool_t isGoodMuonTMOneStationTight [MAX_LEPTONS];
  Bool_t isPFMuon [MAX_LEPTONS];
  Bool_t MuIDGlobalMuonPromptTight [MAX_LEPTONS];
  Float_t MuGlobalNormalizedChi2 [MAX_LEPTONS];
  Float_t MuCaloCompat [MAX_LEPTONS];
  Int_t MuNChambers [MAX_LEPTONS];
  Int_t MuNChambersMatchesSegment [MAX_LEPTONS];
  Int_t MuNMatchedStations [MAX_LEPTONS];
  Int_t MuNLostOuterHits [MAX_LEPTONS];
  Int_t MuNMuonhits [MAX_LEPTONS];
  Int_t MuDThits [MAX_LEPTONS];
  Int_t MuCSChits [MAX_LEPTONS];
  Int_t MuRPChits [MAX_LEPTONS];
  Int_t MuType [MAX_LEPTONS];
  Int_t MuontimenDof [MAX_LEPTONS];
  Float_t MuontimeAtIpInOut [MAX_LEPTONS];
  Float_t MuontimeAtIpOutIn [MAX_LEPTONS];
  Int_t Muondirection [MAX_LEPTONS];
  Float_t innerTracknormalizedChi2 [MAX_LEPTONS];
  Float_t MuInnerPtError [MAX_LEPTONS];
  Float_t MuGlobalPtError [MAX_LEPTONS];
  Float_t MuInnerTrackDz [MAX_LEPTONS];
  Float_t MuInnerTrackD0 [MAX_LEPTONS];
  Float_t MuInnerTrackDxy_BS [MAX_LEPTONS];
  Float_t MuInnerTrackDxy_PV [MAX_LEPTONS];
  Float_t MuInnerTrackDxy_PVBS [MAX_LEPTONS];
  Int_t MuInnerTrackNHits [MAX_LEPTONS];
  Int_t MuNTrackerHits [MAX_LEPTONS];
  Int_t MuNLostInnerHits [MAX_LEPTONS];
  Float_t vertexZ [MAX_LEPTONS];
  Int_t MuNPixelLayers [MAX_LEPTONS];
  Int_t MuNPixelLayersWMeasurement [MAX_LEPTONS];
  Int_t MuNTrackLayersWMeasurement [MAX_LEPTONS];
  Int_t ChargeGsf [MAX_LEPTONS];
  Int_t ChargeScPix [MAX_LEPTONS];
  Int_t isEcalDriven [MAX_LEPTONS];
  Int_t isTrackerDriven [MAX_LEPTONS];
  Float_t caloEta [MAX_LEPTONS];
  Float_t e1x5 [MAX_LEPTONS];
  Float_t e2x5Max [MAX_LEPTONS];
  Float_t e5x5 [MAX_LEPTONS];
  Float_t HcalDepth1Iso [MAX_LEPTONS];
  Float_t HcalDepth2Iso [MAX_LEPTONS];
  Float_t EgammaMVANonTrig [MAX_LEPTONS];
  Float_t EgammaMVATrig [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdTRIGGERTIGHT [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdTRIGGERWP70 [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdVETO [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdLOOSE [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdMEDIUM [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdTIGHT [MAX_LEPTONS];
  Bool_t EgammaCutBasedEleIdHEEP [MAX_LEPTONS];
  Float_t Eldr03HcalDepth1TowerSumEtBc [MAX_LEPTONS];
  Float_t Eldr03HcalDepth2TowerSumEtBc [MAX_LEPTONS];
  Float_t Eldr04HcalDepth1TowerSumEtBc [MAX_LEPTONS];
  Float_t Eldr04HcalDepth2TowerSumEtBc [MAX_LEPTONS];
  Float_t ElhcalOverEcalBc [MAX_LEPTONS];
  Float_t ElEcalE [MAX_LEPTONS];
  Float_t ElEoverP [MAX_LEPTONS];
  Float_t EldeltaEta [MAX_LEPTONS];
  Float_t EldeltaPhi [MAX_LEPTONS];
  Float_t ElHadoverEm [MAX_LEPTONS];
  Float_t ElsigmaIetaIeta [MAX_LEPTONS];
  Float_t ElscSigmaIetaIeta [MAX_LEPTONS];
  Float_t ElEnergyErr [MAX_LEPTONS];
  Float_t ElMomentumErr [MAX_LEPTONS];
  Float_t ElSharedHitsFraction [MAX_LEPTONS];
  Bool_t ElhasConv [MAX_LEPTONS];
  Int_t ElTrackNHits [MAX_LEPTONS];
  Float_t ElTrackNLostHits [MAX_LEPTONS];
  Float_t ElTrackDz [MAX_LEPTONS];
  Float_t ElTrackDz_BS [MAX_LEPTONS];
  Float_t ElTrackD0 [MAX_LEPTONS];
  Float_t ElTrackDxy_BS [MAX_LEPTONS];
  Float_t ElTrackDxy_PV [MAX_LEPTONS];
  Float_t ElTrackDxy_PVBS [MAX_LEPTONS];
  Int_t ElNClusters [MAX_LEPTONS];
  Int_t ElClassification [MAX_LEPTONS];
  Float_t ElFBrem [MAX_LEPTONS];
  Int_t NumberOfExpectedInnerHits [MAX_LEPTONS];
  Float_t Eldist [MAX_LEPTONS];
  Float_t Eldcot [MAX_LEPTONS];
  Float_t Elconvradius [MAX_LEPTONS];
  Float_t dcotdist [MAX_LEPTONS];
  Float_t ElseedEoverP [MAX_LEPTONS];
  Float_t ElEcalIso04 [MAX_LEPTONS];
  Float_t ElHcalIso04 [MAX_LEPTONS];
  Int_t ElNumberOfBrems [MAX_LEPTONS];
  Int_t isPFTau [MAX_LEPTONS];
  Float_t GenPt [MAX_LEPTONS];
  Float_t GenEta [MAX_LEPTONS];
  Float_t GenPhi [MAX_LEPTONS];
  Int_t GenPdgID [MAX_LEPTONS];
  Int_t GenMCTag [MAX_LEPTONS];
  Float_t TempPt1 [MAX_LEPTONS];
  Float_t TempEta1 [MAX_LEPTONS];
  Float_t TempPhi1 [MAX_LEPTONS];
  Float_t TempEnergy1 [MAX_LEPTONS];
  Float_t TempPt2 [MAX_LEPTONS];
  Float_t TempEta2 [MAX_LEPTONS];
  Float_t TempPhi2 [MAX_LEPTONS];
  Float_t TempEnergy2 [MAX_LEPTONS];
  Float_t TempFloat1 [MAX_LEPTONS];
  Float_t TempFloat2 [MAX_LEPTONS];
  Float_t TempFloat3 [MAX_LEPTONS];
  Float_t TempFloat4 [MAX_LEPTONS];

  void
  RegisterTree( TTree* root, const std::string& name = "LepInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),                            &Size,                           ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".Index" ).c_str(),                           Index,                           ( name+".Index["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".LeptonType" ).c_str(),                      LeptonType,                      ( name+".LeptonType["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Charge" ).c_str(),                          Charge,                          ( name+".Charge["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Pt" ).c_str(),                              Pt,                              ( name+".Pt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Et" ).c_str(),                              Et,                              ( name+".Et["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta" ).c_str(),                             Eta,                             ( name+".Eta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi" ).c_str(),                             Phi,                             ( name+".Phi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Px" ).c_str(),                              Px,                              ( name+".Px["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Py" ).c_str(),                              Py,                              ( name+".Py["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Pz" ).c_str(),                              Pz,                              ( name+".Pz["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Energy" ).c_str(),                          Energy,                          ( name+".Energy["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TrackIso" ).c_str(),                        TrackIso,                        ( name+".TrackIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EcalIso" ).c_str(),                         EcalIso,                         ( name+".EcalIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".HcalIso" ).c_str(),                         HcalIso,                         ( name+".HcalIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ChargedHadronIso" ).c_str(),                ChargedHadronIso,                ( name+".ChargedHadronIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NeutralHadronIso" ).c_str(),                NeutralHadronIso,                ( name+".NeutralHadronIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PhotonIso" ).c_str(),                       PhotonIso,                       ( name+".PhotonIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".SumPUPt" ).c_str(),                         SumPUPt,                         ( name+".SumPUPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ChargedHadronIsoR03" ).c_str(),             ChargedHadronIsoR03,             ( name+".ChargedHadronIsoR03["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NeutralHadronIsoR03" ).c_str(),             NeutralHadronIsoR03,             ( name+".NeutralHadronIsoR03["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PhotonIsoR03" ).c_str(),                    PhotonIsoR03,                    ( name+".PhotonIsoR03["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".sumPUPtR03" ).c_str(),                      sumPUPtR03,                      ( name+".sumPUPtR03["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".IsoRhoCorrR03" ).c_str(),                   IsoRhoCorrR03,                   ( name+".IsoRhoCorrR03["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ChargedHadronIsoR04" ).c_str(),             ChargedHadronIsoR04,             ( name+".ChargedHadronIsoR04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NeutralHadronIsoR04" ).c_str(),             NeutralHadronIsoR04,             ( name+".NeutralHadronIsoR04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".PhotonIsoR04" ).c_str(),                    PhotonIsoR04,                    ( name+".PhotonIsoR04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".sumPUPtR04" ).c_str(),                      sumPUPtR04,                      ( name+".sumPUPtR04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".IsoRhoCorrR04" ).c_str(),                   IsoRhoCorrR04,                   ( name+".IsoRhoCorrR04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Ip3dPV" ).c_str(),                          Ip3dPV,                          ( name+".Ip3dPV["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Ip3dPVErr" ).c_str(),                       Ip3dPVErr,                       ( name+".Ip3dPVErr["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Ip3dPVSignificance" ).c_str(),              Ip3dPVSignificance,              ( name+".Ip3dPVSignificance["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MiniIso" ).c_str(),                         MiniIso,                         ( name+".MiniIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".CaloEnergy" ).c_str(),                      CaloEnergy,                      ( name+".CaloEnergy["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".isGoodMuonTMOneStationTight" ).c_str(),     isGoodMuonTMOneStationTight,     ( name+".isGoodMuonTMOneStationTight["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".isPFMuon" ).c_str(),                        isPFMuon,                        ( name+".isPFMuon["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".MuIDGlobalMuonPromptTight" ).c_str(),       MuIDGlobalMuonPromptTight,       ( name+".MuIDGlobalMuonPromptTight["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".MuGlobalNormalizedChi2" ).c_str(),          MuGlobalNormalizedChi2,          ( name+".MuGlobalNormalizedChi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuCaloCompat" ).c_str(),                    MuCaloCompat,                    ( name+".MuCaloCompat["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuNChambers" ).c_str(),                     MuNChambers,                     ( name+".MuNChambers["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNChambersMatchesSegment" ).c_str(),       MuNChambersMatchesSegment,       ( name+".MuNChambersMatchesSegment["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNMatchedStations" ).c_str(),              MuNMatchedStations,              ( name+".MuNMatchedStations["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNLostOuterHits" ).c_str(),                MuNLostOuterHits,                ( name+".MuNLostOuterHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNMuonhits" ).c_str(),                     MuNMuonhits,                     ( name+".MuNMuonhits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuDThits" ).c_str(),                        MuDThits,                        ( name+".MuDThits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuCSChits" ).c_str(),                       MuCSChits,                       ( name+".MuCSChits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuRPChits" ).c_str(),                       MuRPChits,                       ( name+".MuRPChits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuType" ).c_str(),                          MuType,                          ( name+".MuType["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuontimenDof" ).c_str(),                    MuontimenDof,                    ( name+".MuontimenDof["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuontimeAtIpInOut" ).c_str(),               MuontimeAtIpInOut,               ( name+".MuontimeAtIpInOut["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuontimeAtIpOutIn" ).c_str(),               MuontimeAtIpOutIn,               ( name+".MuontimeAtIpOutIn["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Muondirection" ).c_str(),                   Muondirection,                   ( name+".Muondirection["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".innerTracknormalizedChi2" ).c_str(),        innerTracknormalizedChi2,        ( name+".innerTracknormalizedChi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerPtError" ).c_str(),                  MuInnerPtError,                  ( name+".MuInnerPtError["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuGlobalPtError" ).c_str(),                 MuGlobalPtError,                 ( name+".MuGlobalPtError["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackDz" ).c_str(),                  MuInnerTrackDz,                  ( name+".MuInnerTrackDz["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackD0" ).c_str(),                  MuInnerTrackD0,                  ( name+".MuInnerTrackD0["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackDxy_BS" ).c_str(),              MuInnerTrackDxy_BS,              ( name+".MuInnerTrackDxy_BS["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackDxy_PV" ).c_str(),              MuInnerTrackDxy_PV,              ( name+".MuInnerTrackDxy_PV["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackDxy_PVBS" ).c_str(),            MuInnerTrackDxy_PVBS,            ( name+".MuInnerTrackDxy_PVBS["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuInnerTrackNHits" ).c_str(),               MuInnerTrackNHits,               ( name+".MuInnerTrackNHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNTrackerHits" ).c_str(),                  MuNTrackerHits,                  ( name+".MuNTrackerHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNLostInnerHits" ).c_str(),                MuNLostInnerHits,                ( name+".MuNLostInnerHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".vertexZ" ).c_str(),                         vertexZ,                         ( name+".vertexZ["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".MuNPixelLayers" ).c_str(),                  MuNPixelLayers,                  ( name+".MuNPixelLayers["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNPixelLayersWMeasurement" ).c_str(),      MuNPixelLayersWMeasurement,      ( name+".MuNPixelLayersWMeasurement["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".MuNTrackLayersWMeasurement" ).c_str(),      MuNTrackLayersWMeasurement,      ( name+".MuNTrackLayersWMeasurement["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".ChargeGsf" ).c_str(),                       ChargeGsf,                       ( name+".ChargeGsf["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".ChargeScPix" ).c_str(),                     ChargeScPix,                     ( name+".ChargeScPix["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".isEcalDriven" ).c_str(),                    isEcalDriven,                    ( name+".isEcalDriven["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".isTrackerDriven" ).c_str(),                 isTrackerDriven,                 ( name+".isTrackerDriven["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".caloEta" ).c_str(),                         caloEta,                         ( name+".caloEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".e1x5" ).c_str(),                            e1x5,                            ( name+".e1x5["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".e2x5Max" ).c_str(),                         e2x5Max,                         ( name+".e2x5Max["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".e5x5" ).c_str(),                            e5x5,                            ( name+".e5x5["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".HcalDepth1Iso" ).c_str(),                   HcalDepth1Iso,                   ( name+".HcalDepth1Iso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".HcalDepth2Iso" ).c_str(),                   HcalDepth2Iso,                   ( name+".HcalDepth2Iso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EgammaMVANonTrig" ).c_str(),                EgammaMVANonTrig,                ( name+".EgammaMVANonTrig["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EgammaMVATrig" ).c_str(),                   EgammaMVATrig,                   ( name+".EgammaMVATrig["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdTRIGGERTIGHT" ).c_str(), EgammaCutBasedEleIdTRIGGERTIGHT, ( name+".EgammaCutBasedEleIdTRIGGERTIGHT["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdTRIGGERWP70" ).c_str(),  EgammaCutBasedEleIdTRIGGERWP70,  ( name+".EgammaCutBasedEleIdTRIGGERWP70["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdVETO" ).c_str(),         EgammaCutBasedEleIdVETO,         ( name+".EgammaCutBasedEleIdVETO["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdLOOSE" ).c_str(),        EgammaCutBasedEleIdLOOSE,        ( name+".EgammaCutBasedEleIdLOOSE["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdMEDIUM" ).c_str(),       EgammaCutBasedEleIdMEDIUM,       ( name+".EgammaCutBasedEleIdMEDIUM["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdTIGHT" ).c_str(),        EgammaCutBasedEleIdTIGHT,        ( name+".EgammaCutBasedEleIdTIGHT["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EgammaCutBasedEleIdHEEP" ).c_str(),         EgammaCutBasedEleIdHEEP,         ( name+".EgammaCutBasedEleIdHEEP["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".Eldr03HcalDepth1TowerSumEtBc" ).c_str(),    Eldr03HcalDepth1TowerSumEtBc,    ( name+".Eldr03HcalDepth1TowerSumEtBc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eldr03HcalDepth2TowerSumEtBc" ).c_str(),    Eldr03HcalDepth2TowerSumEtBc,    ( name+".Eldr03HcalDepth2TowerSumEtBc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eldr04HcalDepth1TowerSumEtBc" ).c_str(),    Eldr04HcalDepth1TowerSumEtBc,    ( name+".Eldr04HcalDepth1TowerSumEtBc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eldr04HcalDepth2TowerSumEtBc" ).c_str(),    Eldr04HcalDepth2TowerSumEtBc,    ( name+".Eldr04HcalDepth2TowerSumEtBc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElhcalOverEcalBc" ).c_str(),                ElhcalOverEcalBc,                ( name+".ElhcalOverEcalBc["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElEcalE" ).c_str(),                         ElEcalE,                         ( name+".ElEcalE["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElEoverP" ).c_str(),                        ElEoverP,                        ( name+".ElEoverP["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EldeltaEta" ).c_str(),                      EldeltaEta,                      ( name+".EldeltaEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".EldeltaPhi" ).c_str(),                      EldeltaPhi,                      ( name+".EldeltaPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElHadoverEm" ).c_str(),                     ElHadoverEm,                     ( name+".ElHadoverEm["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElsigmaIetaIeta" ).c_str(),                 ElsigmaIetaIeta,                 ( name+".ElsigmaIetaIeta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElscSigmaIetaIeta" ).c_str(),               ElscSigmaIetaIeta,               ( name+".ElscSigmaIetaIeta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElEnergyErr" ).c_str(),                     ElEnergyErr,                     ( name+".ElEnergyErr["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElMomentumErr" ).c_str(),                   ElMomentumErr,                   ( name+".ElMomentumErr["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElSharedHitsFraction" ).c_str(),            ElSharedHitsFraction,            ( name+".ElSharedHitsFraction["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElhasConv" ).c_str(),                       ElhasConv,                       ( name+".ElhasConv["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".ElTrackNHits" ).c_str(),                    ElTrackNHits,                    ( name+".ElTrackNHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".ElTrackNLostHits" ).c_str(),                ElTrackNLostHits,                ( name+".ElTrackNLostHits["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackDz" ).c_str(),                       ElTrackDz,                       ( name+".ElTrackDz["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackDz_BS" ).c_str(),                    ElTrackDz_BS,                    ( name+".ElTrackDz_BS["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackD0" ).c_str(),                       ElTrackD0,                       ( name+".ElTrackD0["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackDxy_BS" ).c_str(),                   ElTrackDxy_BS,                   ( name+".ElTrackDxy_BS["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackDxy_PV" ).c_str(),                   ElTrackDxy_PV,                   ( name+".ElTrackDxy_PV["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElTrackDxy_PVBS" ).c_str(),                 ElTrackDxy_PVBS,                 ( name+".ElTrackDxy_PVBS["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElNClusters" ).c_str(),                     ElNClusters,                     ( name+".ElNClusters["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".ElClassification" ).c_str(),                ElClassification,                ( name+".ElClassification["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".ElFBrem" ).c_str(),                         ElFBrem,                         ( name+".ElFBrem["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NumberOfExpectedInnerHits" ).c_str(),       NumberOfExpectedInnerHits,       ( name+".NumberOfExpectedInnerHits["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Eldist" ).c_str(),                          Eldist,                          ( name+".Eldist["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eldcot" ).c_str(),                          Eldcot,                          ( name+".Eldcot["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Elconvradius" ).c_str(),                    Elconvradius,                    ( name+".Elconvradius["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".dcotdist" ).c_str(),                        dcotdist,                        ( name+".dcotdist["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElseedEoverP" ).c_str(),                    ElseedEoverP,                    ( name+".ElseedEoverP["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElEcalIso04" ).c_str(),                     ElEcalIso04,                     ( name+".ElEcalIso04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElHcalIso04" ).c_str(),                     ElHcalIso04,                     ( name+".ElHcalIso04["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".ElNumberOfBrems" ).c_str(),                 ElNumberOfBrems,                 ( name+".ElNumberOfBrems["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".isPFTau" ).c_str(),                         isPFTau,                         ( name+".isPFTau["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GenPt" ).c_str(),                           GenPt,                           ( name+".GenPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenEta" ).c_str(),                          GenEta,                          ( name+".GenEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPhi" ).c_str(),                          GenPhi,                          ( name+".GenPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPdgID" ).c_str(),                        GenPdgID,                        ( name+".GenPdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".GenMCTag" ).c_str(),                        GenMCTag,                        ( name+".GenMCTag["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".TempPt1" ).c_str(),                         TempPt1,                         ( name+".TempPt1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta1" ).c_str(),                        TempEta1,                        ( name+".TempEta1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi1" ).c_str(),                        TempPhi1,                        ( name+".TempPhi1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy1" ).c_str(),                     TempEnergy1,                     ( name+".TempEnergy1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPt2" ).c_str(),                         TempPt2,                         ( name+".TempPt2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta2" ).c_str(),                        TempEta2,                        ( name+".TempEta2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi2" ).c_str(),                        TempPhi2,                        ( name+".TempPhi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy2" ).c_str(),                     TempEnergy2,                     ( name+".TempEnergy2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat1" ).c_str(),                      TempFloat1,                      ( name+".TempFloat1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat2" ).c_str(),                      TempFloat2,                      ( name+".TempFloat2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat3" ).c_str(),                      TempFloat3,                      ( name+".TempFloat3["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat4" ).c_str(),                      TempFloat4,                      ( name+".TempFloat4["+name+".Size]/F" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "LepInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),                            &Size );
    root->SetBranchAddress( ( name+".Index" ).c_str(),                           Index );
    root->SetBranchAddress( ( name+".LeptonType" ).c_str(),                      LeptonType );
    root->SetBranchAddress( ( name+".Charge" ).c_str(),                          Charge );
    root->SetBranchAddress( ( name+".Pt" ).c_str(),                              Pt );
    root->SetBranchAddress( ( name+".Et" ).c_str(),                              Et );
    root->SetBranchAddress( ( name+".Eta" ).c_str(),                             Eta );
    root->SetBranchAddress( ( name+".Phi" ).c_str(),                             Phi );
    root->SetBranchAddress( ( name+".Px" ).c_str(),                              Px );
    root->SetBranchAddress( ( name+".Py" ).c_str(),                              Py );
    root->SetBranchAddress( ( name+".Pz" ).c_str(),                              Pz );
    root->SetBranchAddress( ( name+".Energy" ).c_str(),                          Energy );
    root->SetBranchAddress( ( name+".TrackIso" ).c_str(),                        TrackIso );
    root->SetBranchAddress( ( name+".EcalIso" ).c_str(),                         EcalIso );
    root->SetBranchAddress( ( name+".HcalIso" ).c_str(),                         HcalIso );
    root->SetBranchAddress( ( name+".ChargedHadronIso" ).c_str(),                ChargedHadronIso );
    root->SetBranchAddress( ( name+".NeutralHadronIso" ).c_str(),                NeutralHadronIso );
    root->SetBranchAddress( ( name+".PhotonIso" ).c_str(),                       PhotonIso );
    root->SetBranchAddress( ( name+".SumPUPt" ).c_str(),                         SumPUPt );
    root->SetBranchAddress( ( name+".ChargedHadronIsoR03" ).c_str(),             ChargedHadronIsoR03 );
    root->SetBranchAddress( ( name+".NeutralHadronIsoR03" ).c_str(),             NeutralHadronIsoR03 );
    root->SetBranchAddress( ( name+".PhotonIsoR03" ).c_str(),                    PhotonIsoR03 );
    root->SetBranchAddress( ( name+".sumPUPtR03" ).c_str(),                      sumPUPtR03 );
    root->SetBranchAddress( ( name+".IsoRhoCorrR03" ).c_str(),                   IsoRhoCorrR03 );
    root->SetBranchAddress( ( name+".ChargedHadronIsoR04" ).c_str(),             ChargedHadronIsoR04 );
    root->SetBranchAddress( ( name+".NeutralHadronIsoR04" ).c_str(),             NeutralHadronIsoR04 );
    root->SetBranchAddress( ( name+".PhotonIsoR04" ).c_str(),                    PhotonIsoR04 );
    root->SetBranchAddress( ( name+".sumPUPtR04" ).c_str(),                      sumPUPtR04 );
    root->SetBranchAddress( ( name+".IsoRhoCorrR04" ).c_str(),                   IsoRhoCorrR04 );
    root->SetBranchAddress( ( name+".Ip3dPV" ).c_str(),                          Ip3dPV );
    root->SetBranchAddress( ( name+".Ip3dPVErr" ).c_str(),                       Ip3dPVErr );
    root->SetBranchAddress( ( name+".Ip3dPVSignificance" ).c_str(),              Ip3dPVSignificance );
    root->SetBranchAddress( ( name+".MiniIso" ).c_str(),                         MiniIso );
    root->SetBranchAddress( ( name+".CaloEnergy" ).c_str(),                      CaloEnergy );
    root->SetBranchAddress( ( name+".isGoodMuonTMOneStationTight" ).c_str(),     isGoodMuonTMOneStationTight );
    root->SetBranchAddress( ( name+".isPFMuon" ).c_str(),                        isPFMuon );
    root->SetBranchAddress( ( name+".MuIDGlobalMuonPromptTight" ).c_str(),       MuIDGlobalMuonPromptTight );
    root->SetBranchAddress( ( name+".MuGlobalNormalizedChi2" ).c_str(),          MuGlobalNormalizedChi2 );
    root->SetBranchAddress( ( name+".MuCaloCompat" ).c_str(),                    MuCaloCompat );
    root->SetBranchAddress( ( name+".MuNChambers" ).c_str(),                     MuNChambers );
    root->SetBranchAddress( ( name+".MuNChambersMatchesSegment" ).c_str(),       MuNChambersMatchesSegment );
    root->SetBranchAddress( ( name+".MuNMatchedStations" ).c_str(),              MuNMatchedStations );
    root->SetBranchAddress( ( name+".MuNLostOuterHits" ).c_str(),                MuNLostOuterHits );
    root->SetBranchAddress( ( name+".MuNMuonhits" ).c_str(),                     MuNMuonhits );
    root->SetBranchAddress( ( name+".MuDThits" ).c_str(),                        MuDThits );
    root->SetBranchAddress( ( name+".MuCSChits" ).c_str(),                       MuCSChits );
    root->SetBranchAddress( ( name+".MuRPChits" ).c_str(),                       MuRPChits );
    root->SetBranchAddress( ( name+".MuType" ).c_str(),                          MuType );
    root->SetBranchAddress( ( name+".MuontimenDof" ).c_str(),                    MuontimenDof );
    root->SetBranchAddress( ( name+".MuontimeAtIpInOut" ).c_str(),               MuontimeAtIpInOut );
    root->SetBranchAddress( ( name+".MuontimeAtIpOutIn" ).c_str(),               MuontimeAtIpOutIn );
    root->SetBranchAddress( ( name+".Muondirection" ).c_str(),                   Muondirection );
    root->SetBranchAddress( ( name+".innerTracknormalizedChi2" ).c_str(),        innerTracknormalizedChi2 );
    root->SetBranchAddress( ( name+".MuInnerPtError" ).c_str(),                  MuInnerPtError );
    root->SetBranchAddress( ( name+".MuGlobalPtError" ).c_str(),                 MuGlobalPtError );
    root->SetBranchAddress( ( name+".MuInnerTrackDz" ).c_str(),                  MuInnerTrackDz );
    root->SetBranchAddress( ( name+".MuInnerTrackD0" ).c_str(),                  MuInnerTrackD0 );
    root->SetBranchAddress( ( name+".MuInnerTrackDxy_BS" ).c_str(),              MuInnerTrackDxy_BS );
    root->SetBranchAddress( ( name+".MuInnerTrackDxy_PV" ).c_str(),              MuInnerTrackDxy_PV );
    root->SetBranchAddress( ( name+".MuInnerTrackDxy_PVBS" ).c_str(),            MuInnerTrackDxy_PVBS );
    root->SetBranchAddress( ( name+".MuInnerTrackNHits" ).c_str(),               MuInnerTrackNHits );
    root->SetBranchAddress( ( name+".MuNTrackerHits" ).c_str(),                  MuNTrackerHits );
    root->SetBranchAddress( ( name+".MuNLostInnerHits" ).c_str(),                MuNLostInnerHits );
    root->SetBranchAddress( ( name+".vertexZ" ).c_str(),                         vertexZ );
    root->SetBranchAddress( ( name+".MuNPixelLayers" ).c_str(),                  MuNPixelLayers );
    root->SetBranchAddress( ( name+".MuNPixelLayersWMeasurement" ).c_str(),      MuNPixelLayersWMeasurement );
    root->SetBranchAddress( ( name+".MuNTrackLayersWMeasurement" ).c_str(),      MuNTrackLayersWMeasurement );
    root->SetBranchAddress( ( name+".ChargeGsf" ).c_str(),                       ChargeGsf );
    root->SetBranchAddress( ( name+".ChargeScPix" ).c_str(),                     ChargeScPix );
    root->SetBranchAddress( ( name+".isEcalDriven" ).c_str(),                    isEcalDriven );
    root->SetBranchAddress( ( name+".isTrackerDriven" ).c_str(),                 isTrackerDriven );
    root->SetBranchAddress( ( name+".caloEta" ).c_str(),                         caloEta );
    root->SetBranchAddress( ( name+".e1x5" ).c_str(),                            e1x5 );
    root->SetBranchAddress( ( name+".e2x5Max" ).c_str(),                         e2x5Max );
    root->SetBranchAddress( ( name+".e5x5" ).c_str(),                            e5x5 );
    root->SetBranchAddress( ( name+".HcalDepth1Iso" ).c_str(),                   HcalDepth1Iso );
    root->SetBranchAddress( ( name+".HcalDepth2Iso" ).c_str(),                   HcalDepth2Iso );
    root->SetBranchAddress( ( name+".EgammaMVANonTrig" ).c_str(),                EgammaMVANonTrig );
    root->SetBranchAddress( ( name+".EgammaMVATrig" ).c_str(),                   EgammaMVATrig );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdTRIGGERTIGHT" ).c_str(), EgammaCutBasedEleIdTRIGGERTIGHT );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdTRIGGERWP70" ).c_str(),  EgammaCutBasedEleIdTRIGGERWP70 );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdVETO" ).c_str(),         EgammaCutBasedEleIdVETO );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdLOOSE" ).c_str(),        EgammaCutBasedEleIdLOOSE );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdMEDIUM" ).c_str(),       EgammaCutBasedEleIdMEDIUM );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdTIGHT" ).c_str(),        EgammaCutBasedEleIdTIGHT );
    root->SetBranchAddress( ( name+".EgammaCutBasedEleIdHEEP" ).c_str(),         EgammaCutBasedEleIdHEEP );
    root->SetBranchAddress( ( name+".Eldr03HcalDepth1TowerSumEtBc" ).c_str(),    Eldr03HcalDepth1TowerSumEtBc );
    root->SetBranchAddress( ( name+".Eldr03HcalDepth2TowerSumEtBc" ).c_str(),    Eldr03HcalDepth2TowerSumEtBc );
    root->SetBranchAddress( ( name+".Eldr04HcalDepth1TowerSumEtBc" ).c_str(),    Eldr04HcalDepth1TowerSumEtBc );
    root->SetBranchAddress( ( name+".Eldr04HcalDepth2TowerSumEtBc" ).c_str(),    Eldr04HcalDepth2TowerSumEtBc );
    root->SetBranchAddress( ( name+".ElhcalOverEcalBc" ).c_str(),                ElhcalOverEcalBc );
    root->SetBranchAddress( ( name+".ElEcalE" ).c_str(),                         ElEcalE );
    root->SetBranchAddress( ( name+".ElEoverP" ).c_str(),                        ElEoverP );
    root->SetBranchAddress( ( name+".EldeltaEta" ).c_str(),                      EldeltaEta );
    root->SetBranchAddress( ( name+".EldeltaPhi" ).c_str(),                      EldeltaPhi );
    root->SetBranchAddress( ( name+".ElHadoverEm" ).c_str(),                     ElHadoverEm );
    root->SetBranchAddress( ( name+".ElsigmaIetaIeta" ).c_str(),                 ElsigmaIetaIeta );
    root->SetBranchAddress( ( name+".ElscSigmaIetaIeta" ).c_str(),               ElscSigmaIetaIeta );
    root->SetBranchAddress( ( name+".ElEnergyErr" ).c_str(),                     ElEnergyErr );
    root->SetBranchAddress( ( name+".ElMomentumErr" ).c_str(),                   ElMomentumErr );
    root->SetBranchAddress( ( name+".ElSharedHitsFraction" ).c_str(),            ElSharedHitsFraction );
    root->SetBranchAddress( ( name+".ElhasConv" ).c_str(),                       ElhasConv );
    root->SetBranchAddress( ( name+".ElTrackNHits" ).c_str(),                    ElTrackNHits );
    root->SetBranchAddress( ( name+".ElTrackNLostHits" ).c_str(),                ElTrackNLostHits );
    root->SetBranchAddress( ( name+".ElTrackDz" ).c_str(),                       ElTrackDz );
    root->SetBranchAddress( ( name+".ElTrackDz_BS" ).c_str(),                    ElTrackDz_BS );
    root->SetBranchAddress( ( name+".ElTrackD0" ).c_str(),                       ElTrackD0 );
    root->SetBranchAddress( ( name+".ElTrackDxy_BS" ).c_str(),                   ElTrackDxy_BS );
    root->SetBranchAddress( ( name+".ElTrackDxy_PV" ).c_str(),                   ElTrackDxy_PV );
    root->SetBranchAddress( ( name+".ElTrackDxy_PVBS" ).c_str(),                 ElTrackDxy_PVBS );
    root->SetBranchAddress( ( name+".ElNClusters" ).c_str(),                     ElNClusters );
    root->SetBranchAddress( ( name+".ElClassification" ).c_str(),                ElClassification );
    root->SetBranchAddress( ( name+".ElFBrem" ).c_str(),                         ElFBrem );
    root->SetBranchAddress( ( name+".NumberOfExpectedInnerHits" ).c_str(),       NumberOfExpectedInnerHits );
    root->SetBranchAddress( ( name+".Eldist" ).c_str(),                          Eldist );
    root->SetBranchAddress( ( name+".Eldcot" ).c_str(),                          Eldcot );
    root->SetBranchAddress( ( name+".Elconvradius" ).c_str(),                    Elconvradius );
    root->SetBranchAddress( ( name+".dcotdist" ).c_str(),                        dcotdist );
    root->SetBranchAddress( ( name+".ElseedEoverP" ).c_str(),                    ElseedEoverP );
    root->SetBranchAddress( ( name+".ElEcalIso04" ).c_str(),                     ElEcalIso04 );
    root->SetBranchAddress( ( name+".ElHcalIso04" ).c_str(),                     ElHcalIso04 );
    root->SetBranchAddress( ( name+".ElNumberOfBrems" ).c_str(),                 ElNumberOfBrems );
    root->SetBranchAddress( ( name+".isPFTau" ).c_str(),                         isPFTau );
    root->SetBranchAddress( ( name+".GenPt" ).c_str(),                           GenPt );
    root->SetBranchAddress( ( name+".GenEta" ).c_str(),                          GenEta );
    root->SetBranchAddress( ( name+".GenPhi" ).c_str(),                          GenPhi );
    root->SetBranchAddress( ( name+".GenPdgID" ).c_str(),                        GenPdgID );
    root->SetBranchAddress( ( name+".GenMCTag" ).c_str(),                        GenMCTag );
    root->SetBranchAddress( ( name+".TempPt1" ).c_str(),                         TempPt1 );
    root->SetBranchAddress( ( name+".TempEta1" ).c_str(),                        TempEta1 );
    root->SetBranchAddress( ( name+".TempPhi1" ).c_str(),                        TempPhi1 );
    root->SetBranchAddress( ( name+".TempEnergy1" ).c_str(),                     TempEnergy1 );
    root->SetBranchAddress( ( name+".TempPt2" ).c_str(),                         TempPt2 );
    root->SetBranchAddress( ( name+".TempEta2" ).c_str(),                        TempEta2 );
    root->SetBranchAddress( ( name+".TempPhi2" ).c_str(),                        TempPhi2 );
    root->SetBranchAddress( ( name+".TempEnergy2" ).c_str(),                     TempEnergy2 );
    root->SetBranchAddress( ( name+".TempFloat1" ).c_str(),                      TempFloat1 );
    root->SetBranchAddress( ( name+".TempFloat2" ).c_str(),                      TempFloat2 );
    root->SetBranchAddress( ( name+".TempFloat3" ).c_str(),                      TempFloat3 );
    root->SetBranchAddress( ( name+".TempFloat4" ).c_str(),                      TempFloat4 );
  }
};

class PhotonInfoBranches
{
public:
  Int_t Size;
  Float_t Pt [MAX_PHOTONS];
  Float_t Eta [MAX_PHOTONS];
  Float_t Phi [MAX_PHOTONS];
  Float_t HoverE [MAX_PHOTONS];
  Float_t SigmaIetaIeta [MAX_PHOTONS];
  Float_t hadTowOverEm [MAX_PHOTONS];
  Float_t hcalIsoConeDR04_2012 [MAX_PHOTONS];
  Float_t phoPFChIso [MAX_PHOTONS];
  Float_t phoPFNeuIso [MAX_PHOTONS];
  Float_t phoPFPhoIso [MAX_PHOTONS];
  Float_t sigmaIetaIeta [MAX_PHOTONS];
  Float_t isoChEffArea [MAX_PHOTONS];
  Float_t isoNeuEffArea [MAX_PHOTONS];
  Float_t isoPhoEffArea [MAX_PHOTONS];
  Bool_t phoPassLoose [MAX_PHOTONS];
  Bool_t phoPassMedium [MAX_PHOTONS];
  Bool_t phoPassTight [MAX_PHOTONS];
  Float_t r9 [MAX_PHOTONS];
  Bool_t passelectronveto [MAX_PHOTONS];
  Bool_t hasPixelSeed [MAX_PHOTONS];
  Float_t EcalIso [MAX_PHOTONS];
  Float_t HcalIso [MAX_PHOTONS];
  Float_t TrackIso [MAX_PHOTONS];
  Float_t GenPt [MAX_PHOTONS];
  Float_t GenEta [MAX_PHOTONS];
  Float_t GenPhi [MAX_PHOTONS];
  Int_t GenPdgID [MAX_PHOTONS];
  Float_t TempPt1 [MAX_PHOTONS];
  Float_t TempEta1 [MAX_PHOTONS];
  Float_t TempPhi1 [MAX_PHOTONS];
  Float_t TempEnergy1 [MAX_PHOTONS];
  Float_t TempPt2 [MAX_PHOTONS];
  Float_t TempEta2 [MAX_PHOTONS];
  Float_t TempPhi2 [MAX_PHOTONS];
  Float_t TempEnergy2 [MAX_PHOTONS];
  Float_t TempFloat1 [MAX_PHOTONS];
  Float_t TempFloat2 [MAX_PHOTONS];
  Float_t TempFloat3 [MAX_PHOTONS];
  Float_t TempFloat4 [MAX_PHOTONS];

  void
  RegisterTree( TTree* root, const std::string& name = "PhotonInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),                 &Size,                ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".Pt" ).c_str(),                   Pt,                   ( name+".Pt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta" ).c_str(),                  Eta,                  ( name+".Eta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi" ).c_str(),                  Phi,                  ( name+".Phi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".HoverE" ).c_str(),               HoverE,               ( name+".HoverE["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".SigmaIetaIeta" ).c_str(),        SigmaIetaIeta,        ( name+".SigmaIetaIeta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".hadTowOverEm" ).c_str(),         hadTowOverEm,         ( name+".hadTowOverEm["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".hcalIsoConeDR04_2012" ).c_str(), hcalIsoConeDR04_2012, ( name+".hcalIsoConeDR04_2012["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".phoPFChIso" ).c_str(),           phoPFChIso,           ( name+".phoPFChIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".phoPFNeuIso" ).c_str(),          phoPFNeuIso,          ( name+".phoPFNeuIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".phoPFPhoIso" ).c_str(),          phoPFPhoIso,          ( name+".phoPFPhoIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".sigmaIetaIeta" ).c_str(),        sigmaIetaIeta,        ( name+".sigmaIetaIeta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".isoChEffArea" ).c_str(),         isoChEffArea,         ( name+".isoChEffArea["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".isoNeuEffArea" ).c_str(),        isoNeuEffArea,        ( name+".isoNeuEffArea["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".isoPhoEffArea" ).c_str(),        isoPhoEffArea,        ( name+".isoPhoEffArea["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".phoPassLoose" ).c_str(),         phoPassLoose,         ( name+".phoPassLoose["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".phoPassMedium" ).c_str(),        phoPassMedium,        ( name+".phoPassMedium["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".phoPassTight" ).c_str(),         phoPassTight,         ( name+".phoPassTight["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".r9" ).c_str(),                   r9,                   ( name+".r9["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".passelectronveto" ).c_str(),     passelectronveto,     ( name+".passelectronveto["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".hasPixelSeed" ).c_str(),         hasPixelSeed,         ( name+".hasPixelSeed["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".EcalIso" ).c_str(),              EcalIso,              ( name+".EcalIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".HcalIso" ).c_str(),              HcalIso,              ( name+".HcalIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TrackIso" ).c_str(),             TrackIso,             ( name+".TrackIso["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPt" ).c_str(),                GenPt,                ( name+".GenPt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenEta" ).c_str(),               GenEta,               ( name+".GenEta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPhi" ).c_str(),               GenPhi,               ( name+".GenPhi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".GenPdgID" ).c_str(),             GenPdgID,             ( name+".GenPdgID["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".TempPt1" ).c_str(),              TempPt1,              ( name+".TempPt1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta1" ).c_str(),             TempEta1,             ( name+".TempEta1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi1" ).c_str(),             TempPhi1,             ( name+".TempPhi1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy1" ).c_str(),          TempEnergy1,          ( name+".TempEnergy1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPt2" ).c_str(),              TempPt2,              ( name+".TempPt2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEta2" ).c_str(),             TempEta2,             ( name+".TempEta2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempPhi2" ).c_str(),             TempPhi2,             ( name+".TempPhi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempEnergy2" ).c_str(),          TempEnergy2,          ( name+".TempEnergy2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat1" ).c_str(),           TempFloat1,           ( name+".TempFloat1["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat2" ).c_str(),           TempFloat2,           ( name+".TempFloat2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat3" ).c_str(),           TempFloat3,           ( name+".TempFloat3["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TempFloat4" ).c_str(),           TempFloat4,           ( name+".TempFloat4["+name+".Size]/F" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "PhotonInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),                 &Size );
    root->SetBranchAddress( ( name+".Pt" ).c_str(),                   Pt );
    root->SetBranchAddress( ( name+".Eta" ).c_str(),                  Eta );
    root->SetBranchAddress( ( name+".Phi" ).c_str(),                  Phi );
    root->SetBranchAddress( ( name+".HoverE" ).c_str(),               HoverE );
    root->SetBranchAddress( ( name+".SigmaIetaIeta" ).c_str(),        SigmaIetaIeta );
    root->SetBranchAddress( ( name+".hadTowOverEm" ).c_str(),         hadTowOverEm );
    root->SetBranchAddress( ( name+".hcalIsoConeDR04_2012" ).c_str(), hcalIsoConeDR04_2012 );
    root->SetBranchAddress( ( name+".phoPFChIso" ).c_str(),           phoPFChIso );
    root->SetBranchAddress( ( name+".phoPFNeuIso" ).c_str(),          phoPFNeuIso );
    root->SetBranchAddress( ( name+".phoPFPhoIso" ).c_str(),          phoPFPhoIso );
    root->SetBranchAddress( ( name+".sigmaIetaIeta" ).c_str(),        sigmaIetaIeta );
    root->SetBranchAddress( ( name+".isoChEffArea" ).c_str(),         isoChEffArea );
    root->SetBranchAddress( ( name+".isoNeuEffArea" ).c_str(),        isoNeuEffArea );
    root->SetBranchAddress( ( name+".isoPhoEffArea" ).c_str(),        isoPhoEffArea );
    root->SetBranchAddress( ( name+".phoPassLoose" ).c_str(),         phoPassLoose );
    root->SetBranchAddress( ( name+".phoPassMedium" ).c_str(),        phoPassMedium );
    root->SetBranchAddress( ( name+".phoPassTight" ).c_str(),         phoPassTight );
    root->SetBranchAddress( ( name+".r9" ).c_str(),                   r9 );
    root->SetBranchAddress( ( name+".passelectronveto" ).c_str(),     passelectronveto );
    root->SetBranchAddress( ( name+".hasPixelSeed" ).c_str(),         hasPixelSeed );
    root->SetBranchAddress( ( name+".EcalIso" ).c_str(),              EcalIso );
    root->SetBranchAddress( ( name+".HcalIso" ).c_str(),              HcalIso );
    root->SetBranchAddress( ( name+".TrackIso" ).c_str(),             TrackIso );
    root->SetBranchAddress( ( name+".GenPt" ).c_str(),                GenPt );
    root->SetBranchAddress( ( name+".GenEta" ).c_str(),               GenEta );
    root->SetBranchAddress( ( name+".GenPhi" ).c_str(),               GenPhi );
    root->SetBranchAddress( ( name+".GenPdgID" ).c_str(),             GenPdgID );
    root->SetBranchAddress( ( name+".TempPt1" ).c_str(),              TempPt1 );
    root->SetBranchAddress( ( name+".TempEta1" ).c_str(),             TempEta1 );
    root->SetBranchAddress( ( name+".TempPhi1" ).c_str(),             TempPhi1 );
    root->SetBranchAddress( ( name+".TempEnergy1" ).c_str(),          TempEnergy1 );
    root->SetBranchAddress( ( name+".TempPt2" ).c_str(),              TempPt2 );
    root->SetBranchAddress( ( name+".TempEta2" ).c_str(),             TempEta2 );
    root->SetBranchAddress( ( name+".TempPhi2" ).c_str(),             TempPhi2 );
    root->SetBranchAddress( ( name+".TempEnergy2" ).c_str(),          TempEnergy2 );
    root->SetBranchAddress( ( name+".TempFloat1" ).c_str(),           TempFloat1 );
    root->SetBranchAddress( ( name+".TempFloat2" ).c_str(),           TempFloat2 );
    root->SetBranchAddress( ( name+".TempFloat3" ).c_str(),           TempFloat3 );
    root->SetBranchAddress( ( name+".TempFloat4" ).c_str(),           TempFloat4 );
  }
};

class TrgInfoBranches
{
public:
  Int_t Size;
  Int_t TriggerBit [MAX_TRGOBJS];
  Int_t FilterLabel [MAX_TRGOBJS];
  Float_t Pt [MAX_TRGOBJS];
  Float_t Eta [MAX_TRGOBJS];
  Float_t Phi [MAX_TRGOBJS];
  Float_t Energy [MAX_TRGOBJS];
  Int_t TrgCount;
  Int_t nTrgBook;
  Char_t TrgBook [N_TRIGGER_BOOKINGS];
  Int_t nHLT;
  Float_t HighPurityFraction;
  Int_t NofTracks;
  Int_t HLTPrescaleFactor [512];
  Int_t HLTName2enum [512];
  Bool_t HLTbits [N_TRIGGER_BOOKINGS];
  Int_t L1 [128];
  Int_t TT [64];

  void
  RegisterTree( TTree* root, const std::string& name = "TrgInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),               &Size,               ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".TriggerBit" ).c_str(),         TriggerBit,          ( name+".TriggerBit["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".FilterLabel" ).c_str(),        FilterLabel,         ( name+".FilterLabel["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Pt" ).c_str(),                 Pt,                  ( name+".Pt["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Eta" ).c_str(),                Eta,                 ( name+".Eta["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Phi" ).c_str(),                Phi,                 ( name+".Phi["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Energy" ).c_str(),             Energy,              ( name+".Energy["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".TrgCount" ).c_str(),           &TrgCount,           ( name+"TrgCount/I" ).c_str() );
    root->Branch( ( name+".nTrgBook" ).c_str(),           &nTrgBook,           ( name+"nTrgBook/I" ).c_str() );
    root->Branch( ( name+".TrgBook" ).c_str(),            TrgBook,             ( name+".TrgBook["+name+".nTrgBook]/C" ).c_str() );
    root->Branch( ( name+".nHLT" ).c_str(),               &nHLT,               ( name+"nHLT/I" ).c_str() );
    root->Branch( ( name+".HighPurityFraction" ).c_str(), &HighPurityFraction, ( name+"HighPurityFraction/F" ).c_str() );
    root->Branch( ( name+".NofTracks" ).c_str(),          &NofTracks,          ( name+"NofTracks/I" ).c_str() );
    root->Branch( ( name+".HLTPrescaleFactor" ).c_str(),  HLTPrescaleFactor,   ( name+".HLTPrescaleFactor["+name+".nHLT]/I" ).c_str() );
    root->Branch( ( name+".HLTName2enum" ).c_str(),       HLTName2enum,        ( name+".HLTName2enum["+name+".nHLT]/I" ).c_str() );
    root->Branch( ( name+".HLTbits" ).c_str(),            HLTbits,             ( name+".HLTbits["+name+".nHLT]/O" ).c_str() );
    root->Branch( ( name+".L1" ).c_str(),                 L1,                  ( name+".L1[128]/I" ).c_str() );
    root->Branch( ( name+".TT" ).c_str(),                 TT,                  ( name+".TT[64]/I" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "TrgInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),               &Size );
    root->SetBranchAddress( ( name+".TriggerBit" ).c_str(),         TriggerBit );
    root->SetBranchAddress( ( name+".FilterLabel" ).c_str(),        FilterLabel );
    root->SetBranchAddress( ( name+".Pt" ).c_str(),                 Pt );
    root->SetBranchAddress( ( name+".Eta" ).c_str(),                Eta );
    root->SetBranchAddress( ( name+".Phi" ).c_str(),                Phi );
    root->SetBranchAddress( ( name+".Energy" ).c_str(),             Energy );
    root->SetBranchAddress( ( name+".TrgCount" ).c_str(),           &TrgCount );
    root->SetBranchAddress( ( name+".nTrgBook" ).c_str(),           &nTrgBook );
    root->SetBranchAddress( ( name+".TrgBook" ).c_str(),            TrgBook );
    root->SetBranchAddress( ( name+".nHLT" ).c_str(),               &nHLT );
    root->SetBranchAddress( ( name+".HighPurityFraction" ).c_str(), &HighPurityFraction );
    root->SetBranchAddress( ( name+".NofTracks" ).c_str(),          &NofTracks );
    root->SetBranchAddress( ( name+".HLTPrescaleFactor" ).c_str(),  HLTPrescaleFactor );
    root->SetBranchAddress( ( name+".HLTName2enum" ).c_str(),       HLTName2enum );
    root->SetBranchAddress( ( name+".HLTbits" ).c_str(),            HLTbits );
    root->SetBranchAddress( ( name+".L1" ).c_str(),                 L1 );
    root->SetBranchAddress( ( name+".TT" ).c_str(),                 TT );
  }
};

class VertexInfoBranches
{
public:
  Int_t Size;
  Int_t isValid [MAX_Vertices];
  Bool_t isFake [MAX_Vertices];
  Int_t Type [MAX_Vertices];
  Float_t Ndof [MAX_Vertices];
  Float_t NormalizedChi2 [MAX_Vertices];
  Float_t Pt_Sum [MAX_Vertices];
  Float_t Pt_Sum2 [MAX_Vertices];
  Float_t x [MAX_Vertices];
  Float_t y [MAX_Vertices];
  Float_t z [MAX_Vertices];
  Float_t Rho [MAX_Vertices];

  void
  RegisterTree( TTree* root, const std::string& name = "VertexInfo" )
  {
    root->Branch( ( name+".Size" ).c_str(),           &Size,          ( name+"Size/I" ).c_str() );
    root->Branch( ( name+".isValid" ).c_str(),        isValid,        ( name+".isValid["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".isFake" ).c_str(),         isFake,         ( name+".isFake["+name+".Size]/O" ).c_str() );
    root->Branch( ( name+".Type" ).c_str(),           Type,           ( name+".Type["+name+".Size]/I" ).c_str() );
    root->Branch( ( name+".Ndof" ).c_str(),           Ndof,           ( name+".Ndof["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".NormalizedChi2" ).c_str(), NormalizedChi2, ( name+".NormalizedChi2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Pt_Sum" ).c_str(),         Pt_Sum,         ( name+".Pt_Sum["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Pt_Sum2" ).c_str(),        Pt_Sum2,        ( name+".Pt_Sum2["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".x" ).c_str(),              x,              ( name+".x["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".y" ).c_str(),              y,              ( name+".y["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".z" ).c_str(),              z,              ( name+".z["+name+".Size]/F" ).c_str() );
    root->Branch( ( name+".Rho" ).c_str(),            Rho,            ( name+".Rho["+name+".Size]/F" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "VertexInfo" )
  {
    root->SetBranchAddress( ( name+".Size" ).c_str(),           &Size );
    root->SetBranchAddress( ( name+".isValid" ).c_str(),        isValid );
    root->SetBranchAddress( ( name+".isFake" ).c_str(),         isFake );
    root->SetBranchAddress( ( name+".Type" ).c_str(),           Type );
    root->SetBranchAddress( ( name+".Ndof" ).c_str(),           Ndof );
    root->SetBranchAddress( ( name+".NormalizedChi2" ).c_str(), NormalizedChi2 );
    root->SetBranchAddress( ( name+".Pt_Sum" ).c_str(),         Pt_Sum );
    root->SetBranchAddress( ( name+".Pt_Sum2" ).c_str(),        Pt_Sum2 );
    root->SetBranchAddress( ( name+".x" ).c_str(),              x );
    root->SetBranchAddress( ( name+".y" ).c_str(),              y );
    root->SetBranchAddress( ( name+".z" ).c_str(),              z );
    root->SetBranchAddress( ( name+".Rho" ).c_str(),            Rho );
  }
};

class RunInfoBranches
{
public:
  Int_t PdfID;

  void
  RegisterTree( TTree* root, const std::string& name = "RunInfo" )
  {
    root->Branch( ( name+".PdfID" ).c_str(), &PdfID, ( name+"PdfID/I" ).c_str() );
  }

  void
  Register( TTree* root, const std::string& name = "RunInfo" )
  {
    root->SetBranchAddress( ( name+".PdfID" ).c_str(), &PdfID );
  }
};


#endif// __BPRIMEKIT_FORMAT_H__
