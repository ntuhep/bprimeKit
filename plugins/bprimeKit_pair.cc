/*******************************************************************************
 *
 *  Filename    : bprimeKit_pair.cc
 *  Description : Filling in pair information
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *  
 *  Details     :
 *
*******************************************************************************/


#include "MyAna/bprimeKit/interface/bprimeKit.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//------------------------------------------------------------------------------ 
//   Helper static variables and functions
//------------------------------------------------------------------------------ 
static math::XYZTLorentzVector Sum;
static const reco::GenParticle* par1;
static const reco::GenParticle* par2;
static const reco::Candidate* gen1;
static const reco::Candidate* gen2;
static const reco::Candidate* mon;

bool findAncestor( const reco::Candidate*& , const reco::GenParticle* ) ;

bool bprimeKit::fillLepPair( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   if( debug_ > 5 ) { cout << "\tFill Pair Info."; }
   memset( &PairInfo, 0x00, sizeof( PairInfo ) );

   if( pairColl_ >= ( int )lepcollections_.size() ) { pairColl_ = 0; } //set to something that should be valid

   for( int index1 = 0          ; index1 < LepInfo[pairColl_].Size ; ++index1 ) {
   for( int index2 = index1 + 1 ; index2 < LepInfo[pairColl_].Size ; ++index2 ) {
      if ( PairInfo.Size >= MAX_PAIRS ) {
         cout << "ERROR: number of lepton pairs exceeds the size of array." << endl ;
         index1 = index2 = LepInfo[pairColl_].Size;
         break;
      }
      Sum = LepInfo[pairColl_].CandRef[index1]->p4() + LepInfo[pairColl_].CandRef[index2]->p4();
      if( Sum.mag() > 200. ) {continue;}
      
      PairInfo.Type [PairInfo.Size] = 1; // 1: ll
      fillPairInfo( index1 , index2 );
      if ( !isData && !skipGenInfo_ ) { //mc
         par1 = par2 = NULL;
         if ( LepInfo[pairColl_].LeptonType[index1] == 11 ){ 
            par1 = ( ( pat::Electron* )LepInfo[pairColl_].CandRef[index1] )->genLepton(); 
         } else if ( LepInfo[pairColl_].LeptonType[index1] == 13 ){ 
            par1 = ( ( pat::Muon*     )LepInfo[pairColl_].CandRef[index1] )->genLepton(); }
         if ( LepInfo[pairColl_].LeptonType[index2] == 11 ){ 
            par2 = ( ( pat::Electron* )LepInfo[pairColl_].CandRef[index2] )->genLepton(); 
         } else if ( LepInfo[pairColl_].LeptonType[index2] == 13 ){ 
            par2 = ( ( pat::Muon*     )LepInfo[pairColl_].CandRef[index2] )->genLepton(); }
         fillPairGen();
      }
      PairInfo.Size++;

   }}
   return true;
}

bool bprimeKit::fillJetPair( const edm::Event& iEvent , const edm::EventSetup& iSetup ) 
{
   for( int index1 = 0          ; index1 < JetInfo[0].Size ; index1++ ) {
   for( int index2 = index1 + 1 ; index2 < JetInfo[0].Size ; index2++ ) {

      if ( PairInfo.Size >= MAX_PAIRS ) {
         cerr << "ERROR: number of jet pairs exceeds the size of array." << endl ;
         index1 = index2 = JetInfo[0].Size;
         break; }
      Sum = JetInfo[0].CandRef[index1]->p4() + JetInfo[0].CandRef[index2]->p4();

      if ( JetInfo[0].Pt[index1] > 25. && JetInfo[0].Pt[index2] > 25. && Sum.mag() < 200. ) {
         PairInfo.Type[PairInfo.Size] = 2; // 2: jj
         fillPairInfo( index1 , index2 ) ;

         if ( !isData && !skipGenInfo_ ) {
            par1 = NULL;
            par2 = NULL;
            par1 = ( ( pat::Jet* )JetInfo[0].CandRef[index1] )->genParton();
            par2 = ( ( pat::Jet* )JetInfo[0].CandRef[index2] )->genParton();
            fillPairGen();
         }
         PairInfo.Size++;
      }
   }}
   return true;
}


bool bprimeKit::fillPairInfo( int index1 , int index2 )
{
   PairInfo.Index     [PairInfo.Size] = PairInfo.Size;
   PairInfo.Obj1Index [PairInfo.Size] = index1;
   PairInfo.Obj2Index [PairInfo.Size] = index2;
   PairInfo.Mass      [PairInfo.Size] = Sum.mag();
   PairInfo.Pt        [PairInfo.Size] = Sum.Pt();
   PairInfo.Eta       [PairInfo.Size] = Sum.Eta();
   PairInfo.Phi       [PairInfo.Size] = Sum.Phi();
   return true;
}

bool bprimeKit::fillPairGen()
{
   if( par1 == NULL || par2 == NULL ) return false;
   gen1 = par1;
   gen2 = par2;

   findAncestor( gen1 , par1 );
   findAncestor( gen2 , par2 );

   if ( gen1 != NULL && gen1->numberOfMothers() == 1 && 
        gen2 != NULL && gen2->numberOfMothers() == 1 &&
        gen1->mother( 0 ) == gen2->mother( 0 ) ) {
      mon = gen1->mother( 0 );
      PairInfo.GenMass  [PairInfo.Size] = mon->p4().mag();
      PairInfo.GenPt    [PairInfo.Size] = mon->pt();
      PairInfo.GenEta   [PairInfo.Size] = mon->eta();
      PairInfo.GenPhi   [PairInfo.Size] = mon->phi();
      PairInfo.GenPdgID [PairInfo.Size] = mon->pdgId();
   }
   return true;
}

//------------------------------------------------------------------------------ 
//   Helper function implementation
//------------------------------------------------------------------------------ 
bool findAncestor( const reco::Candidate*& gen , const reco::Particle* par ) 
{
   if( par2 == NULL ) return false;
   while( gen!= NULL && gen->numberofMothers() == 1 && 
          gen->mother(0)->pdgId() == par->pdgId() ){
      gen = gen->mother(0);
   }
   return true;
}
