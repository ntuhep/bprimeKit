/***************************************************************************************************
 *
 *  Filename    : bprimeKit_util.h
 *  Description : Utility functions used in bprimeKit for cleaner coding 
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *  
 *  Details     : All Unless otherwise specified, the functions will be implemented in 
 *                plugins/bprimeKit_utils.cc
 *
***************************************************************************************************/


#ifndef __UTIL_BPRIMEKIT_H__
#define __UTIL_BPRIMEKIT_H__

#include "bpkFrameWork/bprimeKit/interface/bprimeKit.h"

//-------------------------------------------------------------------------------------------------- 
//   MC Tag for particles
//-------------------------------------------------------------------------------------------------- 
extern int getGenMCTag( const reco::GenParticle* );
extern int getGenMCTag( GenIterator& , ElectronIterator& ) ;
extern int getGenMCTag( GenIterator& , MuonIterator&     ) ;
extern int genGenMCTag( GenIterator& , JetIterator&      ) ; 

//-------------------------------------------------------------------------------------------------- 
//   MC tag for GenParticles
//-------------------------------------------------------------------------------------------------- 
extern bool isTprime( const int );


#endif /* end of include guard: __UTIL_BPRIMEKIT_H__ */
