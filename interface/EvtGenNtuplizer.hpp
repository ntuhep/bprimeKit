/*******************************************************************************
*
*  Filename    : EvtGenNtuplizer
*  Description : Class for handling geninfo and event level object ntuplization
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_EVTGENNTUPLIZER_HPP
#define BPKFRAMEWORK_BPRIMEKIT_EVTGENNTUPLIZER_HPP

#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"
#include "format.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

class EvtGenNtuplizer : public NtuplizerBase
{

public:
  EvtGenNtuplizer ( const edm::ParameterSet&, bprimeKit* );
  virtual
  ~EvtGenNtuplizer ();

  void RegisterTree( TTree* );
  void Analyze( const edm::Event&, const edm::EventSetup& );

private:
  EvtInfoBranches EvtInfo;
  GenInfoBranches GenInfo;

  const edm::EDGetToken _rhotoken;
  const edm::EDGetToken _mettoken;
  const edm::EDGetToken _pmettoken;
  const edm::EDGetToken _pileuptoken;
  const edm::EDGetToken _vtxtoken;
  const edm::EDGetToken _vtxBStoken;
  const edm::EDGetToken _beamspottoken;

  const edm::EDGetToken _genevttoken;
  const edm::EDGetToken _genparticletoken;
  const edm::EDGetToken _lhetoken;

  const edm::EDGetToken _mettriggertoken;
  const edm::EDGetToken _metbadmutoken;
  const edm::EDGetToken _metbadchadtoken;

  edm::Handle<double> _rhohandle;
  edm::Handle<std::vector<pat::MET>> _methandle;
  edm::Handle<std::vector<pat::MET>> _pmethandle;
  edm::Handle<std::vector<PileupSummaryInfo> > _pileuphandle;
  edm::Handle<reco::BeamSpot> _beamspothandle;

  edm::Handle<GenEventInfoProduct> _genevthandle;
  edm::Handle<std::vector<reco::GenParticle> > _genparticlehandle;
  edm::Handle<LHEEventProduct> _lhehandle;

  edm::Handle<std::vector<reco::Vertex> > _vtxhandle;
  edm::Handle<std::vector<reco::Vertex> > _vtxBShandle;

  edm::Handle<edm::TriggerResults> _mettriggerhandle;

  /*******************************************************************************
  *   Helper private functions
  *******************************************************************************/
  void FillGen( const edm::Event&, const edm::EventSetup& );
  void FillEvent( const edm::Event&, const edm::EventSetup& );

  /*******************************************************************************
  *   Helper functions for gen info filling
  *******************************************************************************/
  bool IsTprime( const int ) const;
  bool HasTprimeDaughter( const std::vector<reco::GenParticle>::const_iterator& ) const;
  int  PhotonFlag( const std::vector<reco::GenParticle>::const_iterator& ) const;

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_EVTGENNTUPLIZER_HPP */
