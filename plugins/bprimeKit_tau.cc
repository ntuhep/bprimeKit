/*******************************************************************************
 *
 *  Filename    : bprimeKit_tau.cc
 *  Description : Filling in tau information
 *
*******************************************************************************/

#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "DataFormats/PatCandidates/interface/Tau.h"


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
      fLepInfo[icoll].DiscriminationByDecayModeFinding            [fLepInfo[icoll].Size] = it_tau->tauID( "decayModeFinding" ) ;
      fLepInfo[icoll].DiscriminationByDecayModeFindingNewDMs      [fLepInfo[icoll].Size] = it_tau->tauID( "decayModeFindingNewDMs" ) ;
      fLepInfo[icoll].ByLooseCombinedIsolationDeltaBetaCorr3Hits  [fLepInfo[icoll].Size] = it_tau->tauID( "byLooseCombinedIsolationDeltaBetaCorr3Hits" ) ;
      fLepInfo[icoll].ByMediumCombinedIsolationDeltaBetaCorr3Hits [fLepInfo[icoll].Size] = it_tau->tauID( "byMediumCombinedIsolationDeltaBetaCorr3Hits" ) ;
      fLepInfo[icoll].ByTightCombinedIsolationDeltaBetaCorr3Hits  [fLepInfo[icoll].Size] = it_tau->tauID( "byTightCombinedIsolationDeltaBetaCorr3Hits" ) ;
      fLepInfo[icoll].ByCombinedIsolationDeltaBetaCorrRaw3Hits    [fLepInfo[icoll].Size] = it_tau->tauID( "byCombinedIsolationDeltaBetaCorrRaw3Hits" ) ;
      fLepInfo[icoll].ChargedIsoPtSum                             [fLepInfo[icoll].Size] = it_tau->tauID( "chargedIsoPtSum" ) ;
      fLepInfo[icoll].NeutralIsoPtSum                             [fLepInfo[icoll].Size] = it_tau->tauID( "neutralIsoPtSum" ) ;
      fLepInfo[icoll].PuCorrPtSum                                 [fLepInfo[icoll].Size] = it_tau->tauID( "puCorrPtSum" ) ;
      fLepInfo[icoll].ByLoosePileupWeightedIsolation3Hits         [fLepInfo[icoll].Size] = it_tau->tauID( "byLoosePileupWeightedIsolation3Hits" ) ;
      fLepInfo[icoll].ByMediumPileupWeightedIsolation3Hits        [fLepInfo[icoll].Size] = it_tau->tauID( "byMediumPileupWeightedIsolation3Hits" ) ;
      fLepInfo[icoll].ByTightPileupWeightedIsolation3Hits         [fLepInfo[icoll].Size] = it_tau->tauID( "byTightPileupWeightedIsolation3Hits" ) ;
      fLepInfo[icoll].ByPileupWeightedIsolationRaw3Hits           [fLepInfo[icoll].Size] = it_tau->tauID( "byPileupWeightedIsolationRaw3Hits" ) ;
      fLepInfo[icoll].NeutralIsoPtSumWeight                       [fLepInfo[icoll].Size] = it_tau->tauID( "neutralIsoPtSumWeight" ) ;
      fLepInfo[icoll].FootprintCorrection                         [fLepInfo[icoll].Size] = it_tau->tauID( "footprintCorrection" ) ;
      fLepInfo[icoll].PhotonPtSumOutsideSignalCone                [fLepInfo[icoll].Size] = it_tau->tauID( "photonPtSumOutsideSignalCone" ) ;
      fLepInfo[icoll].AgainstMuonLoose3                           [fLepInfo[icoll].Size] = it_tau->tauID( "againstMuonLoose3" ) ;
      fLepInfo[icoll].AgainstMuonTight3                           [fLepInfo[icoll].Size] = it_tau->tauID( "againstMuonTight3" ) ;
      fLepInfo[icoll].AgainstElectronVLooseMVA5                   [fLepInfo[icoll].Size] = it_tau->tauID( "againstElectronVLooseMVA5" ) ;
      fLepInfo[icoll].AgainstElectronLooseMVA5                    [fLepInfo[icoll].Size] = it_tau->tauID( "againstElectronLooseMVA5" ) ;
      fLepInfo[icoll].AgainstElectronMediumMVA5                   [fLepInfo[icoll].Size] = it_tau->tauID( "againstElectronMediumMVA5" ) ;

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
      fLepInfo[icoll].CandRef[fLepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_tau );
      fLepInfo[icoll].Size++;
   }

   return true;
}
