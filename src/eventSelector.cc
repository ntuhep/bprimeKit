#include <iostream>
#include <boost/algorithm/string.hpp>
#include "TMath.h"

#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/objectSelector.h"
#include "MyAna/bprimeKit/interface/eventSelector.h"
#include "MyAna/bprimeKit/interface/TriggerBooking.h"
#include "MyAna/bprimeKit/interface/bpkUtils.h"

double dPhi(double p1,double p2)
{
  double dp = p1 - p2;
  if      (fabs(dp+TMath::Pi()*2.) < fabs(dp)) dp += TMath::Pi()*2.;
  else if (fabs(dp-TMath::Pi()*2.) < fabs(dp)) dp -= TMath::Pi()*2.;

  return fabs(dp);
}

double dR(double e1, double e2, double p1, double p2)
{
  return sqrt(pow(e1-e2,2)+pow(dPhi(p1,p2),2));
}

using namespace std;

eventSelector::eventSelector(const edm::ParameterSet& iConfig,
			       const EvtInfoBranches &evt, const LepInfoBranches &lep, const JetInfoBranches &jet, const VertexInfoBranches &vtx)
  : _event(&evt),
    _leptons(&lep),
    _jets(&jet),
    _vertices(&vtx),
    _oParameters(iConfig.getParameter<edm::ParameterSet>("ObjectParameters")),
    _minDRmujet(iConfig.getUntrackedParameter<double>("MinDRmuonJet",0.3)),
    _minDRjetel(iConfig.getUntrackedParameter<double>("MinDRjetElectron",0.3)),
    _cleanDRjetmu(iConfig.getUntrackedParameter<double>("CleanDRjetMuon",0.1)),
    _muonTriggers(iConfig.getUntrackedParameter< std::vector<std::string> >("MuonTriggers")),
    _muTbeginRange(iConfig.getUntrackedParameter< std::vector<int> >("MuTrigLowerRun")),
    _electronTriggers(iConfig.getUntrackedParameter<std::vector<std::string> >("ElectronTriggers")),
    _elTbeginRange(iConfig.getUntrackedParameter< std::vector<int> >("ElTrigLowerRun")),
    _cutLevels(iConfig.getUntrackedParameter< std::vector<std::string> >("CutLevels")),
    _minNjets(iConfig.getUntrackedParameter<int>("MinNJets",4)),
    _checkJetPtOrder(iConfig.getUntrackedParameter<bool>("CheckJetPtOrder",false)),
    _minJetPts(iConfig.getUntrackedParameter< std::vector<double> >("MinJetPTs")),
    _minMET(iConfig.getUntrackedParameter<double>("MinMET",0.)),
    _debug(iConfig.getUntrackedParameter<bool>("Debug",false))
{
  _oSelector = new objectSelector(_oParameters,*_leptons,*_jets,*_vertices);

  _primaryVertex = -1;
  _primaryLepton = -1;
  _goodJets.clear();
  _goodElectrons.clear();
  _looseElectrons.clear();
  _goodMuons.clear();
  _looseMuons.clear();

  _channel = MUON;
  _passLevel = -1;//start;

  if(_muonTriggers.size() != _muTbeginRange.size()) {
    cout << "WARNING: muon trigger list size does not match run range list size.  Are you sure this is right?\n";
    while (_muonTriggers.size() > _muTbeginRange.size()) {
      _muTbeginRange.push_back(*(_muTbeginRange.end()-1));
    }
  } 
  if(_electronTriggers.size() != _elTbeginRange.size()) {
    cout << "WARNING: electron trigger list size does not match run range list size.  Are you sure this is right?\n";
    while (_electronTriggers.size() > _elTbeginRange.size()) {
      _elTbeginRange.push_back(*(_elTbeginRange.end()-1));
    }
  }

  if(_debug) {
    cout << "Muon Triggers : \n";
    for(unsigned i=0; i<_muonTriggers.size(); i++) cout << "  from " << _muTbeginRange[i] << ": " << _muonTriggers[i];
    cout << endl;
    cout << "Electron Triggers : \n";
    for(unsigned i=0; i<_electronTriggers.size(); i++) cout << "  from " << _elTbeginRange[i] << ": " << _electronTriggers[i];
    cout << endl;
  }
}

void eventSelector::setChannel(int ch)
{
  if(ELECTRON==ch) _channel = ELECTRON;
  else             _channel = MUON;
  if(_debug) cout << "Channel set to " << _channel << endl;
}

bool eventSelector::passes() {
  bool passes = false;
  int code = passCode();
  if(code >= (int)_cutLevels.size()-1) passes=true;
  if(_debug) cout << "Event passcode=" << code 
		  << " of " << _cutLevels.size()-1
		  << ". " << (passes ? "Passes" : "Fails") << endl;
  return passes;
}

int eventSelector::passCode() {
  if(_cutLevels.size()==0) return 0;

  primaryVertex();
  int njets = nGoodJets();
  int nlep = 0;
  int nlooseelec = 0;
  int nloosemuon = 0;
  bool passesConvVeto = true;
  if(ELECTRON==_channel) {
    nlep = nGoodElectrons();
    njets = nCleanJets();
    nlep = nCleanElectrons();
    if(nlep>0) _primaryLepton = _goodElectrons[0];
    passesConvVeto = _oSelector->passesConversionVeto(_primaryLepton);
    nlooseelec = nLooseElectrons();
    nloosemuon = nLooseMuons() + nGoodMuons();//loose and good are exclusive
  }
  else {//muon channel
    nlep = nGoodMuons();
    njets = nCleanJets();
    nlep = nCleanMuons();
    if(nlep>0) _primaryLepton = _goodMuons[0];
    nlooseelec = nLooseElectrons()+nGoodElectrons();//loose and good are exclusive
    nloosemuon = nLooseMuons();
  }

  int level = -1;
  for(int i=0; i<(int)_cutLevels.size(); i++) {
    if(level < i-1) break;
    if(boost::iequals(_cutLevels[i],"trigger"))             { if(passesTrigger())    level=i; }
    else if(boost::iequals(_cutLevels[i],"vertex"))         { if(primaryVertex()>=0) level=i; }
    else if(boost::iequals(_cutLevels[i],"lepton"))         { if(nlep==1)            level=i; }
    else if(boost::iequals(_cutLevels[i],"muonveto"))       { if(nloosemuon==0)      level=i; }
    else if(boost::iequals(_cutLevels[i],"electronveto"))   { if(nlooseelec==0)      level=i; }
    else if(boost::iequals(_cutLevels[i],"conversionveto")) { if(passesConvVeto)     level=i; }
    else if(boost::iequals(_cutLevels[i],"minjets"))        { if(njets>=_minNjets)   level=i; }
    else if(boost::iequals(_cutLevels[i],"tightjetpt"))     { if(jetsPassTightPt())  level=i; }
    else if(boost::iequals(_cutLevels[i],"minmet"))         { if(passesMinMET())     level=i; }

    else level=i;//unknown cut, let it pass
    if(_debug) cout << "Cut level: " << _cutLevels[i] << " code " << level << endl;
  }

  return level;
}

bool eventSelector::passesTrigger()
{
  bool pass = false;

  vector<string> trigs;
  vector<int>    begin;
  if(ELECTRON==_channel) {
    trigs = _electronTriggers;
    begin = _elTbeginRange;
  }
  else {
    trigs = _muonTriggers;
    begin = _muTbeginRange;
  }
  if(trigs.empty()) return true;

  for(unsigned i=0; i<trigs.size(); i++) {
    if(pass) break;
    if(_event->RunNo >= begin[i] && ((i+1)>=begin.size() || _event->RunNo < begin[i+1])) {//right run range, look for trigger
      for(int j=0; j<N_TRIGGER_BOOKINGS; j++) {
	if(pass) break;
	string s = TriggerBooking[j];
	if(string::npos != s.find(trigs[i])) {//found trigger
	  if(_event->TrgBook[j] == 1) {
	    pass = true;
	    if(_debug) cout << "Passed " << TriggerBooking[j] << endl;
	  }
	}
      }
    }
  }

  if(_debug) cout << "Triggered? " << pass << endl;
  return pass;
}
int eventSelector::primaryVertex()
{
  _primaryVertex = -1;
  for(int v=0; v<_vertices->Size; v++) {
    if(_primaryVertex>=0) break;
    if(_oSelector->isPrimaryVertex(v)) _primaryVertex = v;
  }

  if(_debug) cout << "Index of primary vertex is " << _primaryVertex << endl;
  return _primaryVertex;
}
int eventSelector::nGoodElectrons()
{
  _goodElectrons.clear();
  if(MUON==_channel) return 0;
  for(int l=0; l<_leptons->Size; l++) {
    if(_oSelector->isGoodElectron(l,_primaryVertex)) _goodElectrons.push_back(l);
  }

  if(_debug) {
    cout << "Have " << _goodElectrons.size() << " good electrons with indeces: ";
    for(unsigned i=0; i<_goodElectrons.size(); i++)
      cout << _goodElectrons[i] << ", ";
    cout << endl;
  }
  return _goodElectrons.size();
}
int eventSelector::nGoodMuons()
{
  _goodMuons.clear();
  if(ELECTRON==_channel) return 0;
  for(int l=0; l<_leptons->Size; l++) {
    if(_oSelector->isGoodMuon(l,_primaryVertex)) _goodMuons.push_back(l);
  }

  if(_debug) {
    cout << "Have " << _goodMuons.size() << " good muons with indeces: ";
    for(unsigned i=0; i<_goodMuons.size(); i++)
      cout << _goodMuons[i] << ", ";
    cout << endl;
  }
  return _goodMuons.size();
}
int eventSelector::nLooseElectrons()
{
  _looseElectrons.clear();
  for(int l=0; l<_leptons->Size; l++) {
    if(find(_goodElectrons.begin(),_goodElectrons.end(),l)!=_goodElectrons.end()) continue;//don't count if is goodElectron
    if(_oSelector->isLooseElectron(l,_primaryVertex,_channel)) _looseElectrons.push_back(l);
  }

  if(_debug) {
    cout << "Have " << _looseElectrons.size() << " loose electrons with indeces: ";
    for(unsigned i=0; i<_looseElectrons.size(); i++)
      cout << _looseElectrons[i] << ", ";
    cout << endl;
  }
  return _looseElectrons.size();
}
int eventSelector::nLooseMuons()
{
  _looseMuons.clear();
  for(int l=0; l<_leptons->Size; l++) {
    if(find(_goodMuons.begin(),_goodMuons.end(),l)!=_goodMuons.end()) continue;//don't count if is goodMuon
    if(_oSelector->isLooseMuon(l,_primaryVertex,_channel)) _looseMuons.push_back(l);
  }

  if(_debug) {
    cout << "Have " << _looseMuons.size() << " loose muons with indeces: ";
    for(unsigned i=0; i<_looseMuons.size(); i++)
      cout << _looseMuons[i] << ", ";
    cout << endl;
  }
  return _looseMuons.size();
}

int eventSelector::nGoodJets()
{
  _goodJets.clear();
  for(int j=0; j<_jets->Size; j++) {
    if(_oSelector->isGoodJet(j)) _goodJets.push_back(j);
  }

  if(_checkJetPtOrder) ::ptOrderJets(*_jets,_goodJets);

  if(_debug) {
    cout << "Have " << _goodJets.size() << " good jets with indeces: ";
    double lastpt=999999.;
    for(unsigned i=0; i<_goodJets.size(); i++) {
      cout << _goodJets[i] << ", ";
      if(_checkJetPtOrder) {
         double thispt = _jets->Pt[_goodJets[i]];
         if(lastpt < thispt) cout << "\nERROR: This jet pt of " << thispt << " is greater than last jet pt of " << lastpt << endl;
         lastpt=thispt;
      }
    }
    cout << endl;
  }
  return _goodJets.size();
}

int eventSelector::nCleanMuons()
{
  if(_debug) cout << "nCleanMuons:\n";
  if(ELECTRON==_channel) return 0;
  std::vector<int>::iterator mu;
  std::vector<int>::iterator jet = _goodJets.begin();
  for(; jet!=_goodJets.end(); ++jet) {
    for(mu=_goodMuons.begin();mu!=_goodMuons.end(); ++mu) {
      double dr = dR(_leptons->Eta[*mu],_jets->Eta[*jet],_leptons->Phi[*mu],_jets->Phi[*jet]);
      if(_debug) {
	cout << "\tjet " << *jet << "(" << _jets->Eta[*jet] << "," << _jets->Phi[*jet] << ") ";
	cout << "muon " << *mu << "(" << _leptons->Eta[*mu] << "," << _leptons->Phi[*mu] << ") ";
	cout << "dR = " << dr << endl;
      }
      if(dr < _minDRmujet) {
	mu = _goodMuons.erase(mu)-1;
	if(_debug) cout << "Overlap with dR less than minimum of " << _minDRmujet << ". Removed muon.\n";
      }
    }
  }

  if(_debug) {
    cout << "Have " << _goodMuons.size() << " clean muons with indeces: ";
    for(unsigned i=0; i<_goodMuons.size(); i++)
      cout << _goodMuons[i] << ", ";
    cout << endl;
  }
  return _goodMuons.size();
}

int eventSelector::nCleanJets()
{
  if(_debug) cout << "nCleanJets:\n";
  if(ELECTRON==_channel) {
    std::vector<int>::iterator el = _goodElectrons.begin();
    std::vector<int>::iterator jet;
    for(; el!=_goodElectrons.end(); ++el) {
      for(jet=_goodJets.begin();jet!=_goodJets.end(); ++jet) {
	double dr = dR(_leptons->Eta[*el],_jets->Eta[*jet],_leptons->Phi[*el],_jets->Phi[*jet]);
	if(_debug) {
	  cout << "\telectron " << *el << "(" << _leptons->Eta[*el] << "," << _leptons->Phi[*el] << ") ";
	  cout << "jet " << *jet << "(" << _jets->Eta[*jet] << "," << _jets->Phi[*jet] << ") ";
	  cout << "dR = " << dr << endl;
	}
	if(dr < _minDRjetel) {
	  jet = _goodJets.erase(jet)-1;
	  if(_debug) cout << "Overlap with dR less than minimum of " << _minDRjetel << ". Removed jet.\n";
	}
      }
    }
  }
  else if(_cleanDRjetmu > 0) { //muon channel and need to do jet cleaning
    std::vector<int>::iterator mu = _goodMuons.begin();
    std::vector<int>::iterator jet;
    for(; mu!=_goodMuons.end(); ++mu) {
      for(jet=_goodJets.begin();jet!=_goodJets.end(); ++jet) {
	double dr = dR(_leptons->Eta[*mu],_jets->Eta[*jet],_leptons->Phi[*mu],_jets->Phi[*jet]);
	if(_debug) {
	  cout << "\tmuon " << *mu << "(" << _leptons->Eta[*mu] << "," << _leptons->Phi[*mu] << ") ";
	  cout << "jet " << *jet << "(" << _jets->Eta[*jet] << "," << _jets->Phi[*jet] << ") ";
	  cout << "dR = " << dr << endl;
	}
	if(dr < _cleanDRjetmu) {
	  jet = _goodJets.erase(jet)-1;
	if(_debug) cout << "Overlap with dR less than minimum of " << _cleanDRjetmu << ". Cleaned jet.\n";
	}
      }
    }
  }

  if(_debug) {
    cout << "Have " << _goodJets.size() << " clean jets with indeces: ";
    for(unsigned i=0; i<_goodJets.size(); i++)
      cout << _goodJets[i] << ", ";
    cout << endl;
  }
  return _goodJets.size();
}

int eventSelector::nCleanElectrons() 
{
  //do nothing for now, just analogous to nCleanMuons and nCleanJets

  if(_debug) {
    cout << "Have " << _goodElectrons.size() << " clean electrons with indeces: ";
    for(unsigned i=0; i<_goodElectrons.size(); i++)
      cout << _goodElectrons[i] << ", ";
    cout << endl;
  }
  return _goodElectrons.size();
}

bool eventSelector::jetsPassTightPt()
{
  if(_goodJets.size() < _minJetPts.size()) return false;
  for(int j=0; j<(int)_minJetPts.size(); j++) {
    if(_jets->Pt[_goodJets[j]] < _minJetPts[j]) return false;
  }
  return true;
}

bool eventSelector::passesMinMET()
{
  if(_event->PFMET < _minMET) return false;
  return true;
}

int eventSelector::getPrimaryLepton() {
  return _primaryLepton;
}

std::vector<int> eventSelector::getGoodJets() {
  return _goodJets;
}

void eventSelector::printEventStatus() {
  cout << "Run " << _event->RunNo << " event " << _event->EvtNo << " status:\n";
  cout << "\tPrimary vertex " << _primaryVertex;
  cout << "\n\tPrimary lepton " << _primaryLepton;
  
  cout << "\n\t" << _goodMuons.size() << " good muons:";
  for(unsigned i=0; i<_goodMuons.size(); i++)
    cout << "  " << _goodMuons[i];

  cout << "\n\t" << _looseMuons.size() << " loose muons:";
  for(unsigned i=0; i<_looseMuons.size(); i++)
    cout << "  " << _looseMuons[i];

  cout << "\n\t" << _goodElectrons.size() << " good electrons:";
  for(unsigned i=0; i<_goodElectrons.size(); i++)
    cout << "  " << _goodElectrons[i];

  cout << "\n\t" << _looseElectrons.size() << " loose electrons:";
  for(unsigned i=0; i<_looseElectrons.size(); i++)
    cout << "  " << _looseElectrons[i];

  cout << "\n\t" << _goodJets.size() << " good jets:";
  for(unsigned i=0; i<_goodJets.size(); i++)
    cout << "  " << _goodJets[i];

  cout << "\n\tPass level: " << (_passLevel>-1 && _cutLevels.size()>0 ? _cutLevels[_passLevel] : "none") << endl;
}

void eventSelector::reset() {
  _primaryVertex = -1;
  _primaryLepton = -1;
  _goodJets.clear();
  _goodElectrons.clear();
  _looseElectrons.clear();
  _goodMuons.clear();
  _looseMuons.clear();

  _passLevel = -1;
}

std::vector<std::string> eventSelector::getCutLevels() {
  return _cutLevels;
}
