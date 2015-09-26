/*******************************************************************************
 *
 *  Filename    : bprimeKit_electron.cc
 *  Description : filling in the electrons
 *
*******************************************************************************/

//------------------------------------------------------------------------------ 
//   Libraries
//------------------------------------------------------------------------------ 
#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "MyAna/bprimeKit/interface/bprimeKit_util.h"

//-----------------------  Electron specific CMSSW libraries  -----------------------
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TrackingTools/IPTools/interface/IPTools.h"


//-------------------------------------------------------------------------------------------------- 
//   BprimeKit electron information methods
//-------------------------------------------------------------------------------------------------- 
bool bprimeKit::fillElectron( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll  )
{
   //-------------------------------------------------------------------------------------------------- 
   //   Helper variables definition
   //-------------------------------------------------------------------------------------------------- 
   ElectronHandler elecHandle   ;
   GsfHandler      gsfElecHandle;
   edm::Handle<edm::ValueMap<bool>>  veto_id_decisions   ;
   edm::Handle<edm::ValueMap<bool>>  loose_id_decisions  ;
   edm::Handle<edm::ValueMap<bool>>  medium_id_decisions ;
   edm::Handle<edm::ValueMap<bool>>  tight_id_decisions  ;
   edm::Handle<edm::ValueMap<bool>>  heep_id_decisions   ;
//   edm::Handle<edm::ValueMap<float>> eleMVAValues        ; 
   ElectronEffectiveArea::ElectronEffectiveAreaTarget EATarget;

   float dist_     ,dcot_     , rhoPrime, AEffR03  ;
   bool  trigtight , trigwp70 ;
  
   //-------------------------------------------------------------------------------------------------- 
   //   Event wide variables setup
   //-------------------------------------------------------------------------------------------------- 
   iEvent.getByLabel( eleclabel_[icoll]     , elecHandle          ) ;
   iEvent.getByLabel( eleclabel_[icoll]     , gsfElecHandle       ) ;
   iEvent.getByToken( eleVetoIdMapToken_    , veto_id_decisions   ) ;
   iEvent.getByToken( eleLooseIdMapToken_   , loose_id_decisions  ) ;
   iEvent.getByToken( eleMediumIdMapToken_  , medium_id_decisions ) ;
   iEvent.getByToken( eleTightIdMapToken_   , tight_id_decisions  ) ;
   iEvent.getByToken( eleHEEPIdMapToken_    , heep_id_decisions   ) ;
//   iEvent.getByToken( eleMVAValuesMapToken_ , eleMVAValues        ) ;
   
   if( isData ) { 
      EATarget = ElectronEffectiveArea::kEleEAData2012; }
   else {EATarget = ElectronEffectiveArea::kEleEAFall11MC;
   }


   //-------------------------------------------------------------------------------------------------- 
   //   Begin main loop
   //-------------------------------------------------------------------------------------------------- 
   ElectronIterator it_el = elecHandle->begin();
   for( size_t i = 0 ; i < elecHandle->size() ; ++i , ++it_el ) {
      if ( LepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr << "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }
      if( debug_ > 11 ) { 
         cout << "\t>>>Size " << LepInfo[icoll].Size << " el et,eta,phi " << it_el->et() << "," << it_el->superCluster()->eta() << "," 
              << it_el->superCluster()->phi() << endl;
      }

      //----- Inserting generic information  -------------------------------------------------------------
      LepInfo[icoll].LeptonType                [LepInfo[icoll].Size] = 11                                                                                   ;
      LepInfo[icoll].Index                     [LepInfo[icoll].Size] = LepInfo[icoll].Size                                                                  ;
      LepInfo[icoll].isEcalDriven              [LepInfo[icoll].Size] = it_el->ecalDrivenSeed()                                                              ;
      LepInfo[icoll].isTrackerDriven           [LepInfo[icoll].Size] = it_el->trackerDrivenSeed()                                                           ;
      LepInfo[icoll].Charge                    [LepInfo[icoll].Size] = it_el->charge()                                                                      ;
      LepInfo[icoll].ChargeGsf                 [LepInfo[icoll].Size] = it_el->gsfTrack()->charge()                                                          ;
      LepInfo[icoll].ChargeScPix               [LepInfo[icoll].Size] = it_el->scPixCharge()                                                                 ;
      LepInfo[icoll].Pt                        [LepInfo[icoll].Size] = it_el->pt()                                                                          ;
      LepInfo[icoll].Et                        [LepInfo[icoll].Size] = it_el->et()                                                                          ; //Uly 2011-04-04
      LepInfo[icoll].Eta                       [LepInfo[icoll].Size] = it_el->superCluster()->eta()                                                         ; //Uly 2011-04-04
      LepInfo[icoll].caloEta                   [LepInfo[icoll].Size] = it_el->caloPosition().eta()                                                          ;
      LepInfo[icoll].Phi                       [LepInfo[icoll].Size] = it_el->superCluster()->phi()                                                         ; //Uly 2011-04-04
      LepInfo[icoll].Px                        [LepInfo[icoll].Size] = it_el->px()                                                                          ; //Uly 2011-04-04
      LepInfo[icoll].Py                        [LepInfo[icoll].Size] = it_el->py()                                                                          ; //Uly 2011-04-04
      LepInfo[icoll].Pz                        [LepInfo[icoll].Size] = it_el->pz()                                                                          ; //Uly 2011-04-04
      LepInfo[icoll].Energy                    [LepInfo[icoll].Size] = it_el->energy()                                                                      ; //Uly 2011-04-04
      LepInfo[icoll].CaloEnergy                [LepInfo[icoll].Size] = it_el->caloEnergy()                                                                  ;
      LepInfo[icoll].e1x5                      [LepInfo[icoll].Size] = it_el->scE1x5()                                                                      ;
      LepInfo[icoll].e2x5Max                   [LepInfo[icoll].Size] = it_el->scE2x5Max()                                                                   ;
      LepInfo[icoll].e5x5                      [LepInfo[icoll].Size] = it_el->scE5x5()                                                                      ;
      LepInfo[icoll].TrackIso                  [LepInfo[icoll].Size] = it_el->dr03TkSumPt()                                                                 ;
      LepInfo[icoll].EcalIso                   [LepInfo[icoll].Size] = it_el->dr03EcalRecHitSumEt()                                                         ;
      LepInfo[icoll].HcalIso                   [LepInfo[icoll].Size] = it_el->dr03HcalTowerSumEt()                                                          ;
      LepInfo[icoll].HcalDepth1Iso             [LepInfo[icoll].Size] = it_el->dr03HcalDepth1TowerSumEt()                                                    ;
      LepInfo[icoll].HcalDepth2Iso             [LepInfo[icoll].Size] = it_el->dr03HcalDepth2TowerSumEt()                                                    ;
      LepInfo[icoll].ElEoverP                  [LepInfo[icoll].Size] = it_el->eSuperClusterOverP()                                                          ;
      LepInfo[icoll].EldeltaEta                [LepInfo[icoll].Size] = it_el->deltaEtaSuperClusterTrackAtVtx()                                              ;
      LepInfo[icoll].EldeltaPhi                [LepInfo[icoll].Size] = it_el->deltaPhiSuperClusterTrackAtVtx()                                              ;
      LepInfo[icoll].ElHadoverEm               [LepInfo[icoll].Size] = it_el->hadronicOverEm()                                                              ;
      LepInfo[icoll].ElsigmaIetaIeta           [LepInfo[icoll].Size] = it_el->sigmaIetaIeta()                                                               ;
      LepInfo[icoll].ElscSigmaIetaIeta         [LepInfo[icoll].Size] = it_el->scSigmaIEtaIEta()                                                             ;
      LepInfo[icoll].ElEnergyErr               [LepInfo[icoll].Size] = it_el->ecalEnergyError()                                                             ;
      LepInfo[icoll].ElMomentumErr             [LepInfo[icoll].Size] = it_el->trackMomentumError()                                                          ;
      LepInfo[icoll].ElTrackNLostHits          [LepInfo[icoll].Size] = -1                                                                                   ;
      LepInfo[icoll].ElTrackDz                 [LepInfo[icoll].Size] = it_el->gsfTrack()->dz( PrimVtx.position() )                                          ;
      LepInfo[icoll].ElTrackDz_BS              [LepInfo[icoll].Size] = it_el->gsfTrack()->dz( beamSpot.position() )                                         ;
      LepInfo[icoll].ElTrackD0                 [LepInfo[icoll].Size] = it_el->gsfTrack()->d0()                                                              ;
      LepInfo[icoll].ElTrackDxy_BS             [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( beamSpot.position() )                                        ;
      LepInfo[icoll].ElTrackDxy_PV             [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( PrimVtx.position() )                                         ;
      LepInfo[icoll].ElTrackDxy_PVBS           [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( PrimVtx_BS.position() )                                      ;
      LepInfo[icoll].ElNClusters               [LepInfo[icoll].Size] = it_el->basicClustersSize()                                                           ;
      LepInfo[icoll].ElClassification          [LepInfo[icoll].Size] = it_el->classification()                                                              ;
      LepInfo[icoll].ElFBrem                   [LepInfo[icoll].Size] = it_el->fbrem()                                                                       ;
      LepInfo[icoll].ElNumberOfBrems           [LepInfo[icoll].Size] = it_el->numberOfBrems()                                                               ;
      LepInfo[icoll].NumberOfExpectedInnerHits [LepInfo[icoll].Size] = it_el->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ) ; // Add by Jacky
      LepInfo[icoll].vertexZ                   [LepInfo[icoll].Size] = it_el->vertex().z()                                                                  ; //Uly 2011-04-04
      
      //----- Cut based electron ID  ---------------------------------------------------------------------
      dist_ = ( it_el->convDist() == -9999. ? 9999 : it_el->convDist() );
      dcot_ = ( it_el->convDcot() == -9999. ? 9999 : it_el->convDcot() );
      LepInfo[icoll].dcotdist[LepInfo[icoll].Size] = ( ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) > 0 ? ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) : 0 );
      LepInfo[icoll].ElseedEoverP[LepInfo[icoll].Size] = it_el->eSeedClusterOverP();
      LepInfo[icoll].ElHcalIso04[LepInfo[icoll].Size]  = it_el->dr04HcalTowerSumEt();
      LepInfo[icoll].ElEcalIso04[LepInfo[icoll].Size]  = it_el->dr04EcalRecHitSumEt();
      
      //----- Isolation variables  -----------------------------------------------------------------------
      const auto el = gsfElecHandle->ptrAt( i );
      if( !runOnB2G ) {
      LepInfo[icoll].EgammaCutBasedEleIdVETO   [LepInfo[icoll].Size] = (int)((*veto_id_decisions)[el]);
      LepInfo[icoll].EgammaCutBasedEleIdLOOSE  [LepInfo[icoll].Size] = (int)((*loose_id_decisions)[el]);
      LepInfo[icoll].EgammaCutBasedEleIdMEDIUM [LepInfo[icoll].Size] = (int)((*medium_id_decisions)[el]);
      LepInfo[icoll].EgammaCutBasedEleIdTIGHT  [LepInfo[icoll].Size] = (int)((*tight_id_decisions)[el]);
      LepInfo[icoll].EgammaCutBasedEleIdHEEP   [LepInfo[icoll].Size] = (int)((*heep_id_decisions)[el]); 
//         LepInfo[icoll].EgammaMVANonTrig          [LepInfo[icoll].Size] = (*eleMVAValues)[el];
      } else {
         // Still updating
      }
      LepInfo[icoll].ChargedHadronIso          [LepInfo[icoll].Size] = it_el->pfIsolationVariables().sumChargedHadronPt ; 
      LepInfo[icoll].NeutralHadronIso          [LepInfo[icoll].Size] = it_el->pfIsolationVariables().sumPhotonEt;
      LepInfo[icoll].PhotonIso                 [LepInfo[icoll].Size] = it_el->pfIsolationVariables().sumNeutralHadronEt;
      LepInfo[icoll].ElEcalE                     [LepInfo[icoll].Size] = el->ecalEnergy();
      LepInfo[icoll].ElhcalOverEcalBc            [LepInfo[icoll].Size] = el->hcalOverEcalBc();
      LepInfo[icoll].Eldr03HcalDepth1TowerSumEtBc[LepInfo[icoll].Size] = el->dr03HcalDepth1TowerSumEtBc();
      LepInfo[icoll].Eldr03HcalDepth2TowerSumEtBc[LepInfo[icoll].Size] = el->dr03HcalDepth2TowerSumEtBc();
      LepInfo[icoll].Eldr04HcalDepth1TowerSumEtBc[LepInfo[icoll].Size] = el->dr04HcalDepth1TowerSumEtBc();
      LepInfo[icoll].Eldr04HcalDepth2TowerSumEtBc[LepInfo[icoll].Size] = el->dr04HcalDepth2TowerSumEtBc();
            
      // cuts to match tight trigger requirements
      trigtight = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERTIGHT, *el );
      LepInfo[icoll].EgammaCutBasedEleIdTRIGGERTIGHT  [LepInfo[icoll].Size] = trigtight;
      // for 2011 WP70 trigger
      trigwp70 = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERWP70, *el );
      LepInfo[icoll].EgammaCutBasedEleIdTRIGGERWP70 [LepInfo[icoll].Size] = trigwp70;
            
      rhoPrime = max( (double)(EvtInfo.Rho), 0.0 ); 
      AEffR03 = ElectronEffectiveArea::GetElectronEffectiveArea( 
            ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
      LepInfo[icoll].IsoRhoCorrR03[LepInfo[icoll].Size] = LepInfo[icoll].ChargedHadronIsoR03[LepInfo[icoll].Size] +
         max( (double)(LepInfo[icoll].NeutralHadronIsoR03[LepInfo[icoll].Size] + LepInfo[icoll].PhotonIsoR03[LepInfo[icoll].Size] - rhoPrime * AEffR03), 0.0 );
     
      //----- Generation Monte Carlo information  --------------------------------------------------------
      if ( !isData && !skipGenInfo_ ) {
         if( debug_ > 15 ) { cout << "   Getting MC information\n"; }
         const reco::GenParticle* gen = it_el->genLepton(); 
         if ( gen != NULL ) {
            LepInfo[icoll].GenPt        [LepInfo[icoll].Size] = gen->pt();
            LepInfo[icoll].GenEta       [LepInfo[icoll].Size] = gen->eta();
            LepInfo[icoll].GenPhi       [LepInfo[icoll].Size] = gen->phi();
            LepInfo[icoll].GenPdgID     [LepInfo[icoll].Size] = gen->pdgId();
            LepInfo[icoll].GenMCTag     [LepInfo[icoll].Size] = getGenMCTag( gen ) ;
         }
         if( debug_ > 15 ) { cout << "Get GenHandle\n"; }
         if ( LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] == 0 && GenHandle.isValid() ) {
            for( GenIterator it_gen = GenHandle->begin(); it_gen != GenHandle->end() ; it_gen++ ) {
               if( LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] != 0 ) break;
               LepInfo[icoll].GenMCTag[LepInfo[icoll].Size] = getGenMCTag( it_gen , it_el )  ;
            }
         }
         if( debug_ > 15 ) { cout << "   Done getting MC information\n"; }
      }
      
      //----- Impact parameter related  ------------------------------------------------------------------
      // Reference from UserCode/MitProd/TreeFiller/src/FillerElectrons.cc
      const reco::TransientTrack& tt = transientTrackBuilder->build( it_el->gsfTrack() );
      reco::Vertex thevtx = pvCol->at( 0 );
      const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt, thevtx );
      const double gsfsign   = ( ( -it_el->gsfTrack()->dxy( thevtx.position() ) )   >= 0 ) ? 1. : -1.;
      LepInfo[icoll].Ip3dPV[LepInfo[icoll].Size] = gsfsign * ip3dpv.second.value();
      LepInfo[icoll].Ip3dPVErr[LepInfo[icoll].Size] = ip3dpv.second.error();
      LepInfo[icoll].Ip3dPVSignificance[LepInfo[icoll].Size] = gsfsign * ip3dpv.second.value() / ip3dpv.second.error();

      //----- Conversion rejection information  ----------------------------------------------------------
      LepInfo[icoll].Eldist        [LepInfo[icoll].Size] = it_el->convDist();
      LepInfo[icoll].Elconvradius  [LepInfo[icoll].Size] = it_el->convRadius();
      LepInfo[icoll].Eldcot        [LepInfo[icoll].Size] = it_el->convDcot();

      LepInfo[icoll].CandRef[LepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_el );
      LepInfo[icoll].Size++;
   }
   return true;
}

//-------------------------------------------------------------------------------------------------- 
//   Legacy code section
//--------------------------------------------------------------------------------------------------
//LepInfo[icoll].ElhasConv  [LepInfo[icoll].Size] = ConversionTools::hasMatchedConversion( *el, conversions_h, beamSpot.position() );
//
// convInfo = convFinder.getConversionInfo( *it_el, tracks_h, evt_bField );
// ConversionFinder convFinder;
// ConversionInfo convInfo;
// LepInfo[icoll].ElConvPoint_x [LepInfo[icoll].Size] = convInfo.pointOfConversion().x();
// LepInfo[icoll].ElConvPoint_y [LepInfo[icoll].Size] = convInfo.pointOfConversion().y();
// LepInfo[icoll].ElConvPoint_z [LepInfo[icoll].Size] = convInfo.pointOfConversion().z();

//// MVA-ID  -- start
// Vertex dummy;
// const Vertex* pv = &dummy;
// if ( VertexHandle->size() != 0 ) {
//     pv = &*VertexHandle->begin();
// } else { // create a dummy PV
//   Vertex::Error e;
//   e( 0, 0 ) = 0.0015 * 0.0015;
//   e( 1, 1 ) = 0.0015 * 0.0015;
//   e( 2, 2 ) = 15. * 15.;
//   Vertex::Point p( 0, 0, 0 );
//   dummy = Vertex( p, e, 0, 0, 0 );
//}
//
// float myMVATrigMethod = myMVATrig->mvaValue( ( theEGamma[nGsfEle] ), *pv, thebuilder, lazyTools, debugMVAclass );
// LepInfo[icoll].EgammaMVATrig      [LepInfo[icoll].Size] = myMVATrigMethod;
// LepInfo[icoll].sumPUPtR03                [LepInfo[icoll].Size] = isolatorR03.getIsolationChargedAll();
// LepInfo[icoll].sumPUPtR04                [LepInfo[icoll].Size] = isolatorR04.getIsolationChargedAll();
// float AEffR04 = ElectronEffectiveArea::GetElectronEffectiveArea( ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
// LepInfo[icoll].IsoRhoCorrR04             [LepInfo[icoll].Size] = LepInfo[icoll].ChargedHadronIsoR04[LepInfo[icoll].Size] +
//   max( LepInfo[icoll].NeutralHadronIsoR04[LepInfo[icoll].Size] + LepInfo[icoll].PhotonIsoR04[LepInfo[icoll].Size] - rhoPrime * AEffR04, 0.0 );
//
