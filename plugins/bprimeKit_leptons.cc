/*******************************************************************************
 *
 *  Filename    : bprimeKit_leptons.cc
 *  Description : filling in lepton information
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

bool bprimeKit::fillLepton( const edm::Event& iEvent , const edm::EventSetup& iSetup ) 
{
   for( size_t icoll = 0; icoll < lepcollections_.size(); icoll++ ) { 
      if( icoll >= MAX_LEPCOLLECTIONS ) { break; }
      if( debug_ > 5 ) { cout << "Fill lepton info, collection " << icoll << " with name " << lepcollections_[icoll] << endl; }
      memset( &LepInfo[icoll], 0x00, sizeof( LepInfo[icoll] ) ); 
      fillMuon( iEvent , iSetup , icoll ) ; 
      fillElectron( iEvent, iSetup , icoll ) ;
      fillTau( iEvent , iSetup , icoll ) ;
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
//   if ( isData ) {
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
