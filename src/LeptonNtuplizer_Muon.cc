/*******************************************************************************
*
*  Filename    : bprimeKit_muon.cc
*  Description : Filling in the muon informations
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

using namespace std;

void
LeptonNtuplizer::FillMuon( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  MuonEffectiveArea::MuonEffectiveAreaTarget EATarget
    = iEvent.isRealData() ? MuonEffectiveArea::kMuEAData2012 :
      MuonEffectiveArea::kMuEAFall11MC;

  for( auto it_mu = _muonhandle->begin(); it_mu != _muonhandle->end(); ++it_mu ){
    if( LepInfo.Size >= MAX_LEPTONS ){
      cerr << "ERROR: number of leptons exceeds the size of array." << endl;
      break;
    }

    LepInfo.Index                     [LepInfo.Size] = LepInfo.Size;
    LepInfo.LeptonType                [LepInfo.Size] = 13;
    LepInfo.Charge                    [LepInfo.Size] = it_mu->charge();
    LepInfo.Energy                    [LepInfo.Size] = it_mu->energy();
    LepInfo.Pt                        [LepInfo.Size] = it_mu->pt();
    LepInfo.Eta                       [LepInfo.Size] = it_mu->eta();
    LepInfo.Phi                       [LepInfo.Size] = it_mu->phi();
    LepInfo.Px                        [LepInfo.Size] = it_mu->px();// Uly 2011-04-04
    LepInfo.Py                        [LepInfo.Size] = it_mu->py();// Uly 2011-04-04
    LepInfo.Pz                        [LepInfo.Size] = it_mu->pz();// Uly 2011-04-04
    LepInfo.TrackIso                  [LepInfo.Size] = it_mu->trackIso();
    LepInfo.EcalIso                   [LepInfo.Size] = it_mu->ecalIso();
    LepInfo.HcalIso                   [LepInfo.Size] = it_mu->hcalIso();
    LepInfo.isPFMuon                  [LepInfo.Size] = it_mu->isPFMuon();
    LepInfo.ChargedHadronIso          [LepInfo.Size] = it_mu->chargedHadronIso();
    LepInfo.NeutralHadronIso          [LepInfo.Size] = it_mu->neutralHadronIso();
    LepInfo.PhotonIso                 [LepInfo.Size] = it_mu->photonIso();
    LepInfo.MuType                    [LepInfo.Size] = it_mu->type();
    LepInfo.MuCaloCompat              [LepInfo.Size] = it_mu->caloCompatibility();
    LepInfo.MuNChambers               [LepInfo.Size] = it_mu->numberOfChambers();
    LepInfo.MuNChambersMatchesSegment [LepInfo.Size] = it_mu->numberOfMatches();// At least 2 Chambers matched with segments
    LepInfo.MuNMatchedStations        [LepInfo.Size] = it_mu->numberOfMatchedStations();

    // ----- Good Muon selection  -----------------------------------------------------------------------
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Soft_Muon
    LepInfo.isGoodMuonTMOneStationTight    [LepInfo.Size] = muon::isGoodMuon( *it_mu, muon::TMOneStationTight );

    // ----- CutBased Muon ID --------------------------------------------------------------------------
    LepInfo.MuonCutBasedMuIdLOOSE          [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdLoose);
    LepInfo.MuonCutBasedMuIdMEDIUM         [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdMedium);
    LepInfo.MuonCutBasedMuIdTIGHT          [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdTight);

    // ----- MiniPFIsolation -----
    // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
    LepInfo.MiniIso [LepInfo.Size]
      = bprimeKit::GetMiniPFIsolation(
      _packedhandle,
      dynamic_cast<const reco::Candidate*>( &*it_mu ),
      0.05,
      0.2,
      10.,
      false
      );

    // ----- Muon isolation information  ----------------------------------------------------------------
    //  1. Delta Beta     : I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt]/pt
    //  2. Rho Correction : https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=188494
    //  Effective Area    : http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/sixie/Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h?revision=1.7&view=markup
    if( it_mu->isPFMuon() && it_mu->isPFIsolationValid() ){
      LepInfo.ChargedHadronIsoR03  [LepInfo.Size] = it_mu->pfIsolationR03().sumChargedHadronPt;
      LepInfo.NeutralHadronIsoR03  [LepInfo.Size] = it_mu->pfIsolationR03().sumNeutralHadronEt;
      LepInfo.PhotonIsoR03         [LepInfo.Size] = it_mu->pfIsolationR03().sumPhotonEt;
      LepInfo.sumPUPtR03           [LepInfo.Size] = it_mu->pfIsolationR03().sumPUPt;

      LepInfo.ChargedHadronIsoR04  [LepInfo.Size] = it_mu->pfIsolationR04().sumChargedHadronPt;
      LepInfo.NeutralHadronIsoR04  [LepInfo.Size] = it_mu->pfIsolationR04().sumNeutralHadronEt;
      LepInfo.PhotonIsoR04         [LepInfo.Size] = it_mu->pfIsolationR04().sumPhotonEt;
      LepInfo.sumPUPtR04           [LepInfo.Size] = it_mu->pfIsolationR04().sumPUPt;

      const double rhoPrime = std::max( *_rhohandle, 0.0 );

      float AEffR03 = MuonEffectiveArea::GetMuonEffectiveArea(
        MuonEffectiveArea::kMuGammaAndNeutralHadronIso03,
        LepInfo.Eta[LepInfo.Size],
        EATarget );
      LepInfo.IsoRhoCorrR03[LepInfo.Size] = LepInfo.ChargedHadronIsoR03[LepInfo.Size] +
                                             max( LepInfo.NeutralHadronIsoR03[LepInfo.Size] + LepInfo.PhotonIsoR03[LepInfo.Size] - rhoPrime * AEffR03, 0.0 );

      float AEffR04 = MuonEffectiveArea::GetMuonEffectiveArea(
        MuonEffectiveArea::kMuGammaAndNeutralHadronIso04,
        LepInfo.Eta[LepInfo.Size],
        EATarget );
      LepInfo.IsoRhoCorrR04 [LepInfo.Size] = LepInfo.ChargedHadronIsoR04[LepInfo.Size] +
                                             max( LepInfo.NeutralHadronIsoR04[LepInfo.Size] + LepInfo.PhotonIsoR04[LepInfo.Size] - rhoPrime * AEffR04, 0.0 );
    }

    // ----- Cosmic ray filters  ------------------------------------------------------------------------
    if( it_mu->isTimeValid() ){
      // ----- Number of muon stations used  --------------------------------------------------------------
      LepInfo.MuontimenDof[LepInfo.Size] = it_mu->time().nDof;

      // ----- Tim arrival at the impact point for the beta=1 hypothesis  ---------------------------------
      // a) particle is moving from inside out
      // b) particle is moving from outside in
      // Direction:  Outside In = -1, Undefined = 0, Inside Out = 1
      LepInfo.MuontimeAtIpInOut[LepInfo.Size] = it_mu->time().timeAtIpInOut;
      LepInfo.MuontimeAtIpOutIn[LepInfo.Size] = it_mu->time().timeAtIpOutIn;
      LepInfo.Muondirection    [LepInfo.Size] = it_mu->time().direction();
    }
    reco::MuonEnergy muEnergy = it_mu->calEnergy();
    LepInfo.CaloEnergy [LepInfo.Size]                        = muEnergy.em + muEnergy.had + muEnergy.ho;
    LepInfo.MuIDGlobalMuonPromptTight         [LepInfo.Size] = it_mu->muonID( "GlobalMuonPromptTight" );

    // ----- Track and impact parameter information  ----------------------------------------------------
    // Only valid for global Muons and tracker Muon
    LepInfo.Ip3dPV             [LepInfo.Size] = -10000;
    LepInfo.Ip3dPVErr          [LepInfo.Size] = -10000;
    LepInfo.Ip3dPVSignificance [LepInfo.Size] = -10000;
    if( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x04 ) ){
      LepInfo.innerTracknormalizedChi2 [LepInfo.Size] = it_mu->innerTrack()->normalizedChi2();
      LepInfo.MuInnerPtError           [LepInfo.Size] = it_mu->innerTrack()->ptError();
      LepInfo.MuInnerTrackDz           [LepInfo.Size] = it_mu->innerTrack()->dz( _vtxhandle->front().position() );
      LepInfo.MuInnerTrackD0           [LepInfo.Size] = it_mu->innerTrack()->d0();
      LepInfo.MuInnerTrackDxy_BS       [LepInfo.Size] = it_mu->innerTrack()->dxy( _beamspothandle->position() );
      LepInfo.MuInnerTrackDxy_PV       [LepInfo.Size] = it_mu->innerTrack()->dxy( _vtxhandle->front().position() );
      LepInfo.MuInnerTrackDxy_PVBS     [LepInfo.Size] = it_mu->innerTrack()->dxy( _vtxhandle->front().position() );
      LepInfo.MuInnerTrackNHits        [LepInfo.Size] = it_mu->innerTrack()->numberOfValidHits();
      LepInfo.MuNTrackerHits           [LepInfo.Size] = it_mu->innerTrack()->hitPattern().numberOfValidTrackerHits();
      LepInfo.MuNPixelLayers           [LepInfo.Size] = it_mu->innerTrack()->hitPattern().numberOfValidPixelHits();
      // not valid (https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/TrackReco/interface/HitPattern.h)
      LepInfo.MuNLostInnerHits     [LepInfo.Size]      = -1;
      LepInfo.vertexZ              [LepInfo.Size]      = it_mu->vertex().z();   // Uly 2011-04-04
      LepInfo.MuNPixelLayersWMeasurement[LepInfo.Size] = it_mu->innerTrack()->hitPattern().pixelLayersWithMeasurement();// Uly 2011-04-04
      LepInfo.MuNTrackLayersWMeasurement[LepInfo.Size] = it_mu->innerTrack()->hitPattern().trackerLayersWithMeasurement();

      // ----- Impact paramters  --------------------------------------------------------------------------
      // const reco::TransientTrack& tt_mu = (fTrackBuilder_H.product())->build( it_mu->track() );
      // reco::Vertex thevtx = (fVertex_H.product())->at( 0 );
      // const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt_mu, thevtx );
      // const double thesign   = ( ( -it_mu->track()->dxy( thevtx.position() ) ) >= 0 ) ? 1. : -1.;
      // LepInfo.Ip3dPV             [LepInfo.Size] = thesign * ip3dpv.second.value();
      // LepInfo.Ip3dPVErr          [LepInfo.Size] = ip3dpv.second.error();
      // LepInfo.Ip3dPVSignificance [LepInfo.Size] = thesign * ip3dpv.second.value() / ip3dpv.second.error();
    }

    // ----- Global muon specific parameters  -----------------------------------------------------------
    if( it_mu->type() & 0x02 ){
      LepInfo.MuGlobalPtError        [LepInfo.Size] = it_mu->globalTrack()->ptError();
      LepInfo.MuGlobalNormalizedChi2 [LepInfo.Size] = it_mu->globalTrack()->normalizedChi2();
      LepInfo.MuNMuonhits            [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonHits();
      LepInfo.MuDThits               [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonDTHits();
      LepInfo.MuCSChits              [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonCSCHits();
      LepInfo.MuRPChits              [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonRPCHits();
    }
    if( ( it_mu->type() & 0x02 ) || ( it_mu->type() & 0x08 ) ){
      LepInfo.MuNLostOuterHits [LepInfo.Size] = -1;
    }

    // ----- Generation Monte Carlo information  --------------------------------------------------------
    if( !iEvent.isRealData() ){
      const reco::GenParticle* gen = it_mu->genLepton();
      if( gen != NULL ){
        LepInfo.GenPt    [LepInfo.Size] = gen->pt();
        LepInfo.GenEta   [LepInfo.Size] = gen->eta();
        LepInfo.GenPhi   [LepInfo.Size] = gen->phi();
        LepInfo.GenPdgID [LepInfo.Size] = gen->pdgId();
        LepInfo.GenMCTag [LepInfo.Size] = bprimeKit::GetGenMCTag( gen );
      }
    }
    if( LepInfo.GenMCTag[LepInfo.Size] == 0 && !iEvent.isRealData() ){
      LepInfo.GenMCTag [LepInfo.Size] = GetGenMCTag( it_mu );
    }
    LepInfo.Size++;
  }
}
