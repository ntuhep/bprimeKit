/*******************************************************************************
 *
 *  Filename    : bprimeKit_leptons.cc
 *  Description : Filling in lepton information
 *  Details     : Since all leptons are stored in the same branch in the
 *                bprimeKit rather than being stored by type, as with the
 *                miniAOD format, it sublayer is required for stripping
 *                information out of the miniAOD.
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

using namespace std;
//------------------------------------------------------------------------------
//   Static helper variables
//------------------------------------------------------------------------------

bool bprimeKit::FillLepton( const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   for( size_t icoll = 0; icoll < fLeptonCollections.size(); icoll++ ) {
      if( icoll >= MAX_LEPCOLLECTIONS ) { break; }
      memset( &fLepInfo[icoll], 0x00, sizeof( fLepInfo[icoll] ) );
      FillMuon( iEvent , iSetup , icoll ) ;
      FillElectron( iEvent, iSetup , icoll ) ;
      FillTau( iEvent , iSetup , icoll ) ;
   }
   return true;
}
