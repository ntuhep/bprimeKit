/*******************************************************************************
*
*  Filename    : PhotonNtuplizer.hpp
*  Description : Class for handling objects required to ntuplize the photon
*                collection
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP
#define BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP

#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"
#include "bpkFrameWork/bprimeKit/interface/format.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

class PhotonNtuplizer : public NtuplizerBase
{

public:
  PhotonNtuplizer ( const edm::ParameterSet&, bprimeKit* );
  virtual
  ~PhotonNtuplizer ();

  void RegisterTree( TTree* );
  void Analyze( const edm::Event&, const edm::EventSetup& );

private:
  PhotonInfoBranches PhotonInfo;

  const std::string _photonname;
  const edm::EDGetToken _rhotoken;
  const edm::EDGetToken _photontoken;
  const edm::EDGetToken _photonLooseIDToken;
  const edm::EDGetToken _photonMediumIDToken;
  const edm::EDGetToken _photonTightIDToken;
  const edm::EDGetToken _photonIsolation_Charged_Token;
  const edm::EDGetToken _photonIsolation_Neutral_Token;
  const edm::EDGetToken _photonIsolation_Photon_Token;
  const edm::EDGetToken _photonSignaIEtaIEtaToken;
  EffectiveAreas _photonEffectiveArea_ChargeHadron;
  EffectiveAreas _photonEffectiveArea_NeutralHadron;
  EffectiveAreas _photonEffectiveArea_Photons;

  edm::Handle<double> _rhohandle;
  edm::Handle<std::vector<pat::Photon> > _photonhandle;
  edm::Handle<edm::ValueMap<bool> > _photonIDLoose;
  edm::Handle<edm::ValueMap<bool> > _photonIDMedium;
  edm::Handle<edm::ValueMap<bool> > _photonIDTight;
  edm::Handle<edm::ValueMap<float> > _photonIsolation_Charged_H;
  edm::Handle<edm::ValueMap<float> > _photonIsolation_Neutral_H;
  edm::Handle<edm::ValueMap<float> > _photonIsolation_Photon_H;
  edm::Handle<edm::ValueMap<float> > _photonSigmaIEtaIEta_H;

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP */
