#ifndef __OBJECTSELECTOR__
#define __OBJECTSELECTOR__

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class LepInfoBranches;
class JetInfoBranches;
class VertexInfoBranches;

//
//Define channels: electron and muon
//

static const int ELECTRON = 11;
static const int MUON     = 13;

//
//Define pass codes:
//FAIL obviously means fail
//TIGHT means it passes primary object selection, 
//LOOSE_MU/EL means it passes to be used as a veto object in mu/el channel
//
//Add other codes as necessary
//

static const int FAIL     = 0;

static const int TIGHT    = 1; //ttbar lepton+jets tight/primary level
static const int LOOSE_MU = 2; //ttbar lepton+jets loose/veto level in muon channel
static const int LOOSE_EL = 4; //ttbar lepton+jets loose/veto level in electron channel

class objectSelector {

  //
  // Lepton info
  //
 public:

  objectSelector(const edm::ParameterSet&,
		 const LepInfoBranches &lep, const JetInfoBranches &jet, const VertexInfoBranches &vtx);

  friend std::ostream& operator<<(std::ostream& s, const objectSelector& c);

  bool isGoodElectron(const int lepIdx, const int vtxIdx) const;
  bool isGoodMuon    (const int lepIdx, const int vtxIdx) const;

  bool isLooseElectron(const int lepIdx, const int vtxIdx, const int channel) const;
  bool isLooseMuon    (const int lepIdx, const int vtxIdx, const int channel) const;

  int  selectionCode (const int lepIdx, const int vtxId) const;

  int  qualityLep (const int lepIdx, const int vtxIdx) const;
  int  isolatedLep (const int lepIdx) const;
  int  fiducialLep (const int lepIdx) const;
  int  passesConversionVeto (const int lepIdx) const;

 private:

  const LepInfoBranches* _leptons;

  //cuts
  double _minElEtaCrack, _maxElEtaCrack;   

  double _maxElDZ, _maxElDxy, _maxElD0BS;
  double _minElMVA, _maxElMVA;
  double _maxElIso;
  double _minElPt, _maxElEta;
  bool   _useElID;

  int    _convNHits;       
  double _convDist,  _convDcot;

  double _muVetoPt,     _muVetoEta,     _muVetoIso;
  double _minLooseElPt, _maxLooseElEta, _maxLooseElIso;

  double _maxMuDxy, _maxMuDZ, _maxMuChi2;//_maxMuD0BS
  int    _minTrNHit, _minMuPixHit, _minMatch, _minMuHits;
  double _maxMuIso;
  double _minMuPt,   _maxMuEta;

  double _elVetoEt,  _elVetoEta,   _elVetoIso;

  bool   _usePUCorrIso;
  bool   _usePFIso;

  //
  // Jet info
  //
 public:

  bool isGoodJet(const int jetIdx) const;

  int  qualityJet  (const int jetIdx) const;
  int  fiducialJet (const int jetIdx) const;

 private:

  const JetInfoBranches* _jets;

  double _minJetPt, _maxJetEta;
  bool   _useJetID;
  int    _minNOD, _minNCH;
  double _maxCEF, _maxNHF, _maxNEF, _minCHF;
  double _jetEtaSwitch;

  //
  // Vertex info
  //
 public:

  bool isPrimaryVertex(const int vtxIdx) const;

 private:

  const VertexInfoBranches* _vertices;

  //cuts
  bool   _firstVtxOnly;
  double _minNDOF, _maxZ, _maxRho;

  bool _debug;

};

#endif
