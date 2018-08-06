/*******************************************************************************
*
*  Filename    : bprimeKit.h
*  Description : The b' analysis kit
*  Details     : Strips the standard CMSSW data format into a standalone
*                fBaseTree ntuple
*  Authors     : May 31, 2015 - Split info mutifile format by Enoch.
*                Jul 05, 2010 - Add Electron ID by Jacky, Add PFMet by Chiyi
*                May 19, 2010 - Add Muon Iso variables, MCTag, and PF Jet  - by Dmitry Hits & Yeong-jyi Kei
*                Apr 15, 2010 - Update to CMSSW_3_5_6, fix bTag bug  - by Yeong-jyi Lei
*                Sep 24, 2009 - Updates label name, and triggerbook. Add fPhotonInfo - by Yeong-jyi Lei
*                Sep 11, 2009 - Updates with CMSSW_3_1_2 - by Kai-Feng Chen
*                May 29, 2009 - Include fGenInfo, electron classification, bug fix on vtx - by Yeong-jyi Lei
*                Mar 10, 2009 - Change the trigger table
*                Feb 20, 2009 - Protection added for missing InnerTrack() from muons, adding MC top mass, bug fix on fJetInfo.
*                Jan 03, 2009 - Updates according to CMSSW_2_2_3 release (clean 2_2_3 + PhysicsTools/PatAlgos V04-14-15)
*
*******************************************************************************/
#ifndef __BPRIMEKIT_H__
#define __BPRIMEKIT_H__

// ----- ED Analyzer requirements  ----------------------------------------------
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PFIsolation.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

#include "bpkFrameWork/bprimeKit/interface/format.h"
#include <TTree.h>
#include <map>

/*******************************************************************************
*   Forward declaration of NtuplizerBase
*******************************************************************************/
class NtuplizerBase;

/*******************************************************************************
*   Main ED analyzer class
*******************************************************************************/

class bprimeKit : public edm::EDAnalyzer
{
public:
  explicit
  bprimeKit( const edm::ParameterSet& );
  ~bprimeKit();

  /*******************************************************************************
  *   Static member functions for uniformed object formats,
  *   For implementations see bprimeKit_utils_*.cc files
  *******************************************************************************/
  static double GetMiniPFIso(
    const edm::Handle<pat::PackedCandidateCollection>& pfcands,
    const reco::Candidate*                             ptcl,
    const double&                                      r_iso_min,
    const double&                                      r_iso_max,
    const double&                                      kt_scale,
    const bool&                                        charged_only );

  static double GetMiniPFIsoRhoCorr(
    const pat::PFIsolation& iso,
    const double& pt,
    const double& rho,
    const double& effarea );

  static int GetGenMCTag( const reco::GenParticle* );

  static int GetTriggerIdx( const std::string& );

private:
  /*******************************************************************************
  *   Inherited methods
  *******************************************************************************/
  virtual void beginJob();
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void endJob();
  virtual void beginRun( const edm::Run& iRun, const edm::EventSetup& iSetup );
  virtual void endRun( const edm::Run&, const edm::EventSetup& );

  // ----- RunInfo, See bprimeKit/plugins/bprimeKit_runInfo.cc  -------------------
  void GetRunObjects( const edm::Run&, const edm::EventSetup& );
  void FillRunInfo();

  // ----- Ntuple interaction variables  --------------------------------------
  TTree* BaseTree;

  friend class NtuplizerBase;
  std::vector<NtuplizerBase*> _ntuplizerlist;


  /*******************************************************************************
  *   Run level info still handled directly by bprimeKit
  *******************************************************************************/
  TTree* RunTree;
  RunInfoBranches RunInfo;
  const edm::EDGetToken _lheruntoken;
  edm::Handle<LHERunInfoProduct> _runinfohandle;

  // Common interfacer for Triggerbooking
  static std::map<std::string,int> _hltmap;

};

#endif/* end of include guard: __BPRIMEKIT_H__ */
