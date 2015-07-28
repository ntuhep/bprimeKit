/*******************************************************************************
 *
 *  Filename    : bprimeKit_genInfo.cc
 *  Description : filling in generation information as well as some event info
 *
 *******************************************************************************/


#include "MyAna/bprimeKit/interface/bprimeKit.h"

//-----------------------  GenInfo specific CMSSW libraries  ------------------------
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


//-------------------------------------------------------------------------------------------------- 
//   Helper functions
//-------------------------------------------------------------------------------------------------- 
bool isHardProcesStatus( const int ) ;
bool isGeneratorStatus(  const int ) ;
bool isValidGenParticle( const GenIterator& ) ;
int  photonFlag( const GenIterator& ) ; 
//------------------------------------------------------------------------------ 
//   Typedefs and enums
//------------------------------------------------------------------------------ 
typedef std::vector<const reco::Candidate*>              CandidateList;
typedef CandidateList::const_iterator                    CandidateIterator;
typedef edm::Handle<GenEventInfoProduct>                 GenInfoHandle;

//------------------------------------------------------------------------------ 
//   bprimeKit method implementation
//------------------------------------------------------------------------------ 

bool bprimeKit::fillGenInfo( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   const reco::Candidate* MCDaughters[14];
   const reco::Candidate* dau1;
   const reco::Candidate* dau2;
   GenInfoHandle          genEventInfo;
   CandidateList          cands;
   CandidateIterator      daughter1, daughter2;
   CandidateIterator      mother1  , mother2;
   CandidateIterator      gmother1 , gmother2;
   int NMo, NDa;
   int pdgId , dauId1 , dauId2 , monId ; 
   double evWeight;

   //-------------------------------------------------------------------------------------------------- 
   //   Setting up common variables
   //-------------------------------------------------------------------------------------------------- 
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

   //-------------------------------------------------------------------------------------------------- 
   //   Begin main loop
   //-------------------------------------------------------------------------------------------------- 
   for( GenIterator it_gen = GenHandle->begin(); it_gen != GenHandle->end(); ++it_gen  ) {

      //----- Setting up common variable  ----------------------------------------------------------------
      dauId1 = dauId2 = monId = 0;
      dau1   = dau2   = NULL;
      pdgId  = it_gen->pdgId();
      NMo    = it_gen->numberOfMothers();
      NDa    = it_gen->numberOfDaughters();

      //----- GenInfo Branch insertion  ------------------------------------------------------------------
      if( isValidGenParticle(it_gen) ){
         if( debug_ ) { cout << "Enter GenInfo" << endl; } 

         GenInfo.Pt             [GenInfo.Size] = it_gen->pt()                ;
         GenInfo.Eta            [GenInfo.Size] = it_gen->eta()               ;
         GenInfo.Phi            [GenInfo.Size] = it_gen->phi()               ;
         GenInfo.Mass           [GenInfo.Size] = it_gen->mass()              ;
         GenInfo.PdgID          [GenInfo.Size] = it_gen->pdgId()             ;
         GenInfo.Status         [GenInfo.Size] = it_gen->status()            ;
         GenInfo.nMo            [GenInfo.Size] = it_gen->numberOfMothers()   ;
         GenInfo.nDa            [GenInfo.Size] = it_gen->numberOfDaughters() ;
         GenInfo.Mo1            [GenInfo.Size] = -1                          ;
         GenInfo.Mo2            [GenInfo.Size] = -1                          ;
         GenInfo.Da1            [GenInfo.Size] = -1                          ;
         GenInfo.Da2            [GenInfo.Size] = -1                          ;
         GenInfo.Mo1PdgID       [GenInfo.Size] =  0                          ;
         GenInfo.Mo2PdgID       [GenInfo.Size] =  0                          ;
         GenInfo.Da1PdgID       [GenInfo.Size] =  0                          ;
         GenInfo.Da2PdgID       [GenInfo.Size] =  0                          ;
         GenInfo.GrandMo1PdgID  [GenInfo.Size] =  0                          ;
         GenInfo.GrandMo2PdgID  [GenInfo.Size] =  0                          ;
         GenInfo.Mo1Status      [GenInfo.Size] = -1                          ;
         GenInfo.Mo2Status      [GenInfo.Size] = -1                          ;
         GenInfo.GrandMo1Status [GenInfo.Size] = -1                          ;
         GenInfo.GrandMo2Status [GenInfo.Size] = -1                          ;

         //----- Parent/Daughter information insertion  -----------------------------------------------------
         if( debug_ ) { cout << ">>Gen>> Getting parent candidates" << endl;}

         mother1 = find( cands.begin(), cands.end(), it_gen->mother(0) );
         if( mother1 != cands.end() ) { 
            GenInfo.Mo1       [GenInfo.Size] = mother1 - cands.begin() ;
            GenInfo.Mo1PdgID  [GenInfo.Size] = (*mother1)->pdgId() ;  
            GenInfo.Mo1Status [GenInfo.Size] = (*mother1)->status();
            gmother1 = find( cands.begin(), cands.end(), (*mother1)->mother(0) );
            if( gmother1 != cands.end() ) { 
               GenInfo.GrandMo1PdgID [GenInfo.Size] = (*gmother1)->pdgId();
               GenInfo.GrandMo1Status[GenInfo.Size] = (*gmother1)->status();
            }
         } 
         mother2 = find( cands.begin(), cands.end(), it_gen->mother(NMo-1) );
         if( mother2 != cands.end() ) { 
            GenInfo.Mo2       [GenInfo.Size] = mother2 - cands.begin() ;
            GenInfo.Mo2PdgID  [GenInfo.Size] = (*mother2)->pdgId() ;  
            GenInfo.Mo2Status [GenInfo.Size] = (*mother2)->status();
            gmother2 = find( cands.begin(), cands.end(), (*mother2)->mother(0) );
            if( gmother2 != cands.end() ) { 
               GenInfo.GrandMo2PdgID [GenInfo.Size] = (*gmother2)->pdgId();
               GenInfo.GrandMo2Status[GenInfo.Size] = (*gmother2)->status();
            }
         }
         daughter1 = find( cands.begin(), cands.end(), it_gen->daughter(0) );
         if( daughter1 != cands.end() ) { 
            GenInfo.Da1 [GenInfo.Size] = daughter1 - cands.begin() ; 
            GenInfo.Da1PdgID [GenInfo.Size]  = it_gen->daughter(0)->pdgId();
         }
         daughter2 = find( cands.begin(), cands.end(), it_gen->daughter(NDa-1) );
         if( daughter2 != cands.end() ) { 
            GenInfo.Da2 [GenInfo.Size]= daughter2 - cands.begin() ;
            GenInfo.Da2PdgID [GenInfo.Size]  = it_gen->daughter(NDa-1)->pdgId();
         }

         //----- Photon Flag, see definition below  ---------------------------------------------------------
         GenInfo.PhotonFlag[GenInfo.Size] = photonFlag( it_gen ) ;
         ++GenInfo.Size;
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

      if( debug_ ) { cout << "Getting decay mode" << endl; }
      //----- b' decay mode  -----------------------------------------------------------------------------
      // b' decay - 0:other, 1:tW, 2:cW, 3:bZ 4:bH
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

      //----- t' decay mode  -----------------------------------------------------------------------------
      // 0:other, 1bW, 2:tZ, 3:tH , 4:tGamma
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

      //----- W decay mode  ------------------------------------------------------------------------------
      // 0:others, 1:enu, 2:munu, 3:taunu, 4:jj
      else if ( pdgId == -24 && ( monId == +7 || monId == -8 ) ) { // W- from b' or t' bar
         EvtInfo.McWMass[0] = it_gen->p4().mag();
         if ( dauId1 == 11 && dauId2 == 12 ) { EvtInfo.McWMode[0] = 1; }
         if ( dauId1 == 13 && dauId2 == 14 ) { EvtInfo.McWMode[0] = 2; }
         if ( dauId1 == 15 && dauId2 == 16 ) { EvtInfo.McWMode[0] = 3; }
         if ( dauId1 >= 1  && dauId2 <= 5  ) { EvtInfo.McWMode[0] = 4; }
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
      //----- Z decay mode  ------------------------------------------------------------------------------
      // 0:others, 1:ee, 2:mumu, 3:tautau, 4:nunu, 5:bb, b:jj
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

   //----- Fill generation daughters  -----------------------------------------------------------------
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



/***************************************************************************************************
 *
 *  Note : Valid GenParticle Criteria,
 *
 *  Main reference: http://home.thep.lu.se/~torbjorn/pythia81html/ParticleProperties.html
 *
 ***************************************************************************************************/
bool isGeneratorStatus( const int status ) 
{
   return true;
}

bool isHardProcesStatus( const int status ) 
{
   return true;
}

bool isValidGenParticle( const GenIterator& ) 
{
   // WZ H quarks(1-6) gluon leptons photons
   // Stroe the first 50? How do we discard the extra hadrons.....
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

int photonFlag( const GenIterator& particle )
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
