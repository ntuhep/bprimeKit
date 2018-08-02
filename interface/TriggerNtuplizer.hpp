/*******************************************************************************
*
*  Filename    : TriggerNtuplizer.hpp
*  Description : Class for handling trigger object ntuplizing
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef BPKFRAMEWORK_BPRIMEKIT_TRIGGERNTUPLIZER
#define BPKFRAMEWORK_BPRIMEKIT_TRIGGERNTUPLIZER

#include "bpkFrameWork/bprimeKit/interface/NtuplizerBase.hpp"

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

class TriggerNtuplizer : public NtuplizerBase
{
public:
  TriggerNtuplizer ( const edm::ParameterSet&, bprimeKit* );
  virtual
  ~TriggerNtuplizer ();

  void RegisterTree( TTree* );
  void Analyze( const edm::Event&, const edm::EventSetup& );

private:
  TrgInfoBranches TrgInfo;

  const edm::EDGetToken _triggertoken;
  const edm::EDGetToken _triggerprescalestoken;
  const edm::EDGetToken _triggerobjtoken;
  const edm::EDGetToken _gendigitoken;

  edm::Handle<edm::TriggerResults> _triggerhandle;
  edm::Handle<pat::PackedTriggerPrescales> _triggerprescaleshandle;
  edm::Handle<std::vector<pat::TriggerObjectStandAlone>> _triggerobjhandle;
  edm::Handle<L1GlobalTriggerReadoutRecord> _recordhandle;

  // Helper container classes
  std::vector<std::pair<std::string,std::string>> _triggerlist;

  /*******************************************************************************
  *   Trigger parsing helper private methods
  *******************************************************************************/
  static bool TrigObjMatchPathFilter(
    const pat::TriggerObjectStandAlone&,
    const std::string& pathglob,
    std::string&       matchedpath,
    const std::string& filterglob,
    std::string&       matchedfilter
    );
  static std::string GlobToRegex( const std::string& );
};

#endif/* end of include guard: BPKFRAMEWORK_BPRIMEKIT_TRIGGERNTUPLIZER */
