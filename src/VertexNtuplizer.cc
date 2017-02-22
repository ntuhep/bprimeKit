/*******************************************************************************
*
*  Filename    : bprimeKit_vertex.cc
*  Description : vertex info Filling
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/VertexNtuplizer.hpp"

using namespace std;

/*******************************************************************************
*   VertexNtuplizer constructor and desctructor
*******************************************************************************/
VertexNtuplizer::VertexNtuplizer( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
  NtuplizerBase( iConfig, bpk ),
  _vtxtoken( GetToken<vector<reco::Vertex> >( "vtxsrc" ) ),
  _vtxBStoken( GetToken<vector<reco::Vertex> >( "vtxBSsrc" ) )
{

}

/******************************************************************************/

VertexNtuplizer::~VertexNtuplizer()
{}


/*******************************************************************************
*   Main loop
*******************************************************************************/
void
VertexNtuplizer::RegisterTree( TTree* tree )
{
  VertexInfo.RegisterTree( tree );
}

/******************************************************************************/


void
VertexNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _vtxtoken,   _vtxhandle   );

  memset( &VertexInfo, 0x00, sizeof( VertexInfo ) );

  // ----- Vertices without beamspot constraints  -----------------------------------------------------
  for( auto it_vtx = _vtxhandle->begin(); it_vtx != _vtxhandle->end(); ++it_vtx ){
    if( VertexInfo.Size >= MAX_Vertices ){
      cerr << "ERROR: number of Tracks exceeds the size of array." << endl;
      break;
    }
    VertexInfo.Type           [VertexInfo.Size] = 0;// Vertices WITHOUT the Beam Spot constraint
    VertexInfo.isValid        [VertexInfo.Size] = it_vtx->isValid();
    VertexInfo.isFake         [VertexInfo.Size] = it_vtx->isFake();// Uly 2011-05-16
    VertexInfo.Ndof           [VertexInfo.Size] = it_vtx->ndof();
    VertexInfo.NormalizedChi2 [VertexInfo.Size] = it_vtx->normalizedChi2();
    VertexInfo.x              [VertexInfo.Size] = it_vtx->x();
    VertexInfo.y              [VertexInfo.Size] = it_vtx->y();
    VertexInfo.z              [VertexInfo.Size] = it_vtx->z();
    VertexInfo.Rho            [VertexInfo.Size] = it_vtx->position().Rho();
    VertexInfo.Pt_Sum         [VertexInfo.Size] = 0.;
    VertexInfo.Pt_Sum2        [VertexInfo.Size] = 0.;

    for( auto it = it_vtx->tracks_begin(); it != it_vtx->tracks_end(); it++ ){
      VertexInfo.Pt_Sum  [ VertexInfo.Size ] += ( *it )->pt();
      VertexInfo.Pt_Sum2 [ VertexInfo.Size ] += ( ( *it )->pt() * ( *it )->pt() );
    }

    VertexInfo.Size++;
  }
}
