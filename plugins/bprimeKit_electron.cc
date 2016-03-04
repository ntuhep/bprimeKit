/*******************************************************************************
 *
 *  Filename    : bprimeKit_electron.cc
 *  Description : Filling in the electrons
 *
*******************************************************************************/

//------------------------------------------------------------------------------
//   Libraries
//------------------------------------------------------------------------------
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

//-----------------------  Electron specific CMSSW libraries  -----------------------
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "TrackingTools/IPTools/interface/IPTools.h"


using namespace std;

//--------------------------------------------------------------------------------------------------
//   BprimeKit electron information methods
//--------------------------------------------------------------------------------------------------
bool bprimeKit::FillElectron( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll  )
{
   //--------------------------------------------------------------------------------------------------
   //   Helper variables definition
   //--------------------------------------------------------------------------------------------------
   ElectronEffectiveArea::ElectronEffectiveAreaTarget EATarget;

   float dist_     ,dcot_     , rhoPrime, AEffR03  ;
   bool  trigtight , trigwp70 ;

   if( fIsData ) {
      EATarget = ElectronEffectiveArea::kEleEAData2012; }
   else {
      EATarget = ElectronEffectiveArea::kEleEAFall11MC;
   }

   //--------------------------------------------------------------------------------------------------
   //   Begin main loop
   //--------------------------------------------------------------------------------------------------
   ElectronIterator it_el = fElectronList_Hs[icoll]->begin();
   for( size_t i = 0 ; i < fElectronList_Hs[icoll]->size() ; ++i , ++it_el ) {
      if ( fLepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr << "ERROR: number of leptons exceeds the size of array." << endl;
         break;
      }
      if( fDebug > 2 ) {
         std::cerr << "\t\t[2]Size " << fLepInfo[icoll].Size
              << " el et,eta,phi " << it_el->et() << "," << it_el->superCluster()->eta() << ","
              << it_el->superCluster()->phi() << endl;
      }

      //----- Inserting generic information  -------------------------------------------------------------
      fLepInfo[icoll].LeptonType        [fLepInfo[icoll].Size] = 11                                                     ;
      fLepInfo[icoll].Index             [fLepInfo[icoll].Size] = fLepInfo[icoll].Size                                   ;
      fLepInfo[icoll].isEcalDriven      [fLepInfo[icoll].Size] = it_el->ecalDrivenSeed()                                ;
      fLepInfo[icoll].isTrackerDriven   [fLepInfo[icoll].Size] = it_el->trackerDrivenSeed()                             ;
      fLepInfo[icoll].Charge            [fLepInfo[icoll].Size] = it_el->charge()                                        ;
      fLepInfo[icoll].ChargeGsf         [fLepInfo[icoll].Size] = it_el->gsfTrack()->charge()                            ;
      fLepInfo[icoll].ChargeScPix       [fLepInfo[icoll].Size] = it_el->scPixCharge()                                   ;
      fLepInfo[icoll].Pt                [fLepInfo[icoll].Size] = it_el->pt()                                            ;
      fLepInfo[icoll].Et                [fLepInfo[icoll].Size] = it_el->et()                                            ; //Uly 2011-04-04
      fLepInfo[icoll].Eta               [fLepInfo[icoll].Size] = it_el->superCluster()->eta()                           ; //Uly 2011-04-04
      fLepInfo[icoll].caloEta           [fLepInfo[icoll].Size] = it_el->caloPosition().eta()                            ;
      fLepInfo[icoll].Phi               [fLepInfo[icoll].Size] = it_el->superCluster()->phi()                           ; //Uly 2011-04-04
      fLepInfo[icoll].Px                [fLepInfo[icoll].Size] = it_el->px()                                            ; //Uly 2011-04-04
      fLepInfo[icoll].Py                [fLepInfo[icoll].Size] = it_el->py()                                            ; //Uly 2011-04-04
      fLepInfo[icoll].Pz                [fLepInfo[icoll].Size] = it_el->pz()                                            ; //Uly 2011-04-04
      fLepInfo[icoll].Energy            [fLepInfo[icoll].Size] = it_el->energy()                                        ; //Uly 2011-04-04
      fLepInfo[icoll].CaloEnergy        [fLepInfo[icoll].Size] = it_el->caloEnergy()                                    ;
      fLepInfo[icoll].e1x5              [fLepInfo[icoll].Size] = it_el->scE1x5()                                        ;
      fLepInfo[icoll].e2x5Max           [fLepInfo[icoll].Size] = it_el->scE2x5Max()                                     ;
      fLepInfo[icoll].e5x5              [fLepInfo[icoll].Size] = it_el->scE5x5()                                        ;
      fLepInfo[icoll].TrackIso          [fLepInfo[icoll].Size] = it_el->dr03TkSumPt()                                   ;
      fLepInfo[icoll].EcalIso           [fLepInfo[icoll].Size] = it_el->dr03EcalRecHitSumEt()                           ;
      fLepInfo[icoll].HcalIso           [fLepInfo[icoll].Size] = it_el->dr03HcalTowerSumEt()                            ;
      fLepInfo[icoll].HcalDepth1Iso     [fLepInfo[icoll].Size] = it_el->dr03HcalDepth1TowerSumEt()                      ;
      fLepInfo[icoll].HcalDepth2Iso     [fLepInfo[icoll].Size] = it_el->dr03HcalDepth2TowerSumEt()                      ;
      fLepInfo[icoll].ElEoverP          [fLepInfo[icoll].Size] = it_el->eSuperClusterOverP()                            ;
      fLepInfo[icoll].EldeltaEta        [fLepInfo[icoll].Size] = it_el->deltaEtaSuperClusterTrackAtVtx()                ;
      fLepInfo[icoll].EldeltaPhi        [fLepInfo[icoll].Size] = it_el->deltaPhiSuperClusterTrackAtVtx()                ;
      fLepInfo[icoll].ElHadoverEm       [fLepInfo[icoll].Size] = it_el->hadronicOverEm()                                ;
      fLepInfo[icoll].ElsigmaIetaIeta   [fLepInfo[icoll].Size] = it_el->sigmaIetaIeta()                                 ;
      fLepInfo[icoll].ElscSigmaIetaIeta [fLepInfo[icoll].Size] = it_el->scSigmaIEtaIEta()                               ;
      fLepInfo[icoll].ElEnergyErr       [fLepInfo[icoll].Size] = it_el->ecalEnergyError()                               ;
      fLepInfo[icoll].ElMomentumErr     [fLepInfo[icoll].Size] = it_el->trackMomentumError()                            ;
      fLepInfo[icoll].ElTrackNLostHits  [fLepInfo[icoll].Size] = -1                                                     ;
      fLepInfo[icoll].ElTrackDz         [fLepInfo[icoll].Size] = it_el->gsfTrack()->dz( fPrimaryVertex.position() )     ;
      fLepInfo[icoll].ElTrackDz_BS      [fLepInfo[icoll].Size] = it_el->gsfTrack()->dz( fBeamSpot.position() )          ;
      fLepInfo[icoll].ElTrackD0         [fLepInfo[icoll].Size] = it_el->gsfTrack()->d0()                                ;
      fLepInfo[icoll].ElTrackDxy_BS     [fLepInfo[icoll].Size] = it_el->gsfTrack()->dxy( fBeamSpot.position() )         ;
      fLepInfo[icoll].ElTrackDxy_PV     [fLepInfo[icoll].Size] = it_el->gsfTrack()->dxy( fPrimaryVertex.position() )    ;
      fLepInfo[icoll].ElTrackDxy_PVBS   [fLepInfo[icoll].Size] = it_el->gsfTrack()->dxy( fPrimaryVertex_BS.position() ) ;
      fLepInfo[icoll].ElNClusters       [fLepInfo[icoll].Size] = it_el->basicClustersSize()                             ;
      fLepInfo[icoll].ElClassification  [fLepInfo[icoll].Size] = it_el->classification()                                ;
      fLepInfo[icoll].ElFBrem           [fLepInfo[icoll].Size] = it_el->fbrem()                                         ;
      fLepInfo[icoll].ElNumberOfBrems   [fLepInfo[icoll].Size] = it_el->numberOfBrems()                                 ;
      fLepInfo[icoll].vertexZ           [fLepInfo[icoll].Size] = it_el->vertex().z()                                    ; //Uly 2011-04-04
      fLepInfo[icoll].NumberOfExpectedInnerHits [fLepInfo[icoll].Size]
         = it_el->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ) ; // Add by Jacky

      //----- MiniPFIsolation -----
      // https://github.com/manuelfs/CfANtupler/blob/master/minicfa/interface/miniAdHocNTupler.h#L54
      fLepInfo[icoll].MiniIso [fLepInfo[icoll].Size]
         = bprimeKit::GetMiniPFIsolation(
            fPackedCand_H ,
            dynamic_cast<const reco::Candidate*>(&*it_el),
            0.05,
            0.2,
            10.,
            false);

      //----- Cut based electron ID  ---------------------------------------------------------------------
      dist_ = ( it_el->convDist() == -9999. ? 9999 : it_el->convDist() );
      dcot_ = ( it_el->convDcot() == -9999. ? 9999 : it_el->convDcot() );
      fLepInfo[icoll].dcotdist[fLepInfo[icoll].Size]
         = ( ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) > 0 ? ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) : 0 );
      fLepInfo[icoll].ElseedEoverP[fLepInfo[icoll].Size] = it_el->eSeedClusterOverP();
      fLepInfo[icoll].ElHcalIso04[fLepInfo[icoll].Size]  = it_el->dr04HcalTowerSumEt();
      fLepInfo[icoll].ElEcalIso04[fLepInfo[icoll].Size]  = it_el->dr04EcalRecHitSumEt();

      //----- Isolation variables  -----------------------------------------------------------------------
      const edm::Ptr<pat::Electron> el( fElectronList_Hs[icoll], i );
      if( !fRunOnB2G ) {
         fLepInfo[icoll].EgammaCutBasedEleIdVETO   [fLepInfo[icoll].Size] = (int)((*fElectronIDVeto_H)[el]);
         fLepInfo[icoll].EgammaCutBasedEleIdLOOSE  [fLepInfo[icoll].Size] = (int)((*fElectronIDLoose_H)[el]);
         fLepInfo[icoll].EgammaCutBasedEleIdMEDIUM [fLepInfo[icoll].Size] = (int)((*fElectronIDMedium_H)[el]);
         fLepInfo[icoll].EgammaCutBasedEleIdTIGHT  [fLepInfo[icoll].Size] = (int)((*fElectronIDTight_H)[el]);
         fLepInfo[icoll].EgammaCutBasedEleIdHEEP   [fLepInfo[icoll].Size] = (int)((*fElectronIDHEEP_H)[el]);
      } else {
         fLepInfo[icoll].EgammaCutBasedEleIdVETO   [fLepInfo[icoll].Size] = it_el->userFloat( "vidVeto" ) ;
         fLepInfo[icoll].EgammaCutBasedEleIdLOOSE  [fLepInfo[icoll].Size] = it_el->userFloat( "vidLoose" ) ;
         fLepInfo[icoll].EgammaCutBasedEleIdMEDIUM [fLepInfo[icoll].Size] = it_el->userFloat( "vidMedium" ) ;
         fLepInfo[icoll].EgammaCutBasedEleIdTIGHT  [fLepInfo[icoll].Size] = it_el->userFloat( "vidTight" ) ;
         fLepInfo[icoll].EgammaCutBasedEleIdHEEP   [fLepInfo[icoll].Size] = it_el->userFloat( "vidHEEP" ) ;
      }
      fLepInfo[icoll].ChargedHadronIso          [fLepInfo[icoll].Size] = it_el->pfIsolationVariables().sumChargedHadronPt ;
      fLepInfo[icoll].NeutralHadronIso          [fLepInfo[icoll].Size] = it_el->pfIsolationVariables().sumPhotonEt;
      fLepInfo[icoll].PhotonIso                 [fLepInfo[icoll].Size] = it_el->pfIsolationVariables().sumNeutralHadronEt;
      fLepInfo[icoll].SumPUPt                   [fLepInfo[icoll].Size] = it_el->pfIsolationVariables().sumPUPt;
      fLepInfo[icoll].ElEcalE                     [fLepInfo[icoll].Size] = el->ecalEnergy();
      fLepInfo[icoll].ElhcalOverEcalBc            [fLepInfo[icoll].Size] = el->hcalOverEcalBc();
      fLepInfo[icoll].Eldr03HcalDepth1TowerSumEtBc[fLepInfo[icoll].Size] = el->dr03HcalDepth1TowerSumEtBc();
      fLepInfo[icoll].Eldr03HcalDepth2TowerSumEtBc[fLepInfo[icoll].Size] = el->dr03HcalDepth2TowerSumEtBc();
      fLepInfo[icoll].Eldr04HcalDepth1TowerSumEtBc[fLepInfo[icoll].Size] = el->dr04HcalDepth1TowerSumEtBc();
      fLepInfo[icoll].Eldr04HcalDepth2TowerSumEtBc[fLepInfo[icoll].Size] = el->dr04HcalDepth2TowerSumEtBc();
      fLepInfo[icoll].ElhasConv                   [fLepInfo[icoll].Size] =
         ConversionTools::hasMatchedConversion( *el, fConversions_H, fBeamSpot.position() );
      trigtight = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERTIGHT, *el );
      fLepInfo[icoll].EgammaCutBasedEleIdTRIGGERTIGHT  [fLepInfo[icoll].Size] = trigtight;
      trigwp70 = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERWP70, *el );
      fLepInfo[icoll].EgammaCutBasedEleIdTRIGGERWP70 [fLepInfo[icoll].Size] = trigwp70;

      rhoPrime = max( (double)(fEvtInfo.Rho), 0.0 );
      AEffR03 = ElectronEffectiveArea::GetElectronEffectiveArea(
            ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, fLepInfo[icoll].Eta[fLepInfo[icoll].Size], EATarget );
      fLepInfo[icoll].IsoRhoCorrR03[fLepInfo[icoll].Size] = fLepInfo[icoll].ChargedHadronIsoR03[fLepInfo[icoll].Size] +
         max( (double)(fLepInfo[icoll].NeutralHadronIsoR03[fLepInfo[icoll].Size] + fLepInfo[icoll].PhotonIsoR03[fLepInfo[icoll].Size] - rhoPrime * AEffR03), 0.0 );

      //----- Generation Monte Carlo information  --------------------------------------------------------
      if ( !fIsData && !fSkipfGenInfo ) {
         if( fDebug > 3 ) { cout << "\t\t\t[3]Getting Electron MC information\n"; }
         const reco::GenParticle* gen = it_el->genLepton();
         if ( gen != NULL ) {
            fLepInfo[icoll].GenPt        [fLepInfo[icoll].Size] = gen->pt();
            fLepInfo[icoll].GenEta       [fLepInfo[icoll].Size] = gen->eta();
            fLepInfo[icoll].GenPhi       [fLepInfo[icoll].Size] = gen->phi();
            fLepInfo[icoll].GenPdgID     [fLepInfo[icoll].Size] = gen->pdgId();
            fLepInfo[icoll].GenMCTag     [fLepInfo[icoll].Size] = GetGenMCTag( gen ) ;
         }
         if( fDebug > 3 ) { cout << "\t\t\t[3]Get Electron Gen Particle\n"; }
         if ( fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] == 0 && fGenParticle_H.isValid() ) {
            for( GenIterator it_gen = fGenParticle_H->begin(); it_gen != fGenParticle_H->end() ; it_gen++ ) {
               if( fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] != 0 ) break;
               fLepInfo[icoll].GenMCTag[fLepInfo[icoll].Size] = GetGenMCTag( it_gen , it_el )  ;
            }
         }
         if( fDebug > 3 ) { cout << "\t\t\t[3]Done getting MC information\n"; }
      }

      //----- Impact parameter related  ------------------------------------------------------------------
      // Reference from UserCode/MitProd/TreeFiller/src/FillerElectrons.cc
      const reco::TransientTrack& tt = (fTrackBuilder_H.product())->build( it_el->gsfTrack() );
      const reco::Vertex thevtx = (fVertex_H.product())->at( 0 );
      const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt, thevtx );
      const double gsfsign = ( ( -it_el->gsfTrack()->dxy( thevtx.position() ) )   >= 0 ) ? 1. : -1.;
      fLepInfo[icoll].Ip3dPV[fLepInfo[icoll].Size]             = gsfsign * ip3dpv.second.value();
      fLepInfo[icoll].Ip3dPVSignificance[fLepInfo[icoll].Size] = gsfsign * ip3dpv.second.value() / ip3dpv.second.error();
      fLepInfo[icoll].Ip3dPVErr[fLepInfo[icoll].Size]          = ip3dpv.second.error();

      //----- Conversion rejection information  ----------------------------------------------------------
      fLepInfo[icoll].Eldist        [fLepInfo[icoll].Size] = it_el->convDist();
      fLepInfo[icoll].Elconvradius  [fLepInfo[icoll].Size] = it_el->convRadius();
      fLepInfo[icoll].Eldcot        [fLepInfo[icoll].Size] = it_el->convDcot();

      fLepInfo[icoll].CandRef[fLepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_el );
      fLepInfo[icoll].Size++;
   }
   return true;
}

//--------------------------------------------------------------------------------------------------
//   Legacy code section
//--------------------------------------------------------------------------------------------------
//fLepInfo[icoll].ElhasConv  [fLepInfo[icoll].Size] = ConversionTools::hasMatchedConversion( *el, fConversions_H, fBeamSpot.position() );
//
// convInfo = convFinder.getConversionInfo( *it_el, tracks_h, evt_bField );
// ConversionFinder convFinder;
// ConversionInfo convInfo;
// fLepInfo[icoll].ElConvPoint_x [fLepInfo[icoll].Size] = convInfo.pointOfConversion().x();
// fLepInfo[icoll].ElConvPoint_y [fLepInfo[icoll].Size] = convInfo.pointOfConversion().y();
// fLepInfo[icoll].ElConvPoint_z [fLepInfo[icoll].Size] = convInfo.pointOfConversion().z();

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
// fLepInfo[icoll].EgammaMVATrig      [fLepInfo[icoll].Size] = myMVATrigMethod;
// fLepInfo[icoll].sumPUPtR03                [fLepInfo[icoll].Size] = isolatorR03.getIsolationChargedAll();
// fLepInfo[icoll].sumPUPtR04                [fLepInfo[icoll].Size] = isolatorR04.getIsolationChargedAll();
// float AEffR04 = ElectronEffectiveArea::GetElectronEffectiveArea( ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, fLepInfo[icoll].Eta[fLepInfo[icoll].Size], EATarget );
// fLepInfo[icoll].IsoRhoCorrR04             [fLepInfo[icoll].Size] = fLepInfo[icoll].ChargedHadronIsoR04[fLepInfo[icoll].Size] +
//   max( fLepInfo[icoll].NeutralHadronIsoR04[fLepInfo[icoll].Size] + fLepInfo[icoll].PhotonIsoR04[fLepInfo[icoll].Size] - rhoPrime * AEffR04, 0.0 );
//
