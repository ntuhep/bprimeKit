#include "TTree.h"
#include "TChain.h"
#include "iostream"
#include "string.h"

// Files that must be manually added!
#include "format.h"
#include "checkEvtTool.h"

using namespace std;

void example()
{
   bool debug=true;
   checkEvtTool checkEvt; //or checkEvtTool checkEvt( debug );
   // Supports multiple File input
   checkEvt.addJson("Cert_254833_13TeV_PromptReco_Collisions15_JSON.txt");
   checkEvt.addJson("Cert_246908-254879_13TeV_PromptReco_Collisions15_JSON.txt");
   checkEvt.makeJsonMap();

   TChain *root = new TChain("bprimeKit/root");
   root->Add("bprimeKitTest.root");

   EvtInfoBranches EvtInfo ;  EvtInfo.Register(root);
   JetInfoBranches jetInfo ;  jetInfo.Register( root, "JetInfo" ); 
   JetInfoBranches wjetInfo;  wjetInfo.Register( root, "AK8BosonJetInfo" );

   for( int entry=0; entry < root->GetEntries() ; ++entry) { //Loop over all events
      root->GetEntry(entry); 

      // Golden JSON File check!
      if( !checkEvt.isGoodEvt(EvtInfo.RunNo,EvtInfo.LumiNo) ){ 
         continue; }
      for( int i = 0 ; i< jetInfo.Size ; i++ ){ // Loop over all jets in "JetInfo" collection
         cout << jetInfo.Pt[i] << endl;
      }
      for( int i = 0 ; i < wjetInfo.Size; ++i ) { // Loop over all jets in "AK8BosonJetInfo" collections
         for( int j = 0 ; j < wjetInfo.NSubjets[i] ; ++j ){ // Loop over all subjets for jet i 
            cout << wjetInfo.SubjetMass[ j + wjetInfo.SubjetsIdxStart[i] ] << endl ; 
         } 
      }
   }

   checkEvt.saveJson("Cert_246908-254833-254879_13TeV_PromptReco_Collisions15_JSON.txt");
}
