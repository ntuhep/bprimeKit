/*******************************************************************************
 *
 *  Filename    : bprimeKit_utils.cpp
 *  Description : The common functions used in different branches
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/

#include "MyAna/bprimeKit/interface/bprimeKit.h"
#include "DataFormats/Math/interface/deltaR.h"

//------------------------------------------------------------------------------ 
//   Prototype of all functions, static variables are function specific
//------------------------------------------------------------------------------ 
int getGenMCTag( const reco::GenParticle* );
int getGenMCTag( GenIterator& , double , double , double ); //Required by the below
int getGenMCTag( GenIterator& , ElectronIterator& );
int getGenMCTag( GenIterator& , MuonIterator& ) ;


//------------------------------------------------------------------------------ 
//   Begin implemenation
//------------------------------------------------------------------------------ 
static const reco::Candidate* genCand ; 
static int bprimeTag;
static int returnTag;

int getGenMCTag( const reco::GenParticle* gen )
{
   genCand    = gen;
   bprimeTag  = 0;  // 0: not b' or t'; 1: b'; 2:t'
   returnTag  = 0;
   while( genCand != NULL && genCand->numberOfMothers() == 1 ) {
      genCand = genCand->mother( 0 );
      if ( abs( genCand->pdgId() ) == 7 ) { bprimeTag = 1; } // check if it's bprime
      if ( abs( genCand->pdgId() ) == 8 ) { bprimeTag = 2; } // check if it's tprime.
      if ( returnTag == 0 ) {
         if      ( abs( genCand->pdgId() )                    == 23 ) { returnTag = 2; }
         else if ( abs( genCand->pdgId() )                    == 24 ) { returnTag = 1; }
         else if ( abs( genCand->pdgId() )                    == 5  ) { returnTag = 3; } // from a b quark
         else if ( ( abs( genCand->pdgId() ) % 1000 ) / 100   == 5  ) { returnTag = 3; } // from a B meson
         else if ( ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 5  ) { returnTag = 3; } // from a bottom baryon
         else if ( abs( genCand->pdgId() )                    == 4  ) { returnTag = 4; } // from a c quark
         else if ( ( abs( genCand->pdgId() ) % 1000 ) / 100   == 4  ) { returnTag = 4; } // from a D meson
         else if ( ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 4  ) { returnTag = 4; } // from a charm baryon
         else if ( abs( genCand->pdgId() )                    == 15 ) { returnTag = 7; } // from tau
      }
   }
   if ( bprimeTag == 1 ) { returnTag += 10; }
   if ( bprimeTag == 2 ) { returnTag += 20; }
   return returnTag;
}

//-----------------------  MCTag from particle Eta, phi, pt  ------------------------
static double r;
int getGenMCTag( GenIterator& gen , double pEta , double pPhi , double pPt )
{
   r = deltaR<double>( gen->eta(), gen->phi(), pEta, pPhi );

   if( r > 0.5 ) return 0;
   if( fabs( gen->pt() - pPt ) / gen->pt() > 0.5 ) return 0;
   
   if ( ( abs( gen->pdgId() ) <= 5 || abs( gen->pdgId() ) == 21 ) ){
      if( gen->status() != 3 ) return 0;
      return  5; // matched to a parton (q,g)
   }
   if ( abs( gen->pdgId() ) == 22 ) {
      if( gen->status() != 1 ) return 0;
      return 6; // matched to a photon
   }
   return 0;
}

//-------------------  Dummy functions for function overloading  --------------------
int getGenMCTag( GenIterator& gen , ElectronIterator& el )
{
   return getGenMCTag( gen , el->eta() , el->phi() , el->pt() ) ;
}

int getGenMCTag( GenIterator& gen , MuonIterator& mu ) 
{
   return getGenMCTag( gen , mu->eta() , mu->phi() , mu->pt() ) ; 
}
