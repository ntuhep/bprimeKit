/*******************************************************************************
*
*  Filename    : bprimeKit_photon.cc
*  Description : Fill the ntuple with photon information
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"


using namespace std;
// ------------------------------------------------------------------------------
//   bprimeKit method implementation
// ------------------------------------------------------------------------------
bool
bprimeKit::FillPhoton( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   for( unsigned icoll = 0; icoll < fPhotonCollections.size(); icoll++ ){
      if( icoll >= MAX_PHOCOLLECTIONS ){ break; }
      if( fPhotonList_Hs.size() <= icoll   ){ continue; }
      memset( &fPhotonInfo[icoll], 0x00, sizeof( fPhotonInfo[icoll] ) );
      if( fPhotonList_Hs[icoll]->size() == 3 ){ continue; }

      for( PhotonIterator it_pho = fPhotonList_Hs[icoll]->begin(); it_pho != fPhotonList_Hs[icoll]->end(); it_pho++ ){
         if( fPhotonInfo[icoll].Size >= MAX_PHOTONS ){
            cerr <<  "ERROR: number of photons exceeds the size of array." << endl;
            break;// exit(0);
         }

         if( it_pho->pt() < 10.  ){ continue; }
         if( fabs( it_pho->eta() ) > 2.4 ){ continue; }

         // ------------------------  Filling in generic information  -------------------------
         fPhotonInfo[icoll].Pt                   [fPhotonInfo[icoll].Size] = it_pho->pt();
         fPhotonInfo[icoll].Eta                  [fPhotonInfo[icoll].Size] = it_pho->eta();
         fPhotonInfo[icoll].Phi                  [fPhotonInfo[icoll].Size] = it_pho->phi();
         fPhotonInfo[icoll].TrackIso             [fPhotonInfo[icoll].Size] = it_pho->trackIso();
         fPhotonInfo[icoll].EcalIso              [fPhotonInfo[icoll].Size] = it_pho->ecalIso();
         fPhotonInfo[icoll].HcalIso              [fPhotonInfo[icoll].Size] = it_pho->hcalIso();
         fPhotonInfo[icoll].HoverE               [fPhotonInfo[icoll].Size] = it_pho->hadronicOverEm();
         fPhotonInfo[icoll].SigmaIetaIeta        [fPhotonInfo[icoll].Size] = it_pho->sigmaIetaIeta();
         fPhotonInfo[icoll].hadTowOverEm         [fPhotonInfo[icoll].Size] = it_pho->hadTowOverEm();
         fPhotonInfo[icoll].hcalIsoConeDR04_2012 [fPhotonInfo[icoll].Size] = it_pho->hcalTowerSumEtConeDR04() +
                                                                             ( it_pho->hadronicOverEm() - it_pho->hadTowOverEm() ) * it_pho->superCluster()->energy() / cosh( it_pho->superCluster()->eta() );
         fPhotonInfo[icoll].passelectronveto     [fPhotonInfo[icoll].Size] = (int)it_pho->passElectronVeto();
         fPhotonInfo[icoll].hasPixelSeed         [fPhotonInfo[icoll].Size] = it_pho->hasPixelSeed();
         fPhotonInfo[icoll].r9                   [fPhotonInfo[icoll].Size] = it_pho->r9();

         // -----------------------  Filling in isolation information  ------------------------
         const auto pho = fPhotonList_Hs[icoll]->ptrAt( it_pho - fPhotonList_Hs[icoll]->begin() );
         try {
            fPhotonInfo[icoll].phoPFChIso    [fPhotonInfo[icoll].Size] = ( *fPhotonIsolation_Charged_H )[pho];
            fPhotonInfo[icoll].phoPFPhoIso   [fPhotonInfo[icoll].Size] = ( *fPhotonIsolation_Photon_H )[pho];
            fPhotonInfo[icoll].phoPFNeuIso   [fPhotonInfo[icoll].Size] = ( *fPhotonIsolation_Neutral_H )[pho];
            fPhotonInfo[icoll].sigmaIetaIeta [fPhotonInfo[icoll].Size] = ( *fPhotonSigmaIEtaIEta_H )[pho];
            fPhotonInfo[icoll].phoPassLoose  [fPhotonInfo[icoll].Size] = ( *fPhotonIDLoose )[pho];
            fPhotonInfo[icoll].phoPassMedium [fPhotonInfo[icoll].Size] = ( *fPhotonIDMedium )[pho];
            fPhotonInfo[icoll].phoPassTight  [fPhotonInfo[icoll].Size] = ( *fPhotonIDTight )[pho];

            fPhotonInfo[icoll].isoChEffArea  [fPhotonInfo[icoll].Size]
               = std::max( float(0.0), ( *fPhotonIsolation_Charged_H )[pho] - fEvtInfo.Rho*fPhotonEffectiveArea_ChargeHadron.getEffectiveArea( abs( it_pho->eta() ) ) );
            fPhotonInfo[icoll].isoPhoEffArea [fPhotonInfo[icoll].Size]
               = std::max( float(0.0), ( *fPhotonIsolation_Photon_H )[pho] - fEvtInfo.Rho*fPhotonEffectiveArea_Photons.getEffectiveArea( abs( it_pho->eta() ) ) );
            fPhotonInfo[icoll].isoNeuEffArea [fPhotonInfo[icoll].Size]
               = std::max( float(0.0), ( *fPhotonIsolation_Neutral_H )[pho] - fEvtInfo.Rho*fPhotonEffectiveArea_NeutralHadron.getEffectiveArea( abs( it_pho->eta() ) ) );
         } catch( std::exception& e ){
            cout << "Wierd photon found!!" << endl;
         }

         // ----- Generation MC information  ---------------------------------------------
         if( !fIsData && !fSkipfGenInfo ){
            const reco::Candidate* gen = it_pho->genPhoton();
            if( gen != NULL ){
               fPhotonInfo[icoll].GenPt        [fPhotonInfo[icoll].Size] = gen->pt();
               fPhotonInfo[icoll].GenEta       [fPhotonInfo[icoll].Size] = gen->eta();
               fPhotonInfo[icoll].GenPhi       [fPhotonInfo[icoll].Size] = gen->phi();
               fPhotonInfo[icoll].GenPdgID     [fPhotonInfo[icoll].Size] = gen->pdgId();
            }
         }
         fPhotonInfo[icoll].Size++;
      }
   }

   return true;
}
