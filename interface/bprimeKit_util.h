#ifndef __UTIL_BPRIMEKIT_H__
#define __UTIL_BPRIMEKIT_H__

#include "MyAna/bprimeKit/interface/bprimeKit.h"

//---------------------  Defined in plugins/bprimeKit_utils.cc  ---------------------
extern int getGenMCTag( const reco::GenParticle* );
extern int getGenMCTag( GenIterator& , ElectronIterator& ) ;
extern int getGenMCTag( GenIterator& , MuonIterator&     ) ;
extern int genGenMCTag( GenIterator& , JetIterator&      ) ; 

#endif /* end of include guard: __UTIL_BPRIMEKIT_H__ */
