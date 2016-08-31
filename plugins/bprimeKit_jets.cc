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
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

using namespace std;

//------------------------------------------------------------------------------
//   Helper functions
//------------------------------------------------------------------------------
JetIterator GetSubjetBunch( const JetIterator&, const JetHandle& );
string GetUserFloatName( const string& );
string GetUserFloatPrefix( const string& );

//------------------------------------------------------------------------------
//   Method implementation
//------------------------------------------------------------------------------
bool bprimeKit::FillJet( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   //----- EDM interaction variables  -----------------------------------------
	edm::ESHandle<JetCorrectorParametersCollection> jetCorParColl;
   //----- Jet collection type handling  --------------------------------------
   bool        pfjetcoll, fatjetcoll  ;
   float       pt_cut ;
   string userFloat_name;
   string userFloat_prefix;
   string jetCorrectionUncertaintyTag;

   //----- Algorithm specific helper functions  -------------------------------
   bool  jetID      ;
	pat::strbitset ret ;
	edm::ParameterSet* PS_Jets;
   TLorentzVector cleanedJet;

   for( unsigned icoll = 0; icoll < fJetCollections.size(); icoll++ ) { //loop over collections
      if( icoll >= MAX_JETCOLLECTIONS ) { cerr << "To many jets!!"; break; }
      if( fJetList_Hs.size() <= icoll ) { cerr << "Size to large!"; continue ; }

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
      userFloat_name = GetUserFloatName( fJetCollections[icoll] );
      userFloat_prefix = GetUserFloatPrefix( fJetCollections[icoll] );

      iSetup.get<JetCorrectionsRecord>().get( jetCorrectionUncertaintyTag.c_str() , jetCorParColl );
      const JetCorrectorParameters& jetCorPar = ( *jetCorParColl )["Uncertainty"];
      JetCorrectionUncertainty      jecUnc( jetCorPar );

      const string jerpt_tag  = jetCorrectionUncertaintyTag + "_pt";
      const string jerphi_tag = jetCorrectionUncertaintyTag + "_phi";
      const string jersf_tag  = jetCorrectionUncertaintyTag;
      const JME::JetResolution  jetptres ( JME::JetResolution::get(iSetup, jerpt_tag ) );
      const JME::JetResolution  jetphires( JME::JetResolution::get(iSetup, jerphi_tag) );
      const JME::JetResolutionScaleFactor jetressf ( JME::JetResolutionScaleFactor::get(iSetup, jersf_tag) );


      for( auto it_jet = fJetList_Hs[icoll]->begin(); it_jet != fJetList_Hs[icoll]->end(); it_jet++ ) {
         if( fJetInfo[icoll].Size >= MAX_JETS ){
            cerr << "ERROR: number of jets exceeds the size of array." << std::endl ;
            break;
         }
         if ( it_jet->pt() <= pt_cut ) { continue; } // IMPORTANT: Only book jet with pt>15 GeV.

         //----- Generic Jet Information  -------------------------------------
         fJetInfo[icoll].Index       [fJetInfo[icoll].Size] = fJetInfo[icoll].Size              ;
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

         //----- Cleaned Jet four momentum  -----------------------------------------
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

         //----- Jet ID string insertions  -----------------------------------------
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

         //----- Jet Uncertainty  ----------------------------------------------------
         jecUnc.setJetEta( it_jet->eta() );
         jecUnc.setJetPt( it_jet->pt() ); // here you must use the CORRECTED jet pt
         if( fabs( it_jet->eta() ) <= 5.0 ) {
            float temp = jecUnc.getUncertainty(true);
            fJetInfo[icoll].Unc[fJetInfo[icoll].Size]    = temp;
            fJetInfo[icoll].JesUnc[fJetInfo[icoll].Size] = temp;
         }

         // Jet Resolution information

         JME::JetParameters jetparm;
         jetparm.setJetPt( it_jet->pt() ).setJetEta( it_jet->eta() ).setRho(fEvtInfo.Rho);
         fJetInfo[icoll].JERPt [fJetInfo[icoll].Size]   = jetptres .getResolution( jetparm );
         fJetInfo[icoll].JERPhi[fJetInfo[icoll].Size]   = jetphires.getResolution( jetparm );
         fJetInfo[icoll].JERScale[fJetInfo[icoll].Size] = jetressf .getScaleFactor( jetparm );


         //------------------------------------------------------------------------------
         //   AK4 Jet Specific variables
         //------------------------------------------------------------------------------
         if( pfjetcoll ){
            if( fJetCollections[icoll] == "JetInfo" ){
               fJetInfo[icoll].QGTagsLikelihood [fJetInfo[icoll].Size] = it_jet->userFloat("QGTaggerAK4PFCHS:qgLikelihood");
            } else if( fJetCollections[icoll] == "JetInfoPuppi" ){
            }
            //----- Particle flow information  -----------------------------------------------------------------
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
            fJetInfo[icoll].pfBoostedDoubleSecondaryVertexAK8BJetTags[fJetInfo[icoll].Size] = it_jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");
            fJetInfo[icoll].NjettinessAK8tau1        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau1"       ) ;
            fJetInfo[icoll].NjettinessAK8tau2        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau2"       ) ;
            fJetInfo[icoll].NjettinessAK8tau3        [fJetInfo[icoll].Size]=it_jet->userFloat( "Njettiness" + userFloat_name + ":tau3"       ) ;
            fJetInfo[icoll].ak8PFJetsCHSSoftDropMass [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "SoftDropMass" ) ;
            fJetInfo[icoll].ak8PFJetsCHSPrunedMass   [fJetInfo[icoll].Size]=it_jet->userFloat( userFloat_prefix + "PrunedMass"   ) ;

            fJetInfo[icoll].NSubjets        [fJetInfo[icoll].Size] = 0;
            fJetInfo[icoll].SubjetsIdxStart [fJetInfo[icoll].Size] = 0;
            for( int idx_pre = 0; idx_pre < fJetInfo[icoll].Size; ++idx_pre ){
               fJetInfo[icoll].SubjetsIdxStart[fJetInfo[icoll].Size] += fJetInfo[icoll].NSubjets[idx_pre];
            }

            JetIterator subjet_bunch = GetSubjetBunch( it_jet,fSubjetList_Hs[icoll] );
            if( subjet_bunch != fSubjetList_Hs[icoll]->end() ){
               fJetInfo[icoll].NSubjets[fJetInfo[icoll].Size] = subjet_bunch->numberOfDaughters();
               for( unsigned i = 0 ; i < subjet_bunch->numberOfDaughters(); ++i ){
                  const pat::Jet* subjet =  (pat::Jet*) subjet_bunch->daughter(i);
                  fJetInfo[icoll].SubjetMass_w.push_back ( subjet->mass()    );
                  fJetInfo[icoll].SubjetPt_w.push_back   ( subjet->pt()      );
                  fJetInfo[icoll].SubjetEt_w.push_back   ( subjet->et()      );
                  fJetInfo[icoll].SubjetEta_w.push_back  ( subjet->eta()     );
                  fJetInfo[icoll].SubjetPhi_w.push_back  ( subjet->phi()     );
                  fJetInfo[icoll].SubjetArea_w.push_back ( subjet->jetArea() );
                  fJetInfo[icoll].SubjetPtUncorr_w.push_back( subjet->pt()*subjet->jecFactor("Uncorrected"));
                  fJetInfo[icoll].SubjetCombinedSVBJetTags_w.push_back( subjet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags" ) );
                  if( !fIsData && !fSkipfGenInfo ){
                     fJetInfo[icoll].SubjetHadronFlavour_w.push_back( subjet->hadronFlavour() );
                     fJetInfo[icoll].SubjetGenFlavour_w.push_back( subjet->hadronFlavour() );
                     fJetInfo[icoll].SubjetGenPdgId_w.push_back( subjet->pdgId() );
                  }
               }
            }
         }

         //----- Generation MC Data  --------------------------------------------------
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
         fJetInfo[icoll].Size++;
      }
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

string GetUserFloatName( const string& collection )
{
   if( collection.find( "Puppi" ) != std::string::npos ){
      return "AK8Puppi" ;
   } else if( collection.find("MiniAOD") != std::string::npos ){
      return "AK8";
   } else {
      return "AK8CHS" ;
   }
}

string GetUserFloatPrefix( const string& collection )
{
   if( collection.find( "Puppi" ) != std::string::npos ){
      return "ak8PFJetsPuppi";
   } else if( collection.find("MiniAOD") != std::string::npos ){
      return "ak8PFJetsCHS";
   } else {
      return "ak8PFJetsCHS" ;
   }
}
