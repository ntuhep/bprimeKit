/*******************************************************************************
 *
 *  Filename    : bprimeKit_event.cc
 *  Description : Filling in event information
 *
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit.h"

//------------------------  Event specific CMSSW libraries  -------------------------
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

//------------------------------------------------------------------------------ 
//   Custom typedefs and enums
//------------------------------------------------------------------------------ 
typedef std::vector<pat::MET>   METList;
typedef edm::Handle<METList>    METHandler;
typedef METList::const_iterator METIterator;
typedef edm::Handle<std::vector<PileupSummaryInfo>>      PileupHandle;
typedef std::vector<PileupSummaryInfo>::const_iterator   PileupIterator;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 
bool bprimeKit::fillEvent( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( debug_ > 0 ) { cout << "Begin filling EvtInfoBranches" << endl ;}
   //-------------------------------------------------------------------------------------------------- 
   //   Helper variables definition
   //-------------------------------------------------------------------------------------------------- 
   PileupHandle     PUInfo;
   PileupIterator   PVI;
   METHandler       METHandle;
   METHandler       pfMETHandle_TempPlus;
   METHandler       pfMETHandle_TempDown;
   METIterator      it_met  ; 
   edm::Handle<GenEventInfoProduct> GenEventInfoHandle;
   edm::Handle<TriggerResults>      TrgResultsHandle;
   edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
   edm::Handle<double> rhoHandle;
   std::pair<int,int>  psValueCombo;

   if( debug_ > 0 ){ cout <<">>>Evt: Getting variables"<< endl;}
   iEvent.getByToken( rhoLabel_ , rhoHandle );
   iEvent.getByLabel( metlabel_[0],  METHandle );
   iEvent.getByLabel( offlineBSlabel_[0], beamSpotHandle );
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnUp",  pfMETHandle_TempPlus );
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnDown",  pfMETHandle_TempDown );
   if( ! isData ) {
      iEvent.getByLabel( puInfoLabel_[0], PUInfo ); }
   
   if( debug_ > 0 ){ cout <<">>>Evt: Inserting generic information"<< endl;}
   EvtInfo.RunNo    = iEvent.id().run();
   EvtInfo.EvtNo    = iEvent.id().event();
   EvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
   EvtInfo.BxNo     = iEvent.bunchCrossing();
   EvtInfo.LumiNo   = iEvent.luminosityBlock();
   EvtInfo.Orbit    = iEvent.orbitNumber();
   EvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
   EvtInfo.ptHat    = -1.;
   EvtInfo.Rho      = * (rhoHandle.product()) ; 
   
   if( debug_ > 0 ){ cout <<">>>Evt: Inserting Pile up information information"<< endl;}
   //----- Pile up information  -----------------------------------------------------------------------
   if( PUInfo.isValid() ) {
      for( PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI ) {
         EvtInfo.nPU[EvtInfo.nBX] = PVI->getPU_NumInteractions();
         EvtInfo.BXPU[EvtInfo.nBX] = PVI->getBunchCrossing();
         EvtInfo.TrueIT[EvtInfo.nBX] = PVI->getTrueNumInteractions();
         ++EvtInfo.nBX ;
      }
   }
   
   //----- Getting beamspot information  --------------------------------------------------------------
   if( debug_ > 5 ) { cout << ">>>Evt: Get beam spot."<< endl; }
   if ( beamSpotHandle.isValid() ) {
      beamSpot = *beamSpotHandle.product();
      EvtInfo.BeamSpotX = beamSpot.position().x();
      EvtInfo.BeamSpotY = beamSpot.position().y();
      EvtInfo.BeamSpotZ = beamSpot.position().z();
   } else {
      edm::LogInfo( "MyAnalyzer" )
            << "No beam spot available from EventSetup \n";
   }

   //----- Getting Monte Carlo mode  ------------------------------------------------------------------
   // See code in bprimeKit_genInfo.cc
   int mclep_count[2] = {0, 0};
   if ( EvtInfo.McWMode[0] == 1 || EvtInfo.McWMode[0] == 2 ) { mclep_count[1]++; }
   if ( EvtInfo.McWMode[1] == 1 || EvtInfo.McWMode[1] == 2 ) { mclep_count[0]++; }
   if ( EvtInfo.McWMode[2] == 1 || EvtInfo.McWMode[2] == 2 ) { mclep_count[0]++; }
   if ( EvtInfo.McWMode[3] == 1 || EvtInfo.McWMode[3] == 2 ) { mclep_count[1]++; }
   if ( EvtInfo.McZMode[0] == 1 || EvtInfo.McZMode[0] == 2 ) {
      mclep_count[0]++; mclep_count[1]++;}
   if ( EvtInfo.McZMode[1] == 1 || EvtInfo.McZMode[1] == 2 ) {
      mclep_count[0]++; mclep_count[1]++; }

   //------  McSigTag - 0:others, 1:opposite-sing dilepton, 2:same-sign dilepton  ------
   //---------------------------  3: trilepton, 4: 4-lepton  ---------------------------
   if ( mclep_count[0] == 1 && mclep_count[1] == 1 ) { EvtInfo.McSigTag = 1; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 0 ) { EvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 0 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 2; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 1 ) { EvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 1 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 3; }
   if ( mclep_count[0] == 2 && mclep_count[1] == 2 ) { EvtInfo.McSigTag = 4; }

   //----- Getting missing momentum information  ------------------------------------------------------
   if( debug_ > 5 ) { cout << ">>>Evt: Get missing momentum."<< endl; }
   for( it_met = METHandle->begin(); it_met != METHandle->end(); it_met++ ) {
      EvtInfo.PFMET              = it_met->pt()             ;
      EvtInfo.PFMETPhi           = it_met->phi()            ;
      EvtInfo.PFRawMET           = it_met->uncorrectedPt()  ;
      EvtInfo.PFRawMETPhi        = it_met->uncorrectedPhi() ;
      EvtInfo.PFMETx             = it_met->px()             ; //Uly 2011-04-04
      EvtInfo.PFMETy             = it_met->py()             ; //Uly 2011-04-04
      EvtInfo.PFSumEt            = it_met->sumEt()          ;
      EvtInfo.PFMETSig           = it_met->mEtSig()         ; //MET Significance = MET / std::sqrt(SumET)
      EvtInfo.PFMETRealSig       = it_met->significance()   ; //real MET significance
      EvtInfo.PFMETlongitudinal  = it_met->e_longitudinal() ; //longitudinal component of the vector sum of energy over all object
      const reco::GenMET* genmet = it_met->genMET()         ;
      if ( !skipGenInfo_ && genmet != NULL ) {
         EvtInfo.PFGenMET        = genmet->pt();
         EvtInfo.PFGenMETPhi     = genmet->phi();
      }
   }
   //----- Missing momentum correction  ---------------------------------------------------------------
   if( debug_ > 5 ) { cout << ">>>Evt: Get missing momentum corrections."<< endl; }
   if( pfMETHandle_TempPlus.isValid() ){
      for( it_met = pfMETHandle_TempPlus->begin(); it_met != pfMETHandle_TempPlus->end(); it_met++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp           = it_met->pt(); }
   }
   if( pfMETHandle_TempDown.isValid() ){
      for( it_met = pfMETHandle_TempDown->begin() ; it_met != pfMETHandle_TempDown->end(); it_met++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown           = it_met->pt(); }
   }

   //----- Generation information  --------------------------------------------------------------------
   bool with_GenEventInfo = ( genevtlabel_.size() > 0 ) ? iEvent.getByLabel( genevtlabel_[0], GenEventInfoHandle ) : false;
   if ( with_GenEventInfo && GenEventInfoHandle->hasPDF() ) {
      EvtInfo.PDFid1   = GenEventInfoHandle->pdf()->id.first;
      EvtInfo.PDFid2   = GenEventInfoHandle->pdf()->id.second;
      EvtInfo.PDFx1    = GenEventInfoHandle->pdf()->x.first;
      EvtInfo.PDFx2    = GenEventInfoHandle->pdf()->x.second;
      EvtInfo.PDFscale = GenEventInfoHandle->pdf()->scalePDF;
      EvtInfo.PDFv1    = GenEventInfoHandle->pdf()->xPDF.first;
      EvtInfo.PDFv2    = GenEventInfoHandle->pdf()->xPDF.second;
   }

   //----- High Level Trigger information  ------------------------------------------------------------
   bool with_TriggerResults = ( hltlabel_.size() > 0 ) ? iEvent.getByLabel( hltlabel_[0], TrgResultsHandle ) : false;
   if ( with_TriggerResults ) {
      if( debug_ > 10 ) { cout << "Getting High Level Trigger" << endl; }
      const edm::TriggerNames& TrgNames = iEvent.triggerNames( *TrgResultsHandle );
      EvtInfo.TrgCount = 0;
      for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ) {
         unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
         if ( TrgIndex == TrgNames.size() ) {
            EvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
         } else if ( !TrgResultsHandle->wasrun( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
         } else if ( !TrgResultsHandle->accept( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
         } else if (  TrgResultsHandle->error ( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
         } else {
            EvtInfo.TrgBook[i] = +1; // It's triggered.
            EvtInfo.TrgCount++;
         }
      }

      EvtInfo.nHLT = TrgNames.size();
      for( size_t i = 0; i < TrgNames.size(); ++i ) {
         EvtInfo.HLTbits[i] = ( TrgResultsHandle->accept( i ) == true ) ? 1 : 0;
         const std::string triggerName_ = TrgNames.triggerName( i );
         EvtInfo.HLTPrescaleFactor[i] = hltConfig_.prescaleValuesInDetail( iEvent, iSetup, triggerName_ ).second;
         HLTmaplist_pr = HLTmaplist.find( TrgNames.triggerName( i ) );
         if( HLTmaplist_pr != HLTmaplist.end() ) {
            EvtInfo.HLTName2enum[i] = HLTmaplist_pr->second ;
         } else {
            EvtInfo.HLTName2enum[i] = -1; }
      }
   }
   //------------------  Level 1 trigger and technical trigger bits  -------------------
   if( gtdigilabel_.size() > 0 ) { iEvent.getByLabel( gtdigilabel_[0], gtRecord ); }
   if( gtRecord.isValid() ) {
      DecisionWord dWord = gtRecord->decisionWord();
      if ( ! dWord.empty() ) { // if board not there this is zero
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 128; ++i ) {
            if(dWord[i]!=0 && debug_ ){ cout <<"Dword: "<< i << " " << dWord[i] << ": ";}
            EvtInfo.L1[i] = dWord[i];
         }
      }
      TechnicalTriggerWord tw = gtRecord->technicalTriggerWord();
      if ( ! tw.empty() ) {
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 64; ++i ) {
            EvtInfo.TT[i] = tw[i];
         }
      }
   }

   return true;
}
