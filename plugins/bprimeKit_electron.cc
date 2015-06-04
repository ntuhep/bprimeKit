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


bool bprimeKit::fillElectron( const edm::Event& iEvent , const edm::EventSetup& iSetup , const size_t icoll  )
{
   ElectronHandler elecHandle ; 
   iEvent.getByLabel( eleclabel_[icoll], elecHandle );

   //----------------------  Setting up electron ID information  -----------------------
   edm::Handle<edm::ValueMap<bool>> veto_id_decisions;
   edm::Handle<edm::ValueMap<bool>> loose_id_decisions;
   edm::Handle<edm::ValueMap<bool>> medium_id_decisions;
   edm::Handle<edm::ValueMap<bool>> tight_id_decisions;
   edm::Handle<edm::ValueMap<bool>> heep_id_decisions;
   cout <<"Getting Electron IDs" << endl;
   iEvent.getByToken( eleVetoIdMapToken_ , veto_id_decisions );
   iEvent.getByToken( eleLooseIdMapToken_ , loose_id_decisions );
   iEvent.getByToken( eleMediumIdMapToken_, medium_id_decisions );
   iEvent.getByToken( eleTightIdMapToken_, tight_id_decisions );
   iEvent.getByToken( eleHEEPIdMapToken_ , heep_id_decisions );

   
   for( ElectronIterator it_el = elecHandle->begin(); it_el != elecHandle->end(); it_el++ ) {//loop over electrons in collection
      if( debug_ > 11 ) { 
         cout << "  Size " << LepInfo[icoll].Size << " el et,eta,phi " 
         << it_el->et() << "," << it_el->superCluster()->eta() << "," 
              << it_el->superCluster()->phi() << endl;
      }
      if ( LepInfo[icoll].Size >= MAX_LEPTONS ) {
         cerr << "ERROR: number of leptons exceeds the size of array." << endl;
         break;//exit(0);
      }

      //-------------------------  Inserting generic information  -------------------------
      LepInfo[icoll].Index           [LepInfo[icoll].Size] = LepInfo[icoll].Size               ;
      LepInfo[icoll].isEcalDriven    [LepInfo[icoll].Size] = it_el->ecalDrivenSeed()           ;
      LepInfo[icoll].isTrackerDriven [LepInfo[icoll].Size] = it_el->trackerDrivenSeed()        ;
      LepInfo[icoll].LeptonType      [LepInfo[icoll].Size] = 11                                ;
      LepInfo[icoll].Charge          [LepInfo[icoll].Size] = it_el->charge()                   ;
      LepInfo[icoll].ChargeGsf       [LepInfo[icoll].Size] = it_el->gsfTrack()->charge()       ;
      LepInfo[icoll].ChargeScPix     [LepInfo[icoll].Size] = it_el->scPixCharge()              ;
      LepInfo[icoll].Pt              [LepInfo[icoll].Size] = it_el->pt()                       ;
      LepInfo[icoll].Et              [LepInfo[icoll].Size] = it_el->et()                       ; //Uly 2011-04-04
      LepInfo[icoll].Eta             [LepInfo[icoll].Size] = it_el->superCluster()->eta()      ; //Uly 2011-04-04
      LepInfo[icoll].caloEta         [LepInfo[icoll].Size] = it_el->caloPosition().eta()       ;
      LepInfo[icoll].Phi             [LepInfo[icoll].Size] = it_el->superCluster()->phi()      ; //Uly 2011-04-04
      LepInfo[icoll].Px              [LepInfo[icoll].Size] = it_el->px()                       ; //Uly 2011-04-04
      LepInfo[icoll].Py              [LepInfo[icoll].Size] = it_el->py()                       ; //Uly 2011-04-04
      LepInfo[icoll].Pz              [LepInfo[icoll].Size] = it_el->pz()                       ; //Uly 2011-04-04
      LepInfo[icoll].Energy          [LepInfo[icoll].Size] = it_el->energy()                   ; //Uly 2011-04-04
      LepInfo[icoll].CaloEnergy      [LepInfo[icoll].Size] = it_el->caloEnergy()               ;
      LepInfo[icoll].e1x5            [LepInfo[icoll].Size] = it_el->scE1x5()                   ;
      LepInfo[icoll].e2x5Max         [LepInfo[icoll].Size] = it_el->scE2x5Max()                ;
      LepInfo[icoll].e5x5            [LepInfo[icoll].Size] = it_el->scE5x5()                   ;
      LepInfo[icoll].TrackIso        [LepInfo[icoll].Size] = it_el->dr03TkSumPt()              ;
      LepInfo[icoll].EcalIso         [LepInfo[icoll].Size] = it_el->dr03EcalRecHitSumEt()      ;
      LepInfo[icoll].HcalIso         [LepInfo[icoll].Size] = it_el->dr03HcalTowerSumEt()       ;
      LepInfo[icoll].HcalDepth1Iso   [LepInfo[icoll].Size] = it_el->dr03HcalDepth1TowerSumEt() ;
      LepInfo[icoll].HcalDepth2Iso   [LepInfo[icoll].Size] = it_el->dr03HcalDepth2TowerSumEt() ;

      if( !TurnOffInCMSSW73x )
         if( tracks_h.isValid() && it_el->closestCtfTrackRef().isNonnull() ) //tracks_h may not be exactly right, but should work?
         { LepInfo[icoll].ChargeCtf       [LepInfo[icoll].Size] = it_el->closestCtfTrackRef()->charge(); }
      //PFIso
      //LepInfo[icoll].ChargedHadronIso  [LepInfo[icoll].Size] = it_el->chargedHadronIso();
      //LepInfo[icoll].NeutralHadronIso  [LepInfo[icoll].Size] = it_el->neutralHadronIso();
      //LepInfo[icoll].PhotonIso         [LepInfo[icoll].Size] = it_el->photonIso();

      //-----------------------  New Isolation information: Enoch  ------------------------
      cout <<">>> Getting isolation information"<< endl;
      const auto el = elecHandle->ptrAt( LepInfo[icoll].Size );
      bool isPassVeto   = ( *veto_id_decisions   ) [el];
      bool isPassLoose  = ( *loose_id_decisions  ) [el];
      bool isPassMedium = ( *medium_id_decisions ) [el];
      bool isPassTight  = ( *tight_id_decisions  ) [el];
      bool isPassHEEP   = ( *heep_id_decisions   ) [el];
      LepInfo[icoll].EgammaCutBasedEleIdVETO   [LepInfo[icoll].Size] = isPassVeto ;
      LepInfo[icoll].EgammaCutBasedEleIdLOOSE  [LepInfo[icoll].Size] = isPassLoose ;
      LepInfo[icoll].EgammaCutBasedEleIdMEDIUM [LepInfo[icoll].Size] = isPassMedium;
      LepInfo[icoll].EgammaCutBasedEleIdTIGHT  [LepInfo[icoll].Size] = isPassTight;
      LepInfo[icoll].EgammaCutBasedEleIdHEEP   [LepInfo[icoll].Size] = isPassHEEP;

      //-------------------------  Getting isolation information  -------------------------
      if( getElectronID_ ) {
         // Add 2012 EID (https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification)
         if( !TurnOffInCMSSW73x ){
            size_t nGsfEle = 0 ;
            for( GsfIterator gsfEle=els_h->begin() ; gsfEle != els_h->end(); ++gsfEle , ++nGsfEle ){
               if ( gsfEle->gsfTrack()->pt() != it_el->gsfTrack()->pt() ) { continue ; }
               reco::GsfElectronRef ele( els_h, nGsfEle );
               const GsfElectronCollection theEGamma = *( els_h.product() );
               // MVA-ID  -- start
               Vertex dummy;
               if( TurnOffInCMSSW73x ) {
               //const Vertex* pv = &dummy;
               }
               if ( VertexHandle->size() != 0 ) {
               //   pv = &*VertexHandle->begin();
               } else { // create a dummy PV
                  Vertex::Error e;
                  e( 0, 0 ) = 0.0015 * 0.0015;
                  e( 1, 1 ) = 0.0015 * 0.0015;
                  e( 2, 2 ) = 15. * 15.;
                  Vertex::Point p( 0, 0, 0 );
                  dummy = Vertex( p, e, 0, 0, 0 );
               }

               //bool debugMVAclass = false;
               if ( TurnOnInCMSSW_7_4_1 ) {
               //   float myMVANonTrigMethod =
               //      myMVANonTrig->mvaValue( ( theEGamma[nGsfEle] ), *pv, thebuilder, lazyTools, debugMVAclass );
               //   float myMVATrigMethod =
               //      myMVATrig->mvaValue( ( theEGamma[nGsfEle] ), *pv, thebuilder, lazyTools, debugMVAclass );
               //   LepInfo[icoll].EgammaMVANonTrig   [LepInfo[icoll].Size] = myMVANonTrigMethod;
               //   LepInfo[icoll].EgammaMVATrig      [LepInfo[icoll].Size] = myMVATrigMethod;
               }
               // MVA-ID  -- end

               // alternative way to access to PF iso
               unsigned int ivtx = 0;
               VertexRef myVtxRef( VertexHandle, ivtx );

               if( !TurnOffInCMSSW73x ) {
               //   isolatorR03.fGetIsolation( &*gsfEle, &thePfColl, myVtxRef, VertexHandle );
               //   isolatorR04.fGetIsolation( &*gsfEle, &thePfColl, myVtxRef, VertexHandle );
               }

               // get particle flow isolation
               // For CMSSW_5_3_11
               //double iso_ch = it_el->pfIsolationVariables().chargedHadronIso;
               //double iso_em = it_el->pfIsolationVariables().photonIso;
               //double iso_nh = it_el->pfIsolationVariables().neutralHadronIso;
               // For CMSSW_7_2_0
               // Reference to https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/EgammaCandidates/interface/GsfElectron.h
               double iso_ch = it_el->pfIsolationVariables().sumChargedHadronPt;
               double iso_em = it_el->pfIsolationVariables().sumPhotonEt;
               double iso_nh = it_el->pfIsolationVariables().sumNeutralHadronEt;

               ElectronEffectiveArea::ElectronEffectiveAreaTarget EATarget = ElectronEffectiveArea::kEleEAFall11MC;
               if( isData ) { EATarget = ElectronEffectiveArea::kEleEAData2012; }

               // working points
               // update for CMSSW_7_2_0 (https://cmssdt.cern.ch/SDT/lxr/source//EgammaAnalysis/ElectronTools/src/EGammaCutBasedEleId.cc)

               // cuts to match tight trigger requirements
               bool trigtight = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERTIGHT, ele );
               LepInfo[icoll].EgammaCutBasedEleIdTRIGGERTIGHT  [LepInfo[icoll].Size] = trigtight;

               // for 2011 WP70 trigger
               bool trigwp70 = EgammaCutBasedEleId::PassTriggerCuts( EgammaCutBasedEleId::TRIGGERWP70, ele );
               LepInfo[icoll].EgammaCutBasedEleIdTRIGGERWP70 [LepInfo[icoll].Size] = trigwp70;

               //PFIso
               /*
                  iso_correct = iso_ch + max(iso_nh + iso_em - rhoPrime * AEff, 0.0) .
                  Effective area :
                      http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h?revision=1.3&view=markup
                  */
               LepInfo[icoll].ChargedHadronIso          [LepInfo[icoll].Size] = iso_ch;
               LepInfo[icoll].NeutralHadronIso          [LepInfo[icoll].Size] = iso_nh;
               LepInfo[icoll].PhotonIso                 [LepInfo[icoll].Size] = iso_em;
               LepInfo[icoll].ChargedHadronIsoR03       [LepInfo[icoll].Size] = isolatorR03.getIsolationCharged();
               LepInfo[icoll].NeutralHadronIsoR03       [LepInfo[icoll].Size] = isolatorR03.getIsolationNeutral();
               LepInfo[icoll].PhotonIsoR03              [LepInfo[icoll].Size] = isolatorR03.getIsolationPhoton();
               LepInfo[icoll].sumPUPtR03                [LepInfo[icoll].Size] = isolatorR03.getIsolationChargedAll();
               float AEffR03 = ElectronEffectiveArea::GetElectronEffectiveArea( ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
               double rhoPrime = 0.;
               if( !TurnOffInCMSSW73x )
               rhoPrime = max( ( double ) * ( rhoH[0].product() ), 0.0 ); 
               
               LepInfo[icoll].IsoRhoCorrR03             [LepInfo[icoll].Size] =
                  LepInfo[icoll].ChargedHadronIsoR03[LepInfo[icoll].Size] +
                  max( LepInfo[icoll].NeutralHadronIsoR03[LepInfo[icoll].Size]
                       + LepInfo[icoll].PhotonIsoR03[LepInfo[icoll].Size] - rhoPrime * AEffR03, 0.0 );

               LepInfo[icoll].ChargedHadronIsoR04       [LepInfo[icoll].Size] = isolatorR04.getIsolationCharged();
               LepInfo[icoll].NeutralHadronIsoR04       [LepInfo[icoll].Size] = isolatorR04.getIsolationNeutral();
               LepInfo[icoll].PhotonIsoR04              [LepInfo[icoll].Size] = isolatorR04.getIsolationPhoton();
               LepInfo[icoll].sumPUPtR04                [LepInfo[icoll].Size] = isolatorR04.getIsolationChargedAll();

               float AEffR04 = ElectronEffectiveArea::GetElectronEffectiveArea( ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, LepInfo[icoll].Eta[LepInfo[icoll].Size], EATarget );
               LepInfo[icoll].IsoRhoCorrR04             [LepInfo[icoll].Size] =
                  LepInfo[icoll].ChargedHadronIsoR04[LepInfo[icoll].Size] +
                  max( LepInfo[icoll].NeutralHadronIsoR04[LepInfo[icoll].Size]
                       + LepInfo[icoll].PhotonIsoR04[LepInfo[icoll].Size] - rhoPrime * AEffR04, 0.0 );

               LepInfo[icoll].ElEcalE                   [LepInfo[icoll].Size] = ele->ecalEnergy();
               LepInfo[icoll].ElhasConv                 [LepInfo[icoll].Size]
                  = ConversionTools::hasMatchedConversion( *ele, conversions_h, beamSpot.position() );
               /*
                  hOverE2012 = (hcalDepth1 + hcalDepth2)/ele->superCluster()->energy();
                  */
               LepInfo[icoll].ElhcalOverEcalBc          [LepInfo[icoll].Size] = ele->hcalOverEcalBc();
               LepInfo[icoll].Eldr03HcalDepth1TowerSumEtBc[LepInfo[icoll].Size] = ele->dr03HcalDepth1TowerSumEtBc();
               LepInfo[icoll].Eldr03HcalDepth2TowerSumEtBc[LepInfo[icoll].Size] = ele->dr03HcalDepth2TowerSumEtBc();
               LepInfo[icoll].Eldr04HcalDepth1TowerSumEtBc[LepInfo[icoll].Size] = ele->dr04HcalDepth1TowerSumEtBc();
               LepInfo[icoll].Eldr04HcalDepth2TowerSumEtBc[LepInfo[icoll].Size] = ele->dr04HcalDepth2TowerSumEtBc();
            }
         }
         //simpleEleId status : (Add by Jacky)
         //0: fails
         //1: passes electron ID only
         //2: passes electron Isolation only
         //3: passes electron ID and Isolation only
         //4: passes conversion rejection
         //5: passes conversion rejection and ID
         //6: passes conversion rejection and Isolation
         //7: passes the whole selection
         //                 LepInfo[icoll].simpleEleId95relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId95relIso");
         //                 LepInfo[icoll].simpleEleId90relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId90relIso");
         //                 LepInfo[icoll].simpleEleId85relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId85relIso");
         //                 LepInfo[icoll].simpleEleId80relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId80relIso");
         //                 LepInfo[icoll].simpleEleId70relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId70relIso");
         //                 LepInfo[icoll].simpleEleId60relIso   [LepInfo[icoll].Size] = it_el->electronID("simpleEleId60relIso");
         //                 LepInfo[icoll].simpleEleId95cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId95cIso");
         //                 LepInfo[icoll].simpleEleId90cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId90cIso");
         //                 LepInfo[icoll].simpleEleId85cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId85cIso");
         //                 LepInfo[icoll].simpleEleId80cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId80cIso");
         //                 LepInfo[icoll].simpleEleId70cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId70cIso");
         //                 LepInfo[icoll].simpleEleId60cIso     [LepInfo[icoll].Size] = it_el->electronID("simpleEleId60cIso");

         /*
            EID -- CIC
            0 - no cut passed
            1 - eID cuts passed
            2 - iso cuts passed
            4 - conversion rejection
            8 - ip cut
            Below three examples to check eId results:
            eID+Iso+ConversionRejection+IP -> ((eIDmap[electronRef] &15) == 15)
            Iso only -> ((eIDmap[electronRef] & 2) == 2)
            eID+ConversionRejection+IP -> ((eIDmap[electronRef] & 13) == 13)
          */
         //CIC without ISO
         //                 LepInfo[icoll].eidVeryLoose[LepInfo[icoll].Size] = it_el->electronID("eidVeryLoose");
         //                 LepInfo[icoll].eidLoose[LepInfo[icoll].Size] = it_el->electronID("eidLoose");
         //                 LepInfo[icoll].eidMedium[LepInfo[icoll].Size] = it_el->electronID("eidMedium");
         //                 LepInfo[icoll].eidTight[LepInfo[icoll].Size] = it_el->electronID("eidTight");
         //                 LepInfo[icoll].eidSuperTight[LepInfo[icoll].Size] = it_el->electronID("eidSuperTight");
         //                 LepInfo[icoll].eidHyperTight1[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight1");
         //                 LepInfo[icoll].eidHyperTight2[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight2");
         //                 LepInfo[icoll].eidHyperTight3[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight3");
         //                 LepInfo[icoll].eidHyperTight4[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight4");

         // CIC with ISO
         //                 LepInfo[icoll].eidVeryLooseMC[LepInfo[icoll].Size] = it_el->electronID("eidVeryLooseMC");
         //                 LepInfo[icoll].eidLooseMC[LepInfo[icoll].Size] = it_el->electronID("eidLooseMC");
         //                 LepInfo[icoll].eidMediumMC[LepInfo[icoll].Size] = it_el->electronID("eidMediumMC");
         //                 LepInfo[icoll].eidTightMC[LepInfo[icoll].Size] = it_el->electronID("eidTightMC");
         //                 LepInfo[icoll].eidSuperTightMC[LepInfo[icoll].Size] = it_el->electronID("eidSuperTightMC");
         //                 LepInfo[icoll].eidHyperTight1MC[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight1MC");
         //                 LepInfo[icoll].eidHyperTight2MC[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight2MC");
         //                 LepInfo[icoll].eidHyperTight3MC[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight3MC");
         //                 LepInfo[icoll].eidHyperTight4MC[LepInfo[icoll].Size] = it_el->electronID("eidHyperTight4MC");
      }

      LepInfo[icoll].ElEoverP              [LepInfo[icoll].Size] = it_el->eSuperClusterOverP();
      LepInfo[icoll].EldeltaEta            [LepInfo[icoll].Size] = it_el->deltaEtaSuperClusterTrackAtVtx();
      LepInfo[icoll].EldeltaPhi            [LepInfo[icoll].Size] = it_el->deltaPhiSuperClusterTrackAtVtx();
      LepInfo[icoll].ElHadoverEm           [LepInfo[icoll].Size] = it_el->hadronicOverEm();
      LepInfo[icoll].ElsigmaIetaIeta       [LepInfo[icoll].Size] = it_el->sigmaIetaIeta();
      LepInfo[icoll].ElscSigmaIetaIeta     [LepInfo[icoll].Size] = it_el->scSigmaIEtaIEta();
      LepInfo[icoll].ElEnergyErr           [LepInfo[icoll].Size] = it_el->ecalEnergyError();
      LepInfo[icoll].ElMomentumErr         [LepInfo[icoll].Size] = it_el->trackMomentumError();
      //      LepInfo[icoll].ElTrackNHits          [LepInfo[icoll].Size] = it_el->gsfTrack()->recHitsSize();   //CANNOT RUN THIS LINE WITH AODSIM
      //LepInfo[icoll].ElTrackNLostHits      [LepInfo[icoll].Size] = it_el->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
      // not valid (https://cmssdt.cern.ch/SDT/lxr/source//DataFormats/TrackReco/interface/HitPattern.h)
      LepInfo[icoll].ElTrackNLostHits      [LepInfo[icoll].Size] = -1;
      LepInfo[icoll].ElTrackDz             [LepInfo[icoll].Size] = it_el->gsfTrack()->dz( PrimVtx.position() );
      LepInfo[icoll].ElTrackDz_BS          [LepInfo[icoll].Size] = it_el->gsfTrack()->dz( beamSpot.position() );
      LepInfo[icoll].ElTrackD0             [LepInfo[icoll].Size] = it_el->gsfTrack()->d0();
      LepInfo[icoll].ElTrackDxy_BS         [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( beamSpot.position() );
      LepInfo[icoll].ElTrackDxy_PV         [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( PrimVtx.position() );
      LepInfo[icoll].ElTrackDxy_PVBS       [LepInfo[icoll].Size] = it_el->gsfTrack()->dxy( PrimVtx_BS.position() );

      // Reference from UserCode/MitProd/TreeFiller/src/FillerElectrons.cc
      if( TurnOnInCMSSW_7_4_1 ) {
      //   const reco::TransientTrack& tt = transientTrackBuilder->build( it_el->gsfTrack() );
      //   reco::Vertex thevtx = pvCol->at( 0 );
      //   const std::pair<bool, Measurement1D>& ip3dpv =  IPTools::absoluteImpactParameter3D( tt, thevtx );
      //   const double gsfsign   = ( ( -it_el->gsfTrack()->dxy( thevtx.position() ) )   >= 0 ) ? 1. : -1.;
      //   //std::cout<<"Electron Ip3dPVSignificance : "<<gsfsign*ip3dpv.second.value()/ip3dpv.second.error() <<std::endl;
      //   LepInfo[icoll].Ip3dPV[LepInfo[icoll].Size] = gsfsign * ip3dpv.second.value();
      //   LepInfo[icoll].Ip3dPVErr[LepInfo[icoll].Size] = ip3dpv.second.error();
      //   LepInfo[icoll].Ip3dPVSignificance[LepInfo[icoll].Size] = gsfsign * ip3dpv.second.value() / ip3dpv.second.error();
      }

      LepInfo[icoll].ElNClusters           [LepInfo[icoll].Size] = it_el->basicClustersSize();
      LepInfo[icoll].ElClassification      [LepInfo[icoll].Size] = it_el->classification();
      LepInfo[icoll].ElFBrem               [LepInfo[icoll].Size] = it_el->fbrem();
      LepInfo[icoll].ElNumberOfBrems       [LepInfo[icoll].Size] = it_el->numberOfBrems();
      //LepInfo[icoll].NumberOfExpectedInnerHits[LepInfo[icoll].Size] = it_el->gsfTrack()->trackerExpectedHitsInner().numberOfHits();// Add by Jacky
      LepInfo[icoll].NumberOfExpectedInnerHits[LepInfo[icoll].Size] = it_el->gsfTrack()->hitPattern().numberOfHits( reco::HitPattern::MISSING_INNER_HITS ); // Add by Jacky

      LepInfo[icoll].vertexZ               [LepInfo[icoll].Size] = it_el->vertex().z();//Uly 2011-04-04

      //Conversion rejection (Add by Jacky)
      ConversionFinder convFinder;

      // Conversion rejection (Version2).
      if( !TurnOffInCMSSW73x )
         if( tracks_h.isValid() ) { // turn off on CMSSW73X
            if( debug_ > 15 ) { cout << "   Get conversion info\n"; }
            ConversionInfo convInfo = convFinder.getConversionInfo( *it_el, tracks_h, evt_bField );
            LepInfo[icoll].Eldist        [LepInfo[icoll].Size] = convInfo.dist();
            LepInfo[icoll].Eldcot        [LepInfo[icoll].Size] = convInfo.dcot();
            LepInfo[icoll].Elconvradius  [LepInfo[icoll].Size] = convInfo.radiusOfConversion();
            LepInfo[icoll].ElConvPoint_x [LepInfo[icoll].Size] = convInfo.pointOfConversion().x();
            LepInfo[icoll].ElConvPoint_y [LepInfo[icoll].Size] = convInfo.pointOfConversion().y();
            LepInfo[icoll].ElConvPoint_z [LepInfo[icoll].Size] = convInfo.pointOfConversion().z();
         }

      //   static bool hasMatchedConversion(const reco::GsfElectron &ele,
      //       const edm::Handle<reco::ConversionCollection> &convCol, const math::XYZPoint &beamspot,
      //       bool allowCkfMatch=true, float lxyMin=2.0, float probMin=1e-6, uint nHitsBeforeVtxMax=0);
      //LepInfo[icoll].ElhasConv[LepInfo[icoll].Size] = ConversionTools::hasMatchedConversion(*it_el, conversions_h, beamSpot.position());
      //   float mHits = it_el->gsfTrack()->trackerExpectedHitsInner().numberOfHits();

      // For CIC (CutBasedElectronID.cc) First check electron categories from CutBasedElectronID.cc
      float dist_ = ( it_el->convDist() == -9999. ? 9999 : it_el->convDist() );
      float dcot_ = ( it_el->convDcot() == -9999. ? 9999 : it_el->convDcot() );
      LepInfo[icoll].dcotdist[LepInfo[icoll].Size] = ( ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) > 0 ? ( 0.04 - std::max( fabs( dist_ ), fabs( dcot_ ) ) ) : 0 );
      LepInfo[icoll].ElseedEoverP[LepInfo[icoll].Size] = it_el->eSeedClusterOverP();
      LepInfo[icoll].ElHcalIso04[LepInfo[icoll].Size]  = it_el->dr04HcalTowerSumEt();
      // ElTrackDxy_PVBS for ip_gsf
      LepInfo[icoll].ElEcalIso04[LepInfo[icoll].Size]  = it_el->dr04EcalRecHitSumEt();
      // iso_sumoet = iso_sum*(40./scEt);

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
      LepInfo[icoll].CandRef[LepInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_el );
      LepInfo[icoll].Size++;
   }
   return true;
}

