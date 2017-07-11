/*******************************************************************************
*
*  Filename    : DumpTriggerObjects.cc
*  Description : Simple script for dumping exhuastive informatin
                  about all trigger objects stored in miniAOD file
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "TFile.h"
#include <boost/program_options.hpp>
#include <cstdio>
#include <string>

using namespace std;
namespace opt = boost::program_options;

int
main( int argc, char const* argv[] )
{

  opt::options_description desc( "Options for dumping trigger object information" );

  desc.add_options()
    ( "file,f", opt::value<string>(), "Path to file" )
    ( "path,p", opt::value<string>(), "HLT Path to use" )
    ( "HLT,h", opt::value<string>(), "HLT Process to use (HLT for data, HLT2 for reHLT MC samples)" )
    ( "filter,l", opt::value<string>(), "HLT Filter path to use" )
    ( "help", "produces help message and exit" )
  ;

  opt::variables_map input;
  opt::store( opt::parse_command_line( argc, argv, desc ), input );

  if( input.count( "help" ) ){
    cout << desc << endl;
    return 0;
  }
  if( !input.count( "file" ) ){ cout << "file not specified!" << endl;     return 1; }
  if( !input.count( "path" ) ){ cout << "HLT path not specified!" << endl; return 1; }
  if( !input.count( "HLT" ) ){ cout << "HLT process not specified!" << endl; return 1; }
  if( !input.count( "filter" ) ){ cout << "HLT filter process not specified!" << endl; return 1; }

  const string filename = input["file"].as<string>();
  const string reqpath  = input["path"].as<string>();
  const string hlt      = input["HLT"].as<string>();
  const string filter   = input["filter"].as<string>();

  fwlite::Event ev( TFile::Open( filename.c_str() ) );
  fwlite::Handle<pat::TriggerObjectStandAloneCollection> triggerobj_handle;
  fwlite::Handle<edm::TriggerResults> trigger_handle;

  unsigned i = 1;

  for( ev.toBegin(); !ev.atEnd(); ++ev, ++i ){
    triggerobj_handle.getByLabel( ev, "slimmedPatTrigger" );
    trigger_handle.getByLabel( ev, "TriggerResults", "", hlt.c_str() );

    fprintf( stdout, "Event [%u/%lld]\n", i, ev.size() );

    unsigned j = 0;

    for( auto obj : *triggerobj_handle ){
      obj.unpackNamesAndLabels( ev, *trigger_handle );

      // Checking path
      bool haspath = false;

      for( const auto& path : obj.pathNames() ){
        if( path.find( reqpath ) != std::string::npos && obj.hasPathName( path ) ){ haspath = true; break; }
      }

      if( !haspath ){ continue; }
      // Checking filter
      bool hasfilter = false;

      for( const auto& objfilter : obj.filterLabels() ){
        if( objfilter.find( filter ) != std::string::npos ){ hasfilter = true; break; }
      }

      if( !hasfilter ){ continue; }

      fprintf(
        stdout, "\tTrigger object [%d] pt:%lf eta:%lf phi%lf\n",
        j, obj.pt(), obj.eta(), obj.phi()
        );

      fprintf(
        stdout, "\t\tCollection\t %s\n",
        obj.collection().c_str()
        );

      fprintf( stdout, "\t\tFilter IDs:\t" );

      for( const auto& id : obj.filterIds() ){
        fprintf( stdout, " %d", id );
      }

      fprintf( stdout, "\n" );

      fprintf( stdout, "\t\tFilter Names:\t" );

      for( const auto& filter : obj.filterLabels() ){
        fprintf( stdout, " %s", filter.c_str() );
      }

      fprintf( stdout, "\n" );

      fprintf( stdout, "\t\tHLT Paths:\t" );

      for( const auto& path : obj.pathNames() ){
        fprintf( stdout, " %s", path.c_str() );
      }

      fprintf( stdout, "\n\n" );
      ++j;
    }
  }

  return 0;
}
