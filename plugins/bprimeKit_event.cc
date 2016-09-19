/*******************************************************************************
*
*  Filename    : bprimeKit_event.cc
*  Description : Filling in event information
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include <iostream>
using namespace std;

// ------------------------------------------------------------------------------
//   bprimeKit method implementation
// ------------------------------------------------------------------------------
bool
bprimeKit::FillEvent( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   cout << "Filling Event ID information " << endl;
   fEvtInfo.RunNo    = iEvent.id().run();
   fEvtInfo.EvtNo    = iEvent.id().event();
   fEvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
   fEvtInfo.BxNo     = iEvent.bunchCrossing();
   fEvtInfo.LumiNo   = iEvent.luminosityBlock();
   fEvtInfo.Orbit    = iEvent.orbitNumber();
   fEvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
   fEvtInfo.Rho      = *fRho_H;

   // ----- Pile up information  -----------------------------------------------------------------------
   cout << "Filling Pileup information" << endl;
   if( fPileup_H.isValid() ){ // Need to shutdown for Data
      for( auto PVI = fPileup_H->begin(); PVI != fPileup_H->end(); ++PVI ){
         fEvtInfo.nPU[fEvtInfo.nBX]    = PVI->getPU_NumInteractions();
         fEvtInfo.BXPU[fEvtInfo.nBX]   = PVI->getBunchCrossing();
         fEvtInfo.TrueIT[fEvtInfo.nBX] = PVI->getTrueNumInteractions();
         ++fEvtInfo.nBX;
      }
   }

   cout << "Filling beamspot information" << endl;
   const auto& fBeamSpot = *( fBeamSpot_H.product() );
   fEvtInfo.BeamSpotX    = fBeamSpot.position().x();
   fEvtInfo.BeamSpotY    = fBeamSpot.position().y();
   fEvtInfo.BeamSpotZ    = fBeamSpot.position().z();

   // ----- Getting Monte Carlo mode  ------------------------------------------------------------------
   // See code in bprimeKit_genInfo.cc
   cout << "Filling MC mode information information " << endl;
   int mclep_count[2] = {0, 0};
   if( fEvtInfo.McWMode[0] == 1 || fEvtInfo.McWMode[0] == 2 ){ mclep_count[1]++; }
   if( fEvtInfo.McWMode[1] == 1 || fEvtInfo.McWMode[1] == 2 ){ mclep_count[0]++; }
   if( fEvtInfo.McWMode[2] == 1 || fEvtInfo.McWMode[2] == 2 ){ mclep_count[0]++; }
   if( fEvtInfo.McWMode[3] == 1 || fEvtInfo.McWMode[3] == 2 ){ mclep_count[1]++; }
   if( fEvtInfo.McZMode[0] == 1 || fEvtInfo.McZMode[0] == 2 ){ mclep_count[0]++; mclep_count[1]++; }
   if( fEvtInfo.McZMode[1] == 1 || fEvtInfo.McZMode[1] == 2 ){ mclep_count[0]++; mclep_count[1]++; }

   // ------  McSigTag - 0:others, 1:opposite-sing dilepton, 2:same-sign dilepton  ------
   // ---------------------------  3: trilepton, 4: 4-lepton  ---------------------------
   if( mclep_count[0] == 1 && mclep_count[1] == 1 ){ fEvtInfo.McSigTag = 1; }
   if( mclep_count[0] == 2 && mclep_count[1] == 0 ){ fEvtInfo.McSigTag = 2; }
   if( mclep_count[0] == 0 && mclep_count[1] == 2 ){ fEvtInfo.McSigTag = 2; }
   if( mclep_count[0] == 2 && mclep_count[1] == 1 ){ fEvtInfo.McSigTag = 3; }
   if( mclep_count[0] == 1 && mclep_count[1] == 2 ){ fEvtInfo.McSigTag = 3; }
   if( mclep_count[0] == 2 && mclep_count[1] == 2 ){ fEvtInfo.McSigTag = 4; }

   // ----- Getting missing momentum information  --------------------------------
   cout << "Filling MET information " << endl;

   for( auto it_met = fMET_H->begin(); it_met != fMET_H->end(); it_met++ ){
      fEvtInfo.PFMET             = it_met->pt();
      fEvtInfo.PFMETPhi          = it_met->phi();
      fEvtInfo.PFRawMET          = it_met->uncorPt();
      fEvtInfo.PFRawMETPhi       = it_met->uncorPhi();
      fEvtInfo.PFMETx            = it_met->px();         // Uly 2011-04-04
      fEvtInfo.PFMETy            = it_met->py();         // Uly 2011-04-04
      fEvtInfo.PFSumEt           = it_met->sumEt();
      fEvtInfo.PFMETSig          = it_met->mEtSig();     // MET Significance = MET / std::sqrt(SumET)
      fEvtInfo.PFMETRealSig      = it_met->significance();     // real MET significance
      fEvtInfo.PFMETlongitudinal = it_met->e_longitudinal();// longitudinal component of the vector sum of energy over all object
      const reco::GenMET* genmet = it_met->genMET();
      if( !fSkipfGenInfo && genmet != NULL ){
         fEvtInfo.PFGenMET    = genmet->pt();
         fEvtInfo.PFGenMETPhi = genmet->phi();
      }
   }

   for( auto it_met = fPuppiMET_H->begin(); it_met != fPuppiMET_H->end(); it_met++ ){
      fEvtInfo.PuppiMET             = it_met->pt();
      fEvtInfo.PuppiMETPhi          = it_met->phi();
      fEvtInfo.PuppiRawMET          = it_met->uncorPt();
      fEvtInfo.PuppiRawMETPhi       = it_met->uncorPhi();
      fEvtInfo.PuppiSumEt           = it_met->sumEt();
      fEvtInfo.PuppiMETSig          = it_met->mEtSig();     // MET Significance = MET / std::sqrt(SumET)
      fEvtInfo.PuppiMETRealSig      = it_met->significance();     // real MET significance
      fEvtInfo.PuppiMETlongitudinal = it_met->e_longitudinal();// longitudinal component of the vector sum of energy over all object
      const reco::GenMET* genmet = it_met->genMET();
      if( !fSkipfGenInfo && genmet != NULL ){
         fEvtInfo.PuppiGenMET    = genmet->pt();
         fEvtInfo.PuppiGenMETPhi = genmet->phi();
      }
   }

   // ----- Missing momentum correction  ---------------------------------------
   if( fMETTempPlus_H.isValid() ){
      fEvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp = fMETTempPlus_H->begin()->pt();
   }
   if( fMETTempDown_H.isValid() ){
      fEvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown = fMETTempDown_H->begin()->pt();
   }

   // ----- Generation information  --------------------------------------------
   cout << "Filling PDF information information " << endl;
   if( fGenEvent_H.isValid() && fGenEvent_H->hasPDF() ){
      fEvtInfo.PDFid1   = fGenEvent_H->pdf()->id.first;
      fEvtInfo.PDFid2   = fGenEvent_H->pdf()->id.second;
      fEvtInfo.PDFx1    = fGenEvent_H->pdf()->x.first;
      fEvtInfo.PDFx2    = fGenEvent_H->pdf()->x.second;
      fEvtInfo.PDFscale = fGenEvent_H->pdf()->scalePDF;
      fEvtInfo.PDFv1    = fGenEvent_H->pdf()->xPDF.first;
      fEvtInfo.PDFv2    = fGenEvent_H->pdf()->xPDF.second;
   }

   // ----- Level 1 trigger and technical trigger bits  ------------------------
   cout << "Filling L1 Trigger information " << endl;
   if( fRecord_H.isValid() ){
      DecisionWord dWord = fRecord_H->decisionWord();
      if( !dWord.empty() ){// if board not there this is zero
         // loop over dec. bit to get total rate (no overlap)
         for( int i = 0; i < 128; ++i ){
            fEvtInfo.L1[i] = dWord[i];
         }
      }
      TechnicalTriggerWord tw = fRecord_H->technicalTriggerWord();
      if( !tw.empty() ){
         // loop over dec. bit to get total rate (no overlap)
         for( int i = 0; i < 64; ++i ){
            fEvtInfo.TT[i] = tw[i];
         }
      }
   }

   return true;
}
