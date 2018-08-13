/*******************************************************************************
*
*  Filename    : bprimeKit_leptons.cc
*  Description : Filling in lepton information
*  Details     : Since all leptons are stored in the same branch in the
*                bprimeKit rather than being stored by type, as with the
*                miniAOD format, it sublayer is required for stripping
*                information out of the miniAOD.
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/LeptonNtuplizer.hpp"

using namespace std;

/*******************************************************************************
*   LeptonNtuplizer  constructor and desctructor
*******************************************************************************/
LeptonNtuplizer::LeptonNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
  NtuplizerBase( iConfig, bpk ),
  _leptonname( iConfig.getParameter<string>( "leptonname" ) ),
  _rhotoken( GetToken<double>( "rhosrc" ) ),
  _muontoken( GetToken<std::vector<pat::Muon> >( "muonsrc"      ) ),
  _electrontoken( GetToken<std::vector<pat::Electron> >( "elecsrc"      ) ),
  _tautoken( GetToken<std::vector<pat::Tau> >( "tausrc"       ) ),
  _gentoken( GetToken<std::vector<reco::GenParticle> >( "gensrc" ) ),
  _packedcandtoken( GetToken<pat::PackedCandidateCollection>( "packedsrc" ) ),
  //_electronID_vetotoken( GetToken<edm::ValueMap<bool> >( "eleVetoIdMap"    ) ),
  //_electronID_loosetoken( GetToken<edm::ValueMap<bool> >( "eleLooseIdMap"   ) ),
  //_electronID_mediumtoken( GetToken<edm::ValueMap<bool> >( "eleMediumIdMap"  ) ),
  //_electronID_tighttoken( GetToken<edm::ValueMap<bool> >( "eleTightIdMap"   ) ),
  //_electronID_HEEPtoken( GetToken<edm::ValueMap<bool> >( "eleHEEPIdMap"    ) ),
  _electronID_vetomap( iConfig.getParameter<string>( "eleVetoIdMap"    ) ),
  _electronID_loosemap( iConfig.getParameter<string>( "eleLooseIdMap"   ) ),
  _electronID_mediummap( iConfig.getParameter<string>( "eleMediumIdMap"  ) ),
  _electronID_tightmap( iConfig.getParameter<string>( "eleTightIdMap"   ) ),
  _electronID_HEEPmap( iConfig.getParameter<string>( "eleHEEPIdMap"    ) ),
  _electronEffectiveAreaR03_NeuHadronAndPhoton( iConfig.getParameter<edm::FileInPath>( "eleffAreaNeuHadPhoFile" ).fullPath() ),
  _muonEffectiveAreaR03_NeuHadronAndPhoton( iConfig.getParameter<edm::FileInPath>( "mueffAreaNeuHadPhoFile" ).fullPath() ),
  _conversionstoken( GetToken<reco::ConversionCollection>( "conversionsrc" ) ),
  _vtxtoken( GetToken<std::vector<reco::Vertex> >( "vtxsrc" ) ),
  _beamspottoken( GetToken<reco::BeamSpot>( "beamspotsrc" ) )
{

}

/******************************************************************************/

LeptonNtuplizer::~LeptonNtuplizer()
{}

/*******************************************************************************
*   Inherited methods overloading
*******************************************************************************/
void
LeptonNtuplizer::RegisterTree( TTree* tree )
{
  LepInfo.RegisterTree( tree, _leptonname );
}

/******************************************************************************/

void
LeptonNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _rhotoken,               _rhohandle      );
  iEvent.getByToken( _muontoken,              _muonhandle     );
  iEvent.getByToken( _electrontoken,          _electronhandle );
  iEvent.getByToken( _tautoken,               _tauhandle      );
  iEvent.getByToken( _gentoken,               _genhandle      );
  iEvent.getByToken( _vtxtoken,               _vtxhandle      );
  iEvent.getByToken( _beamspottoken,          _beamspothandle );

  iEvent.getByToken( _packedcandtoken,        _packedhandle     );
  iEvent.getByToken( _conversionstoken,       _conversionhandle );
  //iEvent.getByToken( _electronID_vetotoken,   _electronIDVeto   );
  //iEvent.getByToken( _electronID_loosetoken,  _electronIDLoose  );
  //iEvent.getByToken( _electronID_mediumtoken, _electronIDMedium );
  //iEvent.getByToken( _electronID_tighttoken,  _electronIDTight  );
  //iEvent.getByToken( _electronID_HEEPtoken,   _electronIDHEEP   );

  memset( &LepInfo, 0x00, sizeof( LepInfo ) );

  FillMuon( iEvent, iSetup  );
  FillElectron( iEvent, iSetup  );
  FillTau( iEvent, iSetup  );

}

/*******************************************************************************
*   Common helper functions
*******************************************************************************/
int
LeptonNtuplizer::GetGenMCTag( double pt, double eta, double phi ) const
{
  for( auto gen = _genhandle->begin(); gen != _genhandle->end(); gen++ ){
    const double r = deltaR<double>( gen->eta(), gen->phi(), eta, phi );

    if( r > 0.5 ){ continue; }
    if( fabs( gen->pt() - pt ) / gen->pt() > 0.5 ){ continue; }

    if( ( abs( gen->pdgId() ) <= 5 || abs( gen->pdgId() ) == 21 ) ){
      if( gen->status() != 3 ){ continue; }
      return 5;// matched to a parton (q,g)
    }
    if( abs( gen->pdgId() ) == 22 ){
      if( gen->status() != 1 ){ continue; }
      return 6;// matched to a photon
    }
  }

  return 0;
}

/******************************************************************************/

int
LeptonNtuplizer::GetGenMCTag( const vector<pat::Electron>::const_iterator& el ) const
{
  return GetGenMCTag( el->pt(), el->eta(), el->phi() );
}

/******************************************************************************/

int
LeptonNtuplizer::GetGenMCTag( const vector<pat::Muon>::const_iterator& mu ) const
{
  return GetGenMCTag( mu->pt(), mu->eta(), mu->phi() );
}
