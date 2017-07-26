/*******************************************************************************
*
*  Filename    : bprimeKit_genInfo.cc
*  Description : Filling in generation information as well as some event info
*
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/EvtGenNtuplizer.hpp"

using namespace std;

/*******************************************************************************
*   Main control flow
*******************************************************************************/

void
EvtGenNtuplizer::FillGen( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  // Early exit for data
  if( iEvent.isRealData() ){ return; }

  const reco::Candidate* dau1;
  const reco::Candidate* dau2;
  std::vector<const reco::Candidate*> cands;
  std::vector<const reco::Candidate*>::const_iterator found;
  std::vector<const reco::Candidate*>::const_iterator daughter1, daughter2;
  std::vector<const reco::Candidate*>::const_iterator mother1, mother2;
  std::vector<const reco::Candidate*>::const_iterator gmother1, gmother2;
  int NMo, NDa;
  int dauId1, dauId2, monId;

  bool isTZTZ = false;
  bool isTZTH = false;
  bool isTHTH = false;
  bool isTHBW = false;
  bool isTZBW = false;
  bool isBWBW = false;
  vector<int> quarkID;
  vector<int> bosonID;

  for( auto it_gen = _genparticlehandle->begin(); it_gen != _genparticlehandle->end(); it_gen++ ){
    cands.push_back( &*it_gen );
  }

  // Event wide objects
  GenInfo.Weight            = _genevthandle->weight();
  GenInfo.ptHat             = _genevthandle->qScale();
  GenInfo.LHENominalWeight  = _lhehandle->hepeup().XWGTUP;
  GenInfo.LHEOriginalWeight = _lhehandle->originalXWGTUP();
  GenInfo.LHESize           = std::min( MAX_LHE, (int)( _lhehandle->weights().size() ) );

  // ----- Generation information  --------------------------------------------
  if( !iEvent.isRealData() && _genevthandle->hasPDF() ){
    GenInfo.PDFid1   = _genevthandle->pdf()->id.first;
    GenInfo.PDFid2   = _genevthandle->pdf()->id.second;
    GenInfo.PDFx1    = _genevthandle->pdf()->x.first;
    GenInfo.PDFx2    = _genevthandle->pdf()->x.second;
    GenInfo.PDFscale = _genevthandle->pdf()->scalePDF;
    GenInfo.PDFv1    = _genevthandle->pdf()->xPDF.first;
    GenInfo.PDFv2    = _genevthandle->pdf()->xPDF.second;
  }
  
  for( int i = 0; i < GenInfo.LHESize; ++i ){
    GenInfo.LHESystematicWeights[i] = _lhehandle->weights().at( i ).wgt;
    GenInfo.LHESystematicId[i]      = std::stoi( _lhehandle->weights().at( i ).id.data() );
  }

  /*******************************************************************************
  *   Gen particle loop
  *******************************************************************************/
  for( auto it_gen = _genparticlehandle->begin(); it_gen != _genparticlehandle->end(); ++it_gen  ){

    // ----- Setting up common variable  -------------------------------------
    dauId1 = dauId2 = monId = 0;
    dau1   = dau2 = NULL;
    NMo    = it_gen->numberOfMothers();
    NDa    = it_gen->numberOfDaughters();

    // ----- GenInfo Branch insertion  --------------------------------------
    if( GenInfo.Size < 60 ){
      GenInfo.Pt             [GenInfo.Size] = it_gen->pt();
      GenInfo.Eta            [GenInfo.Size] = it_gen->eta();
      GenInfo.Phi            [GenInfo.Size] = it_gen->phi();
      GenInfo.Mass           [GenInfo.Size] = it_gen->mass();
      GenInfo.PdgID          [GenInfo.Size] = it_gen->pdgId();
      GenInfo.Status         [GenInfo.Size] = it_gen->status();
      GenInfo.nMo            [GenInfo.Size] = it_gen->numberOfMothers();
      GenInfo.nDa            [GenInfo.Size] = it_gen->numberOfDaughters();
      GenInfo.Mo1            [GenInfo.Size] = -1;
      GenInfo.Mo2            [GenInfo.Size] = -1;
      GenInfo.Da1            [GenInfo.Size] = -1;
      GenInfo.Da2            [GenInfo.Size] = -1;
      GenInfo.Mo1PdgID       [GenInfo.Size] = 0;
      GenInfo.Mo2PdgID       [GenInfo.Size] = 0;
      GenInfo.Da1PdgID       [GenInfo.Size] = 0;
      GenInfo.Da2PdgID       [GenInfo.Size] = 0;
      GenInfo.GrandMo1PdgID  [GenInfo.Size] = 0;
      GenInfo.GrandMo2PdgID  [GenInfo.Size] = 0;
      GenInfo.Mo1Status      [GenInfo.Size] = -1;
      GenInfo.Mo2Status      [GenInfo.Size] = -1;
      GenInfo.GrandMo1Status [GenInfo.Size] = -1;
      GenInfo.GrandMo2Status [GenInfo.Size] = -1;

      // ----- Parent/Daughter information insertion  -----------------------
      mother1 = find( cands.begin(), cands.end(), it_gen->mother( 0 ) );
      if( mother1 != cands.end() ){
        GenInfo.Mo1       [GenInfo.Size] = mother1 - cands.begin();
        GenInfo.Mo1PdgID  [GenInfo.Size] = ( *mother1 )->pdgId();
        GenInfo.Mo1Status [GenInfo.Size] = ( *mother1 )->status();
        gmother1                         = find( cands.begin(), cands.end(), ( *mother1 )->mother( 0 ) );
        if( gmother1 != cands.end() ){
          GenInfo.GrandMo1PdgID [GenInfo.Size] = ( *gmother1 )->pdgId();
          GenInfo.GrandMo1Status[GenInfo.Size] = ( *gmother1 )->status();
        }
      }
      mother2 = find( cands.begin(), cands.end(), it_gen->mother( NMo-1 ) );
      if( mother2 != cands.end() ){
        GenInfo.Mo2       [GenInfo.Size] = mother2 - cands.begin();
        GenInfo.Mo2PdgID  [GenInfo.Size] = ( *mother2 )->pdgId();
        GenInfo.Mo2Status [GenInfo.Size] = ( *mother2 )->status();
        gmother2                         = find( cands.begin(), cands.end(), ( *mother2 )->mother( 0 ) );
        if( gmother2 != cands.end() ){
          GenInfo.GrandMo2PdgID [GenInfo.Size] = ( *gmother2 )->pdgId();
          GenInfo.GrandMo2Status[GenInfo.Size] = ( *gmother2 )->status();
        }
      }
      daughter1 = find( cands.begin(), cands.end(), it_gen->daughter( 0 ) );
      if( daughter1 != cands.end() ){
        GenInfo.Da1 [GenInfo.Size]      = daughter1 - cands.begin();
        GenInfo.Da1PdgID [GenInfo.Size] = it_gen->daughter( 0 )->pdgId();
      }
      daughter2 = find( cands.begin(), cands.end(), it_gen->daughter( NDa-1 ) );
      if( daughter2 != cands.end() ){
        GenInfo.Da2 [GenInfo.Size]      = daughter2 - cands.begin();
        GenInfo.Da2PdgID [GenInfo.Size] = it_gen->daughter( NDa-1 )->pdgId();
      }

      // ----- Photon Flag, see definition below  ---------------------------
      GenInfo.PhotonFlag[GenInfo.Size] = PhotonFlag( it_gen );
      ++GenInfo.Size;

      // ----- Getting information for ljmet algorithm  ---------------------
      if( !IsTprime( it_gen->pdgId() ) || HasTprimeDaughter( it_gen ) ){ continue; }

      for( size_t i = 0; i < it_gen->numberOfDaughters(); ++i ){
        int daughterId = it_gen->daughter( i )->pdgId();
        if( abs( daughterId ) == 5 || abs( daughterId ) == 6 ){
          quarkID.push_back( daughterId );
        } else if( abs( daughterId ) > 22 && abs( daughterId ) < 26 ){
          bosonID.push_back( daughterId );
        }
      }
    }

    // -----------------------------------------------------------------------
    //   Inserting Event Level information
    // -----------------------------------------------------------------------
    if( NDa >= 2 ){
      dau1   = it_gen->daughter( 0 );
      dau2   = it_gen->daughter( 1 );
      dauId1 = abs( dau1->pdgId() );
      dauId2 = abs( dau2->pdgId() );
      if( dauId1 > dauId2 ){// always keep dauId2 > dauId1
        const reco::Candidate* tmp = dau1;
        dau1   = dau2;
        dau2   = tmp;
        dauId1 = abs( dau1->pdgId() );
        dauId2 = abs( dau2->pdgId() );
      }
    }
    if( NMo >= 1 ){
      monId = it_gen->mother( 0 )->pdgId();
    }
  }
  // --------------------------------------------------------------------------
  //   Begin main part of ljmet algorithm
  //   Main reference: https://github.com/cms-ljmet/Ljmet-Com/blob/master/src/TpTpCalc.cc
  // --------------------------------------------------------------------------
  if( quarkID.size() < 2 ){
    return ; // Early exit. ljmet algorithm is last sequence in this function
  } else if( quarkID.size() != 2 ){
    double test = quarkID[0]*quarkID[1];
    int sign    = -1;
    if( test > 0 ){ sign = 1; }
    if( sign > 0 ){
      if( quarkID.size() == 4 ){
        swap( quarkID[2], quarkID[3] );
      }
      swap( quarkID[1], quarkID[2] );
      test = quarkID[0]*quarkID[1];
      sign = -1;
      if( test > 0 ){ sign = 1; }
      if( sign < 0 ){cout << "Signs are fixed!" << endl; }
    }
    if( quarkID.size() > 3 && abs( quarkID[3] ) == 6 ){
      swap( quarkID[2], quarkID[3] );
    }
    if( quarkID.size() > 2 && abs( quarkID[2] ) == 6 ){
      swap( quarkID[1], quarkID[2] );
    }
  }

  // tag the decay chains according to ID'd quarks and bosons.
  // After the fixes above there should not be errors (if we've done it right!)

  // 2 b quarks, check for matching W's
  if( abs( quarkID[0] ) == 5 && abs( quarkID[1] ) == 5 ){
    if( abs( bosonID[0] ) == 24 && abs( bosonID[1] ) == 24 ){ isBWBW = true; }
  }
  // 2 t quarks, check for Z's and H's
  else if( abs( quarkID[0] ) == 6 && abs( quarkID[1] ) == 6 ){
    if( bosonID[0] == 23 && bosonID[1] == 23 ){
      isTZTZ = true;
    } else if( bosonID[0] == 25 && bosonID[1] == 25 ){
      isTHTH = true;
    } else if( bosonID[0] == 25 && bosonID[1] == 23 ){
      isTZTH = true;
    } else if( bosonID[0] == 23 && bosonID[1] == 25 ){
      isTZTH = true;
    } else { cout << "2 t daughters didn't match tZtZ, tHtH, or tZtH" << bosonID[0] << ", " << bosonID[1] << endl; }
  }
  // t-b pairs, check for correlating bosons in the right spots
  else if( abs( quarkID[0] ) == 6 && abs( quarkID[1] ) == 5 ){
    if( bosonID[0] == 23 && abs( bosonID[1] ) == 24 ){
      isTZBW = true;
    } else if( bosonID[0] == 25 && abs( bosonID[1] ) == 24 ){
      isTHBW = true;
    } else { cout << "t - b pair didn't match Z/H - W pair" << bosonID[0] << ", " << bosonID[1] << endl; }
  }
  // b-t pairs, check for correlating bosons in the right spots
  else if( abs( quarkID[1] ) == 6 && abs( quarkID[0] ) == 5 ){
    if( bosonID[1] == 23 && abs( bosonID[0] ) == 24 ){
      isTZBW = true;
    } else if( bosonID[1] == 25 && abs( bosonID[0] ) == 24 ){
      isTHBW = true;
    } else { cout << "b - t pair didn't match W - Z/H pair" << bosonID[0] << ", " << bosonID[1] << endl; }
  }
  // error messages if we found something else entirely
  else {
    cout << "daughters didn't match a recognized pattern" << endl;

    for( size_t i = 0; i < quarkID.size(); i++ ){
      cout << "quark " << i << " = " << quarkID[i] << endl;
    }

    for( size_t i = 0; i < bosonID.size(); i++ ){
      cout << "boson " << i << " = " << bosonID[i] << endl;
    }
 }

  
  GenInfo.McIsTZTZ = isTZTZ;
  GenInfo.McIsTHTH = isTHTH;
  GenInfo.McIsTZTH = isTZTH;
  GenInfo.McIsTZBW = isTZBW;
  GenInfo.McIsTHBW = isTHBW;
  GenInfo.McIsBWBW = isBWBW;
  
 
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

int
EvtGenNtuplizer::PhotonFlag( const std::vector<reco::GenParticle>::const_iterator& particle ) const
{
  int numMo = particle->numberOfMothers();
  if( particle->status() == 3 ){
    if( particle->pdgId() == 22 ){ return PROMPT_PHOTON; } else { return UNKNOWN_FLAG; }
  } else if( particle->status() == 1 && particle->pdgId() == 22 && numMo > 1 ){
    auto mother1 = particle->mother( 0 );
    auto mother2 = particle->mother( numMo - 1 );
    if( mother1->pdgId()  == 22 && mother2->status() == 22 &&
        mother1->status() == 3  && mother2->status() == 3 ){
      return PROMPT_PHOTON;
    } else if( mother1->status() == 2 && mother2->status() == 2 ){
      return DECAY_IN_FLIGHT;
    } else if( mother1->status()  == 3 && mother2->status()  == 3 &&
               mother1->mother( 0 ) != 0 && mother2->mother( 0 ) != 0 &&
               mother1->mother( 0 )->pdgId() == 2212 &&
               mother2->mother( 0 )->pdgId() == 2212 ){
      if( (  abs( mother1->pdgId() ) < 6 || mother1->pdgId()       == 21 ) &&
          (  abs( mother2->pdgId() ) < 6 || mother2->pdgId() == 21 ) ){
        return ISR_PHOTON;
      } else {return FSR_PHOTON; }
    } else {return UNKNOWN_FLAG; }
  }
  return UNKNOWN_FLAG;
}

/******************************************************************************/

bool
EvtGenNtuplizer::IsTprime( const int pdgid ) const
{
  if( abs( pdgid ) == 8 ){return true; }
  if( abs( pdgid ) == 8000001 ){return true; }
  return false;
}

/******************************************************************************/

bool
EvtGenNtuplizer::HasTprimeDaughter( const std::vector<reco::GenParticle>::const_iterator& particle ) const
{
  for( size_t i = 0; i < particle->numberOfDaughters(); ++i ){
    if( IsTprime( particle->daughter( i )->pdgId() ) ){
      return true;
    }
  }

  return false;
}
