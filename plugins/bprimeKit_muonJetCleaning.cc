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
bool bprimeKit::IsSelectedMuon( const MuonIterator& mu )
{
   if( mu->pt() < 40. ) { return false; }
   if( !muon::isTightMuon( *mu ,  fPrimaryVertex ) ) { return false; }
   return true;
}


bool bprimeKit::PassMuonJetClean( JetIterator jet )
{
   if( fDebug ) { std::cout << "Checking Overlap" << std::endl; }

   TLorentzVector jetP4;
   pat::Jet tmpJet = jet->correctedJet( 0 );
   std::vector<reco::CandidatePtr> muDaughters;
   bool hasClean = false ; 

   if ( !fMySelectedMuons.empty() && deltaR( fMySelectedMuons[0]->p4(), jet->p4() ) < 0.6 ) {
      if ( fDebug ) {
         std::cout << "Jet Overlaps with the Muon... Cleaning jet..." << std::endl;
         std::cout << "Lepton  : pT = " << fMySelectedMuons[0]->pt() << " eta = " << fMySelectedMuons[0]->eta() << " phi = " << fMySelectedMuons[0]->phi() << std::endl;
         std::cout << "Raw Jet : pT = " << jet->pt() << " eta = " << jet->eta() << " phi = " << jet->phi() << std::endl;
         std::cout << "Mu origin ref = " << fMySelectedMuons[0]->originalObjectRef().key() << std::endl; 
      }
      for ( unsigned int isrc = 0; isrc < fMySelectedMuons[0]->numberOfSourceCandidatePtrs(); ++isrc ) {
         if ( fMySelectedMuons[0]->sourceCandidatePtr( isrc ).isAvailable() ) {
            muDaughters.push_back( fMySelectedMuons[0]->sourceCandidatePtr( isrc ) );
            if ( fDebug ) { 
               std::cout << "Mu daughter ref = " << fMySelectedMuons[0]->sourceCandidatePtr( isrc ).key() << std::endl; 
            }
         }
      }
   }

   // Recasting to edm::Ptr<> is required!!
   const std::vector<edm::Ptr<reco::Candidate>> jet_contitutes = jet->daughterPtrVector();
   for ( unsigned int muI = 0; muI < muDaughters.size(); muI++ ) {
      for ( const auto& jet_const : jet_contitutes  ) {
         if ( fDebug ) { std::cout << "Jet constituent ref = " << jet_const.key() << std::endl; }
         if ( jet_const.key() == muDaughters[muI].key() ) {
            tmpJet.setP4( tmpJet.p4() - muDaughters[muI]->p4() );
            muDaughters.erase( muDaughters.begin() + muI );
            jetP4 = bprimeKit::CorrectJet( tmpJet , false );
            hasClean = true; 
            if ( fDebug ) { 
               printf("  Cleaned Jet: pT=%f eta=%f phi=%f\n", tmpJet.pt(), tmpJet.eta(), tmpJet.phi() );
               printf("Clean Raw Jet: pT=%f eta=%f phi-%f\n", tmpJet.correctedJet(0).pt(), tmpJet.correctedJet(0).eta(), tmpJet.correctedJet(0).phi() );
               printf("Corrected Jet: pt=%f eta=%f phi=%f\n", jetP4.Pt(),  jetP4.Eta(), jetP4.Phi() ) ; 
            } 
         }
      }
   }

   //Applying addition correction if not already 
   if( !hasClean ){ jetP4 = bprimeKit::CorrectJet( *jet ); }

   // Selection after cleaning has been applied 
   if( jetP4.Pt()  < 15.  ) { return false; }
   if( jetP4.Eta() >  4.7 ) { return false; }
   if( fDebug ) {
      std::cout << "Passed!" << std::endl;}
   return true;
}

TLorentzVector bprimeKit::CorrectJet( const pat::Jet& jet , bool doAK8Corr )
{
   pat::Jet correctedJet = jet.correctedJet(0);                 //copy original jet

   double ptscale = 1.0;
   double unc = 1.0;
   double pt = correctedJet.pt();
   double correction = 1.0;
   double rho = std::max( (double)fEvtInfo.Rho , 0.0);

   if ( ! fIsData ){ 
      double pt_raw = jet.correctedJet(0).pt();
      if (doAK8Corr){
         fJetCorrectorAK8->setJetEta(jet.eta());
         fJetCorrectorAK8->setJetPt(pt_raw);
         fJetCorrectorAK8->setJetA(jet.jetArea());
         fJetCorrectorAK8->setRho(rho); 

         try{
            correction = fJetCorrectorAK8->getCorrection();
         } catch(...){
            std::cout << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
            std::cout << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
            std::cout << "WARNING! Jet/MET will remain uncorrected." << std::endl;
         }
      } else {
         fJetCorrector->setJetEta(jet.eta());
         fJetCorrector->setJetPt(pt_raw);
         fJetCorrector->setJetA(jet.jetArea());
         fJetCorrector->setRho(rho); 

         try{
            correction = fJetCorrector->getCorrection();
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
         fJetCorrectionUncertainty->setJetEta(jet.eta());
         fJetCorrectionUncertainty->setJetPt(pt*ptscale);

         if ( 1 ) { //|| mbPar["JECup"]) { 
            try{
               unc = fJetCorrectionUncertainty->getUncertainty(true);
            } catch(...){ // catch all exceptions. Jet Uncertainty tool throws when binning out of range
               std::cout  << "WARNING! Exception thrown by JetCorrectionUncertainty!" << std::endl;
               std::cout  << "WARNING! Possibly, trying to correct a jet/MET outside correction range." << std::endl;
               std::cout  << "WARNING! Jet/MET will remain uncorrected." << std::endl;
               unc = 0.0;
            }
            unc = 1 + unc; 
         } else { 
            try{
               unc = fJetCorrectionUncertainty->getUncertainty(false);
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
         fJetCorrector->setJetEta(jet.eta());
         fJetCorrector->setJetPt(pt_raw);
         fJetCorrector->setJetA(jet.jetArea());
         fJetCorrector->setRho(rho); 

         try{
            correction = fJetCorrector->getCorrection();
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

