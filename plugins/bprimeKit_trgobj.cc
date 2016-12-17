/*******************************************************************************
*
*  Filename    : bprimeKit_trgobj.cc
*  Description : Storing trigger objects for dedicated triggers
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"
#include <regex>
using namespace std;

/*******************************************************************************
*   Prototyping helper functions
*******************************************************************************/
bool TrigObjMatchPathFilter(
   const pat::TriggerObjectStandAlone&,
   const std::string& pathglob,
   std::string&       matchedpath,
   const std::string& filterglob,
   std::string&       matchedfilter
   );

bool
bprimeKit::FillTrgObj( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
   const edm::TriggerNames& TrgNames = iEvent.triggerNames( *fTrigger_H );

   // Getting trigger table
   fEvtInfo.TrgCount = 0;

   for( size_t i = 0; i < N_TRIGGER_BOOKINGS; ++i ){
      unsigned int TrgIndex = TrgNames.triggerIndex( TriggerBooking[i] );
      if( TrgIndex == TrgNames.size() ){
         fEvtInfo.TrgBook[i] = -4;// The trigger path is not known in this event.
      } else if( !fTrigger_H->wasrun( TrgIndex ) ){
         fEvtInfo.TrgBook[i] = -3;// The trigger path was not included in this event.
      } else if( !fTrigger_H->accept( TrgIndex ) ){
         fEvtInfo.TrgBook[i] = -2;// The trigger path was not accepted in this event.
      } else if( fTrigger_H->error( TrgIndex ) ){
         fEvtInfo.TrgBook[i] = -1;// The trigger path has an error in this event.
      } else {
         fEvtInfo.TrgBook[i] = +1;// It's triggered.
         fEvtInfo.TrgCount++;
      }
   }

   fEvtInfo.nHLT = TrgNames.size();

   // TODO Assuming prescale set = 0 , check.
   // Enoch 2016-02-04
   for( size_t i = 0; i < TrgNames.size(); ++i ){
      const std::string name = TrgNames.triggerName( i );
      fEvtInfo.HLTbits[i]           = ( fTrigger_H->accept( i ) == true ) ? 1 : 0;
      fEvtInfo.HLTPrescaleFactor[i] = fHighLevelTriggerConfig.prescaleValue( 0, name );
      fEvtInfo.HLTName2enum[i]      = GetTriggerIdx( name );
   }

   bool changedConfig = false;
   if( !fHighLevelTriggerConfig.init( iEvent.getRun(), iSetup, "HLT", changedConfig ) ){
      std::cout << "Initialization of HLTConfigProvider failed!!" << std::endl;
      return 1;
   }

   // Trigger object part
   // Initializing
   memset( &fTrgInfo, 0x00, sizeof( fTrgInfo ) );

   for( auto obj : *fTriggerObjList_H ){
      obj.unpackPathNames( TrgNames );

      for( const auto& hltpair : fTrgList ){
         if( fTrgInfo.Size >= 64 ){ break; }

         std::string matchedpath   = "";
         std::string matchedfilter = "";
         bool matchresult          = TrigObjMatchPathFilter(
            obj,
            hltpair.first, matchedpath,
            hltpair.second, matchedfilter
            );
         if( !matchresult ){ continue; }
         fTrgInfo.Pt        [fTrgInfo.Size] = obj.pt();
         fTrgInfo.Eta       [fTrgInfo.Size] = obj.eta();
         fTrgInfo.Phi       [fTrgInfo.Size] = obj.phi();
         fTrgInfo.Energy    [fTrgInfo.Size] = obj.energy();
         fTrgInfo.TriggerBit[fTrgInfo.Size] = GetTriggerIdx( matchedpath );
         for( const auto& filterid : obj.filterIds() ){
            fTrgInfo.FilterLabel[fTrgInfo.Size] *= 100;
            fTrgInfo.FilterLabel[fTrgInfo.Size] += filterid;
         }

         fTrgInfo.Size++;
      }
   }

   return true;
}

int
bprimeKit::GetTriggerIdx( const string& triggername ) const
{
   const auto& result = fHighLevelTriggerMap.find( triggername  );
   if( result != fHighLevelTriggerMap.end() ){
      return result->second;
   } else {
      // cout << "Cannot find trigger index for " << triggername << endl;
      return -1;
   }
}


/*******************************************************************************
*   Helper functions implementations
*******************************************************************************/
string GlobToRegex( const std::string& );

bool
TrigObjMatchPathFilter(
   const pat::TriggerObjectStandAlone& obj,
   const std::string&                  path,
   std::string&                        matchedpath,
   const std::string&                  filter,
   std::string&                        matchedfilter
   )
{
   bool hasmatchpath   = false;
   bool hasmatchfilter = false;

   const std::regex pathreg( GlobToRegex( path ) );

   for( const auto& objpath : obj.pathNames() ){
      if( std::regex_match( objpath, pathreg ) && obj.hasPathName( objpath ) ){
         hasmatchpath = true;
         matchedpath  = objpath;
         break;
      }
   }

   const std::regex filterreg( GlobToRegex( filter ) );

   for( const auto& objfilter : obj.filterLabels() ){
      if( std::regex_match( objfilter, filterreg ) && obj.hasFilterLabel( objfilter ) ){
         hasmatchfilter = true;
         matchedfilter  = objfilter;
         break;
      }
   }

   return hasmatchpath && hasmatchfilter;
}




string
GlobToRegex( const std::string& query )
{
   static const std::regex starmatch( "\\*" );
   static const std::string starrep( ".*" );

   static const std::regex qmmatch( "\\?" );
   static const std::string qmrep( "." );

   string ans = query;
   ans = std::regex_replace( ans, starmatch, starrep );
   ans = std::regex_replace( ans, qmmatch, qmrep );
   return ans;
}
