/*******************************************************************************
 *
 *  Filename    : Types.h
 *  Description : Typedefs for all edm interaction classes
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __TYPES_H__
#define __TYPES_H__

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

typedef std::vector<pat::MET>   METList;
typedef edm::Handle<METList>    METHandle;
typedef METList::const_iterator METIterator;

typedef edm::Handle<std::vector<PileupSummaryInfo>>      PileupHandle;
typedef std::vector<PileupSummaryInfo>::const_iterator   PileupIterator;
   
typedef edm::Handle<TriggerResults>                TriggerHandle;
typedef edm::Handle<L1GlobalTriggerReadoutRecord>  RecordHandle;
typedef edm::Handle<reco::BeamSpot>                BeamSpotHandle ;

typedef edm::Handle<GenEventInfoProduct> GenInfoHandle;
typedef std::vector<reco::GenParticle>   GenList;
typedef edm::Handle<GenList>             GenHandle;
typedef GenList::const_iterator          GenIterator ;
typedef edm::Handle<LHEEventProduct>     LHEHandle;

typedef std::vector<reco::Vertex>  VertexList;
typedef edm::Handle<VertexList>    VertexHandle   ;
typedef VertexList::const_iterator VertexIterator;

typedef std::vector<edm::Handle<edm::ValueMap<reco::IsoDeposit>>>   IsoDepositMaps;
typedef std::vector<edm::Handle<edm::ValueMap<double>>>             IsoDepositVals;

typedef edm::View<pat::Electron>       ElectronList        ;
typedef edm::Handle<ElectronList>      ElectronHandle      ;
typedef ElectronList::const_iterator   ElectronIterator    ;
typedef edm::View<reco::GsfElectron>   GsfList             ;
typedef edm::Handle<GsfList>           GsfHandle           ;
typedef GsfList::const_iterator        GsfIterator         ;

typedef edm::View<pat::Muon>     MuonList;
typedef edm::Handle<MuonList>    MuonHandle;
typedef std::vector<MuonHandle>  MuonHandleList;
typedef MuonList::const_iterator MuonIterator ;

typedef std::vector<pat::Tau>   TauList;
typedef edm::Handle<TauList>    TauHandle;
typedef std::vector<TauHandle>  TauHandleList;
typedef TauList::const_iterator TauIterator;

typedef edm::View<pat::Photon>     PhotonList; 
typedef edm::Handle<PhotonList>    PhotonHandle;
typedef std::vector<PhotonHandle>  PhotonHandleList;
typedef PhotonList::const_iterator PhotonIterator ;

typedef edm::View<pat::Jet>     JetList;
typedef edm::Handle<JetList>    JetHandle;
typedef std::vector<JetHandle>  JetHandleList;
typedef JetList::const_iterator JetIterator;

typedef edm::Handle<reco::TrackCollection>       TrackHandle        ;
typedef edm::Handle<reco::ConversionCollection>  ConversionHandle    ;
typedef edm::Handle<DcsStatusCollection>         DcsStatusHandle     ;

typedef vector<const reco::Candidate*>   CandidateList;
typedef CandidateList::const_iterator    CandidateIterator;


#endif // __TYPES_H__
