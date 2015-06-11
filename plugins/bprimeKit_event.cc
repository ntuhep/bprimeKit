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

//------------------------------------------------------------------------------ 
//   Custom typedefs and enums
//------------------------------------------------------------------------------ 
typedef std::vector<pat::MET>   METList;
typedef edm::Handle<METList>    METHandler;
typedef METList::const_iterator METIterator;

//------------------------------------------------------------------------------ 
//   Helper static variables and functions
//------------------------------------------------------------------------------ 
static METHandler  METHandle;
static METHandler  pfMETHandle;
static METHandler  pfMETHandle_TempPlus;
static METHandler  pfMETHandle_TempDown;
static METIterator it_pfmet  ; 
static edm::Handle<GenEventInfoProduct> GenEventInfoHandle;
static edm::Handle<TriggerResults> TrgResultsHandle;
static edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
static std::pair<int,int>  psValueCombo;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 
bool bprimeKit::fillEvent( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( pfmetlabel_.size() > 0 ) { iEvent.getByLabel( pfmetlabel_[0],  pfMETHandle ); }
   const edm::View<reco::Track>& tracks = *TrackHandle;
   
   EvtInfo.RunNo    = iEvent.id().run();
   EvtInfo.EvtNo    = iEvent.id().event();
   EvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
   EvtInfo.BxNo     = iEvent.bunchCrossing();
   EvtInfo.LumiNo   = iEvent.luminosityBlock();
   EvtInfo.Orbit    = iEvent.orbitNumber();
   EvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
   EvtInfo.ptHat    = -1.;
   
   //-------------------------------  Getting beam spot  -------------------------------
   if( debug_ > 5 ) { cout << "\tGet beam spot.\n"; }
   if( offlineBSlabel_.size() > 0 ) {
      iEvent.getByLabel( offlineBSlabel_[0], beamSpotHandle );
      if ( beamSpotHandle.isValid() ) {
         beamSpot = *beamSpotHandle.product();
         EvtInfo.BeamSpotX = beamSpot.position().x();
         EvtInfo.BeamSpotY = beamSpot.position().y();
         EvtInfo.BeamSpotZ = beamSpot.position().z();
      } else {
         edm::LogInfo( "MyAnalyzer" )
               << "No beam spot available from EventSetup \n";
      }
   }

   if( !TurnOffInCMSSW73x ){
   for( unsigned int ri_ = 0; ri_ < 2; ri_++ ) {
      if( rhoH[ri_].isValid() ) { EvtInfo.RhoPU[ri_] = *( rhoH[ri_].product() ); }
      if( sigmaHandle[ri_].isValid() ) { EvtInfo.SigmaPU[ri_] = *( sigmaHandle[ri_].product() ); }
   }
   }

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



   if( pfMETHandle.isValid() ) {
      if( debug_ > 15 ) { cout << "Get pfMET info\n"; }
      for( it_pfmet = pfMETHandle->begin(); it_pfmet != pfMETHandle->end(); it_pfmet++ ) {
         EvtInfo.PFMET              = it_pfmet->pt()             ;
         EvtInfo.PFMETPhi           = it_pfmet->phi()            ;
         EvtInfo.PFRawMET           = it_pfmet->uncorrectedPt()  ;
         EvtInfo.PFRawMETPhi        = it_pfmet->uncorrectedPhi() ;
         EvtInfo.PFMETx             = it_pfmet->px()             ; //Uly 2011-04-04
         EvtInfo.PFMETy             = it_pfmet->py()             ; //Uly 2011-04-04
         EvtInfo.PFSumEt            = it_pfmet->sumEt()          ;
         EvtInfo.PFMETSig           = it_pfmet->mEtSig()         ; //MET Significance = MET / std::sqrt(SumET)
         EvtInfo.PFMETRealSig       = it_pfmet->significance()   ; //real MET significance
         EvtInfo.PFMETlongitudinal  = it_pfmet->e_longitudinal() ; //longitudinal component of the vector sum of energy over all object
         const reco::GenMET* genmet = it_pfmet->genMET()         ;
         if ( !skipGenInfo_ && genmet != NULL ) {
            EvtInfo.PFGenMET        = genmet->pt();
            EvtInfo.PFGenMETPhi     = genmet->phi();
         }
      }
   }
   //--------------------------  MET correction information  --------------------------- 
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnUp",  pfMETHandle_TempPlus );
   if( pfMETHandle_TempPlus.isValid() ){
      for( it_pfmet = pfMETHandle_TempPlus->begin(); it_pfmet != pfMETHandle_TempPlus->end(); it_pfmet++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnUp           = it_pfmet->pt(); }
   }
   iEvent.getByLabel( "patType1CorrectedPFMetUnclusteredEnDown",  pfMETHandle_TempDown );
   if( pfMETHandle_TempDown.isValid() ){
      for( it_pfmet = pfMETHandle_TempDown->begin() ; it_pfmet != pfMETHandle_TempDown->end(); it_pfmet++ ) {
         EvtInfo.PFMETType1CorrectedPFMetUnclusteredEnDown           = it_pfmet->pt(); }
   }

   //----------------------------  Generation information  -----------------------------
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

   //------------------------  High level trigger information  -------------------------
//   bool with_TriggerResults = ( hltlabel_.size() > 0 ) ? iEvent.getByLabel( hltlabel_[0], TrgResultsHandle ) : false;
//   if ( with_TriggerResults ) {
//      const edm::TriggerNames& TrgNames = iEvent.triggerNames( *TrgResultsHandle );
//      EvtInfo.TrgCount = 0;
//      for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ) {
//         unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
//         if ( TrgIndex == TrgNames.size() ) {
//            EvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
//         } else if ( !TrgResultsHandle->wasrun( TrgIndex ) ) {
//            EvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
//         } else if ( !TrgResultsHandle->accept( TrgIndex ) ) {
//            EvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
//         } else if (  TrgResultsHandle->error ( TrgIndex ) ) {
//            EvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
//         } else {
//            EvtInfo.TrgBook[i] = +1; // It's triggered.
//            EvtInfo.TrgCount++;
//         }
//      }
//      EvtInfo.nHLT = TrgNames.size();
//      for( size_t i = 0; i < TrgNames.size(); i++ ) {
//         EvtInfo.HLTbits[i] = ( TrgResultsHandle->accept( i ) == true ) ? 1 : 0;
//         const std::string triggerName_ = TrgNames.triggerName( i );
//         psValueCombo = hltConfig_.prescaleValues( iEvent, iSetup, triggerName_ );
//         EvtInfo.HLTPrescaleFactor[i] = ( int )psValueCombo.second;
//         HLTmaplist_pr = HLTmaplist.find( TrgNames.triggerName( i ) );
//         if( HLTmaplist_pr != HLTmaplist.end() ) {
//            EvtInfo.HLTName2enum[i] = HLTmaplist_pr->second ;
//         } else {
//            EvtInfo.HLTName2enum[i] = -1; }
//      }
//   }
//   //------------------  Level 1 trigger and technical trigger bits  -------------------
//   if( gtdigilabel_.size() > 0 ) { iEvent.getByLabel( gtdigilabel_[0], gtRecord ); }
//   if( gtRecord.isValid() ) {
//      DecisionWord dWord = gtRecord->decisionWord();
//      if ( ! dWord.empty() ) { // if board not there this is zero
//         // loop over dec. bit to get total rate (no overlap)
//         for ( int i = 0; i < 128; ++i ) {
//            //  if(dWord[i]!=0 && debug)cout << i << " " << dWord[i] << ": ";
//            EvtInfo.L1[i] = dWord[i];
//         }
//      }
//      TechnicalTriggerWord tw = gtRecord->technicalTriggerWord();
//      if ( ! tw.empty() ) {
//         // loop over dec. bit to get total rate (no overlap)
//         for ( int i = 0; i < 64; ++i ) {
//            //  if(tw[i]!=0 && debug) cout << i << "  " << tw[i] << ": ";
//            EvtInfo.TT[i] = tw[i];
//         }
//      }
//   }

   //------------------------------------------------------------------------------ 
   //   TODO Under construction
   //------------------------------------------------------------------------------ 
   if( !TurnOffInCMSSW73x ){
   for( unsigned int ri_ = 0; ri_ < 2; ri_++ ) {
      if( rhoH[ri_].isValid() ) { EvtInfo.RhoPU[ri_] = *( rhoH[ri_].product() ); }
      if( sigmaHandle[ri_].isValid() ) { EvtInfo.SigmaPU[ri_] = *( sigmaHandle[ri_].product() ); }
   }
   }
   
   //   if(METHandle.isValid()) {
   //     if(debug_>15) cout << "Get MET info\n";
   //     for( std::vector<pat::MET>::const_iterator it_met = METHandle->begin();
   //    it_met != METHandle->end(); it_met++ ) {
   //       EvtInfo.MET        = it_met->pt();
   //       EvtInfo.METPhi        = it_met->phi();
   //       EvtInfo.RawMET        = it_met->uncorrectedPt();
   //       EvtInfo.RawMETPhi     = it_met->uncorrectedPhi();
   //       EvtInfo.SumEt         = it_met->sumEt();
   //       EvtInfo.METSig        = it_met->mEtSig();
   //       EvtInfo.eLong         = it_met->e_longitudinal();
   //       EvtInfo.MaxHadTower   = it_met->maxEtInHadTowers();
   //       EvtInfo.MaxEmTower    = it_met->maxEtInEmTowers();
   //       EvtInfo.FracHad       = it_met->etFractionHadronic();
   //       EvtInfo.FracEm        = it_met->emEtFraction();
   //       const reco::GenMET * genmet = it_met->genMET();
   //       if (genmet!=NULL) {
   //         EvtInfo.GenMET        = genmet->pt();
   //         EvtInfo.GenMETPhi     = genmet->phi();
   //       }
   //     }
   //   }
   //by Dmitry Hits for filtering the real data
   //*******************************************************************
   //Add the number of tracks and fraction of high purity tracks in the track collection to the EvtInfo
   //*******************************************************************
   //
   //The desciption of tracks quality selection is here:
   //https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFinalTrackSelectors
   //
   //Cuts which determine the high purity depend on the CMSSW release:
   //http://cmslxr.fnal.gov/lxr/source/RecoTracker/FinalTrackSelectors/python/selectHighPurity_cfi.py
   //
   //For details see CMS IN-2008/017
   //Currently, if the number of tracks > 10,
   //then 25% of HighPurity tracks have to be present in the event for it to be called good event
   int numhighpurity = 0;
   if( !TurnOffInCMSSW73x ){
   if( TrackHandle.isValid() && !TrackHandle.failedToGet() && TrackHandle->size() > 0 ) {
      reco::TrackBase::TrackQuality _trackQuality = reco::TrackBase::qualityByName( "highPurity" );
      edm::View<reco::Track>::const_iterator itk = tracks.begin();
      edm::View<reco::Track>::const_iterator itk_e = tracks.end();
      for( ; itk != itk_e; ++itk ) {
         if( itk->quality( _trackQuality ) ) { numhighpurity++; }
      }
      EvtInfo.NofTracks = TrackHandle->size();
      EvtInfo.HighPurityFraction = ( float )numhighpurity / ( float )TrackHandle->size();
   }
   }
   return true;
}
