/*******************************************************************************
 *
 *  Filename    : bprimeKit_vertex.cc
 *  Description : vertex info Filling 
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"


//------------------------------------------------------------------------------ 
//   bprimeKit method implementation 
//------------------------------------------------------------------------------ 
bool bprimeKit::FillVertex( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   bool   gotfPrimaryVertex;
   double PVBS_Pt_Max;
      
   if( fDebug > 1 ) { cout << "\t[1]Fill Vertex Info.\n"; }
   memset( &fVertexInfo, 0x00, sizeof( fVertexInfo ) );
   
   //----- Vertices without beamspot constraints  -----------------------------------------------------
   gotfPrimaryVertex = false;
   if( fVertex_H.isValid() && !fVertex_H.failedToGet() && fVertex_H->size() > 0 ) {
      fPrimaryVertex = *( fVertex_H->begin() ); //Uly 2012-07-19: just get something valid to start with
      for( VertexIterator it_vtx = fVertex_H->begin(); it_vtx != fVertex_H->end(); ++it_vtx ) {
         if ( fVertexInfo.Size >= MAX_Vertices ) {
            cout << "PV " << fVertexInfo.Size << endl;
            cerr << "ERROR: number of Tracks exceeds the size of array." << endl;
            break;
         }
         fVertexInfo.Type           [fVertexInfo.Size] = 0                        ; //Vertices WITHOUT the Beam Spot constraint
         fVertexInfo.isValid        [fVertexInfo.Size] = it_vtx->isValid()        ;
         fVertexInfo.isFake         [fVertexInfo.Size] = it_vtx->isFake()         ; //Uly 2011-05-16
         fVertexInfo.Ndof           [fVertexInfo.Size] = it_vtx->ndof()           ;
         fVertexInfo.NormalizedChi2 [fVertexInfo.Size] = it_vtx->normalizedChi2() ;
         fVertexInfo.x              [fVertexInfo.Size] = it_vtx->x()              ;
         fVertexInfo.y              [fVertexInfo.Size] = it_vtx->y()              ;
         fVertexInfo.z              [fVertexInfo.Size] = it_vtx->z()              ;
         fVertexInfo.Rho            [fVertexInfo.Size] = it_vtx->position().Rho() ;
         fVertexInfo.Pt_Sum         [fVertexInfo.Size] = 0.                       ;
         fVertexInfo.Pt_Sum2        [fVertexInfo.Size] = 0.                       ;

         for ( auto it = it_vtx->tracks_begin(); it != it_vtx->tracks_end(); it++ ) {
            fVertexInfo.Pt_Sum  [ fVertexInfo.Size ] += ( *it )->pt();
            fVertexInfo.Pt_Sum2 [ fVertexInfo.Size ] += ( ( *it )->pt() * ( *it )->pt() );
         }
         if( !gotfPrimaryVertex && ( !it_vtx->isFake() && it_vtx->ndof() >= 4. && it_vtx->z() <= 24. && it_vtx->position().Rho() <= 2. ) ) {
            fPrimaryVertex = *it_vtx;
            gotfPrimaryVertex = true;
         }
         fVertexInfo.Size++;
      }
   }

   //----- Vertices with beamspot constraint  ---------------------------------------------------------
   PVBS_Pt_Max = -100. ; 
   if( fVertexWithBeamSpot_H.isValid() && !fVertexWithBeamSpot_H.failedToGet() && fVertexWithBeamSpot_H->size() > 0 ) {
      for( VertexIterator it_vtx = fVertexWithBeamSpot_H->begin(); it_vtx != fVertexWithBeamSpot_H->end(); ++it_vtx ) {
         if ( fVertexInfo.Size >= MAX_Vertices ) {
            cout << "PVBS " << fVertexInfo.Size << endl;
            cerr << "ERROR: number of  Vertices exceeds the size of array." << endl;
            break;//exit(0);
         }
         fVertexInfo.Type           [ fVertexInfo.Size ] = 1                        ; //Vertices WITH the Beam Spot constraint
         fVertexInfo.isValid        [ fVertexInfo.Size ] = it_vtx->isValid()        ;
         fVertexInfo.isFake         [ fVertexInfo.Size ] = it_vtx->isFake()         ; //Uly 2011-05-16
         fVertexInfo.Ndof           [ fVertexInfo.Size ] = it_vtx->ndof()           ;
         fVertexInfo.NormalizedChi2 [ fVertexInfo.Size ] = it_vtx->normalizedChi2() ;
         fVertexInfo.x              [ fVertexInfo.Size ] = it_vtx->x()              ;
         fVertexInfo.y              [ fVertexInfo.Size ] = it_vtx->y()              ;
         fVertexInfo.z              [ fVertexInfo.Size ] = it_vtx->z()              ;
         fVertexInfo.Rho            [ fVertexInfo.Size ] = it_vtx->position().Rho() ;
         fVertexInfo.Pt_Sum         [ fVertexInfo.Size ] = 0.                       ;
         fVertexInfo.Pt_Sum2        [ fVertexInfo.Size ] = 0.                       ;

         for ( auto it = it_vtx->tracks_begin(); it != it_vtx->tracks_end(); it++ ) {
            fVertexInfo.Pt_Sum  [ fVertexInfo.Size ] += ( *it )->pt();
            fVertexInfo.Pt_Sum2 [ fVertexInfo.Size ] += ( ( *it )->pt() * ( *it )->pt() );
         }
         if( fVertexInfo.Pt_Sum[fVertexInfo.Size] >= PVBS_Pt_Max ) {
            PVBS_Pt_Max = fVertexInfo.Pt_Sum[fVertexInfo.Size];
            fPrimaryVertex_BS = *it_vtx;
         }
         fVertexInfo.Size++;
      }
   }
   return true;
}
