/*******************************************************************************
*
*  Filename    : bprimeKit_electron.cc
*  Description : Filling in the electrons
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

// -----------------------  Electron specific CMSSW libraries  -----------------------
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

using namespace std;

/******************************************************************************/

void
LeptonNtuplizer::FillElectron( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  for( auto it_el = _electronhandle->begin(); it_el != _electronhandle->end(); ++it_el ){
    if( LepInfo.Size >= MAX_LEPTONS ){
      cerr << "ERROR: number of leptons exceeds the size of array." << endl;
      break;
    }
    if ( it_el->pt() < 5 ) continue;

    // ----- Inserting generic information  ----------------------------------
    LepInfo.LeptonType        [LepInfo.Size] = 11;
    LepInfo.Index             [LepInfo.Size] = LepInfo.Size;
    LepInfo.Charge            [LepInfo.Size] = it_el->charge();
    LepInfo.Pt                [LepInfo.Size] = it_el->pt();
    LepInfo.Et                [LepInfo.Size] = it_el->et();
    LepInfo.Eta               [LepInfo.Size] = it_el->eta();
    LepInfo.Phi               [LepInfo.Size] = it_el->phi();
    LepInfo.Px                [LepInfo.Size] = it_el->px();
    LepInfo.Py                [LepInfo.Size] = it_el->py();
    LepInfo.Pz                [LepInfo.Size] = it_el->pz();
    LepInfo.Energy            [LepInfo.Size] = it_el->energy();

    // ----- Energy scale and smear correction and associated uncertainties -------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2
    // Internal Code : https://github.com/cms-sw/cmssw/blob/CMSSW_9_4_X/RecoEgamma/EgammaTools/src/ElectronEnergyCalibrator.cc#L97
    LepInfo.ElEnergyCorrFactor          [LepInfo.Size] = it_el->userFloat("ecalTrkEnergyPostCorr") / it_el->energy();
    LepInfo.ElEnergyPreCorrErr          [LepInfo.Size] = it_el->userFloat("ecalTrkEnergyErrPreCorr");
    LepInfo.ElEnergyPostCorrErr         [LepInfo.Size] = it_el->userFloat("ecalTrkEnergyErrPostCorr"); 
    LepInfo.ElEnergyPostCorrScaleUp     [LepInfo.Size] = it_el->userFloat("energyScaleUp");
    LepInfo.ElEnergyPostCorrScaleDown   [LepInfo.Size] = it_el->userFloat("energyScaleDown");
    LepInfo.ElEnergyPostCorrSmearUp     [LepInfo.Size] = it_el->userFloat("energySigmaUp");
    LepInfo.ElEnergyPostCorrSmearDown   [LepInfo.Size] = it_el->userFloat("energySigmaDown");

    // ----- Cut based electron ID  ------------------------------------------------------------------------
    // NOTE : This is Cut-based ID V1 for 2017. V2 need to be done by user.
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Working_points_for_92X_samples_R
    //const edm::Ptr<pat::Electron> el( _electronhandle, it_el - _electronhandle->begin() );
    //LepInfo.EgammaCutBasedEleIdVETO     [LepInfo.Size] = (int)( ( *_electronIDVeto )[el] );
    //LepInfo.EgammaCutBasedEleIdLOOSE    [LepInfo.Size] = (int)( ( *_electronIDLoose )[el] );
    //LepInfo.EgammaCutBasedEleIdMEDIUM   [LepInfo.Size] = (int)( ( *_electronIDMedium )[el] );
    //LepInfo.EgammaCutBasedEleIdTIGHT    [LepInfo.Size] = (int)( ( *_electronIDTight )[el] );
    //LepInfo.EgammaCutBasedEleIdHEEP     [LepInfo.Size] = (int)( ( *_electronIDHEEP )[el] );
    LepInfo.EgammaCutBasedEleIdVETO     [LepInfo.Size] = (int) it_el->electronID( _electronID_vetomap   ); 
    LepInfo.EgammaCutBasedEleIdLOOSE    [LepInfo.Size] = (int) it_el->electronID( _electronID_loosemap  ); 
    LepInfo.EgammaCutBasedEleIdMEDIUM   [LepInfo.Size] = (int) it_el->electronID( _electronID_mediummap ); 
    LepInfo.EgammaCutBasedEleIdTIGHT    [LepInfo.Size] = (int) it_el->electronID( _electronID_tightmap  ); 
    LepInfo.EgammaCutBasedEleIdHEEP     [LepInfo.Size] = (int) it_el->electronID( _electronID_HEEPmap   ); 

    // ----- Isolation variables  --------------------------------------------------------------------------

    // ----- Non particle flow isolation variables -----
    LepInfo.TrackIso          [LepInfo.Size] = it_el->dr03TkSumPt();
    LepInfo.EcalIso           [LepInfo.Size] = it_el->dr03EcalRecHitSumEt();
    LepInfo.HcalIso           [LepInfo.Size] = it_el->dr03HcalTowerSumEt();
    LepInfo.HcalDepth1Iso     [LepInfo.Size] = it_el->dr03HcalDepth1TowerSumEt();
    LepInfo.HcalDepth2Iso     [LepInfo.Size] = it_el->dr03HcalDepth2TowerSumEt();
    LepInfo.HEEPTrackIso      [LepInfo.Size] = it_el->userFloat("heepTrkPtIso");

    // ----- Non particle flow isolation (R = 0.3) variables -----
    LepInfo.ChargedHadronIsoR03         [LepInfo.Size] = it_el->pfIsolationVariables().sumChargedHadronPt;
    LepInfo.NeutralHadronIsoR03         [LepInfo.Size] = it_el->pfIsolationVariables().sumPhotonEt;
    LepInfo.PhotonIsoR03                [LepInfo.Size] = it_el->pfIsolationVariables().sumNeutralHadronEt;
    LepInfo.sumPUPtR03                  [LepInfo.Size] = it_el->pfIsolationVariables().sumPUPt;

    const double rho      = _rhohandle.isValid() ? (float)(*_rhohandle) : 0;
    const double AEffR03  = _electronEffectiveAreaR03_NeuHadronAndPhoton.getEffectiveArea( abs( it_el->superCluster()->eta() ) );
    LepInfo.IsoRhoCorrR03[LepInfo.Size] = LepInfo.ChargedHadronIsoR03[LepInfo.Size] +
                                          max( 0.0, (double)( LepInfo.NeutralHadronIsoR03[LepInfo.Size] + LepInfo.PhotonIsoR03[LepInfo.Size] - rho * AEffR03 ) );

    // ----- MiniPFIsolation -----
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMS/MiniIsolationSUSY
    // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
    // Modify to rho correction : https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/PatUtils/src/MiniIsolation.cc#L58
    LepInfo.MiniIso [LepInfo.Size] 
        = bprimeKit::GetMiniPFIsoRhoCorr( it_el->miniPFIsolation(), it_el->pt(), rho, AEffR03 );

    // ----- Special isolation variables for 2016 HLT safe selection -----
    LepInfo.EcalPFIsoRhoCorr [LepInfo.Size] = max( 0.0, it_el->ecalPFClusterIso() - rho * AEffR03 );
    LepInfo.HcalPFIsoRhoCorr [LepInfo.Size] = max( 0.0, it_el->hcalPFClusterIso() - rho * AEffR03 );
    
    //Electron track and supercluster format : https://github.com/cms-sw/cmssw/blob/CMSSW_9_4_X/DataFormats/EgammaCandidates/interface/GsfElectron.h

    // ----- Electron superCluster and shrower shape (Full 5x5, no ZS version) variables -------------------
    // ----- superCluster variables -----
    LepInfo.EtaSC          [LepInfo.Size] = it_el->superCluster()->eta();
    LepInfo.PhiSC          [LepInfo.Size] = it_el->superCluster()->phi();
    LepInfo.rawEnergySC    [LepInfo.Size] = it_el->superCluster()->rawEnergy();
    LepInfo.EnergySC       [LepInfo.Size] = it_el->superCluster()->energy();
    LepInfo.ESEnergySC     [LepInfo.Size] = it_el->superCluster()->preshowerEnergy();

    // ----- shrower shape (Full 5x5, no ZS version) variables -----
    LepInfo.R9             [LepInfo.Size] = it_el->full5x5_r9();
    LepInfo.E1x5           [LepInfo.Size] = it_el->full5x5_e1x5();
    LepInfo.E2x5Max        [LepInfo.Size] = it_el->full5x5_e2x5Max();
    LepInfo.E5x5           [LepInfo.Size] = it_el->full5x5_e5x5();
    LepInfo.SigmaIetaIeta  [LepInfo.Size] = it_el->full5x5_sigmaIetaIeta();
    LepInfo.SigmaIetaIphi  [LepInfo.Size] = it_el->full5x5_showerShape().sigmaIetaIphi;
    LepInfo.SigmaIphiIphi  [LepInfo.Size] = it_el->full5x5_sigmaIphiIphi();
    LepInfo.EtaWidth       [LepInfo.Size] = it_el->superCluster()->etaWidth();
    LepInfo.PhiWidth       [LepInfo.Size] = it_el->superCluster()->phiWidth();
    LepInfo.ElHadoverEm    [LepInfo.Size] = it_el->hadronicOverEm();
 
    /////////////////LepInfo.vertexZ           [LepInfo.Size] = it_el->vertex().z();// Uly 2011-04-04

    // ----- Electron GSF track informations ---------------------------------------------------------------
    // Twiki : https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGsfElectronObject
    // HitPattern format : https://github.com/cms-sw/cmssw/blob/CMSSW_9_4_X/DataFormats/TrackReco/interface/HitPattern.h
    // ----- Basic variables -----
    LepInfo.isEcalDriven               [LepInfo.Size] = it_el->ecalDrivenSeed();    
    LepInfo.isTrackerDriven            [LepInfo.Size] = it_el->trackerDrivenSeed();
    LepInfo.ElTightCharge              [LepInfo.Size] = it_el->isGsfScPixChargeConsistent() + it_el->isGsfCtfScPixChargeConsistent();
    LepInfo.ElMomentum                 [LepInfo.Size] = it_el->trackMomentumAtVtx().R();
    LepInfo.ElMomentumErr              [LepInfo.Size] = it_el->trackMomentumError();
    LepInfo.ElTrackPt                  [LepInfo.Size] = it_el->gsfTrack()->pt();
    LepInfo.ElTrackEta                 [LepInfo.Size] = it_el->gsfTrack()->eta();
    LepInfo.ElTrackPhi                 [LepInfo.Size] = it_el->gsfTrack()->phi();
    LepInfo.ElTrackDz                  [LepInfo.Size] = it_el->gsfTrack()->dz( _vtxhandle->front().position() );
    LepInfo.ElTrackDxy                 [LepInfo.Size] = it_el->gsfTrack()->dxy( _vtxhandle->front().position() );
    LepInfo.ElTrackNormalizedChi2      [LepInfo.Size] = it_el->gsfTrack().isNonnull() ? 
                                                   it_el->gsfTrack()->normalizedChi2() : std::numeric_limits<float>::max();
    LepInfo.ElNTrackLayersWMeasurement [LepInfo.Size] = it_el->gsfTrack()->hitPattern().trackerLayersWithMeasurement();
    LepInfo.ElTrackNHits               [LepInfo.Size] = it_el->gsfTrack()->hitPattern().numberOfValidHits();
    LepInfo.NumberOfExpectedInnerHits  [LepInfo.Size]
      = it_el->gsfTrack()->hitPattern().numberOfAllHits( reco::HitPattern::MISSING_INNER_HITS );
    LepInfo.ElhasConv                  [LepInfo.Size] = ConversionTools::hasMatchedConversion( *it_el, _conversionhandle, _beamspothandle->position() );
 
    // ----- Impact paramters -----
    LepInfo.Ip3dPV             [LepInfo.Size] = it_el->dB(pat::Electron::PV3D);
    LepInfo.Ip3dPVErr          [LepInfo.Size] = it_el->edB(pat::Electron::PV3D);
    LepInfo.Ip3dPVSignificance [LepInfo.Size] = it_el->dB(pat::Electron::PV3D) / it_el->edB(pat::Electron::PV3D);

    // ----- Electron track Bremsstrahlung informations -----
    LepInfo.ElClassification  [LepInfo.Size] = it_el->classification();
    LepInfo.ElFBrem           [LepInfo.Size] = it_el->fbrem();
    LepInfo.ElNumberOfBrems   [LepInfo.Size] = it_el->numberOfBrems();

    // ----- Difference of Electron track and superCluster informations ------------------------------------
    LepInfo.ElEcalE           [LepInfo.Size] = it_el->ecalEnergy();
    LepInfo.ElEnergyErr       [LepInfo.Size] = it_el->ecalEnergyError();
    LepInfo.ElEoverP          [LepInfo.Size] = it_el->eSuperClusterOverP();
    LepInfo.EldeltaEta        [LepInfo.Size] = it_el->deltaEtaSuperClusterTrackAtVtx();
    LepInfo.EldeltaPhi        [LepInfo.Size] = it_el->deltaPhiSuperClusterTrackAtVtx();
    LepInfo.EldeltaPhiSeed    [LepInfo.Size] = it_el->deltaEtaSeedClusterTrackAtVtx();

    // ----- Generation Monte Carlo information  -----------------------------------------------------------
    if( !iEvent.isRealData() ){
      const reco::GenParticle* gen = it_el->genLepton();
      if( gen != NULL ){
        LepInfo.GenPt        [LepInfo.Size] = gen->pt();
        LepInfo.GenEta       [LepInfo.Size] = gen->eta();
        LepInfo.GenPhi       [LepInfo.Size] = gen->phi();
        LepInfo.GenPdgID     [LepInfo.Size] = gen->pdgId();
        LepInfo.GenMCTag     [LepInfo.Size] = bprimeKit::GetGenMCTag( gen );
      }
      if( LepInfo.GenMCTag[LepInfo.Size] == 0 ){
        LepInfo.GenMCTag     [LepInfo.Size] = GetGenMCTag( it_el );
      }
    }

    LepInfo.Size++;
  }

}
