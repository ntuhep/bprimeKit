/*******************************************************************************
*
*  Filename    : JetNtuplizer_MuonCleaning.cc
*  Description : Helper function for Jet Cleaning
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Reference file:
*
*  Only the Muon part will be implemented
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/JetNtuplizer.hpp"

using namespace std;

/******************************************************************************/

bool
JetNtuplizer::IsSelectedMuon( const vector<pat::Muon>::const_iterator& mu ) const
{
  if( mu->pt() < 40. ){ return false; }
  if( !muon::isTightMuon( *mu,  _vtxhandle->front() ) ){ return false; }
  return true;
}

/******************************************************************************/
// *  https://github.com/cms-ljmet/Ljmet-Com/blob/CMSSW_7_4_X/src/singleLepEventSelector.cc#L929-L1041

TLorentzVector
JetNtuplizer::CleanAK4Jet( const vector<pat::Jet>::const_iterator& jet )
{
  TLorentzVector cleanedJetP4 = TLorentzVector( jet->px(), jet->py(), jet->pz(), jet->energy() );
  TLorentzVector muonP4;

  for( const auto& muon : _selectedmuons ){
    // Cleaning against muons in a delta R radius of 0.4
    if( deltaR( jet->p4(), muon->p4() ) > 0.4 ){ continue; }
    muonP4.SetPtEtaPhiE( muon->pt(), muon->eta(), muon->phi(), muon->energy() );

    for( const auto& jet_const : jet->daughterPtrVector() ){
      if( HasOverLap( muon, jet_const ) ){
        cleanedJetP4 -= muonP4;
        break;
      }
    }
  }

  return cleanedJetP4;
}

/******************************************************************************/

TLorentzVector
JetNtuplizer::CleanAK8Jet( const vector<pat::Jet>::const_iterator& fatjet )
{
  TLorentzVector cleanedJetP4 = TLorentzVector( fatjet->px(), fatjet->py(), fatjet->pz(), fatjet->energy() );
  TLorentzVector muonP4;
  edm::Ptr<reco::Candidate> jet_const;
  edm::Ptr<reco::Candidate> subjet_const;
  const reco::Jet* subjet;

  for( const auto& muon : _selectedmuons ){
    // Cleaning against muons in a delta R radius of pi/2
    // This is following the rejection scheme used in the B2G Ntuple reader
    if( deltaR( fatjet->p4(), muon->p4() ) > M_PI/2.0  ){ continue; }
    muonP4.SetPtEtaPhiE( muon->pt(), muon->eta(), muon->phi(), muon->energy() );

    for( unsigned i = 0; i < fatjet->numberOfDaughters(); ++i ){
      jet_const = fatjet->daughterPtr( i );

      if( jet_const->numberOfDaughters() > 0 ){
        // If numberOfDaughters > 0 , this constitute is a subjet and requires
        // additional looping of the subjets constituents
        subjet = dynamic_cast<const reco::Jet*>( fatjet->daughter( i ) );

        // Recasting required! daughter() passes back const reco::Candidate* (base class of reco::Jet*)
        for( unsigned i = 0; i < subjet->numberOfDaughters(); ++i ){
          subjet_const = subjet->daughterPtr( i );
          if( HasOverLap( muon, subjet_const ) ){
            cleanedJetP4 -= muonP4;
            break;
          }
        }
      } else {
        // This is a non-subjet constitute, proceed as before
        if( HasOverLap( muon, jet_const ) ){
          cleanedJetP4 -= muonP4;
        }
      }
    }
  }

  return cleanedJetP4;
}

/******************************************************************************/

bool
JetNtuplizer::HasOverLap(
  const vector<pat::Muon>::const_iterator& muon,
  const edm::Ptr<reco::Candidate>&         jet_const ) const 
{
  for( unsigned i = 0; i < muon->numberOfSourceCandidatePtrs(); ++i ){
    if( jet_const.key() == muon->sourceCandidatePtr( i ).key() ){
      return true;
    }
  }

  return false;
}
