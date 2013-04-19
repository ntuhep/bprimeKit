#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "format.h"

void proj()
{
	TH1F *h_pt  = new TH1F("h_pt","Jet p_{T} (GeV/c)",60.,0.,300);
        
        TFile *f1 = new TFile("bprimeKit_ttbar.root");
        TTree *root = (TTree*)f1->Get("bprimeKit/root");
	
        JetInfoBranches JetInfo;
        JetInfo.Register(root);
	
        for(int entry=0;entry<root->GetEntries();entry++) {
                root->GetEntry(entry);
		
                for(int i=0;i<JetInfo.Size;i++)  
			h_pt->Fill(JetInfo.Pt[i]); 
        }
	h_pt->Draw();
}
