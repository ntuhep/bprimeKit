/*******************************************************************************
 *
 *  Filename    : bprimeKit_jets.cc
 *  Description : Filling in jet information
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

//----- Jet Specific CMSSW packages  ---------------------------------------------------------------
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/BTauReco/interface/CATopJetTagInfo.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

using namespace std;

//------------------------------------------------------------------------------
//   Helper functions
//------------------------------------------------------------------------------
JetIterator GetSubjetBunch( const JetIterator&, const JetHandle& );

//------------------------------------------------------------------------------
//   Method implementation
//------------------------------------------------------------------------------
bool bprimeKit::FillJet( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   //----- EDM interaction variables  -----------------------------------------------------------------
	edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
   //----- Jet collection type handling  --------------------------------------------------------------
   bool        pfjetcoll, fatjetcoll  ;
   float       pt_cut ;
   string userFloat_name;
   string userFloat_prefix;
   string jetCorrectionUncertaintyTag;

   //----- Algorithm specific helper functions  -------------------------------------------------------
   bool  jetID      ;
	pat::strbitset ret ;
	edm::ParameterSet* PS_Jets;
   TLorentzVector cleanedJet;

   for( unsigned icoll = 0; icoll < fJetCollections.size(); icoll++ ) { //loop over collections
      if( icoll >= MAX_JETCOLLECTIONS ) { cerr << "To many jets!!"; break; }
      if( fJetList_Hs.size() <= icoll ) { cerr << "Size to large!"; continue ; }
      if( fDebug > 1 ) {
         std::cerr << "\t[1]Fill jet  collection " << icoll
                   << " with name " << fJetCollections[icoll] << std::endl;
      }

      memset( &fJetInfo[icoll], 0x00, sizeof( fJetInfo[icoll] ) );

      if( fJetCollections[icoll] == "JetInfo" || fJetCollections[icoll] == "JetInfoPuppi" ) {
         pfjetcoll  = true ;
         fatjetcoll = false;
         pt_cut = 15.;
         jetCorrectionUncertaintyTag = "AK4PFchs";
      } else {
         pfjetcoll  = false;
         fatjetcoll = true;
         pt_cut = 100.;
         jetCorrectionUncertaintyTag = "AK8PFchs";
      }

      if( fJetCollections[icoll].find( "Puppi" ) != std::string::npos ){
         userFloat_name   = "AK8Puppi" ;
         userFloat_prefix = "ak8PFJetsPuppi";
      } else {
         userFloat_name   = "AK8CHS" ;
         userFloat_prefix = "ak8PFJetsCHS" ;
      }

      iSetup.get<JetCorrectionsRecord>().get( jetCorrectionUncertaintyTag.c_str() , JetCorParColl );
      const JetCorrectorParameters& JetCorPar = ( *JetCorParColl )["Uncertainty"];
      fJetCorrectionUncertainty = new JetCorrectionUncertainty( JetCorPar );

      for( JetIterator it_jet = fJetList_Hs[icoll]->begin(); it_jet != fJetList_Hs[icoll]->end(); it_jet++ ) {
         if ( fJetInfo[icoll].Size >= MAX_JETS ) {
            cerr << "ERROR: number of jets exceeds the size of array." << std::endl ;
            break;
         }
         if ( it_jet->pt() <= pt_cut ) { continue; } // IMPORTANT: Only book jet with pt>15 GeV.
         if( fDebug > 2 ) {
            std::cerr << "\t\t[2]Jet: Size " << fJetInfo[icoll].Size
                      << " jet pt,eta,phi " << it_jet->pt() << ","
                      << it_jet->eta() << "," << it_jet->phi() << endl;
         }

         //----- Generic Jet Information  -------------------------------------------------------------------
         fJetInfo[icoll].Index         [fJetInfo[icoll].Size] = fJetInfo[icoll].Size              ;
         fJetInfo[icoll].NTracks       [fJetInfo[icoll].Size] = it_jet->associatedTracks().size() ;
         fJetInfo[icoll].JetCharge     [fJetInfo[icoll].Size] = it_jet->jetCharge()               ;
         fJetInfo[icoll].NConstituents [fJetInfo[icoll].Size] = it_jet->numberOfDaughters()       ;
         fJetInfo[icoll].Mass          [fJetInfo[icoll].Size] = it_jet->mass()                    ;
         fJetInfo[icoll].Area          [fJetInfo[icoll].Size] = it_jet->jetArea()                 ;
         fJetInfo[icoll].Pt            [fJetInfo[icoll].Size] = it_jet->pt();
         fJetInfo[icoll].Eta           [fJetInfo[icoll].Size] = it_jet->eta();
         fJetInfo[icoll].Phi           [fJetInfo[icoll].Size] = it_jet->phi();
         fJetInfo[icoll].Energy        [fJetInfo[icoll].Size] = it_jet->energy();
         fJetInfo[icoll].Px            [fJetInfo[icoll].Size] = it_jet->px();
         fJetInfo[icoll].Py            [fJetInfo[icoll].Size] = it_jet->py();
         fJetInfo[icoll].Pz            [fJetInfo[icoll].Size] = it_jet->pz();
         fJetInfo[icoll].Et            [fJetInfo[icoll].Size] = it_jet->et();

         //----- Jet Correction Information  ----------------------------------------------------------------
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
         if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting b tags  ..." << endl ;}
         fJetInfo[icoll].pfJetBProbabilityBJetTags                   [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"    );
         // New Btag!
         fJetInfo[icoll].combinedSecondaryVertexBJetTags             [fJetInfo[icoll].Size] = it_jet->bDiscriminator("combinedSecondaryVertexBJetTags"              );
         fJetInfo[icoll].pfJetProbabilityBJetTags                    [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfJetProbabilityBJetTags"                     );
         fJetInfo[icoll].pfTrackCountingHighPurBJetTags              [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighPurBJetTags"               );
         fJetInfo[icoll].pfTrackCountingHighEffBJetTags              [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighEffBJetTags"               );
         fJetInfo[icoll].pfSimpleSecondaryVertexHighEffBJetTags      [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"       );
         fJetInfo[icoll].pfSimpleSecondaryVertexHighPurBJetTags      [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"       );
         fJetInfo[icoll].pfCombinedSecondaryVertexV2BJetTags         [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"          );
         fJetInfo[icoll].pfCombinedInclusiveSecondaryVertexV2BJetTags[fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags" );
         fJetInfo[icoll].pfCombinedSecondaryVertexSoftLeptonBJetTags [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexSoftLeptonBJetTags"  );
         fJetInfo[icoll].pfCombinedMVABJetTags                       [fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedMVABJetTags"                       );

         //----- Cleaned Jet four momentum  ---------------------------------------------
         if( fRunMuonJetCleaning ){
            if( pfjetcoll ){
               cleanedJet = CleanAK4Jet( it_jet ) ;
            } else if( fatjetcoll ){
               cleanedJet = CleanAK8Jet( it_jet  );
            }
            fJetInfo[icoll].Pt_MuonCleaned[fJetInfo[icoll].Size]     = cleanedJet.Pt();
            fJetInfo[icoll].Eta_MuonCleaned[fJetInfo[icoll].Size]    = cleanedJet.Eta();
            fJetInfo[icoll].Phi_MuonCleaned[fJetInfo[icoll].Size]    = cleanedJet.Phi();
            fJetInfo[icoll].Energy_MuonCleaned[fJetInfo[icoll].Size] = cleanedJet.Energy();
         }
         if( fDebug > 2 ){
            cout << "\t\t[2]Cleaned  Jet Pt:" << cleanedJet.Pt() << endl;
            cout << "\t\t[2]Original Jet Pt:" << it_jet->pt() << endl;
            cout << "\t\t[2]Recorded Jet Pt:" << fJetInfo[icoll].Pt[fJetInfo[icoll].Size] << endl;
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
         } else if( fatjetcoll == true ) {
            jetID = true; //Apply jetID in PAT level
         }
         delete PS_Jets;
         fJetInfo[icoll].JetIDLOOSE[fJetInfo[icoll].Size] = ( jetID ) ?  1 : 0;

         //----- Jet Uncertainty  ---------------------------------------------------------------------------
         if( fDebug > 2 ) { std::cerr << "\t\t[2]Jet Getting Uncertainty..." << endl ;}
         fJetCorrectionUncertainty->setJetEta( it_jet->eta() );
         fJetCorrectionUncertainty->setJetPt( it_jet->pt() ); // here you must use the CORRECTED jet pt
         if( fabs( it_jet->eta() ) <= 5.0 ) {
            float temp = fJetCorrectionUncertainty->getUncertainty(true);
            fJetInfo[icoll].Unc[fJetInfo[icoll].Size]    = temp;
            fJetInfo[icoll].JesUnc[fJetInfo[icoll].Size] = temp;
         }

         //------------------------------------------------------------------------------
         //   AK4 Jet Specific variables
         //------------------------------------------------------------------------------
         if( pfjetcoll ){
            if( fDebug > 2 ) { std::cerr << "\t\t[2]Jet: Getting QGTags ..." << endl ;}
            if( fJetCollections[icoll] == "JetInfo" ){
               fJetInfo[icoll].QGTagsLikelihood [fJetInfo[icoll].Size] = it_jet->userFloat("QGTaggerAK4PFCHS:qgLikelihood");
            } else if( fJetCollections[icoll] == "JetInfoPuppi" ){
               fJetInfo[icoll].Puppivtx3DSig   [fJetInfo[icoll].Size] = it_jet->userFloat("vtx3DSig");
               fJetInfo[icoll].Puppivtx3DVal   [fJetInfo[icoll].Size] = it_jet->userFloat("vtx3DVal");
               fJetInfo[icoll].PuppivtxMass    [fJetInfo[icoll].Size] = it_jet->userFloat("vtxMass");
               fJetInfo[icoll].PuppivtxNtracks [fJetInfo[icoll].Size] = it_jet->userFloat("vtxNtracks");
               fJetInfo[icoll].PuppivtxPosX    [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPosX");
               fJetInfo[icoll].PuppivtxPosY    [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPosY");
               fJetInfo[icoll].PuppivtxPosZ    [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPosZ");
               fJetInfo[icoll].PuppivtxPx      [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPx");
               fJetInfo[icoll].PuppivtxPy      [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPy");
               fJetInfo[icoll].PuppivtxPz      [fJetInfo[icoll].Size] = it_jet->userFloat("vtxPz");

            }
            //----- Particle flow information  -----------------------------------------------------------------
            if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting Particle flow information ..." << endl ; }
            fJetInfo[icoll].NCH[fJetInfo[icoll].Size] = it_jet->chargedMultiplicity();
            fJetInfo[icoll].CEF[fJetInfo[icoll].Size] = it_jet->chargedEmEnergyFraction();
            fJetInfo[icoll].NHF[fJetInfo[icoll].Size] = it_jet->neutralHadronEnergyFraction();
            fJetInfo[icoll].NEF[fJetInfo[icoll].Size] = it_jet->neutralEmEnergyFraction();
            fJetInfo[icoll].CHF[fJetInfo[icoll].Size] = it_jet->chargedHadronEnergyFraction();
         }

         //------------------------------------------------------------------------------
         //   AK8 Jet Specific variables
         //------------------------------------------------------------------------------
         if( fatjetcoll ) {
            if( fDebug > 2 ) { cout << "\t\t[2]Jet: Getting Subjet information ..." << endl ;}
            fJetInfo[icoll].NjettinessAK8tau1        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau1"       ) ;
            fJetInfo[icoll].NjettinessAK8tau2        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau2"       ) ;
            fJetInfo[icoll].NjettinessAK8tau3        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau3"       ) ;
            fJetInfo[icoll].ak8PFJetsCHSSoftDropMass [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "SoftDropMass" ) ;
            fJetInfo[icoll].ak8PFJetsCHSPrunedMass   [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "PrunedMass"   ) ;
            fJetInfo[icoll].ak8PFJetsCHSTrimmedMass  [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "TrimmedMass"  ) ;
            fJetInfo[icoll].ak8PFJetsCHSFilteredMass [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "FilteredMass" ) ;

            if( fJetCollections[icoll].find("Top") != std::string::npos ){
               if ( it_jet->hasTagInfo("caTop") ) {
                  const reco::CATopJetTagInfo* tagInfo = dynamic_cast<const reco::CATopJetTagInfo*>( it_jet->tagInfo("caTop"));
                  fJetInfo[icoll].ca8MinMass [fJetInfo[icoll].Size] = tagInfo->properties().minMass;
                  fJetInfo[icoll].ca8TopMass [fJetInfo[icoll].Size] = tagInfo->properties().topMass;
               }
            }

            fJetInfo[icoll].NSubjets        [fJetInfo[icoll].Size] = 0;
            fJetInfo[icoll].SubjetsIdxStart [fJetInfo[icoll].Size] = 0;
            for( int idx_pre = 0; idx_pre < fJetInfo[icoll].Size; ++idx_pre ){
               fJetInfo[icoll].SubjetsIdxStart[fJetInfo[icoll].Size] += fJetInfo[icoll].NSubjets[idx_pre];
            }

            if( fDebug > 2 ){
               cout << "Subjet size (" << fSubjetList_Hs[icoll]->size()
               << ") own size: (" << fJetList_Hs[icoll]->size() << ")" << endl;
            }

            JetIterator subjet_bunch = GetSubjetBunch( it_jet , fSubjetList_Hs[icoll] );
            if( subjet_bunch != fSubjetList_Hs[icoll]->end() ) {
               if( fDebug > 3 ) {
                  cout << subjet_bunch - fSubjetList_Hs[icoll]->begin() << endl;
                  cout << "\t\t\t[SubJets] Number of subjets: " << flush <<subjet_bunch->numberOfDaughters();
               }
               fJetInfo[icoll].NSubjets[fJetInfo[icoll].Size] = subjet_bunch->numberOfDaughters() ;
               for( unsigned i = 0 ; i < subjet_bunch->numberOfDaughters() ; ++i ){
                  const pat::Jet* subjet = (pat::Jet*) subjet_bunch->daughter(i);
                  fJetInfo[icoll].SubjetMass_w.push_back ( subjet->mass() );
                  fJetInfo[icoll].SubjetPt_w.push_back   ( subjet->pt() );
                  fJetInfo[icoll].SubjetEt_w.push_back   ( subjet->et() );
                  fJetInfo[icoll].SubjetEta_w.push_back  ( subjet->eta() );
                  fJetInfo[icoll].SubjetPhi_w.push_back  ( subjet->phi() );
                  fJetInfo[icoll].SubjetArea_w.push_back ( subjet->jetArea() );
                  fJetInfo[icoll].SubjetPtUncorr_w.push_back( subjet->pt() * subjet->jecFactor("Uncorrected") );
                  fJetInfo[icoll].SubjetCombinedSVBJetTags_w.push_back( subjet->bDiscriminator( "pfCombinedInclusiveSecondaryVertexV2BJetTags" ) );
                  if( !fIsData && !fSkipfGenInfo ){
                     fJetInfo[icoll].SubjetHadronFlavour_w.push_back( subjet->hadronFlavour() );
                     fJetInfo[icoll].SubjetGenFlavour_w.push_back( subjet->hadronFlavour() );
                     fJetInfo[icoll].SubjetGenPdgId_w.push_back( subjet->pdgId() );
                  }
               }
            }
         }

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
               fJetInfo[icoll].GenMCTag  [fJetInfo[icoll].Size] = GetGenMCTag( parton ) ;
            }
         }
         fJetInfo[icoll].CandRef [fJetInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_jet );
         fJetInfo[icoll].Size++;
      }
      delete fJetCorrectionUncertainty;
   }
   return true;
}

//------------------------------------------------------------------------------
//   Helper functions implementations
//------------------------------------------------------------------------------
JetIterator GetSubjetBunch( const JetIterator& main_jet , const JetHandle& subjetlist )
{
   for( JetIterator subjet_bunch = subjetlist->begin(); subjet_bunch != subjetlist->end() ; ++subjet_bunch ){
      if( reco::deltaR2( main_jet->eta(), main_jet->phi(), subjet_bunch->eta(), subjet_bunch->phi() ) < 0.8 ){
         return subjet_bunch;
      }
   }
   return subjetlist->end();
}



//------------------------------------------------------------------------------
//   Legacy code sections
//------------------------------------------------------------------------------
// } else if( calojetcoll == true ) {
//    //Jet ID for Jet
//    PS_Jets->addParameter<std::string>( "version", "PURE09" );
//    PS_Jets->addParameter<std::string>( "quality", "LOOSE" );
//    JetIDSelectionFunctor jetIDLOOSEPURE09( *PS_Jets ) ;
//    ret = jetIDLOOSEPURE09.getBitTemplate();
//    ret.set( false );
//    jetID = jetIDLOOSEPURE09( *it_jet, ret );


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
