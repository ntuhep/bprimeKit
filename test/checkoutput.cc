#include <iostream>
#include <string.h>
#include "TTree.h"
#include "TChain.h"

#include "format.h"
#include "TriggerBooking.h"

using namespace std;

void checkoutput()
{
   bool debug = true;
   TChain* root = new TChain( "bprimeKit/root" );
   root->Add( "bpk_ntuple.root" );

   TrgInfoBranches TrgInfo ; TrgInfo.Register( root );
   EvtInfoBranches EvtInfo ; EvtInfo.Register( root );

   for( int entry = 0; entry < root->GetEntries() ; ++entry ) { //Loop over all events
      root->GetEntry( entry );

      cout << "\nentry = "<< entry << endl;

      int ntr = 0;
      for( int i = 0; i < N_TRIGGER_BOOKINGS; i++){
         if( EvtInfo.TrgBook[i] == 1 && TriggerBooking[i].find("HLT_IsoMu27") != std::string::npos ) {
            cout<<"Fired: TriggerBitNumber = "<< i<<",\t" << TriggerBooking[i] << endl;
            ntr++;
         }
      }

      cout << " Fired HLT paths  = " << ntr << endl;
      cout << " TrgInfo.Size  = " << TrgInfo.Size << endl;

      for( int j = 0 ; j < TrgInfo.Size ; ++j ){
         printf( "%4d | %40s | %8d | %6lf %6lf %6lf\n",
            TrgInfo.TriggerBit[j],
            TriggerBooking[ TrgInfo.TriggerBit[j]].c_str(),
            TrgInfo.FilterLabel[j],
            TrgInfo.Pt[j], TrgInfo.Eta[j] , TrgInfo.Phi[j]
       );
      }
   }

}
