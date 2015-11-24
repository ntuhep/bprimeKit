/*******************************************************************************
 *
 *  Filename    : bprimeKit_pair.cc
 *  Description : Filling in pair information
 *
*******************************************************************************/


#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//------------------------------------------------------------------------------ 
//   Helper functions
//------------------------------------------------------------------------------ 
bool findAncestor( const reco::Candidate* & , const reco::GenParticle* ) ;


//------------------------------------------------------------------------------ 
//   Beging method implementation
//------------------------------------------------------------------------------ 

bool bprimeKit::FillLepPair( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   math::XYZTLorentzVector sum;
   const reco::GenParticle* par1;
   const reco::GenParticle* par2;

   if( fDebug > 1 ) { std::cerr << "\t[1]Fill Pair Info." << std::endl; }
   memset( &fPairInfo, 0x00, sizeof( fPairInfo ) );

   if( fPairCollectionType >= ( int )fLeptonCollections.size() ) { fPairCollectionType = 0; } //set to something that should be valid

   for( int index1 = 0          ; index1 < fLepInfo[fPairCollectionType].Size ; ++index1 ) {
   for( int index2 = index1 + 1 ; index2 < fLepInfo[fPairCollectionType].Size ; ++index2 ) {
      if ( fPairInfo.Size >= MAX_PAIRS ) {
         cout << "ERROR: number of lepton pairs exceeds the size of array." << endl ;
         index1 = index2 = fLepInfo[fPairCollectionType].Size;
         break;
      }
      sum = fLepInfo[fPairCollectionType].CandRef[index1]->p4() + fLepInfo[fPairCollectionType].CandRef[index2]->p4();
      if( sum.mag() > 200. ) {continue;}
      
      fPairInfo.Type [fPairInfo.Size] = 1; // 1: ll
      FillfPairInfo( index1 , index2 , sum );
      if ( !fIsData && !fSkipfGenInfo ) { //mc
         par1 = par2 = NULL;
         if ( fLepInfo[fPairCollectionType].LeptonType[index1] == 11 ){ 
            par1 = ( ( pat::Electron* )fLepInfo[fPairCollectionType].CandRef[index1] )->genLepton(); } 
         else if ( fLepInfo[fPairCollectionType].LeptonType[index1] == 13 ){ 
            par1 = ( ( pat::Muon*     )fLepInfo[fPairCollectionType].CandRef[index1] )->genLepton(); }
         if ( fLepInfo[fPairCollectionType].LeptonType[index2] == 11 ){ 
            par2 = ( ( pat::Electron* )fLepInfo[fPairCollectionType].CandRef[index2] )->genLepton(); } 
         else if ( fLepInfo[fPairCollectionType].LeptonType[index2] == 13 ){ 
            par2 = ( ( pat::Muon*     )fLepInfo[fPairCollectionType].CandRef[index2] )->genLepton(); } 
         FillPairGen( par1 , par2 );
      }
      fPairInfo.Size++;
   }}
   return true;
}

bool bprimeKit::FillJetPair( const edm::Event& iEvent , const edm::EventSetup& iSetup ) 
{
   math::XYZTLorentzVector sum;
   const reco::GenParticle* par1;
   const reco::GenParticle* par2;
   
   for( int index1 = 0          ; index1 < fJetInfo[0].Size ; index1++ ) {
   for( int index2 = index1 + 1 ; index2 < fJetInfo[0].Size ; index2++ ) {

      if ( fPairInfo.Size >= MAX_PAIRS ) {
         cerr << "ERROR: number of jet pairs exceeds the size of array." << endl ;
         index1 = index2 = fJetInfo[0].Size;
         break;
      }
      sum = fJetInfo[0].CandRef[index1]->p4() + fJetInfo[0].CandRef[index2]->p4();

      if ( fJetInfo[0].Pt[index1] > 25. && fJetInfo[0].Pt[index2] > 25. && sum.mag() < 200. ) {
         fPairInfo.Type[fPairInfo.Size] = 2; // 2: jj
         FillfPairInfo( index1 , index2, sum ) ;
         if ( !fIsData && !fSkipfGenInfo ) {
            par1 = NULL;
            par2 = NULL;
            par1 = ( ( pat::Jet* )fJetInfo[0].CandRef[index1] )->genParton();
            par2 = ( ( pat::Jet* )fJetInfo[0].CandRef[index2] )->genParton();
            FillPairGen( par1 , par2 );
         }
         fPairInfo.Size++;
      }
   }}
   return true;
}

bool bprimeKit::FillfPairInfo( const int index1 , const int index2 , math::XYZTLorentzVector& sum )
{
   fPairInfo.Index     [fPairInfo.Size] = fPairInfo.Size;
   fPairInfo.Obj1Index [fPairInfo.Size] = index1;
   fPairInfo.Obj2Index [fPairInfo.Size] = index2;
   fPairInfo.Mass      [fPairInfo.Size] = sum.mag();
   fPairInfo.Pt        [fPairInfo.Size] = sum.Pt();
   fPairInfo.Eta       [fPairInfo.Size] = sum.Eta();
   fPairInfo.Phi       [fPairInfo.Size] = sum.Phi();
   return true;
}

bool bprimeKit::FillPairGen( const reco::GenParticle* par1, const reco::GenParticle* par2 )
{
   const reco::Candidate* gen1;
   const reco::Candidate* gen2;
   const reco::Candidate* mon;
   
   if( par1 == NULL || par2 == NULL ) return false;
   gen1 = par1;
   gen2 = par2;

   findAncestor( gen1 , par1 );
   findAncestor( gen2 , par2 );

   if ( gen1 != NULL && gen1->numberOfMothers() == 1 && 
        gen2 != NULL && gen2->numberOfMothers() == 1 &&
        gen1->mother( 0 ) == gen2->mother( 0 ) ) {
      mon = gen1->mother( 0 );
      fPairInfo.GenMass  [fPairInfo.Size] = mon->p4().mag();
      fPairInfo.GenPt    [fPairInfo.Size] = mon->pt();
      fPairInfo.GenEta   [fPairInfo.Size] = mon->eta();
      fPairInfo.GenPhi   [fPairInfo.Size] = mon->phi();
      fPairInfo.GenPdgID [fPairInfo.Size] = mon->pdgId();
   }
   return true;
}

//------------------------------------------------------------------------------ 
//   Helper function implementation
//------------------------------------------------------------------------------ 
bool findAncestor( const reco::Candidate*& gen , const reco::GenParticle* par ) 
{
   if( par == NULL ) return false;
   while( gen!= NULL && gen->numberOfMothers() == 1 && 
          gen->mother(0)->pdgId() == par->pdgId() ){
      gen = gen->mother(0);
   }
   return true;
}
