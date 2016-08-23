/*******************************************************************************
 *
 *  Filename    : bprimeKit_trgobj.cc
 *  Description : Storing trigger objects for dedicated triggers
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
using namespace std;

#define DEBUG_LINE \
if( fRhoToken.index() ) { \
   cout << __FILE__ << " " << __LINE__ << " " << fRhoToken.index() << endl; \
   throw std::invalid_argument("Exiting because fRhoToken has been modified!"); \
}

bool bprimeKit::FillTrgObj( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   // Initializing
   memset( &fTrgInfo , 0x00, sizeof(fTrgInfo) );

   const edm::TriggerNames& TrgNames = iEvent.triggerNames( *fTrigger_H );

   // Getting trigger table
   fEvtInfo.TrgCount = 0;
   for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ) {
      unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
      if ( TrgIndex == TrgNames.size() ) {
         fEvtInfo.TrgBook[i] = -4; // The trigger path is not known in this event.
      } else if ( !fTrigger_H->wasrun( TrgIndex ) ) {
         fEvtInfo.TrgBook[i] = -3; // The trigger path was not included in this event.
      } else if ( !fTrigger_H->accept( TrgIndex ) ) {
         fEvtInfo.TrgBook[i] = -2; // The trigger path was not accepted in this event.
      } else if (  fTrigger_H->error ( TrgIndex ) ) {
         fEvtInfo.TrgBook[i] = -1; // The trigger path has an error in this event.
      } else {
         fEvtInfo.TrgBook[i] = +1; // It's triggered.
         fEvtInfo.TrgCount++;
      }
   }

   fEvtInfo.nHLT = TrgNames.size();
   // TODO Assuming prescale set = 0 , check.
   // Enoch 2016-02-04
   for( size_t i = 0; i < TrgNames.size(); ++i ) {
      const std::string name = TrgNames.triggerName( i );
      fEvtInfo.HLTbits[i]           = ( fTrigger_H->accept( i ) == true ) ? 1 : 0;
      fEvtInfo.HLTPrescaleFactor[i] = fHighLevelTriggerConfig.prescaleValue( 0 , name );
      fEvtInfo.HLTName2enum[i]      = GetTriggerIdx( name );
   }

   for( auto obj : *fTriggerObjList_H ) {
      obj.unpackPathNames(TrgNames);
      for( const auto& path : fTrgList ){
         for( const auto objpath : obj.pathNames() ){
            if( fTrgInfo.Size >= 64 ) { break; }
            if( objpath.find(path) == string::npos ) { continue; }
            if( !obj.hasPathName(objpath) ) { continue; }
            fTrgInfo.Pt     [fTrgInfo.Size]    = obj.pt();
            fTrgInfo.Eta    [fTrgInfo.Size]    = obj.eta();
            fTrgInfo.Phi    [fTrgInfo.Size]    = obj.phi();
            fTrgInfo.Energy [fTrgInfo.Size]    = obj.energy();
            fTrgInfo.TriggerBit[fTrgInfo.Size] = GetTriggerIdx(objpath);
            fTrgInfo.Size++;
         }
      }
   }
   return true;
}

int bprimeKit::GetTriggerIdx( const string& triggername ) const
{
   const auto& result = fHighLevelTriggerMap.find( triggername  );
   if( result != fHighLevelTriggerMap.end() ) {
      return result->second ;
   } else {
      return -1;
   }
}
