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
#include "MyAna/bprimeKit/interface/bprimeKit"

bool bprimeKit::passMuonJetClean( const pat::Jet* jet, const edm::Event iEvent )
{
   if( _debug ) { std::cout << "Checking Overlap" << std::endl; }

   TLorentzVector jetP4;
   pat::Jet tmpJet = jet->correctedJet( 0 );
   std::vector<reco::CandidatePtr> muDaughters;

   if ( _mySelecMuons.empty() ) { return true; } 
   if ( deltaR( _mySelecMuons[0]->p4(), jet->p4() ) > 0.6 ) { return true; }

   if ( _debug ) { std::cout << "Mu origin ref = " << _mySelecMuons[0]->originalObjectRef().key() << std::endl; }
   for ( unsigned int isrc = 0; isrc < _mySelecMuons[0]->numberOfSourceCandidatePtrs(); ++isrc ) {
      if ( _mySelecMuons[0]->sourceCandidatePtr( isrc ).isAvailable() ) {
         muDaughters.push_back( _mySelecMuons[0]->sourceCandidatePtr( isrc ) );
         if ( _debug ) { std::cout << "Mu daughter ref = " << _mySelecMuons[0]->sourceCandidatePtr( isrc ).key() << std::endl; }
      }
   }
   if ( _debug ) {
      std::cout << "Jet Overlaps with the Muon... Cleaning jet..." << std::endl;
      std::cout << "Lepton : pT = " << _mySelecMuons[0]->pt() << " eta = " << _mySelecMuons[0]->eta() << " phi = " << _mySelecMuons[0]->phi() << std::endl;
      std::cout << "      Raw Jet : pT = " << _ijet->pt() << " eta = " << _ijet->eta() << " phi = " << _ijet->phi() << std::endl;
   }
   
   for ( const auto& jet_const : jet->daughterPtrVector()  ) {
      if ( _debug ) { std::cout << "Jet constituent ref = " << ( *jet_const ).key() << std::endl; }
      for ( unsigned int muI = 0; muI < muDaughters.size(); muI++ ) {
         if ( ( *jet_const ).key() == muDaughters[muI].key() ) {
            tmpJet.setP4( tmpJet.p4() - muDaughters[muI]->p4() );
            muDaughters.erase( muDaughters.begin() + muI );
            jetP4 = correctJet( tmpJet, iEvent, false, true );
            if ( _debug ) { 
               printf("  Cleaned Jet: pT=%f eta=%f phi=%f\n", tmpJet.pt(), tmpJet.eta(), tmpJet.phi() );
               printf("Clean Raw Jet: pT=%f eta=%f phi-%f\n", tmpJet.correctedJet(0).pt(), tmpJet.correctedJet(0).eta(), tmpJet.correctedJet(0).phi() );
               printf("Corrected Jet: pt=%f eta=%f phi=%f\n", jetP4.Pt(),  jetP4.Eta(), jetP4.Phi() ) ; 
            } 
            if( jetP4.Pt() > 15. && jetP4.Eta() < 4.7 ) { return true; }
            else {return false;}
         }
      }
   }
   return true;
}
