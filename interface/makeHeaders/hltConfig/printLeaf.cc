{
   TChain* a = new TChain("hltbitanalysis/HltTree");
   a->Add( "./hltOnly.root" );
   a->Print();
}
