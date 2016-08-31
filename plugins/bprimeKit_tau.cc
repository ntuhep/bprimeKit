/*******************************************************************************
 *
 *  Filename    : bprimeKit_tau.cc
 *  Description : Filling in tau information
 *
*******************************************************************************/

#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

using namespace std;
//------------------------------------------------------------------------------
//   bprimeKit method implementation
//------------------------------------------------------------------------------
bool bprimeKit::FillTau( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll )
{
   for( TauIterator it_tau = fTauList_Hs[icoll]->begin(); it_tau != fTauList_Hs[icoll]->end(); it_tau++ ) {
      if ( fLepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr<< "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }
      if( it_tau->pt() < 20 ) { continue; } //Require PT > 20 GeV
      if( fDebug > 2 ) {
         std::cerr << "\t\t[2]Size " << fLepInfo[icoll].Size
                   << " tau pt,eta,phi " << it_tau->pt() << "," << it_tau->eta() << "," << it_tau->phi() << endl;
      }

      //----- Inserting generic information  -------------------------------------------------------------
      fLepInfo[icoll].Index            [fLepInfo[icoll].Size] = fLepInfo[icoll].Size        ;
      fLepInfo[icoll].LeptonType       [fLepInfo[icoll].Size] = 15                         ;
      fLepInfo[icoll].Charge           [fLepInfo[icoll].Size] = it_tau->charge()           ;
      fLepInfo[icoll].Pt               [fLepInfo[icoll].Size] = it_tau->pt()               ;
      fLepInfo[icoll].Eta              [fLepInfo[icoll].Size] = it_tau->eta()              ;
      fLepInfo[icoll].Phi              [fLepInfo[icoll].Size] = it_tau->phi()              ;
      fLepInfo[icoll].TrackIso         [fLepInfo[icoll].Size] = it_tau->trackIso()         ;
      fLepInfo[icoll].EcalIso          [fLepInfo[icoll].Size] = it_tau->ecalIso()          ;
      fLepInfo[icoll].HcalIso          [fLepInfo[icoll].Size] = it_tau->hcalIso()          ;
      fLepInfo[icoll].Px               [fLepInfo[icoll].Size] = it_tau->px()               ; //Uly 2011-04-04
      fLepInfo[icoll].Py               [fLepInfo[icoll].Size] = it_tau->py()               ; //Uly 2011-04-04
      fLepInfo[icoll].Pz               [fLepInfo[icoll].Size] = it_tau->pz()               ; //Uly 2011-04-04
      fLepInfo[icoll].ChargedHadronIso [fLepInfo[icoll].Size] = it_tau->chargedHadronIso() ;
      fLepInfo[icoll].NeutralHadronIso [fLepInfo[icoll].Size] = it_tau->neutralHadronIso() ;
      fLepInfo[icoll].PhotonIso        [fLepInfo[icoll].Size] = it_tau->photonIso()        ;
      fLepInfo[icoll].isPFTau          [fLepInfo[icoll].Size] = it_tau->isPFTau()          ; // YoungKyu 2012-10-16

      //----- Tau ID information  ----------------------------------------------------

      //----------------------------  Generation information  -----------------------------
      if ( !fIsData && !fSkipfGenInfo ) {
         if( fDebug > 3 ) { cout << "\t\t\t[3]Getting Tau MC information\n"; }
         const reco::Candidate* gen = it_tau->genLepton();
         if ( gen != NULL ) {
            fLepInfo[icoll].GenPt        [fLepInfo[icoll].Size] = gen->pt();
            fLepInfo[icoll].GenEta       [fLepInfo[icoll].Size] = gen->eta();
            fLepInfo[icoll].GenPhi       [fLepInfo[icoll].Size] = gen->phi();
            fLepInfo[icoll].GenPdgID     [fLepInfo[icoll].Size] = gen->pdgId();
         }
      }
      fLepInfo[icoll].Size++;
   }

   return true;
}
