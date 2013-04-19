#ifndef __EVENTSELECTOR__
#define __EVENTSELECTOR__

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class EvtInfoBranches;
class LepInfoBranches;
class JetInfoBranches;
class VertexInfoBranches;
class objectSelector;

class eventSelector {

  //
  // Lepton info
  //
 public:

  eventSelector(const edm::ParameterSet&,
		const EvtInfoBranches &evt, const LepInfoBranches &lep, const JetInfoBranches &jet, const VertexInfoBranches &vtx);

  void setChannel(int ch);
  bool passes();
  int passCode();

  //primary event selection
  bool passesTrigger();
  int  primaryVertex();
  int  nGoodElectrons();
  int  nGoodMuons();
  int  nLooseElectrons();
  int  nLooseMuons();
  int  nGoodJets();
  int  nCleanMuons();
  int  nCleanJets();
  int  nCleanElectrons();

  //additional cuts
  bool jetsPassTightPt();
  bool passesMinMET();

  //get information
  int  getPrimaryLepton();
  std::vector<int> getGoodJets();
  void printEventStatus();
  void reset();
  std::vector<std::string> getCutLevels();

 private:

  const EvtInfoBranches* _event;
  const LepInfoBranches* _leptons;
  const JetInfoBranches* _jets;
  const VertexInfoBranches* _vertices;

  edm::ParameterSet _oParameters;
  objectSelector* _oSelector;

  int _primaryVertex;
  int _primaryLepton;
  std::vector<int> _goodJets;
  std::vector<int> _goodElectrons;
  std::vector<int> _looseElectrons;
  std::vector<int> _goodMuons;
  std::vector<int> _looseMuons;

  double _minDRmujet;
  double _minDRjetel;
  double _cleanDRjetmu;

  std::vector<std::string> _muonTriggers;
  std::vector<int>         _muTbeginRange;
  std::vector<std::string> _electronTriggers;
  std::vector<int>         _elTbeginRange;

  std::vector<std::string> _cutLevels;

  int  _minNjets;
  bool _checkJetPtOrder;

  //for additional cuts
  std::vector<double>      _minJetPts;//must be greater than good jet criteria to have any effect
  double                   _minMET;

  bool _debug;
  int _channel;
  int _passLevel;

};

#endif
