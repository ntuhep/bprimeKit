/*******************************************************************************
*
*  Filename    : JetNtuplizer.hpp
*  Description : Settings for getting a ntuplizing a single get collection
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_JETNTUPLIZER_HPP
#define BPKFRAMEWORK_BPRIMEKIT_JETNTUPLIZER_HPP

#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"
#include "bpkFrameWork/bprimeKit/interface/format.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "TLorentzVector.h"

class JetNtuplizer : public NtuplizerBase
{
public:
  JetNtuplizer ( const edm::ParameterSet& iConfig, bprimeKit* );
  virtual
  ~JetNtuplizer ();

  virtual void RegisterTree( TTree* );
  virtual void Analyze( const edm::Event&, const edm::EventSetup& );

private:
  JetInfoBranches JetInfo;

  const std::string _jetname;
  const std::string _jettype;
  const std::string _jecversion;
  const edm::EDGetToken _rhotoken;
  const edm::EDGetToken _vtxtoken;
  const edm::EDGetToken _jettoken;
  const edm::EDGetToken _muontoken;
  const edm::EDGetToken _subjettoken;

  edm::Handle<double> _rhohandle;
  edm::Handle<std::vector<reco::Vertex> > _vtxhandle;
  edm::Handle<std::vector<pat::Jet> > _jethandle;
  edm::Handle<std::vector<pat::Muon> > _muonhandle;
  edm::Handle<std::vector<pat::Jet> > _subjethandle;
  FactorizedJetCorrector* _jetcorrector;
  JetCorrectionUncertainty* _jetunc;

  std::vector<std::vector<pat::Muon>::const_iterator> _selectedmuons;

  /*******************************************************************************
  *   Jet type parsing
  *******************************************************************************/
  bool        IsAK4() const;
  bool        IsWideJet() const;
  std::string UserFloatName() const;
  std::string UserFloatPrefix() const;

  /*******************************************************************************
  *   Fat jet related functions
  *******************************************************************************/

  std::vector<pat::Jet>::const_iterator GetSubjetBunch( const std::vector<pat::Jet>::const_iterator& mainjet );

  /*******************************************************************************
  *   Jet cleaning helper functions
  *******************************************************************************/
  bool HasOverLap(
    const std::vector<pat::Muon>::const_iterator&,
    const edm::Ptr<reco::Candidate>& ) const;
  bool           IsSelectedMuon( const std::vector<pat::Muon>::const_iterator& ) const;
  TLorentzVector CleanAK4Jet( const std::vector<pat::Jet>::const_iterator& );
  TLorentzVector CleanAK8Jet( const std::vector<pat::Jet>::const_iterator& );

};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_JETNTUPLIZER_HPP  */
