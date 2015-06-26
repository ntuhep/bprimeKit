/*******************************************************************************
 *
 *  Filename    : bprimeKit_photon.cc
 *  Description : Fill the ntuple with photon information 
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *    
 *
*******************************************************************************/

#include "MyAna/bprimeKit/interface/bprimeKit.h"
//------------------------  Photon specific CMSSW libraries  ------------------------
#include "DataFormats/PatCandidates/interface/Photon.h"

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation 
//------------------------------------------------------------------------------ 
bool bprimeKit::fillPhoton( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   PhotonHandleList  PhoHandle;
   PhotonIterator    it_pho ;
   
   edm::Handle<edm::ValueMap<bool> >  loose_id_decisions;
   edm::Handle<edm::ValueMap<bool> >  medium_id_decisions;
   edm::Handle<edm::ValueMap<bool> >  tight_id_decisions;
   edm::Handle<edm::ValueMap<float> > mvaValues;
   edm::Handle<edm::ValueMap<float> > phoChargedIsolationMap;
   edm::Handle<edm::ValueMap<float> > phoNeutralHadronIsolationMap;
   edm::Handle<edm::ValueMap<float> > phoPhotonIsolationMap;
   if(runphoIDVID_){
      iEvent.getByToken( phoLooseIdMapToken_             , loose_id_decisions           );
      iEvent.getByToken( phoMediumIdMapToken_            , medium_id_decisions          );
      iEvent.getByToken( phoTightIdMapToken_             , tight_id_decisions           );
      iEvent.getByToken( phoMVAValuesMapToken_           , mvaValues                    );
      iEvent.getByToken( phoChargedIsolationToken_       , phoChargedIsolationMap       );
      iEvent.getByToken( phoNeutralHadronIsolationToken_ , phoNeutralHadronIsolationMap );
      iEvent.getByToken( phoPhotonIsolationToken_        , phoPhotonIsolationMap        );
   }
   
   for( unsigned il = 0; il < pholabel_.size(); il++ ) {
      PhoHandle.push_back( PhotonHandle() );
      iEvent.getByLabel( pholabel_[il], PhoHandle[il] );
      if( debug_ > 10 ) { cout << "photons " << il << " pholabel " << pholabel_[il] << " with " << PhoHandle[il]->size() << " entries\n"; }
   }
   
   for( unsigned icoll = 0; icoll < phocollections_.size(); icoll++ ) {
      if( icoll >= MAX_PHOCOLLECTIONS ) { break    ; }
      if( PhoHandle.size() <= icoll   ) { continue ; }

      memset( &PhotonInfo[icoll], 0x00, sizeof( PhotonInfo[icoll] ) );
      if( debug_ > 5 ) { cout << "Fill photon info, collection " << icoll << " with name " << phocollections_[icoll] << endl; } 
      if( debug_ > 10 ) { cout << " Photon collection size " << PhoHandle[icoll]->size() << endl; }


      for( it_pho = PhoHandle[icoll]->begin(); it_pho != PhoHandle[icoll]->end(); it_pho++ ) {//loop over photon in collection
         if( debug_ > 11 ) { cout << "  Size " << PhotonInfo[icoll].Size << " photon pt,eta,phi " << it_pho->pt() << "," << it_pho->eta() << "," << it_pho->phi() << endl; }
         if ( PhotonInfo[icoll].Size >= MAX_PHOTONS ) {
            cerr <<  "ERROR: number of photons exceeds the size of array." << endl; 
            break;//exit(0);
         }
         //------------------------  Filling in generic information  -------------------------
         PhotonInfo[icoll].Pt                   [PhotonInfo[icoll].Size] = it_pho->pt();
         PhotonInfo[icoll].Eta                  [PhotonInfo[icoll].Size] = it_pho->eta();
         PhotonInfo[icoll].Phi                  [PhotonInfo[icoll].Size] = it_pho->phi();
         PhotonInfo[icoll].TrackIso             [PhotonInfo[icoll].Size] = it_pho->trackIso();
         PhotonInfo[icoll].EcalIso              [PhotonInfo[icoll].Size] = it_pho->ecalIso();
         PhotonInfo[icoll].HcalIso              [PhotonInfo[icoll].Size] = it_pho->hcalIso();
         PhotonInfo[icoll].HoverE               [PhotonInfo[icoll].Size] = it_pho->hadronicOverEm();
         PhotonInfo[icoll].SigmaIetaIeta        [PhotonInfo[icoll].Size] = it_pho->sigmaIetaIeta();
         PhotonInfo[icoll].hadTowOverEm         [PhotonInfo[icoll].Size] = it_pho->hadTowOverEm();
         PhotonInfo[icoll].hcalIsoConeDR04_2012 [PhotonInfo[icoll].Size] = it_pho->hcalTowerSumEtConeDR04() +
               ( it_pho->hadronicOverEm() - it_pho->hadTowOverEm() ) * it_pho->superCluster()->energy() / cosh( it_pho->superCluster()->eta() );
         PhotonInfo[icoll].passelectronveto     [PhotonInfo[icoll].Size] = (int) it_pho->passElectronVeto() ;  
        
         //-----------------------  Filling in isolation information  ------------------------ 
         PhotonInfo[icoll].phoPFChIso    [PhotonInfo[icoll].Size] = (*phoChargedIsolationMap)[it_pho] ; 
         PhotonInfo[icoll].phoPFPhoIso   [PhotonInfo[icoll].Size] = (*phoPhotonIsolationMap)[it_pho] ; 
         PhotonInfo[icoll].phoPFNeuIso   [PhotonInfo[icoll].Size] = (*phoNeutralHadronIsolationMap)[it_pho] ;
         PhotonInfo[icoll].phoPassLoose  [PhotonInfo[icoll].Size] = (*loose_id_decisions)[it_pho];
         PhotonInfo[icoll].phoPassMedium [PhotonInfo[icoll].Size] = (*medium_id_decisions)[it_pho];
         PhotonInfo[icoll].phoPassTight  [PhotonInfo[icoll].Size] = (*tight_id_decisions)[it_pho];
         PhotonInfo[icoll].phoIDMVA      [PhotonInfo[icoll].Size] = (*mvaValues)[it_pho];
        

         //---------------------------  Generation MC information  ---------------------------
         if ( !isData && !skipGenInfo_ ) { //MC
            if( debug_ > 15 ) { cout << ">>> Photon >>> Getting MC information" << endl ; }
            const reco::Candidate* gen = it_pho->genPhoton();
            if ( gen != NULL ) {
               PhotonInfo[icoll].GenPt        [PhotonInfo[icoll].Size] = gen->pt();
               PhotonInfo[icoll].GenEta       [PhotonInfo[icoll].Size] = gen->eta();
               PhotonInfo[icoll].GenPhi       [PhotonInfo[icoll].Size] = gen->phi();
               PhotonInfo[icoll].GenPdgID     [PhotonInfo[icoll].Size] = gen->pdgId();
            }
         }

         PhotonInfo[icoll].r9             [PhotonInfo[icoll].Size]  = it_pho->r9();
         PhotonInfo[icoll].Size++;
      }
   }
   return true;
}
