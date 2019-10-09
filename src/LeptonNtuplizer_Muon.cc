/*******************************************************************************
*
*  Filename    : bprimeKit_muon.cc
*  Description : Filling in the muon informations
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

using namespace std;

void
LeptonNtuplizer::FillMuon( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{

  for( auto it_mu = _muonhandle->begin(); it_mu != _muonhandle->end(); ++it_mu ){
    if( LepInfo.Size >= MAX_LEPTONS ){
      cerr << "ERROR: number of leptons exceeds the size of array." << endl;
      break;
    }

    // ----- Very very loose pre-selection -----------------------------------------------------------------
    if( it_mu->pt() < 5 ) continue;
    if( !(it_mu->isPFMuon() || it_mu->isGlobalMuon() || it_mu->isTrackerMuon()) ) continue;

    // ----- Basic kinematics ------------------------------------------------------------------------------
    LepInfo.Index                     [LepInfo.Size] = LepInfo.Size;
    LepInfo.LeptonType                [LepInfo.Size] = 13;
    LepInfo.Charge                    [LepInfo.Size] = it_mu->charge();
    LepInfo.Energy                    [LepInfo.Size] = it_mu->energy();
    LepInfo.Pt                        [LepInfo.Size] = it_mu->pt();
    LepInfo.Eta                       [LepInfo.Size] = it_mu->eta();
    LepInfo.Phi                       [LepInfo.Size] = it_mu->phi();
    LepInfo.Px                        [LepInfo.Size] = it_mu->px();
    LepInfo.Py                        [LepInfo.Size] = it_mu->py();
    LepInfo.Pz                        [LepInfo.Size] = it_mu->pz();
    LepInfo.MuType                    [LepInfo.Size] = it_mu->type();
    LepInfo.isPFMuon                  [LepInfo.Size] = it_mu->isPFMuon();

    // Pat muon info : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/DataFormats/PatCandidates/interface/Muon.h
    // Reco muon info : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/DataFormats/MuonReco/interface/Muon.h
    // ----- CutBased muon ID ------------------------------------------------------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_selectors_Since_9_4_X
    // Internal code : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/DataFormats/MuonReco/src/MuonSelectors.cc
    // ----- Normal ID -----
    LepInfo.MuonCutBasedMuIdLOOSE          [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdLoose);
    LepInfo.MuonCutBasedMuIdMEDIUM         [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdMedium);
    LepInfo.MuonCutBasedMuIdTIGHT          [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdTight);

    // ----- High pt ID (Non Particle-Flow algorithm) -----
    // Should use "TunePBestTrack" info for kinematics like pt
    LepInfo.MuonCutBasedIdGlobalHighPt     [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdGlobalHighPt);
    LepInfo.MuonCutBasedIdTrkHighPt        [LepInfo.Size] = it_mu->passed(reco::Muon::CutBasedIdTrkHighPt);

    // ----- Good muon selection ---------------------------------------------------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMuonAnalysis#Muon_identification
    // For : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Soft_Muon
    LepInfo.isGoodMuonTMOneStationTight    [LepInfo.Size] = muon::isGoodMuon( *it_mu, muon::TMOneStationTight );
    // For : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#HighPt_Muon
    LepInfo.isGoodMuonGlobalMuonPromptTight[LepInfo.Size] = muon::isGoodMuon( *it_mu, muon::GlobalMuonPromptTight );

    // ----- Muon isolation variables ----------------------------------------------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_Isolation

    // ----- Tracker based isolation (R = 0.3) -----
    LepInfo.TrackerBasedIsoR03     [LepInfo.Size] = it_mu->isolationR03().sumPt;

    // ----- Particle-Flow based isolation with Delta Beta correction (R = 0.4) ----
    LepInfo.ChargedHadronIsoR04    [LepInfo.Size] = it_mu->pfIsolationR04().sumChargedHadronPt;  // = it_mu->chargedHadronIso()
    LepInfo.NeutralHadronIsoR04    [LepInfo.Size] = it_mu->pfIsolationR04().sumNeutralHadronEt;  // = it_mu->neutralHadronIso()
    LepInfo.PhotonIsoR04           [LepInfo.Size] = it_mu->pfIsolationR04().sumPhotonEt;         // = it_mu->photonIso()
    LepInfo.sumPUPtR04             [LepInfo.Size] = it_mu->pfIsolationR04().sumPUPt;
    LepInfo.PFIsoDeltaBetaCorrR04  [LepInfo.Size] = LepInfo.ChargedHadronIsoR04[LepInfo.Size] +
                        max( 0.0, LepInfo.NeutralHadronIsoR04[LepInfo.Size] + LepInfo.PhotonIsoR04[LepInfo.Size] - 0.5 * LepInfo.sumPUPtR04[LepInfo.Size] );

    // ----- MiniPFIsolation -----
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/MiniIsolationSUSY
    // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
    // Modify to rho correction : https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/PatUtils/src/MiniIsolation.cc#L58

    // --- Use Delta Beta corr ---
    // LepInfo.MiniIso [LepInfo.Size]
    //  = bprimeKit::GetMiniPFIso(
    //  _packedhandle,
    //  dynamic_cast<const reco::Candidate*>( &*it_mu ),
    //  0.05,
    //  0.2,
    //  10.,
    //  false
    //  );

    // --- Use rho corr ---
    const double rho      = _rhohandle.isValid() ? (float)(*_rhohandle) : 0.;
    const double AEffR03  = _muonEffectiveAreaR03_NeuHadronAndPhoton.getEffectiveArea( abs( it_mu->pt()) );
    LepInfo.MiniIso [LepInfo.Size] 
        = bprimeKit::GetMiniPFIsoRhoCorr( it_mu->miniPFIsolation(), it_mu->pt(), rho, AEffR03 );

    // ----- Track and impact parameter info ---------------------------------------------------------------
    // Track Info      : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/DataFormats/TrackReco/interface/TrackBase.h
    // HitPattern Info : https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_X/DataFormats/TrackReco/interface/HitPattern.h
    bool hasPV = !_vtxhandle->empty();
    float max_value = numeric_limits<float>::max();

    // ----- Impact paramters -----
    LepInfo.Ip3dPV                      [LepInfo.Size] = hasPV ? it_mu->dB(pat::Muon::PV3D)  : max_value;
    LepInfo.Ip3dPVErr                   [LepInfo.Size] = hasPV ? it_mu->edB(pat::Muon::PV3D) : max_value;
    LepInfo.Ip3dPVSignificance          [LepInfo.Size] = hasPV ? it_mu->dB(pat::Muon::PV3D) / it_mu->edB(pat::Muon::PV3D) : max_value;

    // ----- Muon inner track info -----
    if( it_mu->innerTrack().isNonnull() ){
      LepInfo.MuInnerPt                 [LepInfo.Size] = it_mu->innerTrack()->pt();
      LepInfo.MuInnerEta                [LepInfo.Size] = it_mu->innerTrack()->eta();
      LepInfo.MuInnerPhi                [LepInfo.Size] = it_mu->innerTrack()->phi();
      LepInfo.MuInnerPtError            [LepInfo.Size] = it_mu->innerTrack()->ptError(); 
      LepInfo.innerTracknormalizedChi2  [LepInfo.Size] = it_mu->innerTrack()->normalizedChi2();
      LepInfo.MuInnerTrackDz            [LepInfo.Size] = hasPV ? it_mu->innerTrack()->dz( _vtxhandle->front().position() )  : max_value;
      LepInfo.MuInnerTrackDxy           [LepInfo.Size] = hasPV ? it_mu->innerTrack()->dxy( _vtxhandle->front().position() ) : max_value;
      LepInfo.MuInnerTrackVHitFrac      [LepInfo.Size] = it_mu->innerTrack()->validFraction();
      LepInfo.MuInnerTrackHighPurity    [LepInfo.Size] = it_mu->innerTrack()->quality(reco::TrackBase::highPurity);
      LepInfo.MuInnerTrackNHits         [LepInfo.Size] = it_mu->innerTrack()->numberOfValidHits();
      LepInfo.MuNTrackerHits            [LepInfo.Size] = it_mu->innerTrack()->hitPattern().numberOfValidTrackerHits();
      LepInfo.MuNPixelLayers            [LepInfo.Size] = it_mu->innerTrack()->hitPattern().numberOfValidPixelHits();
      LepInfo.MuNPixelLayersWMeasurement[LepInfo.Size] = it_mu->innerTrack()->hitPattern().pixelLayersWithMeasurement();
      LepInfo.MuNTrackLayersWMeasurement[LepInfo.Size] = it_mu->innerTrack()->hitPattern().trackerLayersWithMeasurement();
    }

    // ----- Global muon infor -----
    if( it_mu->globalTrack().isNonnull() ){
      LepInfo.MuGlobalPt                [LepInfo.Size] = it_mu->globalTrack()->pt();
      LepInfo.MuGlobalEta               [LepInfo.Size] = it_mu->globalTrack()->eta();
      LepInfo.MuGlobalPhi               [LepInfo.Size] = it_mu->globalTrack()->phi();
      LepInfo.MuGlobalPtError           [LepInfo.Size] = it_mu->globalTrack()->ptError();
      LepInfo.MuGlobalNormalizedChi2    [LepInfo.Size] = it_mu->globalTrack()->normalizedChi2();
      LepInfo.MuNMuonhits               [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonHits();
      LepInfo.MuDThits                  [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonDTHits();
      LepInfo.MuCSChits                 [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonCSCHits();
      LepInfo.MuRPChits                 [LepInfo.Size] = it_mu->globalTrack()->hitPattern().numberOfValidMuonRPCHits();
    }
    LepInfo.MuSegCompatibility          [LepInfo.Size] = muon::segmentCompatibility(*it_mu);
    LepInfo.MuSTATKMatchingChi2         [LepInfo.Size] = it_mu->combinedQuality().chi2LocalPosition; 
    LepInfo.MuInnerTrackKinkVal         [LepInfo.Size] = it_mu->combinedQuality().trkKink;
    // Get bit map of stations with matched segments
    // bits 0-1-2-3 = DT stations 1-2-3-4
    // bits 4-5-6-7 = CSC stations 1-2-3-4
    LepInfo.MuStationMask               [LepInfo.Size] = it_mu->stationMask();
    LepInfo.MuNMatchedStations          [LepInfo.Size] = it_mu->numberOfMatchedStations();
    LepInfo.MuExpectedNMatchedStations  [LepInfo.Size] = it_mu->expectedNnumberOfMatchedStations();
    LepInfo.MuNMatchedRPCLayers         [LepInfo.Size] = it_mu->numberOfMatchedRPCLayers();

    // ----- Muon bestTrack info -----
    if( it_mu->muonBestTrack().isNonnull() ){
    // Type {None = 0, InnerTrack = 1, OuterTrack = 2, CombinedTrack = 3, TPFMS = 4, Picky = 5, DYT = 6}
      LepInfo.MuBestTrackType           [LepInfo.Size] = it_mu->muonBestTrackType();
      LepInfo.MuBestTrackPt             [LepInfo.Size] = it_mu->muonBestTrack()->pt();
      LepInfo.MuBestTrackEta            [LepInfo.Size] = it_mu->muonBestTrack()->eta();
      LepInfo.MuBestTrackPhi            [LepInfo.Size] = it_mu->muonBestTrack()->phi();
      LepInfo.MuBestTrackPtError        [LepInfo.Size] = it_mu->muonBestTrack()->ptError();
      LepInfo.MuBestTrackDz             [LepInfo.Size] = hasPV ? it_mu->muonBestTrack()->dz(_vtxhandle->front().position())  : max_value;
      LepInfo.MuBestTrackDxy            [LepInfo.Size] = hasPV ? it_mu->muonBestTrack()->dxy(_vtxhandle->front().position()) : max_value;
      LepInfo.MuBestTrackNMuonhits      [LepInfo.Size] = it_mu->muonBestTrack()->hitPattern().numberOfValidMuonHits();
    }
    
    // ----- Muon tunePBestTrack info (For muon high pt ID) -----
    if( it_mu->tunePMuonBestTrack().isNonnull() ){
    // Type {None = 0, InnerTrack = 1, OuterTrack = 2, CombinedTrack = 3, TPFMS = 4, Picky = 5, DYT = 6}
      LepInfo.MuTunePBestTrackType      [LepInfo.Size] = it_mu->tunePMuonBestTrackType();
      LepInfo.MuTunePBestTrackPt        [LepInfo.Size] = it_mu->tunePMuonBestTrack()->pt();
      LepInfo.MuTunePBestTrackEta       [LepInfo.Size] = it_mu->tunePMuonBestTrack()->eta();
      LepInfo.MuTunePBestTrackPhi       [LepInfo.Size] = it_mu->tunePMuonBestTrack()->phi();
      LepInfo.MuTunePBestTrackPtError   [LepInfo.Size] = it_mu->tunePMuonBestTrack()->ptError();
      LepInfo.MuTunePBestTrackDz        [LepInfo.Size] = hasPV ? it_mu->tunePMuonBestTrack()->dz(_vtxhandle->front().position())  : max_value;
      LepInfo.MuTunePBestTrackDxy       [LepInfo.Size] = hasPV ? it_mu->tunePMuonBestTrack()->dxy(_vtxhandle->front().position()) : max_value;
      LepInfo.MuTunePBestTrackNMuonhits [LepInfo.Size] = it_mu->tunePMuonBestTrack()->hitPattern().numberOfValidMuonHits();
    }

    // ----- Generation Monte Carlo information ------------------------------------------------------------
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
