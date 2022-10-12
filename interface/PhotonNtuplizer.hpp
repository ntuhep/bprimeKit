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
#include "CommonTools/Egamma/interface/EffectiveAreas.h"

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
  const std::string _photonID_loosemap;
  const std::string _photonID_mediummap;
  const std::string _photonID_tightmap;
  const std::string _photonID_mva;
  EffectiveAreas _photonEffectiveArea_ChargeHadron;
  EffectiveAreas _photonEffectiveArea_NeutralHadron;
  EffectiveAreas _photonEffectiveArea_Photons;

  edm::Handle<double> _rhohandle;
  edm::Handle<std::vector<pat::Photon> > _photonhandle;

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_PHOTONNTUPLIZER_HPP */
