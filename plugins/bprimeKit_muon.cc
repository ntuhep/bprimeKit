/*******************************************************************************
*
*  Filename    : bprimeKit_muon.cc
*  Description : Filling in the muon informations
*
*******************************************************************************/
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h"
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

using namespace std;
// ------------------------------------------------------------------------------
//   Begin bprimeKit muon method implementaion
// ------------------------------------------------------------------------------
bool
bprimeKit::FillMuon( const edm::Event& iEvent, const edm::EventSetup& iSetup, const size_t icoll )
{
   MuonEffectiveArea::MuonEffectiveAreaTarget EATarget = MuonEffectiveArea::kMuEAFall11MC;
   if( fIsData ){ EATarget = MuonEffectiveArea::kMuEAData2012; }

   fMySelectedMuons.clear();

   for( MuonIterator it_mu = fMuonList_Hs[icoll]->begin(); it_mu != fMuonList_Hs[icoll]->end(); ++it_mu ){
      if( fLepInfo[icoll].Size >= MAX_LEPTONS ){
         cerr << "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }
      fLepInfo[icoll].Index                     [fLepInfo[icoll].Size] = fLepInfo[icoll].Size;
      fLepInfo[icoll].LeptonType                [fLepInfo[icoll].Size] = 13;
      fLepInfo[icoll].Charge                    [fLepInfo[icoll].Size] = it_mu->charge();
      fLepInfo[icoll].Energy                    [fLepInfo[icoll].Size] = it_mu->energy();
      fLepInfo[icoll].Pt                        [fLepInfo[icoll].Size] = it_mu->pt();
      fLepInfo[icoll].Eta                       [fLepInfo[icoll].Size] = it_mu->eta();
      fLepInfo[icoll].Phi                       [fLepInfo[icoll].Size] = it_mu->phi();
      fLepInfo[icoll].Px                        [fLepInfo[icoll].Size] = it_mu->px();// Uly 2011-04-04
      fLepInfo[icoll].Py                        [fLepInfo[icoll].Size] = it_mu->py();// Uly 2011-04-04
      fLepInfo[icoll].Pz                        [fLepInfo[icoll].Size] = it_mu->pz();// Uly 2011-04-04
      fLepInfo[icoll].TrackIso                  [fLepInfo[icoll].Size] = it_mu->trackIso();
      fLepInfo[icoll].EcalIso                   [fLepInfo[icoll].Size] = it_mu->ecalIso();
      fLepInfo[icoll].HcalIso                   [fLepInfo[icoll].Size] = it_mu->hcalIso();
      fLepInfo[icoll].isPFMuon                  [fLepInfo[icoll].Size] = it_mu->isPFMuon();
      fLepInfo[icoll].ChargedHadronIso          [fLepInfo[icoll].Size] = it_mu->chargedHadronIso();
      fLepInfo[icoll].NeutralHadronIso          [fLepInfo[icoll].Size] = it_mu->neutralHadronIso();
      fLepInfo[icoll].PhotonIso                 [fLepInfo[icoll].Size] = it_mu->photonIso();
      fLepInfo[icoll].MuType                    [fLepInfo[icoll].Size] = it_mu->type();
      fLepInfo[icoll].MuCaloCompat              [fLepInfo[icoll].Size] = it_mu->caloCompatibility();
      fLepInfo[icoll].MuNChambers               [fLepInfo[icoll].Size] = it_mu->numberOfChambers();
      fLepInfo[icoll].MuNChambersMatchesSegment [fLepInfo[icoll].Size] = it_mu->numberOfMatches();// At least 2 Chambers matched with segments
      fLepInfo[icoll].MuNMatchedStations        [fLepInfo[icoll].Size] = it_mu->numberOfMatchedStations();

      // ----- Good Muon selection  -----------------------------------------------------------------------
      // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Soft_Muon
      fLepInfo[icoll].isGoodMuonTMOneStationTight    [fLepInfo[icoll].Size] = muon::isGoodMuon( *it_mu, muon::TMOneStationTight );

      // ----- MiniPFIsolation -----
      // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
      fLepInfo[icoll].MiniIso [fLepInfo[icoll].Size]
         = bprimeKit::GetMiniPFIsolation(
         fPackedCand_H,
         dynamic_cast<const reco::Candidate*>( &*it_mu ),
         0.05,
         0.2,
         10.,
         false );

      // ----- Muon isolation information  ----------------------------------------------------------------
      //  1. Delta Beta     : I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
      //  2. Rho Correction : https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=188494
      //  Effective Area    : http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h?revision=1.7&view=markup
      if( it_mu->isPFMuon() && it_mu->isPFIsolationValid() ){
         fLepInfo[icoll].ChargedHadronIsoR03  [fLepInfo[icoll].Size] = it_mu->pfIsolationR03().sumChargedHadronPt;
         fLepInfo[icoll].NeutralHadronIsoR03  [fLepInfo[icoll].Size] = it_mu->pfIsolationR03().sumNeutralHadronEt;
         fLepInfo[icoll].PhotonIsoR03         [fLepInfo[icoll].Size] = it_mu->pfIsolationR03().sumPhotonEt;
         fLepInfo[icoll].sumPUPtR03           [fLepInfo[icoll].Size] = it_mu->pfIsolationR03().sumPUPt;

         fLepInfo[icoll].ChargedHadronIsoR04  [fLepInfo[icoll].Size] = it_mu->pfIsolationR04().sumChargedHadronPt;
         fLepInfo[icoll].NeutralHadronIsoR04  [fLepInfo[icoll].Size] = it_mu->pfIsolationR04().sumNeutralHadronEt;
         fLepInfo[icoll].PhotonIsoR04         [fLepInfo[icoll].Size] = it_mu->pfIsolationR04().sumPhotonEt;
         fLepInfo[icoll].sumPUPtR04           [fLepInfo[icoll].Size] = it_mu->pfIsolationR04().sumPUPt;

         double rhoPrime = max( (double)( fEvtInfo.Rho ), 0.0 );

         float AEffR03 = MuonEffectiveArea::GetMuonEffectiveArea( MuonEffectiveArea::kMuGammaAndNeutralHadronIso03, fLepInfo[icoll].Eta[fLepInfo[icoll].Size], EATarget );
         fLepInfo[icoll].IsoRhoCorrR03[fLepInfo[icoll].Size] = fLepInfo[icoll].ChargedHadronIsoR03[fLepInfo[icoll].Size] +
                                                                max( fLepInfo[icoll].NeutralHadronIsoR03[fLepInfo[icoll].Size] + fLepInfo[icoll].PhotonIsoR03[fLepInfo[icoll].Size] - rhoPrime * AEffR03, 0.0 );

         float AEffR04 = MuonEffectiveArea::GetMuonEffectiveArea( MuonEffectiveArea::kMuGammaAndNeutralHadronIso04, fLepInfo[icoll].Eta[fLepInfo[icoll].Size], EATarget );
         fLepInfo[icoll].IsoRhoCorrR04 [fLepInfo[icoll].Size] = fLepInfo[icoll].ChargedHadronIsoR04[fLepInfo[icoll].Size] +
                                                                max( fLepInfo[icoll].NeutralHadronIsoR04[fLepInfo[icoll].Size] + fLepInfo[icoll].PhotonIsoR04[fLepInfo[icoll].Size] - rhoPrime * AEffR04, 0.0 );
      }

      // ----- Cosmic ray filters  ------------------------------------------------------------------------
      if( it_mu->isTimeValid() ){
         // ----- Number of muon stations used  --------------------------------------------------------------
         fLepInfo[icoll].MuontimenDof[fLepInfo[icoll].Size] = it_mu->time().nDof;

         // ----- Tim arrival at the impact point for the beta=1 hypothesis  ---------------------------------
         // a) particle is moving from inside out
         // b) particle is moving from outside in
         // Direction:  Outside In = -1, Undefined = 0, Inside Out = 1
         fLepInfo[icoll].MuontimeAtIpInOut[fLepInfo[icoll].Size] = it_mu->time().timeAtIpInOut;
         fLepInfo[icoll].MuontimeAtIpOutIn[fLepInfo[icoll].Size] = it_mu->time().timeAtIpOutIn;
         fLepInfo[icoll].Muondirection    [fLepInfo[icoll].Size] = it_mu->time().direction();
      }
      reco::MuonEnergy muEnergy = it_mu->calEnergy();
      fLepInfo[icoll].CaloEnergy [fLepInfo[icoll].Size]                        = muEnergy.em + muEnergy.had + muEnergy.ho;
      fLepInfo[icoll].MuIDGlobalMuonPromptTight         [fLepInfo[icoll].Size] = it_mu->muonID( "GlobalMuonPromptTight" );

      // ----- Track and impact parameter information  ----------------------------------------------------
      // Only valid for global Muons and tracker Muon
      fLepInfo[icoll].Ip3dPV             [fLepInfo[icoll].Size] = -10000;
      fLepInfo[icoll].Ip3dPVErr          [fLepInfo[icoll].Size] = -10000;
      fLepInfo[icoll].Ip3dPVSignificance [fLepInfo[icoll].Size] = -10000;
      if( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x04 ) ){
         fLepInfo[icoll].innerTracknormalizedChi2 [fLepInfo[icoll].Size] = it_mu->innerTrack()->normalizedChi2();
         fLepInfo[icoll].MuInnerPtError           [fLepInfo[icoll].Size] = it_mu->innerTrack()->ptError();
         fLepInfo[icoll].MuInnerTrackDz           [fLepInfo[icoll].Size] = it_mu->innerTrack()->dz( fPrimaryVertex.position() );
         fLepInfo[icoll].MuInnerTrackD0           [fLepInfo[icoll].Size] = it_mu->innerTrack()->d0();
         fLepInfo[icoll].MuInnerTrackDxy_BS       [fLepInfo[icoll].Size] = it_mu->innerTrack()->dxy( fBeamSpot.position() );
         fLepInfo[icoll].MuInnerTrackDxy_PV       [fLepInfo[icoll].Size] = it_mu->innerTrack()->dxy( fPrimaryVertex.position() );
         fLepInfo[icoll].MuInnerTrackDxy_PVBS     [fLepInfo[icoll].Size] = it_mu->innerTrack()->dxy( fPrimaryVertex_BS.position() );
         fLepInfo[icoll].MuInnerTrackNHits        [fLepInfo[icoll].Size] = it_mu->innerTrack()->numberOfValidHits();
         fLepInfo[icoll].MuNTrackerHits           [fLepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().numberOfValidTrackerHits();
         fLepInfo[icoll].MuNPixelLayers           [fLepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().numberOfValidPixelHits();
         // not valid (https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/TrackReco/interface/HitPattern.h)
         fLepInfo[icoll].MuNLostInnerHits     [fLepInfo[icoll].Size]      = -1;
         fLepInfo[icoll].vertexZ              [fLepInfo[icoll].Size]      = it_mu->vertex().z();// Uly 2011-04-04
         fLepInfo[icoll].MuNPixelLayersWMeasurement[fLepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().pixelLayersWithMeasurement();// Uly 2011-04-04
         fLepInfo[icoll].MuNTrackLayersWMeasurement[fLepInfo[icoll].Size] = it_mu->innerTrack()->hitPattern().trackerLayersWithMeasurement();

         // ----- Impact paramters  --------------------------------------------------------------------------
         // const reco::TransientTrack& tt_mu = (fTrackBuilder_H.product())->build( it_mu->track() );
         // reco::Vertex thevtx = (fVertex_H.product())->at( 0 );
         // const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt_mu, thevtx );
         // const double thesign   = ( ( -it_mu->track()->dxy( thevtx.position() ) ) >= 0 ) ? 1. : -1.;
         // fLepInfo[icoll].Ip3dPV             [fLepInfo[icoll].Size] = thesign * ip3dpv.second.value();
         // fLepInfo[icoll].Ip3dPVErr          [fLepInfo[icoll].Size] = ip3dpv.second.error();
         // fLepInfo[icoll].Ip3dPVSignificance [fLepInfo[icoll].Size] = thesign * ip3dpv.second.value() / ip3dpv.second.error();
      }

      // ----- Global muon specific parameters  -----------------------------------------------------------
      if( it_mu->type() & 0x02 ){
         fLepInfo[icoll].MuGlobalPtError        [fLepInfo[icoll].Size] = it_mu->globalTrack()->ptError();
         fLepInfo[icoll].MuGlobalNormalizedChi2 [fLepInfo[icoll].Size] = it_mu->globalTrack()->normalizedChi2();
         fLepInfo[icoll].MuNMuonhits            [fLepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonHits();
         fLepInfo[icoll].MuDThits               [fLepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonDTHits();
         fLepInfo[icoll].MuCSChits              [fLepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonCSCHits();
         fLepInfo[icoll].MuRPChits              [fLepInfo[icoll].Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonRPCHits();
      }
      if( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x08 ) ){
         fLepInfo[icoll].MuNLostOuterHits [fLepInfo[icoll].Size] = -1;
      }

      // ----- Generation Monte Carlo information  --------------------------------------------------------
      if( !fIsData && !fSkipfGenInfo ){
         const reco::GenParticle* gen = it_mu->genLepton();
         if( gen != NULL ){
            fLepInfo[icoll].GenPt    [fLepInfo[icoll].Size] = gen->pt();
            fLepInfo[icoll].GenEta   [fLepInfo[icoll].Size] = gen->eta();
            fLepInfo[icoll].GenPhi   [fLepInfo[icoll].Size] = gen->phi();
            fLepInfo[icoll].GenPdgID [fLepInfo[icoll].Size] = gen->pdgId();
            fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size]  = GetGenMCTag( gen );
         }
      }
      if( fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] == 0 && !fIsData && !fSkipfGenInfo ){
         for( GenIterator it_gen = fGenParticle_H->begin(); it_gen != fGenParticle_H->end(); it_gen++ ){
            if( fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] != 0 ){ break; }
            fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] = GetGenMCTag( it_gen, it_mu );
         }
      }

      if( IsSelectedMuon( it_mu ) ){ fMySelectedMuons.push_back( it_mu ); }

      fLepInfo[icoll].Size++;

   }

   return true;
}
