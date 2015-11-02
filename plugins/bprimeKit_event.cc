/*******************************************************************************
 *
 *  Filename    : bprimeKit_event.cc
 *  Description : Filling in event information
 *
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit.h"

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 
bool bprimeKit::fillEvent( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( debug_ > 1 ) { cout << "\t[1] Entering EventInfo subroutine" << endl ;}
   //-------------------------------------------------------------------------------------------------- 
   //   Helper variables definition
   //-------------------------------------------------------------------------------------------------- 
   std::pair<int,int>  psValueCombo;

   
   if( debug_ > 1 ){ cout <<"\t[1] Inserting generic Event information"<< endl;}
   EvtInfo.RunNo    = iEvent.id().run();
   EvtInfo.EvtNo    = iEvent.id().event();
   EvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
   EvtInfo.BxNo     = iEvent.bunchCrossing();
   EvtInfo.LumiNo   = iEvent.luminosityBlock();
   EvtInfo.Orbit    = iEvent.orbitNumber();
   EvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
   EvtInfo.Rho      = *(_rhoHandle.product()) ; 
   
   if( debug_ > 1 ){ cout <<"\t[1] Inserting Pile up information information"<< endl;}
   //----- Pile up information  -----------------------------------------------------------------------
   if( _pileupHandle.isValid() ) {
      for( auto PVI = _pileupHandle->begin(); PVI != _pileupHandle->end(); ++PVI ) {
         EvtInfo.nPU[EvtInfo.nBX] = PVI->getPU_NumInteractions();
         EvtInfo.BXPU[EvtInfo.nBX] = PVI->getBunchCrossing();
         EvtInfo.TrueIT[EvtInfo.nBX] = PVI->getTrueNumInteractions();
         ++EvtInfo.nBX ;
      }
   }
   
   //----- Getting beamspot information  --------------------------------------------------------------
   if( debug_ > 1 ) { cout << "\t[1] Get beam spot."<< endl; }
   if ( _beamSpotHandle.isValid() ) {
      const auto& beamSpot = *(_beamSpotHandle.product());
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
   for( auto it_met = _metHandle->begin(); it_met != _metHandle->end(); it_met++ ) {
      EvtInfo.PFMET              = it_met->pt()             ;
      EvtInfo.PFMETPhi           = it_met->phi()            ;
      EvtInfo.PFRawMET           = it_met->uncorPt()  ;
      EvtInfo.PFRawMETPhi        = it_met->uncorPhi() ;
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
   if( debug_ > 1 ) { cout << "\t[1] Get missing momentum corrections."<< endl; }
   if( _metHandle_TempPlus.isValid() ){
      EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp = _metHandle_TempPlus->begin()->pt();
   }
   if( _metHandle_TempDown.isValid() ){
      EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown = _metHandle_TempDown->begin()->pt();
   }

   //----- Generation information  --------------------------------------------------------------------
   iEvent.getByLabel( genevtLabel_ , _genInfoHandle ) ;
   if ( _genInfoHandle.isValid() && _genInfoHandle->hasPDF() ) {
      EvtInfo.PDFid1   = _genInfoHandle->pdf()->id.first;
      EvtInfo.PDFid2   = _genInfoHandle->pdf()->id.second;
      EvtInfo.PDFx1    = _genInfoHandle->pdf()->x.first;
      EvtInfo.PDFx2    = _genInfoHandle->pdf()->x.second;
      EvtInfo.PDFscale = _genInfoHandle->pdf()->scalePDF;
      EvtInfo.PDFv1    = _genInfoHandle->pdf()->xPDF.first;
      EvtInfo.PDFv2    = _genInfoHandle->pdf()->xPDF.second;
   }

   //----- High Level Trigger information  ------------------------------------------------------------
   if ( _triggerHandle.isValid() ) {
      if( debug_ > 1 ) { cout << "\t[1] Getting High Level Trigger" << endl; }
      const edm::TriggerNames& TrgNames = iEvent.triggerNames( *_triggerHandle );
      EvtInfo.TrgCount = 0;
      for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ) {
         unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
         if ( TrgIndex == TrgNames.size() ) {
            EvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
         } else if ( !_triggerHandle->wasrun( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
         } else if ( !_triggerHandle->accept( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
         } else if (  _triggerHandle->error ( TrgIndex ) ) {
            EvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
         } else {
            EvtInfo.TrgBook[i] = +1; // It's triggered.
            EvtInfo.TrgCount++;
         }
      }
      EvtInfo.nHLT = TrgNames.size();
      for( size_t i = 0; i < TrgNames.size(); ++i ) {
         EvtInfo.HLTbits[i] = ( _triggerHandle->accept( i ) == true ) ? 1 : 0;
         const std::string triggerName_ = TrgNames.triggerName( i );
         if( debug_ > 2 ) { cout << "\t\t[2] HLTInfo: "<< i <<"["<< EvtInfo.HLTbits[i]<< "]: \"" << triggerName_ << "\"" << endl ; }
         if( debug_ > 2 ) { cout << "\t\t[2] Getting prescale set: " << hltConfig_.prescaleSet( iEvent , iSetup ) << endl; }
         EvtInfo.HLTPrescaleFactor[i] = hltConfig_.prescaleValue( iEvent, iSetup, triggerName_ );
         HLTmaplist_pr = HLTmaplist.find( triggerName_ );
         if( HLTmaplist_pr != HLTmaplist.end() ) {
            EvtInfo.HLTName2enum[i] = HLTmaplist_pr->second ;
         } else {
            EvtInfo.HLTName2enum[i] = -1; }
         if( debug_ > 2 ) { cout << "\t\t[2] Getting prescale " << EvtInfo.HLTPrescaleFactor[i] << endl ; }
         if( debug_ > 2 ) { cout << "\t\t[2] Getting Trigger int Label " << EvtInfo.HLTName2enum[i] << endl; }
      }
   }

   //----- Level 1 trigger and technical trigger bits  ------------------------------------------------
   if( _gtRecord.isValid() ) {
      DecisionWord dWord = _gtRecord->decisionWord();
      if ( ! dWord.empty() ) { // if board not there this is zero
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 128; ++i ) {
            if(dWord[i]!=0 && debug_ > 2  ){ cout <<"\t\t[2] Dword: "<< i << " " << dWord[i] << ": ";}
            EvtInfo.L1[i] = dWord[i];
         }
      }
      TechnicalTriggerWord tw = _gtRecord->technicalTriggerWord();
      if ( ! tw.empty() ) {
         // loop over dec. bit to get total rate (no overlap)
         for ( int i = 0; i < 64; ++i ) {
            EvtInfo.TT[i] = tw[i];
         }
      }
   }

   return true;
}
