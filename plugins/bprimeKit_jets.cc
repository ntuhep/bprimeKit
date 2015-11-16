/*******************************************************************************
 *
 *  Filename    : bprimeKit_jets.cc
 *  Description : Filling in jet information
 *
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "MyAna/bprimeKit/interface/bprimeKit_util.h"
#include <typeinfo>

//----- Jet Specific CMSSW packages  ---------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/BTauReco/interface/CATopJetTagInfo.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


bool bprimeKit::FillJet( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   //----- EDM interaction variables  -----------------------------------------------------------------

	edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
   //----- Jet collection type handling  --------------------------------------------------------------
   bool        pfjetcoll, calojetcoll, fatjetcoll , CAjetcoll  ;
   std::string subjetName;
	//----- Algorithm specific helper functions  -------------------------------------------------------
   bool jetID      ;
	char bufferJECU[32];
	pat::strbitset ret ;
	edm::ParameterSet* PS_Jets;

   for( unsigned icoll = 0; icoll < fJetCollections.size(); icoll++ ) { //loop over collections
      if( icoll >= MAX_JETCOLLECTIONS ) { cerr << "To many jets!!"; break; }
      if( fJetList_Hs.size() <= icoll ) {  cerr << "Size to large!"; continue ; }
      if( fDebug > 1 ) { 
         std::cerr << "\t[1]Fill jet  collection " << icoll 
            << " with name " << fJetCollections[icoll] << std::endl; 
      }

      memset( &fJetInfo[icoll], 0x00, sizeof( fJetInfo[icoll] ) );

      pfjetcoll   = ( fJetCollections[icoll] == "JetInfo" ) ;
      calojetcoll = ( false ) ;
      CAjetcoll   = ( false ) ;

      if( fJetCollections[icoll] =="AK8BosonJetInfo" ) {
         fatjetcoll = true;
         subjetName = "SoftDrop";
      }
      else if( fJetCollections[icoll] == "CA8TopJetInfo" ) {
         fatjetcoll = true;
         subjetName = "CMSTopTag";
      }else{
         fatjetcoll = false;
         subjetName = "";
      }
      
      //----- For Jet Uncertainties  ---------------------------------------------------------------------
      if( pfjetcoll ) {
         sprintf( bufferJECU, "AK5PFchs" );
      } else if( calojetcoll ) {
         sprintf( bufferJECU, "AK5chs" );
      } else if( fatjetcoll || CAjetcoll ) {
         sprintf( bufferJECU, "AK7PFchs" );
      }

      iSetup.get<JetCorrectionsRecord>().get( bufferJECU, JetCorParColl );
      const JetCorrectorParameters& JetCorPar = ( *JetCorParColl )["Uncertainty"];
      fJetCorrectionUncertainty = new JetCorrectionUncertainty( JetCorPar );

      for( JetIterator it_jet = fJetList_Hs[icoll]->begin(); it_jet != fJetList_Hs[icoll]->end(); it_jet++ ) {
         if ( fJetInfo[icoll].Size >= MAX_JETS ) {
            cerr << "ERROR: number of jets exceeds the size of array." << std::endl ;
            break;
         }
         if ( it_jet->pt() <= 15. ) { continue; } // IMPORTANT: Only book jet with pt>15 GeV.
         if( fRunMuonJetCleaning && !PassMuonJetClean( it_jet ) ) { continue; }
         if( fDebug > 2 ) { 
            std::cerr << "\t\t[2]Jet: Size " << fJetInfo[icoll].Size 
                      << " jet pt,eta,phi " << it_jet->pt() << "," 
                      << it_jet->eta() << "," << it_jet->phi() << endl; 
         }

         //----- Generic Jet Information  -------------------------------------------------------------------
         fJetInfo[icoll].Index         [fJetInfo[icoll].Size] = fJetInfo[icoll].Size               ;
         fJetInfo[icoll].NTracks       [fJetInfo[icoll].Size] = it_jet->associatedTracks().size() ;
         fJetInfo[icoll].Eta           [fJetInfo[icoll].Size] = it_jet->eta()                     ;
         fJetInfo[icoll].Pt            [fJetInfo[icoll].Size] = it_jet->pt()                      ;
         fJetInfo[icoll].Et            [fJetInfo[icoll].Size] = it_jet->et()                      ;
         fJetInfo[icoll].Phi           [fJetInfo[icoll].Size] = it_jet->phi()                     ;
         fJetInfo[icoll].JetCharge     [fJetInfo[icoll].Size] = it_jet->jetCharge()               ;
         fJetInfo[icoll].NConstituents [fJetInfo[icoll].Size] = it_jet->numberOfDaughters()       ;
         fJetInfo[icoll].Px            [fJetInfo[icoll].Size] = it_jet->px()                      ; //Uly 2011-04-04
         fJetInfo[icoll].Py            [fJetInfo[icoll].Size] = it_jet->py()                      ; //Uly 2011-04-04
         fJetInfo[icoll].Pz            [fJetInfo[icoll].Size] = it_jet->pz()                      ; //Uly 2011-04-04
         fJetInfo[icoll].Energy        [fJetInfo[icoll].Size] = it_jet->energy()                  ; //Uly 2011-04-04
         fJetInfo[icoll].Mass          [fJetInfo[icoll].Size] = it_jet->mass()                    ;
         fJetInfo[icoll].Area          [fJetInfo[icoll].Size] = it_jet->jetArea()                 ;
  
         //----- QG Tagger information  ---------------------------------------------------------------------
         if( fDebug > 2 ) { std::cerr << "\t\t[2]Jet: Getting QGTags ..." << endl ;}
         if( pfjetcoll  ) {
            edm::RefToBase<pat::Jet> jetRef( edm::Ref<JetList>( fJetList_Hs[icoll] , it_jet - fJetList_Hs[icoll]->begin() ));
            fJetInfo[icoll].QGTagsLikelihood [fJetInfo[icoll].Size] = (*fQGLikelihood_H)[jetRef];
            fJetInfo[icoll].QGTagsAxis2      [fJetInfo[icoll].Size] = (*fQGAxis2_H)[jetRef];
            fJetInfo[icoll].QGTagsMult       [fJetInfo[icoll].Size] = (*fQGMultiplicity_H)[jetRef];
            fJetInfo[icoll].QGTagsPtD        [fJetInfo[icoll].Size] = (*fQGPtD_H)[jetRef];
         }
         
         //----- Jet Uncertainty  ---------------------------------------------------------------------------
         if( fDebug > 2 ) { std::cerr << "\t\t[2]Jet Getting Uncertainty..." << endl ;}
         fJetCorrectionUncertainty->setJetEta( it_jet->eta() );
         fJetCorrectionUncertainty->setJetPt( it_jet->pt() ); // here you must use the CORRECTED jet pt
         if( fabs( it_jet->eta() ) <= 5.0 ) { 
            fJetInfo[icoll].Unc[fJetInfo[icoll].Size] = fJetCorrectionUncertainty->getUncertainty( true ); 
         }
         

         //----- Particle flow information  -----------------------------------------------------------------
         if( pfjetcoll ) {
            if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting Particle flow information ..." << endl ;}
            fJetInfo[icoll].NCH[fJetInfo[icoll].Size] = it_jet->chargedMultiplicity();
            fJetInfo[icoll].CEF[fJetInfo[icoll].Size] = it_jet->chargedEmEnergyFraction();
            fJetInfo[icoll].NHF[fJetInfo[icoll].Size] = it_jet->neutralHadronEnergyFraction();
            fJetInfo[icoll].NEF[fJetInfo[icoll].Size] = it_jet->neutralEmEnergyFraction();
            fJetInfo[icoll].CHF[fJetInfo[icoll].Size] = it_jet->chargedHadronEnergyFraction();
         }

         //----- Jet Substructure  --------------------------------------------------------------------------
         if( fatjetcoll ) {
            if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting Subjet information ..." << endl ;}

            fJetInfo[icoll].NjettinessAK8tau1        [fJetInfo[icoll].Size]=it_jet->userFloat( "NjettinessAK8:tau1"       ) ;
            fJetInfo[icoll].NjettinessAK8tau2        [fJetInfo[icoll].Size]=it_jet->userFloat( "NjettinessAK8:tau2"       ) ;
            fJetInfo[icoll].NjettinessAK8tau3        [fJetInfo[icoll].Size]=it_jet->userFloat( "NjettinessAK8:tau3"       ) ;
            fJetInfo[icoll].ak8PFJetsCHSSoftDropMass [fJetInfo[icoll].Size]=it_jet->userFloat( "ak8PFJetsCHSSoftDropMass" ) ;
            fJetInfo[icoll].ak8PFJetsCHSPrunedMass   [fJetInfo[icoll].Size]=it_jet->userFloat( "ak8PFJetsCHSPrunedMass"   ) ;
            fJetInfo[icoll].ak8PFJetsCHSTrimmedMass  [fJetInfo[icoll].Size]=it_jet->userFloat( "ak8PFJetsCHSTrimmedMass"  ) ;
            fJetInfo[icoll].ak8PFJetsCHSFilteredMass [fJetInfo[icoll].Size]=it_jet->userFloat( "ak8PFJetsCHSFilteredMass" ) ;
            fJetInfo[icoll].topJetMass               [fJetInfo[icoll].Size]=it_jet->userFloat( "cmsTopTagPFJetsCHSLinksAK8" ) ;

            reco::CATopJetTagInfo const * tagInfo =  dynamic_cast<reco::CATopJetTagInfo const *>( it_jet->tagInfo("caTop"));
            if ( tagInfo != 0 ) {
               fJetInfo[icoll].ca8MinMass [fJetInfo[icoll].Size] = tagInfo->properties().minMass;
               fJetInfo[icoll].ca8TopMass [fJetInfo[icoll].Size] = tagInfo->properties().topMass;
            }

            fJetInfo[icoll].NSubjets        [fJetInfo[icoll].Size] = 0;
            fJetInfo[icoll].SubjetsIdxStart [fJetInfo[icoll].Size] = 0;
            for( int idx_pre = 0; idx_pre < fJetInfo[icoll].Size; ++idx_pre ){ 
               fJetInfo[icoll].SubjetsIdxStart[fJetInfo[icoll].Size] += fJetInfo[icoll].NSubjets[idx_pre]; }
           
            auto subjetsList = it_jet->subjets( subjetName );
            for ( auto const & subjet : subjetsList ) {
               ++fJetInfo[icoll].NSubjets[fJetInfo[icoll].Size] ; 
               fJetInfo[icoll].SubjetMass_w.push_back ( subjet->mass ( ) );
               fJetInfo[icoll].SubjetPt_w.push_back   ( subjet->pt      ( ) );
               fJetInfo[icoll].SubjetEt_w.push_back   ( subjet->et      ( ) );
               fJetInfo[icoll].SubjetEta_w.push_back  ( subjet->eta     ( ) );
               fJetInfo[icoll].SubjetPhi_w.push_back  ( subjet->phi     ( ) );
               fJetInfo[icoll].SubjetArea_w.push_back ( subjet->jetArea ( ) );
               fJetInfo[icoll].SubjetPtUncorr_w.push_back( subjet->pt() * subjet->jecFactor("Uncorrected") );
               fJetInfo[icoll].SubjetCombinedSVBJetTags_w.push_back( subjet->bDiscriminator( "pfCombinedInclusiveSecondaryVertexV2BJetTags" ) );
            }
         }

         //----- Jet ID string insertions  ------------------------------------------------------------------
         if( fDebug > 2 ) { std::cerr << "\t\t[2]Jet: Getting IDs ..." << endl ;}
         jetID = true;
         PS_Jets = new edm::ParameterSet ; 
         if( pfjetcoll == true ) {
            PS_Jets->addParameter<std::string>( "version", "FIRSTDATA" );
            PS_Jets->addParameter<std::string>( "quality", "LOOSE" );
            PFJetIDSelectionFunctor pfjetIDLOOSE( *PS_Jets ) ;
            ret = pfjetIDLOOSE.getBitTemplate() ;
            ret.set( false );
            jetID = pfjetIDLOOSE( *it_jet, ret );
         } else if( calojetcoll == true ) {
            //Jet ID for Jet
            PS_Jets->addParameter<std::string>( "version", "PURE09" );
            PS_Jets->addParameter<std::string>( "quality", "LOOSE" );
            JetIDSelectionFunctor jetIDLOOSEPURE09( *PS_Jets ) ;
            ret = jetIDLOOSEPURE09.getBitTemplate();
            ret.set( false );
            jetID = jetIDLOOSEPURE09( *it_jet, ret );
         } else if( fatjetcoll == true ) {
            jetID = true; //Apply jetID in PAT level
         }
         delete PS_Jets;
         fJetInfo[icoll].JetIDLOOSE[fJetInfo[icoll].Size] = ( jetID ) ?  1 : 0;

         
         //----- Jet Correction Information  ----------------------------------------------------------------
         if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting b tags  ..." << endl ;}
         fJetInfo[icoll].PtCorrRaw   [fJetInfo[icoll].Size] = it_jet->correctedJet( "Uncorrected"       ).pt();
         fJetInfo[icoll].PtCorrL2    [fJetInfo[icoll].Size] = it_jet->correctedJet( "L2Relative"        ).pt(); // L2(rel)
         fJetInfo[icoll].PtCorrL3    [fJetInfo[icoll].Size] = it_jet->correctedJet( "L3Absolute"        ).pt(); // L3(abs)
         if( fIncludeL7 ) {
            fJetInfo[icoll].PtCorrL7g   [fJetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "gluon" ).pt(); // L7(gluon)
            fJetInfo[icoll].PtCorrL7uds [fJetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "uds"   ).pt(); // L7(uds-jet)
            fJetInfo[icoll].PtCorrL7c   [fJetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "charm" ).pt(); // L7(c-jet)
            fJetInfo[icoll].PtCorrL7b   [fJetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "bottom" ).pt(); // L7(b-jet)
         }
         
         //----- B Tagging discriminators  ------------------------------------------------------------------
         fJetInfo[icoll].combinedSecondaryVertexBJetTags             [fJetInfo[icoll].Size] = it_jet->bDiscriminator("combinedSecondaryVertexBJetTags"              );
         fJetInfo[icoll].pfJetBProbabilityBJetTags                   [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfJetBProbabilityBJetTags"                    );
         fJetInfo[icoll].pfJetProbabilityBJetTags                    [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfJetProbabilityBJetTags"                     );
         fJetInfo[icoll].pfTrackCountingHighPurBJetTags              [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighPurBJetTags"               );
         fJetInfo[icoll].pfTrackCountingHighEffBJetTags              [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighEffBJetTags"               );
         fJetInfo[icoll].pfSimpleSecondaryVertexHighEffBJetTags      [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"       );
         fJetInfo[icoll].pfSimpleSecondaryVertexHighPurBJetTags      [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"       );
         fJetInfo[icoll].pfCombinedSecondaryVertexV2BJetTags         [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"          );
         fJetInfo[icoll].pfCombinedInclusiveSecondaryVertexV2BJetTags[fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags" );
         fJetInfo[icoll].pfCombinedSecondaryVertexSoftLeptonBJetTags [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexSoftLeptonBJetTags"  );
         fJetInfo[icoll].pfCombinedMVABJetTags                       [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedMVABJetTags"                       );

         //----- Generation MC Data  ------------------------------------------------------------------------
         if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting MC data set ..." << endl ;} 
         if ( !fIsData && !fSkipfGenInfo ) {
            const reco::GenJet* genjet = it_jet->genJet();
            if ( genjet != NULL ) {
               fJetInfo[icoll].GenJetPt  [fJetInfo[icoll].Size] = genjet->pt();
               fJetInfo[icoll].GenJetEta [fJetInfo[icoll].Size] = genjet->eta();
               fJetInfo[icoll].GenJetPhi [fJetInfo[icoll].Size] = genjet->phi();
            }
            const reco::GenParticle* parton = it_jet->genParton();
            if ( parton != NULL ) {
               fJetInfo[icoll].GenPt     [fJetInfo[icoll].Size] = parton->pt();
               fJetInfo[icoll].GenEta    [fJetInfo[icoll].Size] = parton->eta();
               fJetInfo[icoll].GenPhi    [fJetInfo[icoll].Size] = parton->phi();
               fJetInfo[icoll].GenPdgID  [fJetInfo[icoll].Size] = parton->pdgId();
               fJetInfo[icoll].GenFlavor [fJetInfo[icoll].Size] = it_jet->partonFlavour();
               fJetInfo[icoll].GenHadronFlavor[fJetInfo[icoll].Size] = it_jet->hadronFlavour();
               fJetInfo[icoll].GenMCTag  [fJetInfo[icoll].Size] = getGenMCTag( parton ) ;
            }
         }
         fJetInfo[icoll].CandRef [fJetInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_jet );
         fJetInfo[icoll].Size++;
      }
      delete fJetCorrectionUncertainty;
   }
   return true;
}


//----------------------------  Jet- track association  -----------------------------
//Turned off by Enoch Chen 2015-07-06, Fatal Error: PFJet constituent is not of PFCandidate type
//tracks_x     = 0.;
//tracks_y     = 0.;
//tracks_x_tot = 0.;
//tracks_y_tot = 0.;
//if( pfjetcoll ) { //pf jets
//   for ( unsigned i = 0;  i <  it_jet->numberOfDaughters (); i++ ) {
//      const reco::PFCandidatePtr pfcand = it_jet->getPFConstituent( i );
//      reco::TrackRef trackref = pfcand->trackRef();
//      if( trackref.isNonnull() ) {
//         tracks_x_tot += ( trackref )->px();
//         tracks_y_tot += ( trackref )->py();
//         if ( fabs( ( trackref )->vz() - Signal_Vz ) < 0.1 ) {
//            tracks_x += ( trackref )->px();
//            tracks_y += ( trackref )->py();
//         }
//      }
//   }
//} else { //calo jets
//   const reco::TrackRefVector& TrackCol = it_jet->associatedTracks();
//   for ( reco::TrackRefVector::const_iterator it = TrackCol.begin(); it != TrackCol.end (); it++ ) {
//      tracks_x_tot += ( *it )->px();
//      tracks_y_tot += ( *it )->py();
//      if ( fabs( ( *it )->vz() - Signal_Vz ) < 0.1 ) {
//         tracks_x += ( *it )->px();
//         tracks_y += ( *it )->py();
//      }
//   }
//}
//fJetInfo[icoll].JVAlpha[fJetInfo[icoll].Size] = sqrt( tracks_x * tracks_x + tracks_y * tracks_y ) / it_jet->pt();
//if ( tracks_x_tot != 0. || tracks_y_tot != 0. ) {
//   fJetInfo[icoll].JVBeta[fJetInfo[icoll].Size] = sqrt( tracks_x * tracks_x + tracks_y * tracks_y ) / sqrt( tracks_x_tot * tracks_x_tot + tracks_y_tot * tracks_y_tot );
//} else {
//   fJetInfo[icoll].JVBeta[fJetInfo[icoll].Size] = -1.;
//}

