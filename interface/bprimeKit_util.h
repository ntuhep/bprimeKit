#ifndef __UTIL_BPRIMEKIT_H__
#define __UTIL_BPRIMEKIT_H__

#include "MyAna/bprimeKit/interface/bprimeKitTypes.h"

extern int getGenMCTag( const reco::GenParticle* );
extern int getGenMCTag( GenIterator& , ElectronIterator& ) ;
extern int getGenMCTag( GenIterator& , MuonIterator&     ) ; 

#endif /* end of include guard: __UTIL_BPRIMEKIT_H__ */
