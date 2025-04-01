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
  _Kshorttoken( GetToken<vector<reco::VertexCompositePtrCandidate> >( "Kshortsrc" ) ),
  _Lambdatoken( GetToken<vector<reco::VertexCompositePtrCandidate> >( "Lambdasrc" ) )
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
  V0Info.RegisterTree( tree, "V0Info" );
}

/******************************************************************************/


void
VertexNtuplizer::Analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  iEvent.getByToken( _vtxtoken,   _vtxhandle   );
  iEvent.getByToken( _Kshorttoken,   _Kshorthandle   );
  iEvent.getByToken( _Lambdatoken,   _Lambdahandle   );

  memset( &VertexInfo, 0x00, sizeof( VertexInfo ) );
  memset( &V0Info, 0x00, sizeof( V0Info ) );

  // ----- Vertices without beamspot constraints  -----------------------------------------------------
  for( auto it_vtx = _vtxhandle->begin(); it_vtx != _vtxhandle->end(); ++it_vtx ){
    if( VertexInfo.Size >= MAX_Vertices ){
      cerr << "ERROR: number of Tracks exceeds the size of array." << endl;
      break;
    }
    VertexInfo.isValid        [VertexInfo.Size] = it_vtx->isValid();
    VertexInfo.isFake         [VertexInfo.Size] = it_vtx->isFake();
    VertexInfo.Ndof           [VertexInfo.Size] = it_vtx->ndof();
    VertexInfo.NormalizedChi2 [VertexInfo.Size] = it_vtx->normalizedChi2();
    VertexInfo.x              [VertexInfo.Size] = it_vtx->x();
    VertexInfo.y              [VertexInfo.Size] = it_vtx->y();
    VertexInfo.z              [VertexInfo.Size] = it_vtx->z();
    VertexInfo.Rho            [VertexInfo.Size] = it_vtx->position().Rho();

    VertexInfo.Size++;
  }


  for( auto it_vtx = _Kshorthandle->begin(); it_vtx != _Kshorthandle->end(); ++it_vtx ){

    V0Info.Pt             [V0Info.Size] = it_vtx->pt();
    V0Info.Eta            [V0Info.Size] = it_vtx->eta();
    V0Info.Phi            [V0Info.Size] = it_vtx->phi();
    V0Info.Energy         [V0Info.Size] = it_vtx->energy();
    V0Info.x              [V0Info.Size] = it_vtx->position().x();
    V0Info.y              [V0Info.Size] = it_vtx->position().y();
    V0Info.z              [V0Info.Size] = it_vtx->position().z();
    V0Info.Ndof           [V0Info.Size] = it_vtx->vertexNdof();
    V0Info.Chi2           [V0Info.Size] = it_vtx->vertexChi2();
    V0Info.PdgID          [V0Info.Size] = 310;

    V0Info.Size++;
  }

  for( auto it_vtx = _Lambdahandle->begin(); it_vtx != _Lambdahandle->end(); ++it_vtx ){

    V0Info.Pt             [V0Info.Size] = it_vtx->pt();
    V0Info.Eta            [V0Info.Size] = it_vtx->eta();
    V0Info.Phi            [V0Info.Size] = it_vtx->phi();
    V0Info.Energy         [V0Info.Size] = it_vtx->energy();
    V0Info.x              [V0Info.Size] = it_vtx->position().x();
    V0Info.y              [V0Info.Size] = it_vtx->position().y();
    V0Info.z              [V0Info.Size] = it_vtx->position().z();
    V0Info.Ndof           [V0Info.Size] = it_vtx->vertexNdof();
    V0Info.Chi2           [V0Info.Size] = it_vtx->vertexChi2();
    V0Info.PdgID          [V0Info.Size] = 3122;

    V0Info.Size++;
  }



}
