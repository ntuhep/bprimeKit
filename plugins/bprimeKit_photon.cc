#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "MyAna/bprimeKit/interface/GEDPhoIDTools.h"

// Photon specific CMSSW PACKAGES 
#include "DataFormats/PatCandidates/interface/Photon.h"


std::vector<edm::Handle<std::vector<pat::Photon  >>> PhoHandle;


void bprimeKit::fillPhoton
( const edm::Event& iEvent ,  const edm::EventSetup& iSetup )
{
   for( unsigned il = 0; il < pholabel_.size(); il++ ) {
      PhoHandle.push_back( edm::Handle<std::vector<pat::Photon> >() );
      iEvent.getByLabel( pholabel_[il], PhoHandle[il] );
      if( debug_ > 10 ) { cout << "photons " << il << " pholabel " << pholabel_[il] << " with " << PhoHandle[il]->size() << " entries\n"; }
   }
   
   // Setting up helper variables for photon management
   if( debug_ > 10 ) cout <<"Creating new tool" << endl;
   GEDPhoIDTools* GEDIdTool = new GEDPhoIDTools( iEvent ) ; 
   
   for( unsigned icoll = 0; icoll < phocollections_.size(); icoll++ ) {

      if( icoll >= MAX_PHOCOLLECTIONS ) { break; }
      if( debug_ > 5 ) { cout << "Fill photon info, collection " << icoll << " with name " << phocollections_[icoll] << endl; }

      memset( &PhotonInfo[icoll], 0x00, sizeof( PhotonInfo[icoll] ) );

      if( PhoHandle.size() > icoll ) { //have photon collection
         if( debug_ > 10 ) { cout << " Photon collection size " << PhoHandle[icoll]->size() << endl; }
         for( std::vector<pat::Photon>::const_iterator it_pho = PhoHandle[icoll]->begin();
              it_pho != PhoHandle[icoll]->end(); it_pho++ ) {//loop over photon in collection

            if( debug_ > 11 ) { cout << "  Size " << PhotonInfo[icoll].Size << " photon pt,eta,phi " << it_pho->pt() << "," << it_pho->eta() << "," << it_pho->phi() << endl; }
            if ( PhotonInfo[icoll].Size >= MAX_PHOTONS ) {
               fprintf( stderr, "ERROR: number of photons exceeds the size of array.\n" );
               break;//exit(0);
            }

            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].Pt         [PhotonInfo[icoll].Size] = it_pho->pt();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].Eta        [PhotonInfo[icoll].Size] = it_pho->eta();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].Phi        [PhotonInfo[icoll].Size] = it_pho->phi();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].TrackIso   [PhotonInfo[icoll].Size] = it_pho->trackIso();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].EcalIso    [PhotonInfo[icoll].Size] = it_pho->ecalIso();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].HcalIso    [PhotonInfo[icoll].Size] = it_pho->hcalIso();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].HoverE     [PhotonInfo[icoll].Size] = it_pho->hadronicOverEm();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].SigmaIetaIeta           [PhotonInfo[icoll].Size] = it_pho->sigmaIetaIeta();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].hadTowOverEm            [PhotonInfo[icoll].Size] = it_pho->hadTowOverEm();
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].hcalIsoConeDR04_2012    [PhotonInfo[icoll].Size] = it_pho->hcalTowerSumEtConeDR04() +
                  ( it_pho->hadronicOverEm() - it_pho->hadTowOverEm() ) * it_pho->superCluster()->energy() / cosh( it_pho->superCluster()->eta() );
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            
            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            PhotonInfo[icoll].passelectronveto[PhotonInfo[icoll].Size] = (int) it_pho->passElectronVeto() ;  

            if( debug_ > 11 ) { printf( "[DEBUG] Line%i\n", __LINE__ ); }
            VertexRef myVtxRef( VertexHandle, 0 );
            
            // TODO
            if( debug_ > 10 ) { cout << " B4 Photon getIsolation " << endl; }
            PhotonInfo[icoll].phoPFChIsoDR03[PhotonInfo[icoll].Size]   = GEDIdTool->SolidConeIso( 0.3 , reco::PFCandidate::h     ) ;
            PhotonInfo[icoll].phoPFNeuIsoDR03[PhotonInfo[icoll].Size]  = GEDIdTool->SolidConeIso( 0.3 , reco::PFCandidate::h0    ) ;   
            PhotonInfo[icoll].phoPFPhoIsoDR03[PhotonInfo[icoll].Size]  = GEDIdTool->SolidConeIso( 0.3 , reco::PFCandidate::gamma ) ;  

            if( debug_ > 10 ) { cout << " Af Photon getIsolation " << endl; }
            
            // TODO
            PhotonisolatorR04.fGetIsolation( &*it_pho, &thePfColl, myVtxRef, VertexHandle );
            PhotonInfo[icoll].phoPFChIsoDR04[PhotonInfo[icoll].Size]   = PhotonisolatorR04.getIsolationCharged();
            PhotonInfo[icoll].phoPFNeuIsoDR04[PhotonInfo[icoll].Size]  = PhotonisolatorR04.getIsolationNeutral();
            PhotonInfo[icoll].phoPFPhoIsoDR04[PhotonInfo[icoll].Size]  = PhotonisolatorR04.getIsolationPhoton();

            PhotonInfo[icoll].r9[PhotonInfo[icoll].Size]  = it_pho->r9();

            if ( !isData && !skipGenInfo_ ) { //MC
               if( debug_ > 15 ) { cout << "   Getting MC information\n"; }
               const reco::Candidate* gen = it_pho->genPhoton();

               if ( gen != NULL ) {
                  PhotonInfo[icoll].GenPt        [PhotonInfo[icoll].Size] = gen->pt();
                  PhotonInfo[icoll].GenEta       [PhotonInfo[icoll].Size] = gen->eta();
                  PhotonInfo[icoll].GenPhi       [PhotonInfo[icoll].Size] = gen->phi();
                  PhotonInfo[icoll].GenPdgID     [PhotonInfo[icoll].Size] = gen->pdgId();
               }

            }
            PhotonInfo[icoll].Size++;
         }//loop over photons
      }//have photons collection
   }

   // Removing remaining tool 
   delete GEDIdTool ; 

}
