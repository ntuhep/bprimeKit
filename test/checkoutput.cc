#include "TChain.h"
#include "TTree.h"
#include <iostream>
#include <string.h>

#include "TriggerBooking.h"
#include "format.h"

using namespace std;

void
checkoutput()
{
   bool debug   = true;
   TChain* root = new TChain( "bprimeKit/root" );
   root->Add( "bpk_ntuple.root" );

   root->GetEntry(0);

   EvtInfoBranches EvtInfo;
   EvtInfo.Register( root, "EvtInfo" );

   VertexInfoBranches VertexInfo;
   VertexInfo.Register( root, "VertexInfo" );

   TrgInfoBranches TrgInfo;
   TrgInfo.Register( root, "TrgInfo" );

   GenInfoBranches GenInfo;

   LepInfoBranches LepInfo;
   LepInfo.Register( root, "LepInfo" );

   JetInfoBranches JetInfo[3];
   JetInfo[0].Register( root, "JetInfo" );
   JetInfo[1].Register( root, "JetAK8Info" );
   JetInfo[2].Register( root, "JetCA8Info" );


   for( int entry = 0; entry < root->GetEntries() && entry < 5; ++entry ){
      root->GetEntry( entry );

      cout << "\nentry = "<< entry << endl;

      cout << ">>> Event Info test" << endl;
      int ntr = 0;

      for( int i = 0; i < N_TRIGGER_BOOKINGS; i++ ){
         if( EvtInfo.TrgBook[i] == 1 ){
            cout<<"Fired: TriggerBitNumber = "<< i<<",\t" << TriggerBooking[i] << endl;
            ntr++;
         }
      }

      cout << EvtInfo.L1[0] << " " << EvtInfo.TT[0] << endl;

      cout << " Fired HLT paths  = " << ntr << endl;

      cout << ">>> Trigger Info test " << endl;
      cout << " TrgInfo.Size  = " << TrgInfo.Size << endl;

      for( int j = 0; j < TrgInfo.Size; ++j ){
         printf( "%4d | %40s | %8d | %6lf %6lf %6lf\n",
            TrgInfo.TriggerBit[j],
            TriggerBooking[ TrgInfo.TriggerBit[j]].c_str(),
            TrgInfo.FilterLabel[j],
            TrgInfo.Pt[j], TrgInfo.Eta[j], TrgInfo.Phi[j]
            );
      }

      cout << ">>> JetInfo test" << JetInfo[0].Size << endl;

      for( int j = 0; j < JetInfo[0].Size; ++j ){
         cout << JetInfo[0].Pt[j] << endl;
         cout << JetInfo[0].NSubjets[j] << " "<< JetInfo[0].SubjetsIdxStart[j] <<  endl;

         for( int k = 0; k < JetInfo[0].NSubjets[j]; k++ ){
            cout << JetInfo[0].SubjetPt->at( JetInfo[0].SubjetsIdxStart[j] + k ) << " ";
         }

         cout << endl;
      }
   }

   cout << "Attempt new file creation" << endl;
   TFile* newfile = new TFile( "test_output.root", "recreate" );
   TTree* newtree = root->CloneTree( 0 );
   JetInfoBranches outjets;
   outjets.RegisterTree( newtree, "testjet" );

}
