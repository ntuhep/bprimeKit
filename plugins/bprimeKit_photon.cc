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
//   Helper static variables and functions 
//------------------------------------------------------------------------------ 
static PhotonHandleList  PhoHandle;
static PhotonIterator    it_pho ;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation 
//------------------------------------------------------------------------------ 
bool bprimeKit::fillPhoton( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
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


         //------------------------------------------------------------------------------ 
         //   TODO Debugging
         //------------------------------------------------------------------------------ 
         
         //-----------------------  Filling in isolation information  ------------------------
         VertexRef myVtxRef( VertexHandle, 0 ); 
         // TODO
         if( debug_ > 10 ) { cout << ">>>Photon>>> B4 Photon getIsolation " << endl; }
         // PhotonisolatorR03.fGetIsolation(&*it_pho, &thePfColl, myVtxRef, VertexHandle);
         PhotonInfo[icoll].phoPFChIsoDR03[PhotonInfo[icoll].Size]  = PhotonisolatorR03.getIsolationCharged();
         PhotonInfo[icoll].phoPFNeuIsoDR03[PhotonInfo[icoll].Size]  = PhotonisolatorR03.getIsolationNeutral();
         PhotonInfo[icoll].phoPFPhoIsoDR03[PhotonInfo[icoll].Size]  = PhotonisolatorR03.getIsolationPhoton();
         if( debug_ > 10 ) { cout << ">>> Photon >>> Af Photon getIsolation " << endl; }
         // PhotonisolatorR04.fGetIsolation( &*it_pho, &thePfColl, myVtxRef, VertexHandle );
         PhotonInfo[icoll].phoPFChIsoDR04 [PhotonInfo[icoll].Size]  = PhotonisolatorR04.getIsolationCharged();
         PhotonInfo[icoll].phoPFNeuIsoDR04[PhotonInfo[icoll].Size]  = PhotonisolatorR04.getIsolationNeutral();
         PhotonInfo[icoll].phoPFPhoIsoDR04[PhotonInfo[icoll].Size]  = PhotonisolatorR04.getIsolationPhoton();
         PhotonInfo[icoll].r9             [PhotonInfo[icoll].Size]  = it_pho->r9();
         PhotonInfo[icoll].Size++;
      }
   }
   return true;
}
