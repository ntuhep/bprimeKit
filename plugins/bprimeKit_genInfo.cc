/*******************************************************************************
 *
 *  Filename    : bprimeKit_genInfo.cc
 *  Description : filling in generation information as well as some event info
 *
*******************************************************************************/


#include "MyAna/bprimeKit/interface/bprimeKit.h"

//-----------------------  GenInfo specific CMSSW libraries  ------------------------
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

//------------------------------------------------------------------------------ 
//   Typedefs and enums
//------------------------------------------------------------------------------ 
typedef edm::Handle<std::vector<PileupSummaryInfo>>      PileupHandle;
typedef std::vector<PileupSummaryInfo>::const_iterator   PileupIterator;
typedef std::vector<reco::GenParticle>::const_iterator   GenPIterator;
typedef std::vector<const reco::Candidate*>              CandidateList;
typedef CandidateList::const_iterator                    CandidateIterator;
typedef edm::Handle<GenEventInfoProduct>                 GenInfoHandle;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 

bool bprimeKit::fillGenInfo( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   static const reco::Candidate* MCDaughters[14];
   static const reco::Candidate* dau1;
   static const reco::Candidate* dau2;
   static PileupHandle           PUInfo;
   static PileupIterator         PVI;
   static GenInfoHandle          genEventInfo;
   static CandidateList          cands;
   static CandidateIterator      found;
   static CandidateIterator      mother1 , mother2;
   static int iMo1, iMo2, iDa1, iDa2;
   static int iGrandMo1, iGrandMo2, NMo, NDa;
   static int pdgId , dauId1 , dauId2 , monId ; 
   static int numberOfDaughters , numberOfMothers;
   static double evWeight;

   //-------------------------------------  Setup  ------------------------------------- 
   for( int i = 0; i < 14; i++ ) { MCDaughters[i] = NULL; }
   if( isData || skipGenInfo_  ) return false;
   if( !isData && genlabel_.size() > 0 ) { iEvent.getByLabel( genlabel_[0], GenHandle ); }
   
   for( GenIterator it_gen = GenHandle->begin(); it_gen != GenHandle->end(); it_gen++ ) {
      cands.push_back( &*it_gen ); }

   evWeight = 1.0 ;
   iEvent.getByLabel( "generator", genEventInfo );
   if ( genEventInfo.isValid() ) {
      evWeight      = genEventInfo->weight();
      EvtInfo.ptHat = genEventInfo->qScale();
   }
   GenInfo.Weight = evWeight;
   
   
   if( puInfoLabel_.size() > 0 ) { iEvent.getByLabel( puInfoLabel_[0], PUInfo ); }
   for( PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI ) {
      EvtInfo.nPU[EvtInfo.nBX] = PVI->getPU_NumInteractions();
      EvtInfo.BXPU[EvtInfo.nBX] = PVI->getBunchCrossing();
      EvtInfo.TrueIT[EvtInfo.nBX] = PVI->getTrueNumInteractions();
      EvtInfo.nBX += 1;
   }
   
   if( debug_ > 15 ) { cout << "Getting MC info" << endl ; }

   for( GenIterator it_gen = GenHandle->begin(); it_gen != GenHandle->end(); it_gen++ ) {
      pdgId             = it_gen->pdgId();
      numberOfDaughters = it_gen->numberOfDaughters();
      numberOfMothers   = it_gen->numberOfMothers();
      dauId1 = dauId2 = monId = 0;
      dau1   = dau2   = NULL;

      //--------------------------  Photon decay type flagging  ---------------------------
      if( it_gen->status() == 3 || (it_gen->status() == 1 && it_gen->pt() > 20 )){
         cout << ">>> Geninfo >>> Photon type" << endl;
         iMo1 = iMo2 = iDa1 = iDa2 = -1;
         iGrandMo1 = iGrandMo2 = -1;
         NMo = it_gen->numberOfMothers();
         NDa = it_gen->numberOfDaughters();

         cout << ">>> GenInfo >>> Finding ancestors " << endl;
         mother1 = find( cands.begin(), cands.end(), it_gen->mother( 0 ) );
         if( mother1 != cands.end() ) { 
            iMo1 = mother1 - cands.begin() ; 
            found = find( cands.begin(), cands.end(), (*mother1)->mother( 0 ) );
            if( found != cands.end() ) { iGrandMo1 = found - cands.begin() ; }
         }
         mother2 = find( cands.begin(), cands.end(), it_gen->mother( NMo - 1 ) );
         if( mother2 != cands.end() ) { 
            iMo2 = found - cands.begin() ; 
            found = find( cands.begin(), cands.end(), (*mother2)->mother( 0 ) );
            if( found != cands.end() ) { iGrandMo2 = found - cands.begin() ; }
         }
         found = find( cands.begin(), cands.end(), it_gen->daughter( 0 ) );
         if( found != cands.end() ) { iDa1 = found - cands.begin() ; }
         found = find( cands.begin(), cands.end(), it_gen->daughter( NDa - 1 ) );
         if( found != cands.end() ) { iDa2 = found - cands.begin() ; }

         cout << ">>> GenInfo >>> Inserting information " << endl;
         GenInfo.Pt             [GenInfo.Size] = it_gen->pt()     ;
         GenInfo.Eta            [GenInfo.Size] = it_gen->eta()    ;
         GenInfo.Phi            [GenInfo.Size] = it_gen->phi()    ;
         GenInfo.Mass           [GenInfo.Size] = it_gen->mass()   ;
         GenInfo.PdgID          [GenInfo.Size] = it_gen->pdgId()  ;
         GenInfo.Status         [GenInfo.Size] = it_gen->status() ;
         GenInfo.nMo            [GenInfo.Size] = NMo              ;
         GenInfo.nDa            [GenInfo.Size] = NDa              ;
         GenInfo.Mo1            [GenInfo.Size] = iMo1             ;
         GenInfo.Mo2            [GenInfo.Size] = iMo2             ;
         GenInfo.Da1            [GenInfo.Size] = iDa1             ;
         GenInfo.Da2            [GenInfo.Size] = iDa2             ;
         GenInfo.Mo1PdgID       [GenInfo.Size] =  0               ;
         GenInfo.Mo2PdgID       [GenInfo.Size] =  0               ;
         GenInfo.Mo1Status      [GenInfo.Size] = -1               ;
         GenInfo.Mo2Status      [GenInfo.Size] = -1               ;
         GenInfo.Da1PdgID       [GenInfo.Size] =  0               ;
         GenInfo.Da2PdgID       [GenInfo.Size] =  0               ;
         GenInfo.GrandMo1PdgID  [GenInfo.Size] =  0               ;
         GenInfo.GrandMo2PdgID  [GenInfo.Size] =  0               ;
         GenInfo.GrandMo1Status [GenInfo.Size] = -1               ;
         GenInfo.GrandMo2Status [GenInfo.Size] = -1               ;
         GenInfo.Size++ ;
         if( iDa1 != -1 ) { 
            GenInfo.Da1PdgID[GenInfo.Size]    = ( GenHandle->begin() + iDa1 )->pdgId(); }
         if( iDa2 != -1 ) { 
            GenInfo.Da2PdgID[GenInfo.Size]    = ( GenHandle->begin() + iDa2 )->pdgId(); }
         if( iMo1 != -1 ) {
            GenInfo.Mo1PdgID[GenInfo.Size]      = ( GenHandle->begin() + iMo1 )->pdgId();
            GenInfo.Mo1Status[GenInfo.Size]  = ( GenHandle->begin() + iMo1 )->status(); }
         if( iMo2 != -1 ) {
            GenInfo.Mo2PdgID[GenInfo.Size]      = ( GenHandle->begin() + iMo2 )->pdgId();
            GenInfo.Mo2Status[GenInfo.Size]  = ( GenHandle->begin() + iMo2 )->status(); }
         if( iGrandMo1 != -1 ) {
            GenInfo.GrandMo1PdgID[GenInfo.Size]    = ( GenHandle->begin() + iGrandMo1 )->pdgId();
            GenInfo.GrandMo1Status[GenInfo.Size]      = ( GenHandle->begin() + iGrandMo1 )->status(); }
         if( iGrandMo2 != -1 ) {
            GenInfo.GrandMo2PdgID[GenInfo.Size]    = ( GenHandle->begin() + iGrandMo2 )->pdgId();
            GenInfo.GrandMo2Status[GenInfo.Size]      = ( GenHandle->begin() + iGrandMo2 )->status(); }

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
          *
         *******************************************************************************/ 
         if( it_gen->status() == 3 ) {
            cout << ">>> GenInfo >>> photon status 3 " << endl;
            if( it_gen->pdgId() == 22 ) {
               // -1 : unknown or not photon, 0 : prompt photon, 1 : decay in flight, 2 : ISR, 3 : FSR
               GenInfo.PhotonFlag[GenInfo.Size]    = 0;
            } else {
               GenInfo.PhotonFlag[GenInfo.Size]    = -1; }
         } else if( it_gen->status() == 1 ) {
            if( it_gen->pt() < 20 ) { continue; }
            cout << ">>> GenInfo >>> photon status 1 " << endl;
            GenInfo.PhotonFlag[GenInfo.Size]     = -1;
            cout << ">>> GenInfo >>> photon status 1 >>> finding Photon Flag " << endl;
            if( iMo1 != -1 && iMo2 != -1 && it_gen->pdgId() == 22 ) {
               if( GenInfo.Mo1PdgID[GenInfo.Size] == 22 && GenInfo.Mo2PdgID[GenInfo.Size] == 22 ) {
                  if( ( GenHandle->begin() + iMo1 )->status() == 3 && ( GenHandle->begin() + iMo2 )->status() == 3 )
                  { GenInfo.PhotonFlag[GenInfo.Size]   = 0; }
               } else if( ( GenHandle->begin() + iMo1 )->status() == 2 && ( GenHandle->begin() + iMo2 )->status() == 2 ) {
                  GenInfo.PhotonFlag[GenInfo.Size]      = 1;
               } else if( ( GenHandle->begin() + iMo1 )->status() == 3 && ( GenHandle->begin() + iMo2 )->status() == 3 &&
                          ( ( abs( ( GenHandle->begin() + iMo1 )->pdgId() ) < 6 || ( GenHandle->begin() + iMo1 )->pdgId() == 21 ) &&
                            ( abs( ( GenHandle->begin() + iMo1 )->pdgId() ) < 6 || ( GenHandle->begin() + iMo1 )->pdgId() == 21 ) )
                        ) {
                  if( iGrandMo1 != -1 && iGrandMo2 != -1 )
                     if( ( GenHandle->begin() + iGrandMo1 )->pdgId() == 2212 )
                        if( ( GenHandle->begin() + iGrandMo2 )->pdgId() == 2212 )
                        { GenInfo.PhotonFlag[GenInfo.Size]      = 2; }
               } else if( ( GenHandle->begin() + iMo1 )->status() == 3 && ( GenHandle->begin() + iMo2 )->status() == 3 ) {
                  if( iGrandMo1 != -1 && iGrandMo2 != -1 )
                     if( ( GenHandle->begin() + iGrandMo1 )->pdgId() != 2212 )
                        if( ( GenHandle->begin() + iGrandMo2 )->pdgId() != 2212 )
                        { GenInfo.PhotonFlag[GenInfo.Size]      = 3; }
               }
            } 
      }
      }

      if ( numberOfDaughters >= 2 ) {
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
      if ( numberOfMothers >= 1 )
      { monId   = it_gen->mother( 0 )->pdgId(); }

      //-------------------  b' decay - 0:other, 1:tW, 2:cW, 3:bZ 4:bH  -------------------
      cout << ">>> GenInfo >> Getting decay mode " << endl;
      if ( pdgId == +7 ) { 
         EvtInfo.McbprimeMass[0] = it_gen->p4().mag();
         // b' decay mode      - 0: others, 1: tW, 2: cW, 3: bZ, 4: bH
         if ( dauId1 == 6 && dauId2 == 24 ) { EvtInfo.McbprimeMode[0] = 1; }
         if ( dauId1 == 4 && dauId2 == 24 ) { EvtInfo.McbprimeMode[0] = 2; }
         if ( dauId1 == 5 && dauId2 == 23 ) { EvtInfo.McbprimeMode[0] = 3; }
         if ( dauId1 == 5 && dauId2 == 25 ) { EvtInfo.McbprimeMode[0] = 4; }
         if ( EvtInfo.McbprimeMode[0] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 1 ); }
         } else { MCDaughters[0] = dau1; }
      } 
      else if ( pdgId == -7 ) { 
         EvtInfo.McbprimeMass[1] = it_gen->p4().mag();
         if ( dauId1 == 6 && dauId2 == 24 ) { EvtInfo.McbprimeMode[1] = 1; }
         if ( dauId1 == 4 && dauId2 == 24 ) { EvtInfo.McbprimeMode[1] = 2; }
         if ( dauId1 == 5 && dauId2 == 23 ) { EvtInfo.McbprimeMode[1] = 3; }
         if ( dauId1 == 5 && dauId2 == 25 ) { EvtInfo.McbprimeMode[1] = 4; }
         if ( EvtInfo.McbprimeMode[1] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 1 ); }
         } else { MCDaughters[1] = dau1; }
      }
      //----------------  t' decay - 0:other, 1bW, 2:tZ, 3:tH , 4:tGamma  -----------------
      else if ( pdgId == +8 ) {
         EvtInfo.MctprimeMass[0] = it_gen->p4().mag();
         if ( dauId1 == 5 && dauId2 == 24 ) { EvtInfo.MctprimeMode[0] = 1; }
         if ( dauId1 == 6 && dauId2 == 23 ) { EvtInfo.MctprimeMode[0] = 2; }
         if ( dauId1 == 6 && dauId2 == 25 ) { EvtInfo.MctprimeMode[0] = 3; }
         if ( dauId1 == 6 && dauId2 == 22 ) { EvtInfo.MctprimeMode[0] = 4; }
         if ( EvtInfo.MctprimeMode[0] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[0] = dau1->daughter( 1 ); }
         } else { MCDaughters[0] = dau1; }
      } else if ( pdgId == -8 ) {
         EvtInfo.MctprimeMass[1] = it_gen->p4().mag();
         if ( dauId1 == 5 && dauId2 == 24 ) { EvtInfo.MctprimeMode[1] = 1; }
         if ( dauId1 == 6 && dauId2 == 23 ) { EvtInfo.MctprimeMode[1] = 2; }
         if ( dauId1 == 6 && dauId2 == 25 ) { EvtInfo.MctprimeMode[1] = 3; }
         if ( dauId1 == 6 && dauId2 == 22 ) { EvtInfo.MctprimeMode[1] = 4; }
         if ( EvtInfo.MctprimeMode[1] == 0 ) { continue; }
         if ( dauId1 == 6 ) {
            if ( abs( dau1->daughter( 0 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 0 ); }
            if ( abs( dau1->daughter( 1 )->pdgId() ) == 5 ) { MCDaughters[1] = dau1->daughter( 1 ); }
         } else { MCDaughters[1] = dau1; }
      } else if ( pdgId == +6 ) { // find top
         EvtInfo.MctopMass[0] = it_gen->p4().mag();
      } else if ( pdgId == -6 ) { // find tbar
         EvtInfo.MctopMass[1] = it_gen->p4().mag();
      }
      //-------------  W decay mode: 0:others, 1:enu, 2:munu, 3:taunu, 4:jj  --------------
      else if ( pdgId == -24 && ( monId == +7 || monId == -8 ) ) { // W- from b' or t' bar
         EvtInfo.McWMass[0] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { EvtInfo.McWMode[0] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { EvtInfo.McWMode[0] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { EvtInfo.McWMode[0] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { EvtInfo.McWMode[0] = 4; }
         if ( EvtInfo.McWMode[0] == 0 ) { continue; }
         MCDaughters[2] = dau1;
         MCDaughters[3] = dau2;
      } else if ( pdgId == +24 && monId == +6 ) { // W+ from t
         EvtInfo.McWMass[1] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { EvtInfo.McWMode[1] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { EvtInfo.McWMode[1] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { EvtInfo.McWMode[1] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { EvtInfo.McWMode[1] = 4; }
         if ( EvtInfo.McWMode[1] == 0 ) { continue; }
         MCDaughters[4] = dau1;
         MCDaughters[5] = dau2;
      } else if ( pdgId == +24 && ( monId == -7 || monId == +8 ) ) { // W+ from b'bar or t'
         EvtInfo.McWMass[2] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { EvtInfo.McWMode[2] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { EvtInfo.McWMode[2] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { EvtInfo.McWMode[2] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { EvtInfo.McWMode[2] = 4; }
         if ( EvtInfo.McWMode[2] == 0 ) { continue; }
         MCDaughters[6] = dau1;
         MCDaughters[7] = dau2;
      } else if ( pdgId == -24 && monId == -6 ) { // W- from tbar
         EvtInfo.McWMass[3] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { EvtInfo.McWMode[3] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { EvtInfo.McWMode[3] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { EvtInfo.McWMode[3] = 3; }
         if ( dauId1 >= 1 && dauId2 <= 5 ) { EvtInfo.McWMode[3] = 4; }
         if ( EvtInfo.McWMode[3] == 0 ) { continue; }
         MCDaughters[8] = dau1;
         MCDaughters[9] = dau2;
      } 
      //--------  Z decay - 0:others, 1:ee, 2:mumu, 3:tautau, 4:nunu, 5:bb, b:jj  ---------
      else if ( pdgId == 23 && ( monId == +7 || monId == +8 ) ) { // Z from b' or t'
         EvtInfo.McZMass[0] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 11 )   { EvtInfo.McZMode[0] = 1; }
         if ( dauId1 == 13 && dauId2 == 13 )   { EvtInfo.McZMode[0] = 2; }
         if ( dauId1 == 15 && dauId2 == 15 )   { EvtInfo.McZMode[0] = 3; }
         if ( ( dauId1 == 12 && dauId2 == 12 ) ||
              ( dauId1 == 14 && dauId2 == 14 ) ||
              ( dauId1 == 16 && dauId2 == 16 ) ) { EvtInfo.McZMode[0] = 4; }
         if ( dauId1 == 5 && dauId2 == 5 )   { EvtInfo.McZMode[0] = 5; }
         if ( dauId1 >= 1 && dauId2 <= 4 )   { EvtInfo.McZMode[0] = 6; }
         if ( EvtInfo.McZMode[0] == 0 ) { continue; }
         MCDaughters[10] = dau1;
         MCDaughters[11] = dau2;
      } else if ( pdgId == 23 && ( monId == -7 || monId == -8 ) ) { // Z from b'bar or t'bar
         EvtInfo.McZMass[1] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 11 )  { EvtInfo.McZMode[1] = 1; }
         if ( dauId1 == 13 && dauId2 == 13 )  { EvtInfo.McZMode[1] = 2; }
         if ( dauId1 == 15 && dauId2 == 15 )  { EvtInfo.McZMode[1] = 3; }
         if ( ( dauId1 == 12 && dauId2 == 12 ) ||
              ( dauId1 == 14 && dauId2 == 14 ) ||
              ( dauId1 == 16 && dauId2 == 16 ) ) { EvtInfo.McZMode[1] = 4; }
         if ( dauId1 == 5 && dauId2 == 5 )   { EvtInfo.McZMode[1] = 5; }
         if ( dauId1 >= 1 && dauId2 <= 4 )   { EvtInfo.McZMode[1] = 6; }
         if ( EvtInfo.McZMode[0] == 0 ) { continue; }

         MCDaughters[12] = dau1;
         MCDaughters[13] = dau2;
      }
   }
  
   //---------------------------  Fill generator daughters  ----------------------------
   // MC daughters: 0-1: hard jet from b'bar/t'bar, 2-9: W daughters, 10-13: Z daughters
   for( int i = 0; i < 14; i++ ) {
      if ( MCDaughters[i] == NULL ) { continue; }
      EvtInfo.McDauPt[i]    = MCDaughters[i]->pt();
      EvtInfo.McDauEta[i]   = MCDaughters[i]->eta();
      EvtInfo.McDauPhi[i]   = MCDaughters[i]->phi();
      EvtInfo.McDauPdgID[i] = MCDaughters[i]->pdgId();
   }
   return true;
}

