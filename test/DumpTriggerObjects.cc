/*******************************************************************************
 *
 *  Filename    : DumpTriggerObjects.cc
 *  Description : Simple script for dumping exhuastive informatin
                  about all trigger objects stored in miniAOD file
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "bpkFrameWork/bprimeKit/interface/Types.h"

#include "TFile.h"
#include <cstdio>

int main(int argc, char const *argv[]) {
   if( argc != 2 ){
      fprintf(
         stderr,
         "Error! Takes exactly one argument, which is the path to a miniAOD file!\n"
      );
      return 1;
   }
   fwlite::Event ev( TFile::Open(argv[1]) );
   fwlite::Handle<pat::TriggerObjectStandAloneCollection> triggerobj_handle;
   fwlite::Handle<edm::TriggerResults>                    trigger_handle;

   unsigned i = 0 ;
   for( ev.toBegin() ; !ev.atEnd() && i < 100  ; ++ev ){
      triggerobj_handle.getByLabel( ev, "selectedPatTrigger" );
      trigger_handle.getByLabel( ev, "TriggerResults" , "" , "HLT2" );
      const auto& triggernames = ev.triggerNames( *trigger_handle );

      fprintf( stdout , "Event [%u/%lld]\n", i , ev.size() );

      unsigned j = 0 ;
      for( auto obj : *triggerobj_handle ){
         obj.unpackPathNames(triggernames);
         fprintf(
            stdout, "\tTrigger object [%d] pt:%lf eta:%lf phi%lf\n",
            j, obj.pt(), obj.eta(), obj.phi()
          );

         fprintf(
            stdout, "\t\tCollection\t %s\n",
            obj.collection().c_str()
         );

         fprintf( stdout, "\t\tFilter IDs:\t");
         for( const auto& id: obj.filterIds() ){
            fprintf( stdout, " %d", id );
         }
         fprintf( stdout, "\n" );

         fprintf( stdout, "\t\tFilter Names:\t");
         for( const auto& filter : obj.filterLabels() ){
            fprintf( stdout, " %s", filter.c_str() );
         }
         fprintf( stdout, "\n" );

         fprintf( stdout, "\t\tHLT Paths:\t");
         for( const auto& path : obj.pathNames() ){
            fprintf( stdout, " %s", path.c_str() );
         }
         fprintf( stdout, "\n\n" );
         ++j;
      }
   }
   return 0;
}
