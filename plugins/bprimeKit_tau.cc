#include "MyAna/bprimeKit/interface/bprimeKit.h"


#include "DataFormats/PatCandidates/interface/Tau.h"

typedef std::vector<pat::Tau>  TauList;
typedef edm::Handle<TauList>   TauHandler;
typedef std::vector<TauHandler> TauHandlerList;
typedef TauList::const_iterator TauIterator;

static TauHandlerList  TauHandle;
static TauIterator     it_tau;
static string          NonLabel;

bool bprimeKit::fillTau( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll ) 
{
   NonLabel = "";
   for( unsigned il = 0; il < taulabel_.size(); il++ ) {
      if( NonLabel.compare( taulabel_[il].label() ) == 0 ) { continue; }
      TauHandle.push_back( TauHandler() );
      iEvent.getByLabel( taulabel_[il], TauHandle[il] );
      if( debug_ > 10 ) { cout << "leps " << il << " taulabel " << taulabel_[il] << " with " << TauHandle[il]->size() << " entries\n"; }
   }
  
   if( TauHandle.size() <= icoll ) {  return false; } 
   if( debug_ > 10 ) { cout << " Tau collection size " << TauHandle[icoll]->size() << endl; }

   for( it_tau = TauHandle[icoll]->begin(); it_tau != TauHandle[icoll]->end(); it_tau++ ) {//loop over taus in collection
      if ( LepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr<< "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }

      if( it_tau->pt() < 20 ) { continue; } //Require PT > 20 GeV
      if( debug_ > 11 ) { cout << "  Size " << LepInfo[icoll].Size << " tau pt,eta,phi " << it_tau->pt() << "," << it_tau->eta() << "," << it_tau->phi() << endl; }

      LepInfo[icoll].Index      [LepInfo[icoll].Size] = LepInfo[icoll].Size;
      LepInfo[icoll].LeptonType [LepInfo[icoll].Size] = 15;
      LepInfo[icoll].Charge     [LepInfo[icoll].Size] = it_tau->charge();
      LepInfo[icoll].Pt         [LepInfo[icoll].Size] = it_tau->pt();
      LepInfo[icoll].Eta        [LepInfo[icoll].Size] = it_tau->eta();
      LepInfo[icoll].Phi        [LepInfo[icoll].Size] = it_tau->phi();
      LepInfo[icoll].TrackIso   [LepInfo[icoll].Size] = it_tau->trackIso();
      LepInfo[icoll].EcalIso    [LepInfo[icoll].Size] = it_tau->ecalIso();
      LepInfo[icoll].HcalIso    [LepInfo[icoll].Size] = it_tau->hcalIso();

      LepInfo[icoll].Px         [LepInfo[icoll].Size] = it_tau->px(); //Uly 2011-04-04
      LepInfo[icoll].Py         [LepInfo[icoll].Size] = it_tau->py(); //Uly 2011-04-04
      LepInfo[icoll].Pz         [LepInfo[icoll].Size] = it_tau->pz(); //Uly 2011-04-04

      //PFIso
      LepInfo[icoll].ChargedHadronIso  [LepInfo[icoll].Size] = it_tau->chargedHadronIso();
      LepInfo[icoll].NeutralHadronIso  [LepInfo[icoll].Size] = it_tau->neutralHadronIso();
      LepInfo[icoll].PhotonIso         [LepInfo[icoll].Size] = it_tau->photonIso();

      LepInfo[icoll].isPFTau         [LepInfo[icoll].Size] = it_tau->isPFTau();    // YoungKyu 2012-10-16
      //LepInfo[icoll].signalCharge         [LepInfo[icoll].Size] = it_tau->signalCharge();    // YoungKyu 2012-11-08
      //hpsPFTau ID
      LepInfo[icoll].decayModeFinding[LepInfo[icoll].Size] = it_tau->tauID( "decayModeFinding" ); // YoungKyu 2012-10-31
      if( !TurnOffInCMSSW73x )
      { LepInfo[icoll].byVLooseCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID( "byVLooseCombinedIsolationDeltaBetaCorr" ); } // YoungKyu 2012-10-31
      LepInfo[icoll].byLooseCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID( "byLooseCombinedIsolationDeltaBetaCorr3Hits" ); // YoungKyu 2012-10-31
      LepInfo[icoll].byMediumCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID( "byMediumCombinedIsolationDeltaBetaCorr3Hits" ); // YoungKyu 2012-10-31
      LepInfo[icoll].byTightCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID( "byTightCombinedIsolationDeltaBetaCorr3Hits" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstElectronLoose[LepInfo[icoll].Size] = it_tau->tauID( "againstElectronLoose" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstElectronMedium[LepInfo[icoll].Size] = it_tau->tauID( "againstElectronMedium" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstElectronTight[LepInfo[icoll].Size] = it_tau->tauID( "againstElectronTight" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstElectronMVA[LepInfo[icoll].Size] = it_tau->tauID( "againstElectronMVA5category" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstMuonLoose[LepInfo[icoll].Size] = it_tau->tauID( "againstMuonLoose" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstMuonMedium[LepInfo[icoll].Size] = it_tau->tauID( "againstMuonMedium" ); // YoungKyu 2012-10-31
      LepInfo[icoll].againstMuonTight[LepInfo[icoll].Size] = it_tau->tauID( "againstMuonTight" ); // YoungKyu 2012-10-31
      //LepInfo[icoll].againstElectronMVA[LepInfo[icoll].Size] = it_tau->tauID("againstElectronMVA"); // YoungKyu 2012-10-31
      //LepInfo[icoll].byLooseCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID("byLooseCombinedIsolationDeltaBetaCorr"); // YoungKyu 2012-10-31
      //LepInfo[icoll].byMediumCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID("byMediumCombinedIsolationDeltaBetaCorr"); // YoungKyu 2012-10-31
      //LepInfo[icoll].byTightCombinedIsolationDeltaBetaCorr[LepInfo[icoll].Size] = it_tau->tauID("byTightCombinedIsolationDeltaBetaCorr"); // YoungKyu 2012-10-31

      if ( !isData && !skipGenInfo_ ) { //MC                                            // YoungKyu 2012-11-08
         if( debug_ > 15 ) { cout << "   Getting MC information\n"; }                     // YoungKyu 2012-11-08
         const reco::Candidate* gen = it_tau->genLepton();                            // YoungKyu 2012-11-08

         if ( gen != NULL ) {                                                         // YoungKyu 2012-11-08
            LepInfo[icoll].GenPt        [LepInfo[icoll].Size] = gen->pt();             // YoungKyu 2012-11-08
            LepInfo[icoll].GenEta       [LepInfo[icoll].Size] = gen->eta();            // YoungKyu 2012-11-08
            LepInfo[icoll].GenPhi       [LepInfo[icoll].Size] = gen->phi();            // YoungKyu 2012-11-08
            LepInfo[icoll].GenPdgID     [LepInfo[icoll].Size] = gen->pdgId();          // YoungKyu 2012-11-08
         }

      }
      LepInfo[icoll].CandRef[LepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_tau );
      LepInfo[icoll].Size++;
   }

   return true;
}
