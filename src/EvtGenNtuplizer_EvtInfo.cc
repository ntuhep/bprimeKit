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

  EvtInfo.Flag_METFilter = (
       checkMETfilter( "Flag_HBHENoiseFilter" )
    && checkMETfilter( "Flag_HBHENoiseIsoFilter" )
    && checkMETfilter( "Flag_EcalDeadCellTriggerPrimitiveFilter" )
    && checkMETfilter( "Flag_goodVertices" )
    && checkMETfilter( "Flag_globalSuperTightHalo2016Filter" )
    && checkMETfilter( "Flag_BadPFMuonFilter" )
    && checkMETfilter( "Flag_BadChargedCandidateFilter" )
    && checkMETfilter( "Flag_ecalBadCalibFilter" )
  );
  //MC isn't suggested to use Flag_eeBadScFilter
  if ( iEvent.isRealData() ) EvtInfo.Flag_METFilter = EvtInfo.Flag_METFilter && checkMETfilter( "Flag_eeBadScFilter" );

}
