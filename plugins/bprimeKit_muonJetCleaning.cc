/*******************************************************************************
 *
 *  Filename    : bprimeKit_muonJetCleaning.cc
 *  Description : Helper function for muonJet Cleaning
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Reference file:
 *  https://github.com/cms-ljmet/Ljmet-Com/blob/CMSSW_7_4_X/src/singleLepEventSelector.cc#L929-L1041
 *
 *  Only the Muon part will be implemented
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "TLorentzVector.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
//------------------------------------------------------------------------------
//   Helper static functions
//------------------------------------------------------------------------------
typedef edm::Ptr<reco::Candidate> JetConstituent;
bool HasOverLap( const MuonIterator& , const JetConstituent& );

//------------------------------------------------------------------------------
//   Class method implementation
//------------------------------------------------------------------------------
bool bprimeKit::IsSelectedMuon( const MuonIterator& mu ) const
{
   if( mu->pt() < 40. ) { return false; }
   if( !muon::isTightMuon( *mu ,  fPrimaryVertex ) ) { return false; }
   return true;
}

TLorentzVector bprimeKit::CleanAK4Jet( JetIterator jet )
{
   TLorentzVector cleanedJetP4 = TLorentzVector( jet->px() , jet->py() , jet->pz() , jet->energy() ) ;
   TLorentzVector muonP4;

   for( const auto& muon : fMySelectedMuons ){
      // Cleaning against muons in a delta R radius of 0.4
      if( deltaR( jet->p4() , muon->p4() ) > 0.4 ) { continue; }
      muonP4.SetPtEtaPhiE( muon->pt(), muon->eta(), muon->phi(), muon->energy() );

      for ( const auto& jet_const : jet->daughterPtrVector()  ) {
         if( HasOverLap( muon, jet_const ) ) {
            cleanedJetP4 -= muonP4;
            break;
         }
      }
   }

   return cleanedJetP4;
}

TLorentzVector bprimeKit::CleanAK8Jet( JetIterator fatjet )
{
   TLorentzVector cleanedJetP4 = TLorentzVector( fatjet->px() , fatjet->py() , fatjet->pz() , fatjet->energy() ) ;
   TLorentzVector muonP4;
   JetConstituent   jet_const ;
   JetConstituent   subjet_const ;
   const reco::Jet* subjet;

   for( const auto& muon : fMySelectedMuons ){
      // Cleaning against muons in a delta R radius of pi/2
      // This is following the rejection scheme used in the B2G Ntuple reader
      if( deltaR( fatjet->p4() , muon->p4() ) > M_PI/2.0  ) { continue; }
      muonP4.SetPtEtaPhiE( muon->pt(), muon->eta(), muon->phi(), muon->energy() );

      for( unsigned i = 0 ; i < fatjet->numberOfDaughters() ; ++i ) {
         jet_const = fatjet->daughterPtr(i);

         if( jet_const->numberOfDaughters() > 0 ) {
            // If numberOfDaughters > 0 , this constitute is a subjet and requires
            // additional looping of the subjets constituents
            //
            subjet = dynamic_cast<const reco::Jet*>(fatjet->daughter(i));
            // Recasting required! daughter() passes back const reco::Candidate* (base class of reco::Jet*)
            for( unsigned i = 0 ; i < subjet->numberOfDaughters() ; ++i ){
               subjet_const = subjet->daughterPtr(i);
               if( HasOverLap( muon , subjet_const ) ){
                  cleanedJetP4 -= muonP4;
                  break;
               }
            }
         } else {
            // This is a non-subjet constitute, proceed as before
            if( HasOverLap( muon , jet_const )) {
               cleanedJetP4 -= muonP4 ;
            }
         }
      }
   }

   return cleanedJetP4;
}

//------------------------------------------------------------------------------
//   Helper Function Implementation
//------------------------------------------------------------------------------

bool HasOverLap( const MuonIterator& muon , const JetConstituent& jet_const )
{
   for( unsigned i = 0 ; i < muon->numberOfSourceCandidatePtrs() ; ++i ){
      if( jet_const.key() == muon->sourceCandidatePtr(i).key() ){
         return true;
      }
   }
   return false;
}
