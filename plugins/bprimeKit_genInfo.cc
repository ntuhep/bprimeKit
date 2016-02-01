/*******************************************************************************
 *
 *  Filename    : bprimeKit_genInfo.cc
 *  Description : Filling in generation information as well as some event info
 *
 *******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 
bool bprimeKit::FillfGenInfo( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   //-------------------------------------------------------------------------------------------------- 
   //   Variable declaration
   //-------------------------------------------------------------------------------------------------- 
   const reco::Candidate* MCDaughters[14];
   const reco::Candidate* dau1;
   const reco::Candidate* dau2;
   CandidateList          cands;
   CandidateIterator      found;
   CandidateIterator      daughter1, daughter2;
   CandidateIterator      mother1  , mother2;
   CandidateIterator      gmother1 , gmother2;
   int NMo, NDa;
   int pdgId , dauId1 , dauId2 , monId ; 

   bool isTZTZ = false;
   bool isTZTH = false;
   bool isTHTH = false;
   bool isTHBW = false;
   bool isTZBW = false;
   bool isBWBW = false;
   vector<int> quarkID;
   vector<int> bosonID;

   //-------------------------------------------------------------------------------------------------- 
   //   Setting up common variables
   //--------------------------------------------------------------------------------------------------
   if( fDebug > 1 ) { cout << "\t[1] Entering fGenInfo subroutine"  << endl;}
   for( int i = 0; i < 14; i++ ) { MCDaughters[i] = NULL; }
   if( fIsData || fSkipfGenInfo  ) return false;
   

   for( GenIterator it_gen = fGenParticle_H->begin(); it_gen != fGenParticle_H->end(); it_gen++ ) {
      cands.push_back( &*it_gen ); }

   //------------------------------------------------------------------------------ 
   //   Inserting Event wide variables
   //------------------------------------------------------------------------------
   fGenInfo.Weight = 1.0 ;
   if( fGenEvent_H.isValid() ) {
      fGenInfo.Weight = fGenEvent_H->weight();
      fEvtInfo.ptHat  = fGenEvent_H->qScale();
   }
   if( fLHEInfo_H.isValid() ){
      if( fDebug > 1 ){ cout << "LHE Product is valid" << endl; }
      fGenInfo.LHENominalWeight = fLHEInfo_H->hepeup().XWGTUP;
      fGenInfo.LHEOriginalWeight = fLHEInfo_H->originalXWGTUP(); 
      fGenInfo.LHESize = fLHEInfo_H->weights().size();
      if( fDebug > 1 ){ cout << "LHE Product weight << " << fGenInfo.LHENominalWeight  << endl; }
      for( unsigned i = 0 ; i < fLHEInfo_H->weights().size(); ++i ){
         fGenInfo.LHESystematicWeights[i] = fLHEInfo_H->weights().at(i).wgt;
         fGenInfo.LHESystematicId[i]      = std::stoi(fLHEInfo_H->weights().at(i).id.data());
         if( fDebug > 1 ){ 
            cout << "\t[1] "<< i<<"th LHE Product s weight: [" << fGenInfo.LHESystematicId[i]
                 << "]: " << fGenInfo.LHESystematicWeights[i]  
                 << endl; 
         }
      }
   } else {
      cout << "LHE Prodcut invalid!" << endl;
      cout << fLHELabel << endl;
   }
   //-------------------------------------------------------------------------------------------------- 
   //   Begin main loop
   //-------------------------------------------------------------------------------------------------- 
   if( fDebug > 1 ) { cout << "\t[1] Entering fGenInfo main loop"  << endl;}
   for( GenIterator it_gen = fGenParticle_H->begin(); it_gen != fGenParticle_H->end(); ++it_gen  ) {

      //----- Setting up common variable  ----------------------------------------------------------------
      dauId1 = dauId2 = monId = 0;
      dau1   = dau2   = NULL;
      pdgId  = it_gen->pdgId();
      NMo    = it_gen->numberOfMothers();
      NDa    = it_gen->numberOfDaughters();

      //----- fGenInfo Branch insertion  ------------------------------------------------------------------
      if( fGenInfo.Size < 60 ){
         if( fDebug > 2 ) { cout << "\t\t[2]fGenInfo Particle" << endl; } 
         fGenInfo.Pt             [fGenInfo.Size] = it_gen->pt()                ;
         fGenInfo.Eta            [fGenInfo.Size] = it_gen->eta()               ;
         fGenInfo.Phi            [fGenInfo.Size] = it_gen->phi()               ;
         fGenInfo.Mass           [fGenInfo.Size] = it_gen->mass()              ;
         fGenInfo.PdgID          [fGenInfo.Size] = it_gen->pdgId()             ;
         fGenInfo.Status         [fGenInfo.Size] = it_gen->status()            ;
         fGenInfo.nMo            [fGenInfo.Size] = it_gen->numberOfMothers()   ;
         fGenInfo.nDa            [fGenInfo.Size] = it_gen->numberOfDaughters() ;
         fGenInfo.Mo1            [fGenInfo.Size] = -1                          ;
         fGenInfo.Mo2            [fGenInfo.Size] = -1                          ;
         fGenInfo.Da1            [fGenInfo.Size] = -1                          ;
         fGenInfo.Da2            [fGenInfo.Size] = -1                          ;
         fGenInfo.Mo1PdgID       [fGenInfo.Size] =  0                          ;
         fGenInfo.Mo2PdgID       [fGenInfo.Size] =  0                          ;
         fGenInfo.Da1PdgID       [fGenInfo.Size] =  0                          ;
         fGenInfo.Da2PdgID       [fGenInfo.Size] =  0                          ;
         fGenInfo.GrandMo1PdgID  [fGenInfo.Size] =  0                          ;
         fGenInfo.GrandMo2PdgID  [fGenInfo.Size] =  0                          ;
         fGenInfo.Mo1Status      [fGenInfo.Size] = -1                          ;
         fGenInfo.Mo2Status      [fGenInfo.Size] = -1                          ;
         fGenInfo.GrandMo1Status [fGenInfo.Size] = -1                          ;
         fGenInfo.GrandMo2Status [fGenInfo.Size] = -1                          ;

         //----- Parent/Daughter information insertion  -----------------------------------------------------
         if( fDebug > 2 ) { cout << "\t\t[2] Getting parent candidates" << endl;}

         mother1 = find( cands.begin(), cands.end(), it_gen->mother(0) );
         if( mother1 != cands.end() ) { 
            fGenInfo.Mo1       [fGenInfo.Size] = mother1 - cands.begin() ;
            fGenInfo.Mo1PdgID  [fGenInfo.Size] = (*mother1)->pdgId() ;  
            fGenInfo.Mo1Status [fGenInfo.Size] = (*mother1)->status();
            gmother1 = find( cands.begin(), cands.end(), (*mother1)->mother(0) );
            if( gmother1 != cands.end() ) { 
               fGenInfo.GrandMo1PdgID [fGenInfo.Size] = (*gmother1)->pdgId();
               fGenInfo.GrandMo1Status[fGenInfo.Size] = (*gmother1)->status();
            }
         } 
         mother2 = find( cands.begin(), cands.end(), it_gen->mother(NMo-1) );
         if( mother2 != cands.end() ) { 
            fGenInfo.Mo2       [fGenInfo.Size] = mother2 - cands.begin() ;
            fGenInfo.Mo2PdgID  [fGenInfo.Size] = (*mother2)->pdgId() ;  
            fGenInfo.Mo2Status [fGenInfo.Size] = (*mother2)->status();
            gmother2 = find( cands.begin(), cands.end(), (*mother2)->mother(0) );
            if( gmother2 != cands.end() ) { 
               fGenInfo.GrandMo2PdgID [fGenInfo.Size] = (*gmother2)->pdgId();
               fGenInfo.GrandMo2Status[fGenInfo.Size] = (*gmother2)->status();
            }
         }
         daughter1 = find( cands.begin(), cands.end(), it_gen->daughter(0) );
         if( daughter1 != cands.end() ) { 
            fGenInfo.Da1 [fGenInfo.Size] = daughter1 - cands.begin() ; 
            fGenInfo.Da1PdgID [fGenInfo.Size]  = it_gen->daughter(0)->pdgId();
         }
         daughter2 = find( cands.begin(), cands.end(), it_gen->daughter(NDa-1) );
         if( daughter2 != cands.end() ) { 
            fGenInfo.Da2 [fGenInfo.Size]= daughter2 - cands.begin() ;
            fGenInfo.Da2PdgID [fGenInfo.Size]  = it_gen->daughter(NDa-1)->pdgId();
         }

         //----- Photon Flag, see definition below  ---------------------------------------------------------
         fGenInfo.PhotonFlag[fGenInfo.Size] = PhotonFlag( it_gen ) ;
         ++fGenInfo.Size;

         //----- Getting information for ljmet algorithm  ---------------------------------------------------
         if( !IsTprime( it_gen->pdgId() ) || HasTprimeDaughter( it_gen ) ) { continue ; }
         for( size_t i = 0 ; i< it_gen->numberOfDaughters() ; ++i ){
            int daughterId = it_gen->daughter(i)->pdgId();
            if( abs(daughterId) == 5 || abs(daughterId) == 6 ){ 
               quarkID.push_back( daughterId );
            } else if( abs(daughterId) > 22 && abs(daughterId)< 26 ){
               bosonID.push_back( daughterId );
            }
         }
      }

      //-------------------------------------------------------------------------------------------------- 
      //   Inserting Event Level information
      //-------------------------------------------------------------------------------------------------- 
      if ( NDa >= 2 ) {
         dau1    = it_gen->daughter( 0 );
         dau2    = it_gen->daughter( 1 );
         dauId1 = abs( dau1->pdgId() );
         dauId2 = abs( dau2->pdgId() );
         if ( dauId1 > dauId2 ) { // always keep dauId2 > dauId1
            const reco::Candidate* tmp = dau1;
            dau1    = dau2;
            dau2    = tmp;
            dauId1 = abs( dau1->pdgId() );
            dauId2 = abs( dau2->pdgId() );
         }
      }
      if ( NMo >= 1 )
      { monId   = it_gen->mother( 0 )->pdgId(); }

      if( fDebug > 2 ) { cout << "\t\t[2]Getting decay mode" << endl; }
      //----- b' decay mode  -----------------------------------------------------------------------------
      // b' decay - 0:other, 1:tW, 2:cW, 3:bZ 4:bH
      if ( pdgId == +7 ) { 
         fEvtInfo.McbprimeMass[0] = it_gen->p4().mag();
         // b' decay mode      - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
         if ( dauId1 == 6 && dauId2 == 24 ) { fEvtInfo.McbprimeMode[0] = 1; }
         if ( dauId1 == 4 && dauId2 == 24 ) { fEvtInfo.McbprimeMode[0] = 2; }
         if ( dauId1 == 5 && dauId2 == 23 ) { fEvtInfo.McbprimeMode[0] = 3; }
         if ( dauId1 == 5 && dauId2 == 25 ) { fEvtInfo.McbprimeMode[0] = 4; }
         if ( fEvtInfo.McbprimeMode[0] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 1 ); }
         } else { MCDaughters[0] = dau1; }
      } 
      else if ( pdgId == -7 ) { 
         fEvtInfo.McbprimeMass[1] = it_gen->p4().mag();
         if ( dauId1 == 6 && dauId2 == 24 ) { fEvtInfo.McbprimeMode[1] = 1; }
         if ( dauId1 == 4 && dauId2 == 24 ) { fEvtInfo.McbprimeMode[1] = 2; }
         if ( dauId1 == 5 && dauId2 == 23 ) { fEvtInfo.McbprimeMode[1] = 3; }
         if ( dauId1 == 5 && dauId2 == 25 ) { fEvtInfo.McbprimeMode[1] = 4; }
         if ( fEvtInfo.McbprimeMode[1] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 1 ); }
         } else { MCDaughters[1] = dau1; }
      }

      //----- t' decay mode  -----------------------------------------------------------------------------
      // 0:other, 1bW, 2:tZ, 3:tH , 4:tGamma
      else if ( pdgId == +8 ) {
         fEvtInfo.MctprimeMass[0] = it_gen->p4().mag();
         if ( dauId1 == 5 && dauId2 == 24 ) { fEvtInfo.MctprimeMode[0] = 1; }
         if ( dauId1 == 6 && dauId2 == 23 ) { fEvtInfo.MctprimeMode[0] = 2; }
         if ( dauId1 == 6 && dauId2 == 25 ) { fEvtInfo.MctprimeMode[0] = 3; }
         if ( dauId1 == 6 && dauId2 == 22 ) { fEvtInfo.MctprimeMode[0] = 4; }
         if ( fEvtInfo.MctprimeMode[0] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 1 ); }
         } else { MCDaughters[0] = dau1; }
      } else if ( pdgId == -8 ) {
         fEvtInfo.MctprimeMass[1] = it_gen->p4().mag();
         if ( dauId1 == 5 && dauId2 == 24 ) { fEvtInfo.MctprimeMode[1] = 1; }
         if ( dauId1 == 6 && dauId2 == 23 ) { fEvtInfo.MctprimeMode[1] = 2; }
         if ( dauId1 == 6 && dauId2 == 25 ) { fEvtInfo.MctprimeMode[1] = 3; }
         if ( dauId1 == 6 && dauId2 == 22 ) { fEvtInfo.MctprimeMode[1] = 4; }
         if ( fEvtInfo.MctprimeMode[1] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 1 ); }
         } else { MCDaughters[1] = dau1; }
      } else if ( pdgId == +6 ) { // find top
         fEvtInfo.MctopMass[0] = it_gen->p4().mag();
      } else if ( pdgId == -6 ) { // find tbar
         fEvtInfo.MctopMass[1] = it_gen->p4().mag();
      }

      //----- W decay mode  ------------------------------------------------------------------------------
      // 0:others, 1:enu, 2:munu, 3:taunu, 4:jj
      else if ( pdgId == -24 && ( monId == +7 || monId == -8 ) ) { // W- from b' or t' bar
         fEvtInfo.McWMass[0] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { fEvtInfo.McWMode[0] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { fEvtInfo.McWMode[0] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { fEvtInfo.McWMode[0] = 3; }
         if ( dauId1 >= 1  && dauId2 <= 5  ) { fEvtInfo.McWMode[0] = 4; }
         if ( fEvtInfo.McWMode[0] == 0 ) { continue; }
         MCDaughters[2] = dau1;
         MCDaughters[3] = dau2;
      } else if ( pdgId == +24 && monId == +6 ) { // W+ from t
         fEvtInfo.McWMass[1] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { fEvtInfo.McWMode[1] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { fEvtInfo.McWMode[1] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { fEvtInfo.McWMode[1] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { fEvtInfo.McWMode[1] = 4; }
         if ( fEvtInfo.McWMode[1] == 0 ) { continue; }
         MCDaughters[4] = dau1;
         MCDaughters[5] = dau2;
      } else if ( pdgId == +24 && ( monId == -7 || monId == +8 ) ) { // W+ from b'bar or t'
         fEvtInfo.McWMass[2] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { fEvtInfo.McWMode[2] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { fEvtInfo.McWMode[2] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { fEvtInfo.McWMode[2] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { fEvtInfo.McWMode[2] = 4; }
         if ( fEvtInfo.McWMode[2] == 0 ) { continue; }
         MCDaughters[6] = dau1;
         MCDaughters[7] = dau2;
      } else if ( pdgId == -24 && monId == -6 ) { // W- from tbar
         fEvtInfo.McWMass[3] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { fEvtInfo.McWMode[3] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { fEvtInfo.McWMode[3] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { fEvtInfo.McWMode[3] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { fEvtInfo.McWMode[3] = 4; }
         if ( fEvtInfo.McWMode[3] == 0 ) { continue; }
         MCDaughters[8] = dau1;
         MCDaughters[9] = dau2;
      } 
      //----- Z decay mode  ------------------------------------------------------------------------------
      // 0:others, 1:ee, 2:mumu, 3:tautau, 4:nunu, 5:bb, b:jj
      else if ( pdgId == 23 && ( monId == +7 || monId == +8 ) ) { // Z from b' or t'
         fEvtInfo.McZMass[0] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 11 )   { fEvtInfo.McZMode[0] = 1; }
         if ( dauId1 == 13 && dauId2 == 13 )   { fEvtInfo.McZMode[0] = 2; }
         if ( dauId1 == 15 && dauId2 == 15 )   { fEvtInfo.McZMode[0] = 3; }
         if ( ( dauId1 == 12 && dauId2 == 12 ) ||
               ( dauId1 == 14 && dauId2 == 14 ) ||
               ( dauId1 == 16 && dauId2 == 16 ) ) { fEvtInfo.McZMode[0] = 4; }
         if ( dauId1 == 5 && dauId2 == 5 )   { fEvtInfo.McZMode[0] = 5; }
         if ( dauId1 >= 1 && dauId2 <= 4 )   { fEvtInfo.McZMode[0] = 6; }
         if ( fEvtInfo.McZMode[0] == 0 ) { continue; }
         MCDaughters[10] = dau1;
         MCDaughters[11] = dau2;
      } else if ( pdgId == 23 && ( monId == -7 || monId == -8 ) ) { // Z from b'bar or t'bar
         fEvtInfo.McZMass[1] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 11 )  { fEvtInfo.McZMode[1] = 1; }
         if ( dauId1 == 13 && dauId2 == 13 )  { fEvtInfo.McZMode[1] = 2; }
         if ( dauId1 == 15 && dauId2 == 15 )  { fEvtInfo.McZMode[1] = 3; }
         if ( ( dauId1 == 12 && dauId2 == 12 ) ||
               ( dauId1 == 14 && dauId2 == 14 ) ||
               ( dauId1 == 16 && dauId2 == 16 ) ) { fEvtInfo.McZMode[1] = 4; }
         if ( dauId1 == 5 && dauId2 == 5 )   { fEvtInfo.McZMode[1] = 5; }
         if ( dauId1 >= 1 && dauId2 <= 4 )   { fEvtInfo.McZMode[1] = 6; }
         if ( fEvtInfo.McZMode[0] == 0 ) { continue; }

         MCDaughters[12] = dau1;
         MCDaughters[13] = dau2;
      }
   }

   //----- Fill generation daughters  -----------------------------------------------------------------
   // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters
   for( int i = 0; i < 14; i++ ) {
      if ( MCDaughters[i] == NULL ) { continue; }
      fEvtInfo.McDauPt[i]    = MCDaughters[i]->pt();
      fEvtInfo.McDauEta[i]   = MCDaughters[i]->eta();
      fEvtInfo.McDauPhi[i]   = MCDaughters[i]->phi();
      fEvtInfo.McDauPdgID[i] = MCDaughters[i]->pdgId();
   }

   //-------------------------------------------------------------------------------------------------- 
   //   Begin main part of ljmet algorithm
   //   Main reference: https://github.com/cms-ljmet/Ljmet-Com/blob/master/src/TpTpCalc.cc
   //-------------------------------------------------------------------------------------------------- 
   if(quarkID.size() == 0){ return 0;}
   else if(quarkID.size() != 2){
      double test = quarkID[0]*quarkID[1];
      int sign = -1; 
      if(test > 0) { sign = 1; }
      if(sign > 0){
         if(quarkID.size() == 4){
            swap(quarkID[2],quarkID[3]); }
         swap(quarkID[1],quarkID[2]);
         test = quarkID[0]*quarkID[1];
         sign = -1; if(test > 0) sign = 1;
         if(sign < 0) cout << "Signs are fixed!" << endl;
      }
      if(quarkID.size() > 3 && abs(quarkID[3]) == 6){
         swap(quarkID[2],quarkID[3]); }
      if(quarkID.size() > 2 && abs(quarkID[2]) == 6){
         swap(quarkID[1],quarkID[2]); }
   }

   // tag the decay chains according to ID'd quarks and bosons.
   // After the fixes above there should not be errors (if we've done it right!)

   // 2 b quarks, check for matching W's
   if(abs(quarkID[0]) == 5 && abs(quarkID[1]) == 5){
      if(abs(bosonID[0]) == 24 && abs(bosonID[1]) == 24) { isBWBW = true; }
   }
   // 2 t quarks, check for Z's and H's
   else if(abs(quarkID[0]) == 6 && abs(quarkID[1]) == 6){
      if     (bosonID[0] == 23 && bosonID[1] == 23) isTZTZ = true;
      else if(bosonID[0] == 25 && bosonID[1] == 25) isTHTH = true;
      else if(bosonID[0] == 25 && bosonID[1] == 23) isTZTH = true;
      else if(bosonID[0] == 23 && bosonID[1] == 25) isTZTH = true;
      else { cout << "2 t daughters didn't match tZtZ, tHtH, or tZtH" << bosonID[0] << ", " << bosonID[1] << endl; }
   }
   // t-b pairs, check for correlating bosons in the right spots
   else if(abs(quarkID[0]) == 6 && abs(quarkID[1]) == 5){
      if     (bosonID[0] == 23 && abs(bosonID[1]) == 24) isTZBW = true;
      else if(bosonID[0] == 25 && abs(bosonID[1]) == 24) isTHBW = true;
      else cout << "t - b pair didn't match Z/H - W pair" << bosonID[0] << ", " << bosonID[1] << endl; 
   }
   // b-t pairs, check for correlating bosons in the right spots
   else if(abs(quarkID[1]) == 6 && abs(quarkID[0]) == 5){
      if     (bosonID[1] == 23 && abs(bosonID[0]) == 24) isTZBW = true;
      else if(bosonID[1] == 25 && abs(bosonID[0]) == 24) isTHBW = true;
      else cout << "b - t pair didn't match W - Z/H pair" << bosonID[0] << ", " << bosonID[1] << endl;
   }
   // error messages if we found something else entirely
   else{
      cout << "daughters didn't match a recognized pattern" << endl;
      for(size_t i = 0; i < quarkID.size(); i++){ cout << "quark " << i << " = " << quarkID[i] << endl; }
      for(size_t i = 0; i < bosonID.size(); i++){ cout << "boson " << i << " = " << bosonID[i] << endl; }
   }

   fEvtInfo.McIsTZTZ = isTZTZ;
   fEvtInfo.McIsTHTH = isTHTH;
   fEvtInfo.McIsTZTH = isTZTH;
   fEvtInfo.McIsTZBW = isTZBW;
   fEvtInfo.McIsTHBW = isTHBW;
   fEvtInfo.McIsBWBW = isBWBW;

   return true;
}

/*******************************************************************************
 *
 *  Note : Photons Flag rules - 
 *    -1 : unknown or non photon
 *     0 : prompt photon 
 *         ( pid=22 && status(3) ) || 
 *         ( pid(22) && status(3) && M_pid(22) && status M_pid(3) )
 *     1 : Decay in flight
 *         ( pid(22) && status(1) && M_status(2) ) 
 *     2 : ISR photon
 *          pid(22) && status(1) && M_status(3) && M_pid(<6 || =21) && GM_status(3) && GM_pid(2212)
 *     3 : FSR photon
 *          pid(22) && status(1) && M_status(3) && GM_status(3) && GM_pid(!2212)
 *
 *******************************************************************************/
#define UNKNOWN_FLAG   -1
#define PROMPT_PHOTON   0
#define DECAY_IN_FLIGHT 1
#define ISR_PHOTON      2
#define FSR_PHOTON      3

int bprimeKit::PhotonFlag( const GenIterator& particle ) const
{
   int numMo = particle->numberOfMothers();
   if( particle->status() == 3 ) {
      if( particle->pdgId() == 22 ) { return PROMPT_PHOTON; } 
      else { return UNKNOWN_FLAG; }
   } 
   else if( particle->status() == 1 && particle->pdgId() == 22 && numMo > 1 ) {
      auto mother1 = particle->mother(0);
      auto mother2 = particle->mother( numMo - 1 ) ;
      if( mother1->pdgId()  == 22 && mother2->status() == 22 && 
            mother1->status() == 3  && mother2->status() == 3 ){ 
         return PROMPT_PHOTON ;
      } else if( mother1->status() == 2 && mother2->status() == 2 ) {
         return DECAY_IN_FLIGHT ;
      } else if( mother1->status()  == 3 && mother2->status()  == 3 &&
            mother1->mother(0) != 0 && mother2->mother(0) != 0 &&
            mother1->mother(0)->pdgId() == 2212 && 
            mother2->mother(0)->pdgId() == 2212 ){
         if( (  abs( mother1->pdgId() ) < 6 || mother1->pdgId()       == 21 ) &&
               (  abs( mother2->pdgId() ) < 6 || mother2->pdgId() == 21 ) ) {
            return ISR_PHOTON; 
         } else return FSR_PHOTON;
      } else return UNKNOWN_FLAG ; 
   } 
   return UNKNOWN_FLAG;
}

bool bprimeKit::HasTprimeDaughter( const GenIterator& particle ) const
{
   for( size_t i = 0 ; i < particle->numberOfDaughters() ; ++i ){
      if( IsTprime( particle->daughter(i)->pdgId() ) ){
         return true; }
   }
   return false;
}
