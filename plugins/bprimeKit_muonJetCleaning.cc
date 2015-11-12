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
//   Class method implementation
//------------------------------------------------------------------------------
bool bprimeKit::isSelectedMuon( const MuonIterator& mu )
{
   if( mu->pt() < 40. ) { return false; }
   if( !muon::isTightMuon( *mu ,  PrimVtx ) ) { return false; }
   return true;
}


bool bprimeKit::passMuonJetClean( JetIterator jet )
{
   if( debug_ ) { std::cout << "Checking Overlap" << std::endl; }

   TLorentzVector jetP4;
   pat::Jet tmpJet = jet->correctedJet( 0 );
   std::vector<reco::CandidatePtr> muDaughters;
   bool hasClean = false ; 

   if ( _mySelecMuons.empty() ) { return true; } 
   if ( deltaR( _mySelecMuons[0]->p4(), jet->p4() ) > 0.6 ) { return true; }

   if ( debug_ ) { std::cout << "Mu origin ref = " << _mySelecMuons[0]->originalObjectRef().key() << std::endl; }
   for ( unsigned int isrc = 0; isrc < _mySelecMuons[0]->numberOfSourceCandidatePtrs(); ++isrc ) {
      if ( _mySelecMuons[0]->sourceCandidatePtr( isrc ).isAvailable() ) {
         muDaughters.push_back( _mySelecMuons[0]->sourceCandidatePtr( isrc ) );
         if ( debug_ ) { std::cout << "Mu daughter ref = " << _mySelecMuons[0]->sourceCandidatePtr( isrc ).key() << std::endl; }
      }
   }
   if ( debug_ ) {
      std::cout << "Jet Overlaps with the Muon... Cleaning jet..." << std::endl;
      std::cout << "Lepton : pT = " << _mySelecMuons[0]->pt() << " eta = " << _mySelecMuons[0]->eta() << " phi = " << _mySelecMuons[0]->phi() << std::endl;
      std::cout << "      Raw Jet : pT = " << jet->pt() << " eta = " << jet->eta() << " phi = " << jet->phi() << std::endl;
   }
 
   // Recasting to edm::Ptr<> is required!!
   const std::vector<edm::Ptr<reco::Candidate>> jet_contitutes = jet->daughterPtrVector();
   for ( const auto& jet_const : jet_contitutes  ) {
      if ( debug_ ) { std::cout << "Jet constituent ref = " << jet_const.key() << std::endl; }
      for ( unsigned int muI = 0; muI < muDaughters.size(); muI++ ) {
         if ( jet_const.key() == muDaughters[muI].key() ) {
            tmpJet.setP4( tmpJet.p4() - muDaughters[muI]->p4() );
            muDaughters.erase( muDaughters.begin() + muI );
            jetP4 = correctJet( tmpJet , false );
            hasClean = true; 
            if ( debug_ ) { 
               printf("  Cleaned Jet: pT=%f eta=%f phi=%f\n", tmpJet.pt(), tmpJet.eta(), tmpJet.phi() );
               printf("Clean Raw Jet: pT=%f eta=%f phi-%f\n", tmpJet.correctedJet(0).pt(), tmpJet.correctedJet(0).eta(), tmpJet.correctedJet(0).phi() );
               printf("Corrected Jet: pt=%f eta=%f phi=%f\n", jetP4.Pt(),  jetP4.Eta(), jetP4.Phi() ) ; 
            } 
         }
      }
   }

   //Applying addition correction if not already 
   if( !hasClean ){ jetP4 = correctJet( *jet ); }

   // Selection after cleaning has been applied 
   if( jetP4.Pt() < 15. ) { return false; }
   if( jetP4.Eta() > 4.7) { return false; }
   return true;
}

TLorentzVector bprimeKit::correctJet( const pat::Jet& jet , bool doAK8Corr )
{
   pat::Jet correctedJet = jet.correctedJet(0);                 //copy original jet

   double ptscale = 1.0;
   double unc = 1.0;
   double pt = correctedJet.pt();
   double correction = 1.0;
   double rho = std::max( (double)EvtInfo.Rho , 0.0);

   if ( ! isData ){ 
      double pt_raw = jet.correctedJet(0).pt();
      if (doAK8Corr){
         JetCorrectorAK8->setJetEta(jet.eta());
         JetCorrectorAK8->setJetPt(pt_raw);
         JetCorrectorAK8->setJetA(jet.jetArea());
         JetCorrectorAK8->setRho(rho); 

         try{
            correction = JetCorrectorAK8->getCorrection();
         } catch(...){
            std::cout << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
            std::cout << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
            std::cout << "WARNING! Jet/MET will remain uncorrected." << std::endl;
         }
      } else {
         JetCorrector->setJetEta(jet.eta());
         JetCorrector->setJetPt(pt_raw);
         JetCorrector->setJetA(jet.jetArea());
         JetCorrector->setRho(rho); 

         try{
            correction = JetCorrector->getCorrection();
         } catch(...){
            std::cout << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
            std::cout << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
            std::cout << "WARNING! Jet/MET will remain uncorrected." << std::endl;
         }
      }

      correctedJet.scaleEnergy(correction);
      pt = correctedJet.pt();

      double factor = 0.0; // For Nominal Case
      double theAbsJetEta = abs(jet.eta());

      if ( theAbsJetEta < 0.5 ) {
         factor = .052;
      } else if ( theAbsJetEta < 1.1) {
         factor = 0.057;
      } else if ( theAbsJetEta < 1.7) {
         factor = 0.096;
      } else if ( theAbsJetEta < 2.3) {
         factor = 0.134;
      } else if ( theAbsJetEta < 5.0) {
         factor = 0.288;
      }

      const reco::GenJet* genJet = jet.genJet();
      if ( genJet && genJet->pt()>15. && (abs(genJet->pt()/pt-1)<0.5)){
         double gen_pt = genJet->pt();
         double reco_pt = pt;
         double deltapt = (reco_pt - gen_pt) * factor;
         ptscale = max(0.0, (reco_pt + deltapt) / reco_pt);
      }

      if ( 0 ){ //|| mbPar["JECup"] || mbPar["JECdown"]) {
         jecUnc_->setJetEta(jet.eta());
         jecUnc_->setJetPt(pt*ptscale);

         if ( 1 ) { //|| mbPar["JECup"]) { 
            try{
               unc = jecUnc_->getUncertainty(true);
            } catch(...){ // catch all exceptions. Jet Uncertainty tool throws when binning out of range
               std::cout  << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
               std::cout  << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
               std::cout  << "WARNING! Jet/MET will remain uncorrected." << std::endl;
               unc = 0.0;
            }
            unc = 1 + unc; 
         } else { 
            try{
               unc = jecUnc_->getUncertainty(false);
            }
            catch(...){
               std::cout  << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
               std::cout  << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
               std::cout  << "WARNING! Jet/MET will remain uncorrected." << std::endl;
               unc = 0.0;
            }
            unc = 1 - unc; 
         }
         if (pt*ptscale < 10.0){
            if( 1 ) { // mbPar["JECup"] 
               unc = 2.0 ;
            } else {
               unc = 0.01 ;
            }
         } 
      }
   } else {
      // We need to undo the default corrections and then apply the new ones

      double pt_raw = jet.correctedJet(0).pt();
      JetCorrector->setJetEta(jet.eta());
      JetCorrector->setJetPt(pt_raw);
      JetCorrector->setJetA(jet.jetArea());
      JetCorrector->setRho(rho); 

      try{
         correction = JetCorrector->getCorrection();
      }
      catch(...){
         std::cout  << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
         std::cout  << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
         std::cout  << "WARNING! Jet/MET will remain uncorrected." << std::endl;
      }

      correctedJet.scaleEnergy(correction);
      pt = correctedJet.pt();

   }

   TLorentzVector jetP4;
   jetP4.SetPtEtaPhiM(correctedJet.pt()*unc*ptscale, correctedJet.eta(),correctedJet.phi(), correctedJet.mass() );

   return jetP4;
}

