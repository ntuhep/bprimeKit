/*******************************************************************************
 *
 *  Filename    : bprimeKit_event.cc
 *  Description : Filling in event information
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
using namespace std;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 
bool bprimeKit::FillEvent( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( fDebug > 1 ) { cout << "\t[1] Entering EventInfo subroutine" << endl ;}
   //-------------------------------------------------------------------------------------------------- 
   //   Helper variables definition
   //-------------------------------------------------------------------------------------------------- 
   std::pair<int,int>  psValueCombo;

   
   if( fDebug > 1 ){ cout <<"\t[1] Inserting generic Event information"<< endl;}
   fEvtInfo.RunNo    = iEvent.id().run();
   fEvtInfo.EvtNo    = iEvent.id().event();
   fEvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
   fEvtInfo.BxNo     = iEvent.bunchCrossing();
   fEvtInfo.LumiNo   = iEvent.luminosityBlock();
   fEvtInfo.Orbit    = iEvent.orbitNumber();
   fEvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
   fEvtInfo.Rho      = *(fRho_H.product()) ; 
   
   if( fDebug > 1 ){ cout <<"\t[1] Inserting Pile up information information"<< endl;}
   //----- Pile up information  -----------------------------------------------------------------------
   if( fPileup_H.isValid() ) {
      for( auto PVI = fPileup_H->begin(); PVI != fPileup_H->end(); ++PVI ) {
         fEvtInfo.nPU[fEvtInfo.nBX] = PVI->getPU_NumInteractions();
         fEvtInfo.BXPU[fEvtInfo.nBX] = PVI->getBunchCrossing();
         fEvtInfo.TrueIT[fEvtInfo.nBX] = PVI->getTrueNumInteractions();
         ++fEvtInfo.nBX ;
      }
   }
   
   //----- Getting beamspot information  --------------------------------------------------------------
   if( fDebug > 1 ) { cout << "\t[1] Get beam spot."<< endl; }
   if ( fBeamSpot_H.isValid() ) {
      const auto& fBeamSpot = *(fBeamSpot_H.product());
      fEvtInfo.BeamSpotX = fBeamSpot.position().x();
      fEvtInfo.BeamSpotY = fBeamSpot.position().y();
      fEvtInfo.BeamSpotZ = fBeamSpot.position().z();
   } else {
      edm::LogInfo( "bpkFrameWorklyzer" )
            << "No beam spot available from EventSetup \n";
   }

   //----- Getting Monte Carlo mode  ------------------------------------------------------------------
   // See code in bprimeKit_genInfo.cc
   int mclep_count[2] = {0, 0};
   if ( fEvtInfo.McWMode[0] == 1 || fEvtInfo.McWMode[0] == 2 ) { mclep_count[1]++; }
   if ( fEvtInfo.McWMode[1] == 1 || fEvtInfo.McWMode[1] == 2 ) { mclep_count[0]++; }
   if ( fEvtInfo.McWMode[2] == 1 || fEvtInfo.McWMode[2] == 2 ) { mclep_count[0]++; }
   if ( fEvtInfo.McWMode[3] == 1 || fEvtInfo.McWMode[3] == 2 ) { mclep_count[1]++; }
   if ( fEvtInfo.McZMode[0] == 1 || fEvtInfo.McZMode[0] == 2 ) {
      mclep_count[0]++; mclep_count[1]++;}
   if ( fEvtInfo.McZMode[1] == 1 || fEvtInfo.McZMode[1] == 2 ) {
      mclep_count[0]++; mclep_count[1]++; }

   //------  McSigTag - 0:others, 1:opposite-sing dilepton, 2:same-sign dilepton  ------
   //---------------------------  3: trilepton, 4: 4-lepton  ---------------------------
   if ( mclep_count[0] == 1 && mclep_count[1] == 1 ) { fEvtInfo.McSigTag = 1; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 0 ) { fEvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 0 && mclep_count[1] == 2 ) { fEvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 1 ) { fEvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 1 && mclep_count[1] == 2 ) { fEvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 2 ) { fEvtInfo.McSigTag = 4; }

   //----- Getting missing momentum information  ------------------------------------------------------
   if( fDebug > 1 ) { cout << "\t[1]Evt: Get missing momentum."<< endl; }
   for( auto it_met = fMET_H->begin(); it_met != fMET_H->end(); it_met++ ) {
      fEvtInfo.PFMET              = it_met->pt()             ;
      fEvtInfo.PFMETPhi           = it_met->phi()            ;
      fEvtInfo.PFRawMET           = it_met->uncorPt()  ;
      fEvtInfo.PFRawMETPhi        = it_met->uncorPhi() ;
      fEvtInfo.PFMETx             = it_met->px()             ; //Uly 2011-04-04
      fEvtInfo.PFMETy             = it_met->py()             ; //Uly 2011-04-04
      fEvtInfo.PFSumEt            = it_met->sumEt()          ;
      fEvtInfo.PFMETSig           = it_met->mEtSig()         ; //MET Significance = MET / std::sqrt(SumET)
      fEvtInfo.PFMETRealSig       = it_met->significance()   ; //real MET significance
      fEvtInfo.PFMETlongitudinal  = it_met->e_longitudinal() ; //longitudinal component of the vector sum of energy over all object
      const reco::GenMET* genmet = it_met->genMET()         ;
      if ( !fSkipfGenInfo && genmet != NULL ) {
         fEvtInfo.PFGenMET        = genmet->pt();
         fEvtInfo.PFGenMETPhi     = genmet->phi();
      }
   }
   //----- Missing momentum correction  ---------------------------------------------------------------
   if( fDebug > 1 ) { cout << "\t[1] Get missing momentum corrections."<< endl; }
   if( fMETTempPlus_H.isValid() ){
      fEvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp = fMETTempPlus_H->begin()->pt();
   }
   if( fMETTempDown_H.isValid() ){
      fEvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown = fMETTempDown_H->begin()->pt();
   }

   //----- Generation information  --------------------------------------------------------------------
   if ( fGenEvent_H.isValid() && fGenEvent_H->hasPDF() ) {
      fEvtInfo.PDFid1   = fGenEvent_H->pdf()->id.first;
      fEvtInfo.PDFid2   = fGenEvent_H->pdf()->id.second;
      fEvtInfo.PDFx1    = fGenEvent_H->pdf()->x.first;
      fEvtInfo.PDFx2    = fGenEvent_H->pdf()->x.second;
      fEvtInfo.PDFscale = fGenEvent_H->pdf()->scalePDF;
      fEvtInfo.PDFv1    = fGenEvent_H->pdf()->xPDF.first;
      fEvtInfo.PDFv2    = fGenEvent_H->pdf()->xPDF.second;
   }

   //----- High Level Trigger information  ------------------------------------------------------------
   if ( fTrigger_H.isValid() ) {
      if( fDebug > 1 ) { cout << "\t[1] Getting High Level Trigger" << endl; }
      const edm::TriggerNames& TrgNames = iEvent.triggerNames( *fTrigger_H );
      fEvtInfo.TrgCount = 0;
      for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ) {
         unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
         if ( TrgIndex == TrgNames.size() ) {
            fEvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
         } else if ( !fTrigger_H->wasrun( TrgIndex ) ) {
            fEvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
         } else if ( !fTrigger_H->accept( TrgIndex ) ) {
            fEvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
         } else if (  fTrigger_H->error ( TrgIndex ) ) {
            fEvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
         } else {
            fEvtInfo.TrgBook[i] = +1; // It's triggered.
            fEvtInfo.TrgCount++;
         }
      }
      fEvtInfo.nHLT = TrgNames.size();
      for( size_t i = 0; i < TrgNames.size(); ++i ) {
         fEvtInfo.HLTbits[i] = ( fTrigger_H->accept( i ) == true ) ? 1 : 0;
         const std::string triggerName_ = TrgNames.triggerName( i );
         if( fDebug > 2 ) { cout << "\t\t[2] HLTInfo: "<< i <<"["<< fEvtInfo.HLTbits[i]<< "]: \"" << triggerName_ << "\"" << endl ; }
         // TODO Assuming prescale set = 0 , check.
         // Enoch 2016-02-04
         fEvtInfo.HLTPrescaleFactor[i] = fHighLevelTriggerConfig.prescaleValue( 0 , triggerName_ );
         fHighLevelTriggerMap_pr = fHighLevelTriggerMap.find( triggerName_ );
         if( fHighLevelTriggerMap_pr != fHighLevelTriggerMap.end() ) {
            fEvtInfo.HLTName2enum[i] = fHighLevelTriggerMap_pr->second ;
         } else {
            fEvtInfo.HLTName2enum[i] = -1; }
         if( fDebug > 2 ) { cout << "\t\t[2] Getting prescale " << fEvtInfo.HLTPrescaleFactor[i] << endl ; }
         if( fDebug > 2 ) { cout << "\t\t[2] Getting Trigger int Label " << fEvtInfo.HLTName2enum[i] << endl; }
      }
   }

   //----- Level 1 trigger and technical trigger bits  ------------------------------------------------
   if( fRecord_H.isValid() ) {
      DecisionWord dWord = fRecord_H->decisionWord();
      if ( ! dWord.empty() ) { // if board not there this is zero
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 128; ++i ) {
            if(dWord[i]!=0 && fDebug > 3  ){ cout <<"\t\t\t[3] Dword: "<< i << " " << dWord[i] << endl;}
            fEvtInfo.L1[i] = dWord[i];
         }
      }
      TechnicalTriggerWord tw = fRecord_H->technicalTriggerWord();
      if ( ! tw.empty() ) {
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 64; ++i ) {
            fEvtInfo.TT[i] = tw[i];
         }
      }
   }

   return true;
}
