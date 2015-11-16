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
#include "MyAna/bprimeKit/interface/bprimeKit.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

//------------------------------------------------------------------------------ 
//   Static helper variables
//------------------------------------------------------------------------------

bool bprimeKit::FillLepton( const edm::Event& iEvent , const edm::EventSetup& iSetup ) 
{
   for( size_t icoll = 0; icoll < fLeptonCollections.size(); icoll++ ) { 
      if( icoll >= MAX_LEPCOLLECTIONS ) { break; }
      if( fDebug > 1 ) { 
         std::cerr << "\t[1] Fill lepton info, collection " << icoll 
                   << " with name " << fLeptonCollections[icoll] << std::endl; 
      }
      memset( &fLepInfo[icoll], 0x00, sizeof( fLepInfo[icoll] ) ); 
      FillMuon( iEvent , iSetup , icoll ) ; 
      FillElectron( iEvent, iSetup , icoll ) ;
      FillTau( iEvent , iSetup , icoll ) ;
   }
   return true;
}

//-------------------------------------------------------------------------------------------------- 
//   Legacy code sections
//-------------------------------------------------------------------------------------------------- 
//	const float currentToBFieldScaleFactor = 2.09237036221512717e-04;
//	double current ;
// if( dcslabel_.size() > 0 ) { iEvent.getByLabel( dcslabel_[0], dcsHandle ); }            //refer to ElectroWeakAnalysis/MultiBosons/VgAnalyzerKit.cc (Jacky)
//   //-----------------------------  Magnetic field set-up  -----------------------------
//   // https://twiki.cern.ch/twiki/bin/view/CMS/ConversionBackgroundRejection)
//   cout <<"Getting magnetic field settings" << endl;
//   if ( fIsData ) {
//      if( ( *dcsHandle ).size() != 0 ) {
//         current = ( *dcsHandle )[0].magnetCurrent();
//         evt_bField = current * currentToBFieldScaleFactor;
//      } else {
//         evt_bField = 3.80;
//      }
//   } else {
//      ESHandle<MagneticField> magneticField;
//      iSetup.get<IdealMagneticFieldRecord>().get( magneticField );
//      evt_bField = magneticField->inTesla( Surface::GlobalPoint( 0., 0., 0. ) ).z();
//   }
//
