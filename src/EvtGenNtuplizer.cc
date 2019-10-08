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
  _rhonofastjettoken( GetToken<double>( "rhonofastjetsrc" ) ),
  _mettoken( GetToken<vector<pat::MET> >( "metsrc" ) ),
  _pmettoken( GetToken<vector<pat::MET> >( "puppimetsrc" ) ),
  _pileuptoken( GetToken<vector<PileupSummaryInfo> >( "pusrc" ) ),
  _beamspottoken( GetToken<reco::BeamSpot>( "beamspotsrc" ) ),
  _prefweighttoken( GetToken<double>( "prefwgtsrc" ) ),
  _prefweightuptoken( GetToken<double>( "prefwgtupsrc" ) ),
  _prefweightdowntoken( GetToken<double>( "prefwgtdownsrc" ) ),
  _genevttoken( GetToken<GenEventInfoProduct>( "genevtsrc" ) ),
  _genparticletoken( GetToken<vector<reco::GenParticle> >( "gensrc" ) ),
  _lhetoken( GetToken<LHEEventProduct>( "lhesrc" ) ),
  _mettriggertoken( GetToken<edm::TriggerResults>( "mettriggersrc" ) )
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
  iEvent.getByToken( _rhotoken,             _rhohandle             );
  iEvent.getByToken( _rhonofastjettoken,    _rhonofastjethandle    );
  iEvent.getByToken( _mettoken,             _methandle             );
  iEvent.getByToken( _pmettoken,            _pmethandle            );
  iEvent.getByToken( _beamspottoken,        _beamspothandle        );
  iEvent.getByToken( _prefweighttoken,      _prefweighthandle      );
  iEvent.getByToken( _prefweightuptoken,    _prefweightuphandle    );
  iEvent.getByToken( _prefweightdowntoken,  _prefweightdownhandle  );

  iEvent.getByToken( _pileuptoken,          _pileuphandle          );
  iEvent.getByToken( _genparticletoken,     _genparticlehandle     );
  iEvent.getByToken( _genevttoken,          _genevthandle          );
  iEvent.getByToken( _lhetoken,             _lhehandle             );
  iEvent.getByToken( _mettriggertoken,      _mettriggerhandle      );

  memset( &GenInfo, 0x00, sizeof( GenInfo ) );
  memset( &EvtInfo, 0x00, sizeof( EvtInfo ) );

  FillGen( iEvent, iSetup );
  FillEvent( iEvent, iSetup );
}
