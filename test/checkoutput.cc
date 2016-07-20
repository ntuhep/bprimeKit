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

   EvtInfoBranches EvtInfo ; EvtInfo.Register( root );

   for( int entry = 0; entry < root->GetEntries() ; ++entry ) { //Loop over all events
      root->GetEntry( entry );

      for( int i = 0 ; i < N_TRIGGER_BOOKINGS ; ++i ){
         cout << TriggerBooking[i] << " " << (int)EvtInfo.TrgBook[i] << endl;
      }
   }

}
