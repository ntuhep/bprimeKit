/*******************************************************************************
*
*  Filename    : DumpLHEweightID.cc
*  Description : Get the first run stored in a MC MiniAOD file and dump all the
*                LHE weight ID settings
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Run.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

#include "TFile.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace opt = boost::program_options;

int
main( int argc, char* argv[] )
{

  opt::options_description desc( "Options for DumpLHEweightID" );
  desc.add_options()
    ( "inputfile,i", opt::value<string>()->required(), "MiniAOD file to dump input" )
    ( "help,h", "print usage message" );
  ;
  opt::variables_map args;
  opt::store( opt::parse_command_line( argc, argv, desc ), args );
  if( args.count( "help" ) ){
    cout << "Usage:" << endl;
    cout << desc << endl;
    return 0;
  }

  fwlite::Run run( TFile::Open( args["inputfile"].as<string>().c_str() ) );
  fwlite::Handle<LHERunInfoProduct> lheinfo;
  lheinfo.getByLabel( run, "externalLHEProducer" );

  run.toBegin();

  for( auto iter = lheinfo->headers_begin(); iter != lheinfo->headers_end(); iter++ ){
    if( iter->tag() == "initrwgt" ){
      for( const auto& line : iter->lines() ){
        cout << line;
      }
    }
  }

  return 0;
}
