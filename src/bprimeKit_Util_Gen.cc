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
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.hpp"
#include <iostream>
using namespace std;

/*******************************************************************************
*   Common method for getting pat::Muon, pat::Jet, and pat::Electron gen tag
*******************************************************************************/
int
bprimeKit::GetGenMCTag( const reco::GenParticle* gen )
{
  const reco::Candidate* genCand = gen;
  int bprimeTag                    = 0;// 0: not b' or t'; 1: b'; 2:t'
  int returnTag                    = 0;

  try {
    while( genCand != NULL && genCand->numberOfMothers() == 1 ){
      genCand   = genCand->mother( 0 );
      bprimeTag =
        abs( genCand->pdgId() ) == 7 ? 1 : // check if it's bprime
        abs( genCand->pdgId() ) == 8 ? 2 : // check if it's tprime.
        bprimeTag;

      if( returnTag == 0 ){
        returnTag =
          abs( genCand->pdgId() )                    == 23 ? 2 :
          abs( genCand->pdgId() )                    == 24 ? 1 :
          abs( genCand->pdgId() )                    == 5  ? 3 :  // from a b quark
          ( abs( genCand->pdgId() ) % 1000 ) / 100   == 5  ? 3 :  // from a B meson
          ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 5  ? 3 :   // from a bottom baryon
          abs( genCand->pdgId() )                    == 4  ? 4 :  // from a c quark
          ( abs( genCand->pdgId() ) % 1000 ) / 100   == 4  ? 4 :  // from a D meson
          ( abs( genCand->pdgId() ) % 10000 ) / 1000 == 4  ? 4 :   // from a charm baryon
          abs( genCand->pdgId() )                    == 15 ? 7 :  // from tau
          0;
      }
    }
  } catch( cms::Exception& x ){
  }

  returnTag += 10*bprimeTag;
  return returnTag;
}


/******************************************************************************/
std::map<std::string,int> bprimeKit::_hltmap;

int
bprimeKit::GetTriggerIdx( const string& triggername )
{
  if( _hltmap.count( triggername ) ){
    return _hltmap.at(triggername);
  } else {
    return -1;
  }
}
