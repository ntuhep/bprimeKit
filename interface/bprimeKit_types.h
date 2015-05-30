#ifndef __BPRIMEKIT_TYPE_H__
#define __BPRIMEKIT_TYPE_H__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef std::vector<reco::GenParticle>::const_iterator GenIterator ; 
typedef std::vector<pat::Electron>::const_iterator     ElectronIterator;
typedef std::vector<pat::Muon>::const_iterator         MuonIterator;


#endif /* end of include guard: __BPRIMEKIT_TYPE_H__ */
