/*******************************************************************************
 *
 *  Filename    : bprimeKit_jets.cc
 *  Description : Filling in jet information
 *
*******************************************************************************/
#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "MyAna/bprimeKit/interface/bprimeKit_util.h"

//-------------------------  Jet specific CMSSW libraries  --------------------------
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"


bool bprimeKit::fillJet( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
	JetHandlerList     JetHandle;  //PFJets
	JetIterator        it_jet   ;
   edm::Handle<edm::ValueMap<float>> qgHandle;
   bool pfjetcoll  ;
	bool calojetcoll;
	bool fatjetcoll ;
	bool CAjetcoll  ;
	edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
	bool jetID;
	pat::strbitset ret ;
	JetCorrectionUncertainty* jecUnc;
	char bufferJECU[32];
	edm::ParameterSet* PS_Jets;

   // Turned off, see below
	//double tracks_x    ; 
	//double tracks_y     ;
	//double tracks_x_tot ;
	//double tracks_y_tot ;


   for( unsigned il = 0; il < jetlabel_.size(); il++ ) {
      JetHandle.push_back( JetHandler() );
      iEvent.getByLabel( jetlabel_[il], JetHandle[il] );
      if( debug_ > 10 ) { cout << "jets " << il << " jetlabel " << jetlabel_[il] << " with " << JetHandle[il]->size() << " entries\n"; }
   }

   if( debug_ > 10 ) {cout <<"Getting Q taggers" << endl;}
   iEvent.getByToken(qgToken_ , qgHandle); 

   if( debug_ > 10 ) {cout <<"Begin looping" << endl;}

   for( unsigned icoll = 0; icoll < jetcollections_.size(); icoll++ ) { //loop over collections
      if( icoll >= MAX_JETCOLLECTIONS ) { cerr << "To many jets!!"; break; }
      if( JetHandle.size() <= icoll ) {  cerr << "Size to large!"; continue ; }
      if( debug_ > 10 ) { cout << "Fill jet info, collection " << icoll << " with name " << jetcollections_[icoll] << endl; }

      memset( &JetInfo[icoll], 0x00, sizeof( JetInfo[icoll] ) );

      pfjetcoll   = ( jetcollections_[icoll] == "JetInfo" ) ;
      fatjetcoll  = ( jetcollections_[icoll] == "AK8JetInfo" ) ;
      calojetcoll  = ( false ) ;
      CAjetcoll   = ( false ) ;
      
      //-----------------------------  For Jet Uncertainty  ------------------------------
      if( pfjetcoll ) {
         sprintf( bufferJECU, "AK5PFchs" );
      } else if( calojetcoll ) {
         sprintf( bufferJECU, "AK5chs" );
      } else if( fatjetcoll || CAjetcoll ) {
         sprintf( bufferJECU, "AK7PFchs" );
      }
      iSetup.get<JetCorrectionsRecord>().get( bufferJECU, JetCorParColl );
      JetCorrectorParameters const& JetCorPar = ( *JetCorParColl )["Uncertainty"];
      jecUnc = new JetCorrectionUncertainty( JetCorPar );

      for( it_jet = JetHandle[icoll]->begin(); it_jet != JetHandle[icoll]->end(); it_jet++ ) {
         if ( JetInfo[icoll].Size >= MAX_JETS ) {
            fprintf( stderr, "ERROR: number of jets exceeds the size of array.\n" );
            break;//exit(0);
         }
         if ( it_jet->pt() <= 15. ) { continue; } // IMPORTANT: Only book jet with pt>15 GeV.
         if( debug_ > 11 ) { cout << "  Size " << JetInfo[icoll].Size << " jet pt,eta,phi " << it_jet->pt() << "," << it_jet->eta() << "," << it_jet->phi() << endl; }

         //------------------------------  Generic information  ------------------------------
         JetInfo[icoll].Index         [JetInfo[icoll].Size] = JetInfo[icoll].Size               ;
         JetInfo[icoll].NTracks       [JetInfo[icoll].Size] = it_jet->associatedTracks().size() ;
         JetInfo[icoll].Eta           [JetInfo[icoll].Size] = it_jet->eta()                     ;
         JetInfo[icoll].Pt            [JetInfo[icoll].Size] = it_jet->pt()                      ;
         JetInfo[icoll].Et            [JetInfo[icoll].Size] = it_jet->et()                      ;
         JetInfo[icoll].Phi           [JetInfo[icoll].Size] = it_jet->phi()                     ;
         JetInfo[icoll].JetCharge     [JetInfo[icoll].Size] = it_jet->jetCharge()               ;
         JetInfo[icoll].NConstituents [JetInfo[icoll].Size] = it_jet->numberOfDaughters()       ;
         JetInfo[icoll].Px            [JetInfo[icoll].Size] = it_jet->px()                      ; //Uly 2011-04-04
         JetInfo[icoll].Py            [JetInfo[icoll].Size] = it_jet->py()                      ; //Uly 2011-04-04
         JetInfo[icoll].Pz            [JetInfo[icoll].Size] = it_jet->pz()                      ; //Uly 2011-04-04
         JetInfo[icoll].Energy        [JetInfo[icoll].Size] = it_jet->energy()                  ; //Uly 2011-04-04
         JetInfo[icoll].Mass          [JetInfo[icoll].Size] = it_jet->mass()                    ;
         JetInfo[icoll].Area          [JetInfo[icoll].Size] = it_jet->jetArea()                 ;
  
         //----------------------------  Uncertainty information  ----------------------------
         if( debug_ > 10 ) { cout << ">>Jet>> Getting Uncertainty..." << endl ;}
         jecUnc->setJetEta( it_jet->eta() );
         jecUnc->setJetPt( it_jet->pt() ); // here you must use the CORRECTED jet pt
         if( fabs( it_jet->eta() ) <= 5.0 ) { 
            JetInfo[icoll].Unc         [JetInfo[icoll].Size] = jecUnc->getUncertainty( true ); 
         }
         
         if( debug_ > 10 ) { cout << ">>Jet>> Getting QGTags ..." << endl ;}
         JetInfo[icoll].QGTagsLikelihood  [JetInfo[icoll].Size] = -1;
         if( pfjetcoll ) {
            if( debug_ > 10 ) { cout << ">>Jet>> Getting QGTags ..." << endl ;}
            int ijet = it_jet - JetHandle[icoll]->begin();
            edm::RefToBase<pat::Jet> jetRef( edm::Ref<JetList>( JetHandle[icoll], ijet ) );
            JetInfo[icoll].QGTagsLikelihood[JetInfo[icoll].Size] = (*qgHandle)[jetRef];
         }

         //---------------------------  Particle flow information  ---------------------------
         if( pfjetcoll ) {
            if( debug_ > 10 ) { cout << ">>Jet>> Getting Particle flow information ..." << endl ;}
            JetInfo[icoll].NCH[JetInfo[icoll].Size] = it_jet->chargedMultiplicity();
            JetInfo[icoll].CEF[JetInfo[icoll].Size] = it_jet->chargedEmEnergyFraction();
            JetInfo[icoll].NHF[JetInfo[icoll].Size] = it_jet->neutralHadronEnergyFraction();
            JetInfo[icoll].NEF[JetInfo[icoll].Size] = it_jet->neutralEmEnergyFraction();
            JetInfo[icoll].CHF[JetInfo[icoll].Size] = it_jet->chargedHadronEnergyFraction();
         }

         //-------------------------------  Subjet structure  --------------------------------
         if( fatjetcoll ) {
            cout << ">> Subjets for Using JetCollection" << jetcollections_[icoll] << endl;
            if( debug_ > 10 ) { cout << ">>Jet>> Getting Subjet information ..." << endl ;}
            JetInfo[icoll].NSubjets        [JetInfo[icoll].Size] = 0;
            JetInfo[icoll].SubjetsIdxStart [JetInfo[icoll].Size] = 0;
            it_jet->userFloat("NjettinessAK8CHS:tau2");

            for( int idx_pre = 0; idx_pre < JetInfo[icoll].Size; ++idx_pre ){ 
               JetInfo[icoll].SubjetsIdxStart[JetInfo[icoll].Size] += JetInfo[icoll].NSubjets[idx_pre]; }
            
            auto wSubjets = it_jet->subjets("SoftDrop");
            for ( auto const & subjet : wSubjets ) {
               ++JetInfo[icoll].NSubjets[JetInfo[icoll].Size] ; 
               JetInfo[icoll].SubjetMass_w.push_back ( subjet->mass ( ) );
               JetInfo[icoll].SubjetPt_w.push_back   ( subjet->pt      ( ) );
               JetInfo[icoll].SubjetEt_w.push_back   ( subjet->et      ( ) );
               JetInfo[icoll].SubjetEta_w.push_back  ( subjet->eta     ( ) );
               JetInfo[icoll].SubjetPhi_w.push_back  ( subjet->phi     ( ) );
               JetInfo[icoll].SubjetArea_w.push_back ( subjet->jetArea ( ) );
               JetInfo[icoll].SubjetPtUncorr_w.push_back( subjet->correctedP4( 0 ).pt() );
               JetInfo[icoll].SubjetCombinedSVBJetTags_w.push_back( subjet->bDiscriminator( "pfCombinedInclusiveSecondaryVertexV2BJetTags" ) );
            }
         }

         //----------------------------  Jet ID string insertion  ----------------------------
         if( debug_ > 10 ) { cout << ">>Jet>> Getting IDs ..." << endl ;}
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
         JetInfo[icoll].JetIDLOOSE[JetInfo[icoll].Size] = ( jetID ) ?  1 : 0;

         //----------------------------  Jet- track association  -----------------------------
         if( debug_ > 10 ) { cout << ">>Jet>> Getting track information ..." << endl ;}
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
         //JetInfo[icoll].JVAlpha[JetInfo[icoll].Size] = sqrt( tracks_x * tracks_x + tracks_y * tracks_y ) / it_jet->pt();
         //if ( tracks_x_tot != 0. || tracks_y_tot != 0. ) {
         //   JetInfo[icoll].JVBeta[JetInfo[icoll].Size] = sqrt( tracks_x * tracks_x + tracks_y * tracks_y ) / sqrt( tracks_x_tot * tracks_x_tot + tracks_y_tot * tracks_y_tot );
         //} else {
         //   JetInfo[icoll].JVBeta[JetInfo[icoll].Size] = -1.;
         //}
         
         //----------------  Jet correction, b tagging and jet additional ID  ----------------
         if( debug_ > 10 ) { cout << ">>Jet>> Getting b tags  ..." << endl ;}
         JetInfo[icoll].PtCorrRaw   [JetInfo[icoll].Size] = it_jet->correctedJet( "Uncorrected"       ).pt();
         JetInfo[icoll].PtCorrL2    [JetInfo[icoll].Size] = it_jet->correctedJet( "L2Relative"        ).pt(); // L2(rel)
         JetInfo[icoll].PtCorrL3    [JetInfo[icoll].Size] = it_jet->correctedJet( "L3Absolute"        ).pt(); // L3(abs)
         if( includeL7_ ) {
            JetInfo[icoll].PtCorrL7g   [JetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "gluon" ).pt(); // L7(gluon)
            JetInfo[icoll].PtCorrL7uds [JetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "uds"   ).pt(); // L7(uds-jet)
            JetInfo[icoll].PtCorrL7c   [JetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "charm" ).pt(); // L7(c-jet)
            JetInfo[icoll].PtCorrL7b   [JetInfo[icoll].Size] = it_jet->correctedJet( "L7Parton", "bottom" ).pt(); // L7(b-jet)
         }
         
         JetInfo[icoll].combinedSecondaryVertexBJetTags             [JetInfo[icoll].Size] = it_jet->bDiscriminator("combinedSecondaryVertexBJetTags"              );
         JetInfo[icoll].pfJetBProbabilityBJetTags                   [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfJetBProbabilityBJetTags"                    );
         JetInfo[icoll].pfJetProbabilityBJetTags                    [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfJetProbabilityBJetTags"                     );
         JetInfo[icoll].pfTrackCountingHighPurBJetTags              [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighPurBJetTags"               );
         JetInfo[icoll].pfTrackCountingHighEffBJetTags              [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfTrackCountingHighEffBJetTags"               );
         JetInfo[icoll].pfSimpleSecondaryVertexHighEffBJetTags      [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"       );
         JetInfo[icoll].pfSimpleSecondaryVertexHighPurBJetTags      [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"       );
         JetInfo[icoll].pfCombinedSecondaryVertexV2BJetTags         [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexV2BJetTags"          );
         JetInfo[icoll].pfCombinedInclusiveSecondaryVertexV2BJetTags[JetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags" );
         JetInfo[icoll].pfCombinedSecondaryVertexSoftLeptonBJetTags [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedSecondaryVertexSoftLeptonBJetTags"  );
         JetInfo[icoll].pfCombinedMVABJetTags                       [JetInfo[icoll].Size] = it_jet->bDiscriminator("pfCombinedMVABJetTags"                       );

         //
         // DM: access double secondary vertex info
         //
         //edm::Handle<reco::JetTagCollection> doubleTagHandle;
         //iEvent.getByLabel( "doubleSecondaryVertexHighEffBJetTags", doubleTagHandle);
         //const reco::JetTagCollection & doubleTagColl = *(doubleTagHandle.product());
         //for(reco::JetTagCollection::const_iterator it = doubleTagColl.begin() ; it!=doubleTagColl.end() ; ++it) {
         //  TLorentzVector bjetv((it->first)->px(),(it->first)->py(),(it->first)->pz()) ;
         //  //if ( TMath::Abs((it->first)->pt() - it_jet->pt()) < (0.1*(it->first)->pt()) && (TMath::Abs((it->first)->eta() - it_jet->eta())) < (0.1*(it->first)->eta()) ) {
         //  if ( bjetv.DeltaR(jetv) < 0.5 ) {
         //    JetInfo[icoll].DoubleSecondaryVertexHighEffBJetTags [JetInfo[icoll].Size] = it->second ;
         //    break ;
         //  }}
         //}
         
         //------------------------------  Generation MC data  -------------------------------
         if( debug_ > 10 ) { cout << ">>Jet>> Getting MC data set ..." << endl ;} 
         if ( !isData && !skipGenInfo_ ) {
            const reco::GenJet* genjet = it_jet->genJet();
            if ( genjet != NULL ) {
               JetInfo[icoll].GenJetPt  [JetInfo[icoll].Size] = genjet->pt();
               JetInfo[icoll].GenJetEta [JetInfo[icoll].Size] = genjet->eta();
               JetInfo[icoll].GenJetPhi [JetInfo[icoll].Size] = genjet->phi();
            }
            const reco::GenParticle* parton = it_jet->genParton();
            if ( parton != NULL ) {
               JetInfo[icoll].GenPt     [JetInfo[icoll].Size] = parton->pt();
               JetInfo[icoll].GenEta    [JetInfo[icoll].Size] = parton->eta();
               JetInfo[icoll].GenPhi    [JetInfo[icoll].Size] = parton->phi();
               JetInfo[icoll].GenPdgID  [JetInfo[icoll].Size] = parton->pdgId();
               JetInfo[icoll].GenFlavor [JetInfo[icoll].Size] = it_jet->partonFlavour();
               JetInfo[icoll].GenMCTag  [JetInfo[icoll].Size] = getGenMCTag( parton ) ;
            }
         }
         JetInfo[icoll].CandRef [JetInfo[icoll].Size] = ( reco::Candidate* ) & ( *it_jet );
         JetInfo[icoll].Size++;
      }
      delete jecUnc;
   }
   return true;
}
