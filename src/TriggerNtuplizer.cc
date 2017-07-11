/*******************************************************************************
*
*  Filename    : bprimeKit_trgobj.cc
*  Description : Storing trigger objects for dedicated triggers
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "bpkFrameWork/bprimeKit/interface/TriggerNtuplizer.hpp"
#include <regex>
using namespace std;

/*******************************************************************************
*   TriggerNtuplizer constructor and desctructor
*******************************************************************************/
TriggerNtuplizer::TriggerNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ):
  NtuplizerBase( iConfig, bpk ),
  _triggertoken( GetToken<edm::TriggerResults>("triggersrc") ),
  _triggerobjtoken( GetToken<std::vector<pat::TriggerObjectStandAlone>>("triggerobjsrc") )
{

  for( const auto& paramset : iConfig.getParameter<std::vector<edm::ParameterSet> >( "triggerlist" ) ){
    std::pair<std::string, std::string> mypair;
    mypair.first  = paramset.getParameter<std::string>( "HLTPath" );
    mypair.second = paramset.getParameter<std::string>( "HLTFilter" );
    _triggerlist.push_back( mypair );
  }
}

/******************************************************************************/

TriggerNtuplizer::~TriggerNtuplizer()
{

}

/*******************************************************************************
*   Prototyping helper functions
*******************************************************************************/
void
TriggerNtuplizer::RegisterTree( TTree* tree )
{
  TrgInfo.RegisterTree( tree );
}

/******************************************************************************/

void
TriggerNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _triggertoken, _triggerhandle );
  iEvent.getByToken( _triggerobjtoken, _triggerobjhandle );

  memset( &TrgInfo, 0x00, sizeof( TrgInfo ) );

  for( auto obj : *_triggerobjhandle ){
    obj.unpackNamesAndLabels( iEvent, *_triggerhandle );

    for( const auto& hltpair : _triggerlist ){
      if( TrgInfo.Size >= 64 ){ break; }

      std::string matchedpath   = "";
      std::string matchedfilter = "";
      bool matchresult          = TrigObjMatchPathFilter(
        obj,
        hltpair.first, matchedpath,
        hltpair.second, matchedfilter
        );
      if( !matchresult ){ continue; }
      TrgInfo.Pt        [TrgInfo.Size] = obj.pt();
      TrgInfo.Eta       [TrgInfo.Size] = obj.eta();
      TrgInfo.Phi       [TrgInfo.Size] = obj.phi();
      TrgInfo.Energy    [TrgInfo.Size] = obj.energy();
      TrgInfo.TriggerBit[TrgInfo.Size] = bprimeKit::GetTriggerIdx( matchedpath );

      for( const auto& filterid : obj.filterIds() ){
        TrgInfo.FilterLabel[TrgInfo.Size] *= 100;
        TrgInfo.FilterLabel[TrgInfo.Size] += filterid;
      }

      TrgInfo.Size++;
    }
  }
}


/*******************************************************************************
*   Helper functions implementations
*******************************************************************************/

bool
TriggerNtuplizer::TrigObjMatchPathFilter(
  const pat::TriggerObjectStandAlone& obj,
  const std::string&                  path,
  std::string&                        matchedpath,
  const std::string&                  filter,
  std::string&                        matchedfilter
  )
{
  bool hasmatchpath   = false;
  bool hasmatchfilter = false;

  const std::regex pathreg( GlobToRegex( path ) );

  for( const auto& objpath : obj.pathNames() ){
    if( std::regex_match( objpath, pathreg ) && obj.hasPathName( objpath ) ){
      hasmatchpath = true;
      matchedpath  = objpath;
      break;
    }
  }

  const std::regex filterreg( GlobToRegex( filter ) );

  for( const auto& objfilter : obj.filterLabels() ){
    if( std::regex_match( objfilter, filterreg ) && obj.hasFilterLabel( objfilter ) ){
      hasmatchfilter = true;
      matchedfilter  = objfilter;
      break;
    }
  }

  return hasmatchpath && hasmatchfilter;
}

/******************************************************************************/

string
TriggerNtuplizer::GlobToRegex( const std::string& query )
{
  static const std::regex starmatch( "\\*" );
  static const std::string starrep( ".*" );

  static const std::regex qmmatch( "\\?" );
  static const std::string qmrep( "." );

  string ans = query;
  ans = std::regex_replace( ans, starmatch, starrep );
  ans = std::regex_replace( ans, qmmatch, qmrep );
  return ans;
}
