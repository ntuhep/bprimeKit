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
#include "MyAna/bprimeKit/interface/bprimeKit.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "TLorentzVector.h"
//------------------------------------------------------------------------------ 
//   Helper static functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
//   Class method implementation
//------------------------------------------------------------------------------
bool bprimeKit::IsSelectedMuon( const MuonIterator& mu )
{
   if( mu->pt() < 40. ) { return false; }
   if( !muon::isTightMuon( *mu ,  fPrimaryVertex ) ) { return false; }
   return true;
}


TLorentzVector bprimeKit::CleanedJetP4( JetIterator jet , bool isAK4Jet )
{
   TLorentzVector cleanedJetP4 = TLorentzVector( jet->px() , jet->py() , jet->pz() , jet->energy() ) ;
   TLorentzVector muonP4;
   const std::vector<edm::Ptr<reco::Candidate>> jet_contitutes = jet->daughterPtrVector();

   if( fDebug > 3 ) { std::cout << "\t\t\t[3]Checking Overlap" << std::endl; }
   
   for( const auto& muon : fMySelectedMuons ){
      // Cleaning against muons in a delta R radius of 0.4
      if( deltaR( jet->p4() , muon->p4() ) > 0.4 ) { continue; }
      muonP4.SetPtEtaPhiE( muon->pt(), muon->eta(), muon->phi(), muon->energy() );

      for ( const auto& jet_const : jet_contitutes  ) {
         if( fDebug > 3 ) { 
            std::cout << "\t\t\t[3]Jet constituent ref = " << jet_const.key() << std::endl
                      << "\t\t\t   Muon ref: " << muon.key() << std::endl;
         }  
         if ( jet_const.key() == muon.key() ) {
            cleanedJetP4 -= muonP4;
            if ( fDebug > 3 ) { 
               printf("\t\t\t[3]Raw     Jet: pT=%f eta=%f phi=%f\n", jet->pt(), jet->eta(), jet->phi() );
               printf("\t\t\t[3]Cleaned Jet: pt=%f eta=%f phi=%f\n", cleanedJetP4.Pt(),  cleanedJetP4.Eta(), cleanedJetP4.Phi() ) ; 
            } 
            break;
         }
      }
   }
   fJetCorrector->setJetEta( cleanedJetP4.Eta() );
   fJetCorrector->setJetPt( cleanedJetP4.Pt() );
   fJetCorrector->setJetE( cleanedJetP4.Energy() );
   fJetCorrector->setJetA( jet->jetArea() );
   fJetCorrector->setRho( *(fRho_H.product()) );
   fJetCorrector->setNPV( fVertex_H->size() ); 

   cleanedJetP4 *= fJetCorrector->getCorrection();

   return cleanedJetP4;
}

