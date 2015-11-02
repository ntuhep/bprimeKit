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
bool bprimeKit::fillTau( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll ) 
{
   for( TauIterator it_tau = _tauHandleList[icoll]->begin(); it_tau != _tauHandleList[icoll]->end(); it_tau++ ) { 
      if ( LepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr<< "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }
      if( it_tau->pt() < 20 ) { continue; } //Require PT > 20 GeV
      if( debug_ > 11 ) { cout << "  Size " << LepInfo[icoll].Size << " tau pt,eta,phi " << it_tau->pt() << "," << it_tau->eta() << "," << it_tau->phi() << endl; }
      
      //----- Inserting generic information  -------------------------------------------------------------
      LepInfo[icoll].Index            [LepInfo[icoll].Size] = LepInfo[icoll].Size        ;
      LepInfo[icoll].LeptonType       [LepInfo[icoll].Size] = 15                         ;
      LepInfo[icoll].Charge           [LepInfo[icoll].Size] = it_tau->charge()           ;
      LepInfo[icoll].Pt               [LepInfo[icoll].Size] = it_tau->pt()               ;
      LepInfo[icoll].Eta              [LepInfo[icoll].Size] = it_tau->eta()              ;
      LepInfo[icoll].Phi              [LepInfo[icoll].Size] = it_tau->phi()              ;
      LepInfo[icoll].TrackIso         [LepInfo[icoll].Size] = it_tau->trackIso()         ;
      LepInfo[icoll].EcalIso          [LepInfo[icoll].Size] = it_tau->ecalIso()          ;
      LepInfo[icoll].HcalIso          [LepInfo[icoll].Size] = it_tau->hcalIso()          ;
      LepInfo[icoll].Px               [LepInfo[icoll].Size] = it_tau->px()               ; //Uly 2011-04-04
      LepInfo[icoll].Py               [LepInfo[icoll].Size] = it_tau->py()               ; //Uly 2011-04-04
      LepInfo[icoll].Pz               [LepInfo[icoll].Size] = it_tau->pz()               ; //Uly 2011-04-04
      LepInfo[icoll].ChargedHadronIso [LepInfo[icoll].Size] = it_tau->chargedHadronIso() ;
      LepInfo[icoll].NeutralHadronIso [LepInfo[icoll].Size] = it_tau->neutralHadronIso() ;
      LepInfo[icoll].PhotonIso        [LepInfo[icoll].Size] = it_tau->photonIso()        ;
      LepInfo[icoll].isPFTau          [LepInfo[icoll].Size] = it_tau->isPFTau()          ; // YoungKyu 2012-10-16

      //------------------------------  Tau ID information  -------------------------------
      LepInfo[icoll].DiscriminationByDecayModeFinding           [LepInfo[icoll].Size] =it_tau->tauID ( "decayModeFinding"                            );
      LepInfo[icoll].DiscriminationByDecayModeFindingNewDMs     [LepInfo[icoll].Size] =it_tau->tauID ( "decayModeFindingNewDMs"                      );
      //LepInfo[icoll].ByLooseElectronRejection                   [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronLoose"                        );
      //LepInfo[icoll].ByMediumElectronRejection                  [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronMedium"                       );
      //LepInfo[icoll].ByTightElectronRejection                   [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronTight"                        );
      LepInfo[icoll].ByMVA5LooseElectronRejection               [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronLooseMVA5"                    );
      LepInfo[icoll].ByMVA5MediumElectronRejection              [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronMediumMVA5"                   );
      LepInfo[icoll].ByMVA5TightElectronRejection               [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronTightMVA5"                    );
      LepInfo[icoll].ByMVA5VTightElectronRejection              [LepInfo[icoll].Size] =it_tau->tauID ( "againstElectronVTightMVA5"                   );
      LepInfo[icoll].ByLooseMuonRejection3                      [LepInfo[icoll].Size] =it_tau->tauID ( "againstMuonLoose3"                           );
      LepInfo[icoll].ByTightMuonRejection3                      [LepInfo[icoll].Size] =it_tau->tauID ( "againstMuonTight3"                           );
      //LepInfo[icoll].ByMVALooseMuonRejection                    [LepInfo[icoll].Size] =it_tau->tauID ( "againstMuonLooseMVA"                         );
      //LepInfo[icoll].ByMVAMediumMuonRejection                   [LepInfo[icoll].Size] =it_tau->tauID ( "againstMuonMediumMVA"                        );
      //LepInfo[icoll].ByMVATightMuonRejection                    [LepInfo[icoll].Size] =it_tau->tauID ( "againstMuonTightMVA"                         );
      LepInfo[icoll].ByLooseCombinedIsolationDeltaBetaCorr3Hits [LepInfo[icoll].Size] =it_tau->tauID ( "byLooseCombinedIsolationDeltaBetaCorr3Hits"  );
      LepInfo[icoll].ByMediumCombinedIsolationDeltaBetaCorr3Hits[LepInfo[icoll].Size] =it_tau->tauID ( "byMediumCombinedIsolationDeltaBetaCorr3Hits" );
      LepInfo[icoll].ByTightCombinedIsolationDeltaBetaCorr3Hits [LepInfo[icoll].Size] =it_tau->tauID ( "byTightCombinedIsolationDeltaBetaCorr3Hits"  );
      LepInfo[icoll].CombinedIsolationDeltaBetaCorrRaw3Hits     [LepInfo[icoll].Size] =it_tau->tauID ( "byCombinedIsolationDeltaBetaCorrRaw3Hits"    );
      //LepInfo[icoll].ByVLooseIsolationMVA3newDMwoLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVLooseIsolationMVA3newDMwoLT"              );
      //LepInfo[icoll].ByLooseIsolationMVA3newDMwoLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byLooseIsolationMVA3newDMwoLT"               );
      //LepInfo[icoll].ByMediumIsolationMVA3newDMwoLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byMediumIsolationMVA3newDMwoLT"              );
      //LepInfo[icoll].ByTightIsolationMVA3newDMwoLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byTightIsolationMVA3newDMwoLT"               );
      //LepInfo[icoll].ByVTightIsolationMVA3newDMwoLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVTightIsolationMVA3newDMwoLT"              );
      //LepInfo[icoll].ByVVTightIsolationMVA3newDMwoLT            [LepInfo[icoll].Size] =it_tau->tauID ( "byVVTightIsolationMVA3newDMwoLT"             );
      //LepInfo[icoll].ByIsolationMVA3newDMwoLTraw                [LepInfo[icoll].Size] =it_tau->tauID ( "byIsolationMVA3newDMwoLTraw"                 );
      //LepInfo[icoll].ByVLooseIsolationMVA3oldDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byVLooseIsolationMVA3oldDMwLT"               );
      //LepInfo[icoll].ByLooseIsolationMVA3oldDMwLT               [LepInfo[icoll].Size] =it_tau->tauID ( "byLooseIsolationMVA3oldDMwLT"                );
      //LepInfo[icoll].ByMediumIsolationMVA3oldDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byMediumIsolationMVA3oldDMwLT"               );
      //LepInfo[icoll].ByTightIsolationMVA3oldDMwLT               [LepInfo[icoll].Size] =it_tau->tauID ( "byTightIsolationMVA3oldDMwLT"                );
      //LepInfo[icoll].ByVTightIsolationMVA3oldDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byVTightIsolationMVA3oldDMwLT"               );
      //LepInfo[icoll].ByVVTightIsolationMVA3oldDMwLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVVTightIsolationMVA3oldDMwLT"              );
      //LepInfo[icoll].ByIsolationMVA3oldDMwLTraw                 [LepInfo[icoll].Size] =it_tau->tauID ( "byIsolationMVA3oldDMwLTraw"                  );
      //LepInfo[icoll].ByVLooseIsolationMVA3oldDMwoLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVLooseIsolationMVA3oldDMwoLT"              );
      //LepInfo[icoll].ByLooseIsolationMVA3oldDMwoLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byLooseIsolationMVA3oldDMwoLT"               );
      //LepInfo[icoll].ByTightIsolationMVA3oldDMwoLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byTightIsolationMVA3oldDMwoLT"               );
      //LepInfo[icoll].ByVTightIsolationMVA3oldDMwoLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVTightIsolationMVA3oldDMwoLT"              );
      //LepInfo[icoll].ByVVTightIsolationMVA3oldDMwoLT            [LepInfo[icoll].Size] =it_tau->tauID ( "byVVTightIsolationMVA3oldDMwoLT"             );
      //LepInfo[icoll].ByIsolationMVA3oldDMwoLTraw                [LepInfo[icoll].Size] =it_tau->tauID ( "byIsolationMVA3newDMwoLTraw"                 );
      //LepInfo[icoll].ByVLooseIsolationMVA3newDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byLooseIsolationMVA3newDMwLT"                );
      //LepInfo[icoll].ByLooseIsolationMVA3newDMwLT               [LepInfo[icoll].Size] =it_tau->tauID ( "byVLooseIsolationMVA3newDMwLT"               );
      //LepInfo[icoll].ByMediumIsolationMVA3newDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byMediumIsolationMVA3newDMwLT"               );
      //LepInfo[icoll].ByTightIsolationMVA3newDMwLT               [LepInfo[icoll].Size] =it_tau->tauID ( "byTightIsolationMVA3newDMwLT"                );
      //LepInfo[icoll].ByVTightIsolationMVA3newDMwLT              [LepInfo[icoll].Size] =it_tau->tauID ( "byVTightIsolationMVA3newDMwLT"               );
      //LepInfo[icoll].ByVVTightIsolationMVA3newDMwLT             [LepInfo[icoll].Size] =it_tau->tauID ( "byVVTightIsolationMVA3newDMwLT"              );
      //LepInfo[icoll].ByIsolationMVA3newDMwLTraw                 [LepInfo[icoll].Size] =it_tau->tauID ( "byIsolationMVA3newDMwLTraw"                  );

      //----------------------------  Generation information  -----------------------------
      if ( !isData && !skipGenInfo_ ) {                                       
         if( debug_ > 15 ) { cout << "   Getting MC information\n"; }         
         const reco::Candidate* gen = it_tau->genLepton();                    
         if ( gen != NULL ) {                                                 
            LepInfo[icoll].GenPt        [LepInfo[icoll].Size] = gen->pt();    
            LepInfo[icoll].GenEta       [LepInfo[icoll].Size] = gen->eta();   
            LepInfo[icoll].GenPhi       [LepInfo[icoll].Size] = gen->phi();   
            LepInfo[icoll].GenPdgID     [LepInfo[icoll].Size] = gen->pdgId(); 
         }
      }
      LepInfo[icoll].CandRef[LepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_tau );
      LepInfo[icoll].Size++;
   }

   return true;
}
