/*******************************************************************************
*
*  Filename    : bprimeKit_utils.cpp
*  Description : Implementation of functions given in interface/bprimeKit_utils.h
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Details     : All static global variables will be specified by the function
*                sections
*
*******************************************************************************/

#include "DataFormats/Math/interface/deltaR.h"
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include <iostream>
using namespace std;

// ----- MC tag for pat particles  ------------------------------------------------------------------
int getGenMCTag( GenIterator&, double, double, double );// Required by the below

// ------------------------------------------------------------------------------
//   MC tag for PAT particles
// ------------------------------------------------------------------------------
static const reco::Candidate* genCand;
static int bprimeTag;
static int returnTag;

int
bprimeKit::GetGenMCTag( const reco::GenParticle* gen )
{
  genCand   = gen;
  bprimeTag = 0;// 0: not b' or t'; 1: b'; 2:t'
  returnTag = 0;

  while( genCand != NULL && genCand->numberOfMothers() == 1 ){
    genCand = genCand->mother( 0 );
    bprimeTag =
      abs( genCand->pdgId() ) == 7 ? 1 : // check if it's bprime
      abs( genCand->pdgId() ) == 8 ? 2 : // check if it's tprime.
      bprimeTag;

    if( returnTag == 0 ){
      returnTag =
        abs( genCand->pdgId() ) == 23 ? 2 :
        abs( genCand->pdgId() )                  == 24 ? 1 :
        abs( genCand->pdgId() )                  == 5   ? 3 :  // from a b quark
        ( abs( genCand->pdgId() ) % 1000 ) / 100   == 5 ? 3 :  // from a B meson
        ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 5 ? 3 :   // from a bottom baryon
        abs( genCand->pdgId() )                    == 4 ? 4 :  // from a c quark
        ( abs( genCand->pdgId() ) % 1000 ) / 100   == 4 ? 4 :  // from a D meson
        ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 4 ? 4 :   // from a charm baryon
        abs( genCand->pdgId() )                    == 15 ? 7 :  // from tau
        0;
    }
  }

  returnTag += 10*bprimeTag;
  return returnTag;
}

// -----------------------  MCTag from particle Eta, phi, pt  ------------------------
static double r;
int
getGenMCTag( GenIterator& gen, double pEta, double pPhi, double pPt )
{
  r = deltaR<double>( gen->eta(), gen->phi(), pEta, pPhi );

  if( r > 0.5 ){return 0; }
  if( fabs( gen->pt() - pPt ) / gen->pt() > 0.5 ){return 0; }

  if( ( abs( gen->pdgId() ) <= 5 || abs( gen->pdgId() ) == 21 ) ){
    if( gen->status() != 3 ){return 0; }
    return 5;// matched to a parton (q,g)
  }
  if( abs( gen->pdgId() ) == 22 ){
    if( gen->status() != 1 ){return 0; }
    return 6;// matched to a photon
  }
  return 0;
}

// -------------------  Dummy functions for function overloading  --------------------
int
bprimeKit::GetGenMCTag( GenIterator& gen, ElectronIterator& el ) const
{
  return getGenMCTag( gen, el->eta(), el->phi(), el->pt() );
}

int
bprimeKit::GetGenMCTag( GenIterator& gen, MuonIterator& mu ) const
{
  return getGenMCTag( gen, mu->eta(), mu->phi(), mu->pt() );
}


// --------------------------------------------------------------------------------------------------
//   MC tag for GenParticles
// --------------------------------------------------------------------------------------------------
bool
bprimeKit::IsTprime( const int pdgid ) const
{
  if( abs( pdgid ) == 8 ){return true; }
  if( abs( pdgid ) == 8000001 ){return true; }
  return false;
}
