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
	 const float currentToBFieldScaleFactor = 2.09237036221512717e-04;
	 double current ;
	 string NonLabel;

   //--------------------------  Setting up helper variables  --------------------------
   cout << "Getting lepton Handlers" << endl;
   NonLabel = "";
   for( unsigned il = 0; il < taulabel_.size(); il++ ) {
      if( NonLabel.compare( taulabel_[il].label() ) == 0 ) { continue; }
      TauHandle.push_back( TauHandler() );
      iEvent.getByLabel( taulabel_[il], TauHandle[il] );
      if( debug_ > 10 ) { cout << "leps " << il << " taulabel " << taulabel_[il] << " with " << TauHandle[il]->size() << " entries\n"; }
   }
   iEvent.getByLabel( "reducedEgamma", "reducedGedGsfElectronCores", els_h ); //  for CMSSW73x 
   iEvent.getByLabel( conversionsInputTag_, conversions_h );
   if( !TurnOffInCMSSW73x )
      if( tracklabel_.size() > 0 ) { iEvent.getByLabel( tracklabel_[0], tracks_h ); }            //Add by Jacky
   if( dcslabel_.size() > 0 ) { iEvent.getByLabel( dcslabel_[0], dcsHandle ); }            //refer to ElectroWeakAnalysis/MultiBosons/VgAnalyzerKit.cc (Jacky)
   
   
   //-----------------------------  Magnetic field set-up  -----------------------------
   // https://twiki.cern.ch/twiki/bin/view/CMS/ConversionBackgroundRejection)
   if( TurnOnInCMSSW_7_4_1 ){
   cout <<"Getting magnetic field settings" << endl;
   if ( isData ) {
      if( ( *dcsHandle ).size() != 0 ) {
         current = ( *dcsHandle )[0].magnetCurrent();
         evt_bField = current * currentToBFieldScaleFactor;
      } else {
         evt_bField = 3.80;
      }
   } else {
      ESHandle<MagneticField> magneticField;
      iSetup.get<IdealMagneticFieldRecord>().get( magneticField );
      evt_bField = magneticField->inTesla( Surface::GlobalPoint( 0., 0., 0. ) ).z();
   }
   }

   //------------------------  Setting up isolation parameters  ------------------------
   cout << "Attempting to read rhoiso..." << endl;
   iEvent.getByLabel( conversionsInputTag_, conversions_h );
   if( !TurnOffInCMSSW73x )
   iEvent.getByLabel( rhoIsoInputTag, rhoIso_h ); 
   if( !TurnOffInCMSSW73x )
   rhoIso = 0;
   //rhoIso = *( rhoIso_h.product() );

   //iEvent.getByLabel("gsfElectrons", els_h);
   
   //-------------------------  Begin looping over collection  -------------------------
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


