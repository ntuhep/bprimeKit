/*******************************************************************************
*
*  Filename    : NtuplizerBase.hpp
*  Description : Base class for MiniAOD object Ntuplizing
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_INFONTUPLIZERBASE_HPP
#define BPKFRAMEWORK_BPRIMEKIT_INFONTUPLIZERBASE_HPP

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "bpkFrameWork/bprimeKit/interface/bprimeKit.hpp"

#include "TTree.h"

class NtuplizerBase
{
public:
  NtuplizerBase( const edm::ParameterSet& iConfig, bprimeKit* bpk ) :
    _settings( iConfig ),
    _bpkinstance( bpk )
  {}

  virtual
  ~NtuplizerBase ()
  {}

  // Common functions to use
  template<typename T>
  edm::EDGetToken
  GetToken( const std::string& tag ) const
  {
    return _bpkinstance->consumes<T>( _settings.getParameter<edm::InputTag>( tag ) );
  }

  // Pure virtual function to be overloaded
  virtual void RegisterTree( TTree* )                                       = 0;
  virtual void Analyze( const edm::Event&, const edm::EventSetup& )         = 0;

private:
  const edm::ParameterSet& _settings;
  bprimeKit*  _bpkinstance;
};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_INFONTUPLIZERBASE_HPP */
