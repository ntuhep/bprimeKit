/*******************************************************************************
*
*  Filename    : EvtGenNtuplizer.cc
*  Description : Main control flow for event info and gen info ntuplization
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/EvtGenNtuplizer.hpp"

using namespace std;

/*******************************************************************************
*   EvtGenNtuplizer constructor and destructor
*******************************************************************************/
EvtGenNtuplizer::EvtGenNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
  NtuplizerBase( iConfig, bpk ),
  _rhotoken( GetToken<double>( "rhosrc" ) ),
  _mettoken( GetToken<vector<pat::MET> >( "metsrc" ) ),
  _pmettoken( GetToken<vector<pat::MET> >( "puppimetsrc" ) ),
  _pileuptoken( GetToken<vector<PileupSummaryInfo> >( "pusrc" ) ),
  _hlttoken( GetToken<edm::TriggerResults>( "hltsrc" ) ),
  _beamspottoken( GetToken<reco::BeamSpot>( "beamspotsrc" ) ),

  _genevttoken( GetToken<GenEventInfoProduct>( "genevtsrc" ) ),
  _genparticletoken( GetToken<vector<reco::GenParticle> >( "gensrc" ) ),
  _gendigitoken( GetToken<L1GlobalTriggerReadoutRecord>( "gtdigisrc" ) ),
  _lhetoken( GetToken<LHEEventProduct>( "lhesrc" ) ),

  _mettriggertoken( GetToken<edm::TriggerResults>( "mettriggersrc" ) ),
  _metbadmutoken( GetToken<bool>( "metbadmusrc" ) ),
  _metbadchadtoken( GetToken<bool>( "metbadchadsrc" ) )
{

}

EvtGenNtuplizer::~EvtGenNtuplizer()
{

}

/*******************************************************************************
*   Main control flow
*******************************************************************************/
void
EvtGenNtuplizer::RegisterTree( TTree* tree )
{
  EvtInfo.RegisterTree( tree );
  GenInfo.RegisterTree( tree );
}

/******************************************************************************/

void
EvtGenNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _rhotoken,         _rhohandle      );
  iEvent.getByToken( _mettoken,         _methandle      );
  iEvent.getByToken( _pmettoken,        _pmethandle     );
  iEvent.getByToken( _beamspottoken,    _beamspothandle );
  iEvent.getByToken( _hlttoken,         _triggerhandle  );

  iEvent.getByToken( _pileuptoken,      _pileuphandle      );
  iEvent.getByToken( _genparticletoken, _genparticlehandle );
  iEvent.getByToken( _genevttoken,      _genevthandle      );
  iEvent.getByToken( _lhetoken,         _lhehandle         );
  iEvent.getByToken( _gendigitoken,     _recordhandle      );

  iEvent.getByToken( _mettriggertoken,  _mettriggerhandle );
  iEvent.getByToken( _metbadmutoken,    _metbadmuhandle   );
  iEvent.getByToken( _metbadchadtoken,  _metbadchadhandle );

  bool changed = true;
  _hltconfig.init( iEvent.getRun(), iSetup, "HLT", changed );

  memset( &GenInfo, 0x00, sizeof( GenInfo ) );
  memset( &EvtInfo, 0x00, sizeof( EvtInfo ) );

  FillGen( iEvent, iSetup );
  FillEvent( iEvent, iSetup );
}
