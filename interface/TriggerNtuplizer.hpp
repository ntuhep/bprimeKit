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
#include "DataFormats/Common/interface/TriggerResults.h"


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
  const edm::EDGetToken _triggerobjtoken;

  edm::Handle<edm::TriggerResults> _triggerhandle;
  edm::Handle<std::vector<pat::TriggerObjectStandAlone>> _triggerobjhandle;

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
