/*******************************************************************************
*
*  Filename    : bprimeKit_tau.cc
*  Description : Filling in tau information
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

using namespace std;

/******************************************************************************/

void
LeptonNtuplizer::FillTau( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  for( auto it_tau = _tauhandle->begin(); it_tau != _tauhandle->end(); it_tau++ ){
    if( LepInfo.Size >= MAX_LEPTONS ){
      cerr<< "ERROR: number of leptons exceeds the size of array." << endl;
      break;
    }
    if( it_tau->pt() < 20 ){ continue; }  // Require PT > 20 GeV

    // ----- Inserting generic information  -------------------------------------------------------------
    LepInfo.Index            [LepInfo.Size] = LepInfo.Size;
    LepInfo.LeptonType       [LepInfo.Size] = 15;
    LepInfo.Charge           [LepInfo.Size] = it_tau->charge();
    LepInfo.Pt               [LepInfo.Size] = it_tau->pt();
    LepInfo.Eta              [LepInfo.Size] = it_tau->eta();
    LepInfo.Phi              [LepInfo.Size] = it_tau->phi();
    LepInfo.Px               [LepInfo.Size] = it_tau->px();  
    LepInfo.Py               [LepInfo.Size] = it_tau->py();  
    LepInfo.Pz               [LepInfo.Size] = it_tau->pz();  
    LepInfo.isPFTau          [LepInfo.Size] = it_tau->isPFTau();

    if( !iEvent.isRealData() ){
      const reco::Candidate* gen = it_tau->genLepton();
      if( gen != NULL ){
        LepInfo.GenPt        [LepInfo.Size] = gen->pt();
        LepInfo.GenEta       [LepInfo.Size] = gen->eta();
        LepInfo.GenPhi       [LepInfo.Size] = gen->phi();
        LepInfo.GenPdgID     [LepInfo.Size] = gen->pdgId();
      }
    }
    LepInfo.Size++;
  }
}
