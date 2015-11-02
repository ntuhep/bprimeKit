/*******************************************************************************
 *
 *  Filename    : bprimeKit_photon.cc
 *  Description : Fill the ntuple with photon information 
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/

#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation 
//------------------------------------------------------------------------------ 
bool bprimeKit::fillPhoton( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   if( debug_ > 0 ) { cout << "Entering photon sub-sequence" << endl; }
   PhotonIterator    it_pho ;
   
   for( unsigned icoll = 0; icoll < phocollections_.size(); icoll++ ) {
      if( icoll >= MAX_PHOCOLLECTIONS ) { break    ; }
      if( _photonHandleList.size() <= icoll   ) { continue ; }

      memset( &PhotonInfo[icoll], 0x00, sizeof( PhotonInfo[icoll] ) );
      if( debug_ > 0 ) { 
         cout << "Fill photon info, collection " << icoll << " with name " << phocollections_[icoll] << endl
              << " Photon collection size " << _photonHandleList[icoll]->size() << endl; }

      for( PhotonIterator it_pho = _photonHandleList[icoll]->begin(); it_pho != _photonHandleList[icoll]->end(); it_pho++ ) {
         if( debug_ > 1 ) { 
            cout << "  Size " << PhotonInfo[icoll].Size << " photon pt,eta,phi " 
               << it_pho->pt()  << "," << it_pho->eta() << "," << it_pho->phi() << endl; 
         }
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
         PhotonInfo[icoll].r9                   [PhotonInfo[icoll].Size]  = it_pho->r9();

         //-----------------------  Filling in isolation information  ------------------------ 
         if( !runOnB2G  ) {
            const auto pho = _photonHandleList[icoll]->ptrAt( PhotonInfo[icoll].Size );
            PhotonInfo[icoll].phoPFChIso    [PhotonInfo[icoll].Size] = (*phoChargedIsolationMap)[pho] ; 
            PhotonInfo[icoll].phoPFPhoIso   [PhotonInfo[icoll].Size] = (*phoPhotonIsolationMap)[pho] ; 
            PhotonInfo[icoll].phoPFNeuIso   [PhotonInfo[icoll].Size] = (*phoNeutralHadronIsolationMap)[pho] ;
            PhotonInfo[icoll].sigmaIetaIeta [PhotonInfo[icoll].Size] = (*full5x5SigmaIEtaIEtaMap)[pho];
            PhotonInfo[icoll].phoPassLoose  [PhotonInfo[icoll].Size] = (*_photonLooseID)[pho];
            PhotonInfo[icoll].phoPassMedium [PhotonInfo[icoll].Size] = (*_photonMediumID)[pho];
            PhotonInfo[icoll].phoPassTight  [PhotonInfo[icoll].Size] = (*_photonTightID)[pho];
            PhotonInfo[icoll].isoChEffArea  [PhotonInfo[icoll].Size] 
               = std::max( float(0.0) ,(*phoChargedIsolationMap)[pho] - EvtInfo.Rho*effAreaChHadrons_.getEffectiveArea(abs(it_pho->eta())));
            PhotonInfo[icoll].isoPhoEffArea [PhotonInfo[icoll].Size] 
               = std::max( float(0.0) ,(*phoPhotonIsolationMap)[pho] - EvtInfo.Rho*effAreaPhotons_.getEffectiveArea(abs(it_pho->eta()))) ;
            PhotonInfo[icoll].isoNeuEffArea [PhotonInfo[icoll].Size] 
               = std::max( float(0.0) ,(*phoNeutralHadronIsolationMap)[pho] - EvtInfo.Rho*effAreaNeuHadrons_.getEffectiveArea(abs(it_pho->eta())));
         } else {
            PhotonInfo[icoll].phoPFChIso    [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoC"       ) ;
            PhotonInfo[icoll].phoPFPhoIso   [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoP"       ) ;
            PhotonInfo[icoll].phoPFNeuIso   [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoN"       ) ;
            PhotonInfo[icoll].phoPFNeuIso   [PhotonInfo[icoll].Size] = it_pho->userFloat( "sigmaIetaIeta" ) ;
            PhotonInfo[icoll].phoPassLoose  [PhotonInfo[icoll].Size] = it_pho->userFloat( "isLoose"    ) ;
            PhotonInfo[icoll].phoPassMedium [PhotonInfo[icoll].Size] = it_pho->userFloat( "isMedium"   ) ;
            PhotonInfo[icoll].phoPassTight  [PhotonInfo[icoll].Size] = it_pho->userFloat( "isTight"    ) ;
            PhotonInfo[icoll].isoChEffArea  [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoC_EAcor" ) ;
            PhotonInfo[icoll].isoPhoEffArea [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoP_EAcor" ) ;
            PhotonInfo[icoll].isoNeuEffArea [PhotonInfo[icoll].Size] = it_pho->userFloat( "isoN_EAcor" ) ;
         } 

         //----- Generation MC information  ---------------------------------------------
         if ( !isData && !skipGenInfo_ ) {
            if( debug_ > 15 ) { cout << ">>> Photon >>> Getting MC information" << endl ; }
            const reco::Candidate* gen = it_pho->genPhoton();
            if ( gen != NULL ) {
               PhotonInfo[icoll].GenPt        [PhotonInfo[icoll].Size] = gen->pt();
               PhotonInfo[icoll].GenEta       [PhotonInfo[icoll].Size] = gen->eta();
               PhotonInfo[icoll].GenPhi       [PhotonInfo[icoll].Size] = gen->phi();
               PhotonInfo[icoll].GenPdgID     [PhotonInfo[icoll].Size] = gen->pdgId();
            }
         }
         PhotonInfo[icoll].Size++;
      }
   }
   return true;
}
