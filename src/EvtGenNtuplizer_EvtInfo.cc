/*******************************************************************************
*
*  Filename    : bprimeKit_event.cc
*  Description : Filling in event information
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/EvtGenNtuplizer.hpp"

#include "FWCore/Common/interface/TriggerNames.h"

using namespace std;

/******************************************************************************/

void
EvtGenNtuplizer::FillEvent( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  EvtInfo.RunNo    = iEvent.id().run();
  EvtInfo.EvtNo    = iEvent.id().event();
  EvtInfo.McFlag   = iEvent.isRealData() ? 0 : 1;
  EvtInfo.BxNo     = iEvent.bunchCrossing();
  EvtInfo.LumiNo   = iEvent.luminosityBlock();
  EvtInfo.Orbit    = iEvent.orbitNumber();
  EvtInfo.nTrgBook = N_TRIGGER_BOOKINGS;
  EvtInfo.Rho      = *_rhohandle;

  // ----- Pile up information  -----------------------------------------------------------------------
  if( !iEvent.isRealData() ){// Need to shutdown for Data
    for( auto it = _pileuphandle->begin(); it != _pileuphandle->end(); ++it ){
      EvtInfo.nPU[EvtInfo.nBX]    = it->getPU_NumInteractions();
      EvtInfo.BXPU[EvtInfo.nBX]   = it->getBunchCrossing();
      EvtInfo.TrueIT[EvtInfo.nBX] = it->getTrueNumInteractions();
      ++EvtInfo.nBX;
    }
  }

  EvtInfo.BeamSpotX = _beamspothandle->position().x();
  EvtInfo.BeamSpotY = _beamspothandle->position().y();
  EvtInfo.BeamSpotZ = _beamspothandle->position().z();

  // ----- Getting Monte Carlo mode  ------------------------------------------------------------------
  // See code in bprimeKit_genInfo.cc
  int mclep_count[2] = {0, 0};
  if( EvtInfo.McWMode[0] == 1 || EvtInfo.McWMode[0] == 2 ){ mclep_count[1]++; }
  if( EvtInfo.McWMode[1] == 1 || EvtInfo.McWMode[1] == 2 ){ mclep_count[0]++; }
  if( EvtInfo.McWMode[2] == 1 || EvtInfo.McWMode[2] == 2 ){ mclep_count[0]++; }
  if( EvtInfo.McWMode[3] == 1 || EvtInfo.McWMode[3] == 2 ){ mclep_count[1]++; }
  if( EvtInfo.McZMode[0] == 1 || EvtInfo.McZMode[0] == 2 ){ mclep_count[0]++; mclep_count[1]++; }
  if( EvtInfo.McZMode[1] == 1 || EvtInfo.McZMode[1] == 2 ){ mclep_count[0]++; mclep_count[1]++; }

  // ------  McSigTag - 0:others, 1:opposite-sing dilepton, 2:same-sign dilepton  ------
  // ---------------------------  3: trilepton, 4: 4-lepton  ---------------------------
  if( mclep_count[0] == 1 && mclep_count[1] == 1 ){ EvtInfo.McSigTag = 1; }
  if( mclep_count[0] == 2 && mclep_count[1] == 0 ){ EvtInfo.McSigTag = 2; }
  if( mclep_count[0] == 0 && mclep_count[1] == 2 ){ EvtInfo.McSigTag = 2; }
  if( mclep_count[0] == 2 && mclep_count[1] == 1 ){ EvtInfo.McSigTag = 3; }
  if( mclep_count[0] == 1 && mclep_count[1] == 2 ){ EvtInfo.McSigTag = 3; }
  if( mclep_count[0] == 2 && mclep_count[1] == 2 ){ EvtInfo.McSigTag = 4; }

  // ----- Getting missing momentum information  --------------------------------
  for( auto it_met = _methandle->begin(); it_met != _methandle->end(); it_met++ ){
    EvtInfo.PFMET             = it_met->pt();
    EvtInfo.PFMETPhi          = it_met->phi();
    EvtInfo.PFRawMET          = it_met->uncorPt();
    EvtInfo.PFRawMETPhi       = it_met->uncorPhi();
    EvtInfo.PFMETx            = it_met->px();           // Uly 2011-04-04
    EvtInfo.PFMETy            = it_met->py();           // Uly 2011-04-04
    EvtInfo.PFSumEt           = it_met->sumEt();
    EvtInfo.PFMETSig          = it_met->mEtSig();       // MET Significance = MET / std::sqrt(SumET)
    EvtInfo.PFMETRealSig      = it_met->significance();       // real MET significance
    EvtInfo.PFMETlongitudinal = it_met->e_longitudinal();// longitudinal component of the vector sum of energy over all object
    const reco::GenMET* genmet = it_met->genMET();
    if( genmet != NULL ){
      EvtInfo.PFGenMET    = genmet->pt();
      EvtInfo.PFGenMETPhi = genmet->phi();
    }
  }

  for( auto it_met = _pmethandle->begin(); it_met != _pmethandle->end(); it_met++ ){
    EvtInfo.PuppiMET             = it_met->pt();
    EvtInfo.PuppiMETPhi          = it_met->phi();
    EvtInfo.PuppiRawMET          = it_met->uncorPt();
    EvtInfo.PuppiRawMETPhi       = it_met->uncorPhi();
    EvtInfo.PuppiSumEt           = it_met->sumEt();
    EvtInfo.PuppiMETSig          = it_met->mEtSig();       // MET Significance = MET / std::sqrt(SumET)
    EvtInfo.PuppiMETRealSig      = it_met->significance();       // real MET significance
    EvtInfo.PuppiMETlongitudinal = it_met->e_longitudinal();// longitudinal component of the vector sum of energy over all object
    const reco::GenMET* genmet = it_met->genMET();
    if( genmet != NULL ){
      EvtInfo.PuppiGenMET    = genmet->pt();
      EvtInfo.PuppiGenMETPhi = genmet->phi();
    }
  }

  // ----- Generation information  --------------------------------------------
  if( !iEvent.isRealData() && _genevthandle->hasPDF() ){
    EvtInfo.PDFid1   = _genevthandle->pdf()->id.first;
    EvtInfo.PDFid2   = _genevthandle->pdf()->id.second;
    EvtInfo.PDFx1    = _genevthandle->pdf()->x.first;
    EvtInfo.PDFx2    = _genevthandle->pdf()->x.second;
    EvtInfo.PDFscale = _genevthandle->pdf()->scalePDF;
    EvtInfo.PDFv1    = _genevthandle->pdf()->xPDF.first;
    EvtInfo.PDFv2    = _genevthandle->pdf()->xPDF.second;
  }

  // ----- Level 1 trigger and technical trigger bits  ------------------------
  if( _recordhandle.isValid() ){
    DecisionWord dWord = _recordhandle->decisionWord();
    if( !dWord.empty() ){// if board not there this is zero
      // loop over dec. bit to get total rate (no overlap)
      for( int i = 0; i < 128; ++i ){
        EvtInfo.L1[i] = dWord[i];
      }
    }
    TechnicalTriggerWord tw = _recordhandle->technicalTriggerWord();
    if( !tw.empty() ){
      // loop over dec. bit to get total rate (no overlap)
      for( int i = 0; i < 64; ++i ){
        EvtInfo.TT[i] = tw[i];
      }
    }
  }

  /*******************************************************************************
  *   Trigger variables
  *******************************************************************************/
  const edm::TriggerNames& TrgNames = iEvent.triggerNames( *_triggerhandle );

  // Getting trigger table
  EvtInfo.TrgCount = 0;

  for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ){
    unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
    if( TrgIndex == TrgNames.size() ){
      EvtInfo.TrgBook[i] = -4;// The trigger path is not known in this event.
    } else if( !_triggerhandle->wasrun( TrgIndex ) ){
      EvtInfo.TrgBook[i] = -3;// The trigger path was not included in this event.
    } else if( !_triggerhandle->accept( TrgIndex ) ){
      EvtInfo.TrgBook[i] = -2;// The trigger path was not accepted in this event.
    } else if( _triggerhandle->error( TrgIndex ) ){
      EvtInfo.TrgBook[i] = -1;// The trigger path has an error in this event.
    } else {
      EvtInfo.TrgBook[i] = +1;// It's triggered.
      EvtInfo.TrgCount++;
    }
  }

  EvtInfo.nHLT = TrgNames.size();

  // TODO Assuming prescale set = 0 , check.
  // Enoch 2016-02-04
  for( size_t i = 0; i < TrgNames.size(); ++i ){
    const std::string name = TrgNames.triggerName( i );
    EvtInfo.HLTbits[i]           = _triggerhandle->accept( i ) ? 1 : 0;
    EvtInfo.HLTPrescaleFactor[i] = _hltconfig.prescaleValue( 0, name );
    EvtInfo.HLTName2enum[i]      = bprimeKit::GetTriggerIdx( name );
  }

  /*******************************************************************************
  *   MET filter requirements
  *******************************************************************************/
  const edm::TriggerNames& mettriggername = iEvent.triggerNames( *_mettriggerhandle );

  // lambda function for MET filter trigger parsing
  auto checkMETfilter
    = [
    this,
    &mettriggername
      ]( const std::string& triggername ){
        const unsigned index = mettriggername.triggerIndex( triggername );
        return this->_mettriggerhandle->accept( index ) && this->_mettriggerhandle->wasrun( index ) && !this->_mettriggerhandle->error( index );
      };

  EvtInfo.McSigTag = (
    // *_metbadmuhandle
    //&& *_metbadchadhandle &&
    checkMETfilter( "Flag_HBHENoiseFilter" )
    && checkMETfilter( "Flag_HBHENoiseIsoFilter" )
    && checkMETfilter( "Flag_EcalDeadCellTriggerPrimitiveFilter" )
    && checkMETfilter( "Flag_goodVertices" )
    && checkMETfilter( "Flag_eeBadScFilter" )
    && checkMETfilter( "Flag_globalTightHalo2016Filter" )
    );

}
