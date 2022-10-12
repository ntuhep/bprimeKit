/*******************************************************************************

*  Filename    : bprimeKit.cc
*  Description : The virtual and explicit functions for the bprimeKit
*
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.hpp"

#include "bpkFrameWork/bprimeKit/interface/EvtGenNtuplizer.hpp"
#include "bpkFrameWork/bprimeKit/interface/JetNtuplizer.hpp"
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"
#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"
#include "bpkFrameWork/bprimeKit/interface/PhotonNtuplizer.hpp"
#include "bpkFrameWork/bprimeKit/interface/TriggerNtuplizer.hpp"
#include "bpkFrameWork/bprimeKit/interface/VertexNtuplizer.hpp"

#include <TFile.h>
#include <TTree.h>
#include <iostream>

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/MakerMacros.h"// For plugin definition
#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace std;


/*******************************************************************************
*   Constructor and destructor
*******************************************************************************/

bprimeKit::bprimeKit( const edm::ParameterSet& iConfig ):
  _lheruntoken( consumes<LHERunInfoProduct, edm::InRun>( iConfig.getParameter<edm::InputTag>( "lherunsrc" ) ) ) 
{
  // Event and Gen settings
  const auto& evtgensetting = iConfig.getParameter<edm::ParameterSet>( "evtgensetting" );
  _ntuplizerlist.push_back( new EvtGenNtuplizer( evtgensetting, this ) );

  // Trigger object settings
  const auto& triggersetting = iConfig.getParameter<edm::ParameterSet>( "triggersetting" );
  _ntuplizerlist.push_back( new TriggerNtuplizer( triggersetting, this ) );

  // Vertex settings
  const auto& vtxsetting = iConfig.getParameter<edm::ParameterSet>( "vtxsetting" );
  _ntuplizerlist.push_back( new VertexNtuplizer( vtxsetting, this ) );

  // Electron settings
  for( const auto& lepparam : iConfig.getParameter<edm::VParameterSet>( "leptonsettings" ) ){
    _ntuplizerlist.push_back( new LeptonNtuplizer( lepparam, this ) );
  }

  // Photon settings
  for( const auto& photonparam : iConfig.getParameter<edm::VParameterSet>( "photonsettings" ) ){
    _ntuplizerlist.push_back( new PhotonNtuplizer( photonparam, this ) );
  }

  // Jet settings
  for( const auto& jetparam : iConfig.getParameter<edm::VParameterSet>( "jetsettings" ) ){
    _ntuplizerlist.push_back( new JetNtuplizer( jetparam, this, consumesCollector() ) );
  }

  _hltmap.clear();

  for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ){
    _hltmap.insert( pair<std::string, int>( TriggerBooking[i], i ) );
  }

  /***** MADITORY!! DO NOT REMOVE  *********************************************/
  edm::Service<TFileService> fs;
  TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
  /******************************************************************************/
}

/******************************************************************************/

bprimeKit::~bprimeKit()
{
  for( auto ntuplizer : _ntuplizerlist ){
    delete ntuplizer;
  }
}


/*******************************************************************************
*
*  Note :
*    1. All writing objects (TTrees, TH1Fs) MUST be created here and
*       not in the analyzer constructor!!
*    2. Do not use the WRITE functions and delete operator!! These are
*       automatically handled by the TFileService instance.
*
*******************************************************************************/
void
bprimeKit::beginJob()
{
  BaseTree = new TTree( "root", "root" );
  //RunTree  = new TTree( "run", "run"  );

  for( auto ntuplizer : _ntuplizerlist ){
    ntuplizer->RegisterTree( BaseTree );
  }

  //RunInfo.RegisterTree( RunTree );
}

/******************************************************************************/

void
bprimeKit::endJob()
{
  /***** DO NOT DELETE TREES!  **************************************************/
}


/*******************************************************************************
*   Event based analysis methods
*******************************************************************************/
void
bprimeKit::beginRun( const edm::Run& iRun, const edm::EventSetup& iSetup )
{
}

/******************************************************************************/

void
bprimeKit::endRun( const edm::Run& iRun, const edm::EventSetup& iSetup )
{
  //GetRunObjects( iRun, iSetup );
  //FillRunInfo();
  //RunTree->Fill();
}

/******************************************************************************/

void
bprimeKit::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  for( auto ntuplizer : _ntuplizerlist ){
    ntuplizer->Analyze( iEvent, iSetup );
  }

  BaseTree->Fill();
}


DEFINE_FWK_MODULE( bprimeKit );
