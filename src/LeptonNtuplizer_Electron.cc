/*******************************************************************************
*
*  Filename    : bprimeKit_electron.cc
*  Description : Filling in the electrons
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

// -----------------------  Electron specific CMSSW libraries  -----------------------
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

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

    // ----- Inserting generic information  ----------------------------------
    LepInfo.LeptonType        [LepInfo.Size] = 11;
    LepInfo.Index             [LepInfo.Size] = LepInfo.Size;
    LepInfo.isEcalDriven      [LepInfo.Size] = it_el->ecalDrivenSeed();
    LepInfo.isTrackerDriven   [LepInfo.Size] = it_el->trackerDrivenSeed();
    LepInfo.Charge            [LepInfo.Size] = it_el->charge();
    LepInfo.ChargeGsf         [LepInfo.Size] = it_el->gsfTrack()->charge();
    LepInfo.ChargeScPix       [LepInfo.Size] = it_el->scPixCharge();
    LepInfo.Pt                [LepInfo.Size] = it_el->pt();
    LepInfo.Et                [LepInfo.Size] = it_el->et();// Uly 2011-04-04
    LepInfo.Eta               [LepInfo.Size] = it_el->superCluster()->eta();// Uly 2011-04-04
    LepInfo.caloEta           [LepInfo.Size] = it_el->caloPosition().eta();
    LepInfo.Phi               [LepInfo.Size] = it_el->superCluster()->phi();// Uly 2011-04-04
    LepInfo.Px                [LepInfo.Size] = it_el->px();// Uly 2011-04-04
    LepInfo.Py                [LepInfo.Size] = it_el->py();// Uly 2011-04-04
    LepInfo.Pz                [LepInfo.Size] = it_el->pz();// Uly 2011-04-04
    LepInfo.Energy            [LepInfo.Size] = it_el->energy();// Uly 2011-04-04
    LepInfo.CaloEnergy        [LepInfo.Size] = it_el->caloEnergy();
    LepInfo.e1x5              [LepInfo.Size] = it_el->scE1x5();
    LepInfo.e2x5Max           [LepInfo.Size] = it_el->scE2x5Max();
    LepInfo.e5x5              [LepInfo.Size] = it_el->scE5x5();
    LepInfo.TrackIso          [LepInfo.Size] = it_el->dr03TkSumPt();
    LepInfo.EcalIso           [LepInfo.Size] = it_el->dr03EcalRecHitSumEt();
    LepInfo.HcalIso           [LepInfo.Size] = it_el->dr03HcalTowerSumEt();
    LepInfo.HcalDepth1Iso     [LepInfo.Size] = it_el->dr03HcalDepth1TowerSumEt();
    LepInfo.HcalDepth2Iso     [LepInfo.Size] = it_el->dr03HcalDepth2TowerSumEt();
    LepInfo.ElEoverP          [LepInfo.Size] = it_el->eSuperClusterOverP();
    LepInfo.EldeltaEta        [LepInfo.Size] = it_el->deltaEtaSuperClusterTrackAtVtx();
    LepInfo.EldeltaPhi        [LepInfo.Size] = it_el->deltaPhiSuperClusterTrackAtVtx();
    LepInfo.ElHadoverEm       [LepInfo.Size] = it_el->hadronicOverEm();
    LepInfo.ElsigmaIetaIeta   [LepInfo.Size] = it_el->sigmaIetaIeta();
    LepInfo.ElscSigmaIetaIeta [LepInfo.Size] = it_el->scSigmaIEtaIEta();
    LepInfo.ElEnergyErr       [LepInfo.Size] = it_el->ecalEnergyError();
    LepInfo.ElMomentumErr     [LepInfo.Size] = it_el->trackMomentumError();
    LepInfo.ElTrackNLostHits  [LepInfo.Size] = -1;
    LepInfo.ElTrackDz         [LepInfo.Size] = it_el->gsfTrack()->dz( _vtxhandle->front().position() );
    LepInfo.ElTrackDz_BS      [LepInfo.Size] = it_el->gsfTrack()->dz( _beamspothandle->position() );
    LepInfo.ElTrackD0         [LepInfo.Size] = it_el->gsfTrack()->d0();
    LepInfo.ElTrackDxy_BS     [LepInfo.Size] = it_el->gsfTrack()->dxy( _beamspothandle->position() );
    LepInfo.ElTrackDxy_PV     [LepInfo.Size] = it_el->gsfTrack()->dxy( _vtxhandle->front().position() );
    LepInfo.ElTrackDxy_PVBS   [LepInfo.Size] = it_el->gsfTrack()->dxy( _vtxhandle->front().position() );
    LepInfo.ElNClusters       [LepInfo.Size] = it_el->basicClustersSize();
    LepInfo.ElClassification  [LepInfo.Size] = it_el->classification();
    LepInfo.ElFBrem           [LepInfo.Size] = it_el->fbrem();
    LepInfo.ElNumberOfBrems   [LepInfo.Size] = it_el->numberOfBrems();
    LepInfo.vertexZ           [LepInfo.Size] = it_el->vertex().z();// Uly 2011-04-04
    LepInfo.NumberOfExpectedInnerHits [LepInfo.Size]
      = it_el->gsfTrack()->hitPattern().numberOfAllHits( reco::HitPattern::MISSING_INNER_HITS );// Add by Jacky

    // ----- MiniPFIsolation -----
    // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
    LepInfo.MiniIso [LepInfo.Size]
      = bprimeKit::GetMiniPFIsolation(
      _packedhandle,
      dynamic_cast<const reco::Candidate*>( &*it_el ),
      0.05,
      0.2,
      10.,
      false );

    // ----- Cut based electron ID  ---------------------------------------------------------------------
    const double dist_ = it_el->convDist() == -9999. ? 9999 : it_el->convDist();
    const double dcot_ = it_el->convDcot() == -9999. ? 9999 : it_el->convDcot();
    LepInfo.dcotdist[LepInfo.Size]     = std::max( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ), 0.  );
    LepInfo.ElseedEoverP[LepInfo.Size] = it_el->eSeedClusterOverP();
    LepInfo.ElHcalIso04[LepInfo.Size]  = it_el->dr04HcalTowerSumEt();
    LepInfo.ElEcalIso04[LepInfo.Size]  = it_el->dr04EcalRecHitSumEt();

    // ----- Isolation variables  -----------------------------------------------------------------------
    const edm::Ptr<pat::Electron> el( _electronhandle, it_el - _electronhandle->begin() );
    LepInfo.EgammaCutBasedEleIdVETO     [LepInfo.Size] = (int)( ( *_electronIDVeto )[el] );
    LepInfo.EgammaCutBasedEleIdLOOSE    [LepInfo.Size] = (int)( ( *_electronIDLoose )[el] );
    LepInfo.EgammaCutBasedEleIdMEDIUM   [LepInfo.Size] = (int)( ( *_electronIDMedium )[el] );
    LepInfo.EgammaCutBasedEleIdTIGHT    [LepInfo.Size] = (int)( ( *_electronIDTight )[el] );
    LepInfo.EgammaCutBasedEleIdHEEP     [LepInfo.Size] = (int)( ( *_electronIDHEEP )[el] );
    LepInfo.ChargedHadronIso            [LepInfo.Size] = it_el->pfIsolationVariables().sumChargedHadronPt;
    LepInfo.NeutralHadronIso            [LepInfo.Size] = it_el->pfIsolationVariables().sumPhotonEt;
    LepInfo.PhotonIso                   [LepInfo.Size] = it_el->pfIsolationVariables().sumNeutralHadronEt;
    LepInfo.SumPUPt                     [LepInfo.Size] = it_el->pfIsolationVariables().sumPUPt;
    LepInfo.ElEcalE                     [LepInfo.Size] = it_el->ecalEnergy();
    LepInfo.ElhcalOverEcalBc            [LepInfo.Size] = it_el->hcalOverEcalBc();
    LepInfo.Eldr03HcalDepth1TowerSumEtBc[LepInfo.Size] = it_el->dr03HcalDepth1TowerSumEtBc();
    LepInfo.Eldr03HcalDepth2TowerSumEtBc[LepInfo.Size] = it_el->dr03HcalDepth2TowerSumEtBc();
    LepInfo.Eldr04HcalDepth1TowerSumEtBc[LepInfo.Size] = it_el->dr04HcalDepth1TowerSumEtBc();
    LepInfo.Eldr04HcalDepth2TowerSumEtBc[LepInfo.Size] = it_el->dr04HcalDepth2TowerSumEtBc();
    LepInfo.ElhasConv                   [LepInfo.Size] =
      ConversionTools::hasMatchedConversion( *it_el, _conversionhandle, _beamspothandle->position() );


    LepInfo.EgammaCutBasedEleIdTRIGGERTIGHT  [LepInfo.Size]
      = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERTIGHT, *it_el );
    LepInfo.EgammaCutBasedEleIdTRIGGERWP70 [LepInfo.Size]
      = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERWP70, *it_el );

    const ElectronEffectiveArea::ElectronEffectiveAreaTarget EATarget =
      iEvent.isRealData() ?  ElectronEffectiveArea::kEleEAData2012 :
      ElectronEffectiveArea::kEleEAFall11MC;

    const double rhoPrime = max( *_rhohandle, 0.0 );
    const double AEffR03  = ElectronEffectiveArea::GetElectronEffectiveArea(
      ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, LepInfo.Eta[LepInfo.Size], EATarget );
    LepInfo.IsoRhoCorrR03[LepInfo.Size] = LepInfo.ChargedHadronIsoR03[LepInfo.Size] +
                                          max( (double)( LepInfo.NeutralHadronIsoR03[LepInfo.Size] + LepInfo.PhotonIsoR03[LepInfo.Size] - rhoPrime * AEffR03 ), 0.0 );

    // ----- Generation Monte Carlo information  -----------------------------
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

    // ----- Conversion rejection information  -------------------------------
    LepInfo.Eldist        [LepInfo.Size] = it_el->convDist();
    LepInfo.Elconvradius  [LepInfo.Size] = it_el->convRadius();
    LepInfo.Eldcot        [LepInfo.Size] = it_el->convDcot();

    LepInfo.Size++;
  }

}
