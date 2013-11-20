#include <iostream>
#include "TMath.h"

#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/objectSelector.h"

objectSelector::objectSelector(const edm::ParameterSet& iConfig,
				     const LepInfoBranches &lep, const JetInfoBranches &jet, const VertexInfoBranches &vtx)
  : _leptons(&lep),
    _minElEtaCrack(iConfig.getUntrackedParameter<double>("ElectronEtaCrackMinimum",1.4442)),
    _maxElEtaCrack(iConfig.getUntrackedParameter<double>("ElectronEtaCrackMaximum",1.5660)),
    _maxElDZ      (iConfig.getUntrackedParameter<double>("ElectronMaximumDeltaZ",1.0)),
    _maxElDxy     (iConfig.getUntrackedParameter<double>("ElectronMaximumDxy",0.2)),
//     _maxElD0BS    (iConfig.getUntrackedParameter<double>("ElectronMaximumD0BS",0.2)),
    _minElMVA     (iConfig.getUntrackedParameter<double>("ElectronMinimumMVA",0.)),
    _maxElMVA     (iConfig.getUntrackedParameter<double>("ElectronMaximumMVA",1.)),
    _maxElIso     (iConfig.getUntrackedParameter<double>("ElectronMaximumIsolation",0.1)),
    _minElPt      (iConfig.getUntrackedParameter<double>("ElectronMinimumPT",30.0)),
    _maxElEta     (iConfig.getUntrackedParameter<double>("ElectronMaximumEta",2.5)),
    _useElID      (iConfig.getUntrackedParameter<bool>  ("UseElectronID",true)),
    _convNHits    (iConfig.getUntrackedParameter<int>   ("ElectronMaximumMissingInnerLayers",0)),
    _convDist     (iConfig.getUntrackedParameter<double>("ElectronConversionDist",0.02)),
    _convDcot     (iConfig.getUntrackedParameter<double>("ElectronConversionDcot",0.02)),
    _muVetoPt     (iConfig.getUntrackedParameter<double>("MuonVetoPT",10.)),
    _muVetoEta    (iConfig.getUntrackedParameter<double>("MuonVetoEta",2.5)),
    _muVetoIso    (iConfig.getUntrackedParameter<double>("MuonVetoIsolation",0.2)),
    _minLooseElPt (iConfig.getUntrackedParameter<double>("LooseElectronMinimumET",20.)),
    _maxLooseElEta(iConfig.getUntrackedParameter<double>("LooseElectronMaximumEta",2.5)),
    _maxLooseElIso(iConfig.getUntrackedParameter<double>("LooseElectronMaximumIsolation",1.0)),
    _maxMuDxy     (iConfig.getUntrackedParameter<double>("MuonMaximumDxy",0.2)),
    //_maxMuD0BS    (iConfig.getUntrackedParameter<double>("MuonMaximumD0BS",0.2)),
    _maxMuDZ      (iConfig.getUntrackedParameter<double>("MuonMaximumDeltaZ",1.)),
    _maxMuChi2    (iConfig.getUntrackedParameter<double>("MuonMaximumNormalizedChi2",10.)),
    _minTrNHit    (iConfig.getUntrackedParameter<int>   ("MuonMinimumTrackerHits",10)),
    _minMuPixHit  (iConfig.getUntrackedParameter<int>   ("MuonMinimumPixelHits",1)),
    _minMatch     (iConfig.getUntrackedParameter<int>   ("MuonMinimumMatched",1)),
    _minMuHits    (iConfig.getUntrackedParameter<int>   ("MuonMinimumMuHits",0)),
    _maxMuIso     (iConfig.getUntrackedParameter<double>("MuonMaximumIsolation",0.1)),
    _minMuPt      (iConfig.getUntrackedParameter<double>("MuonMinimumPT",20.)),
    _maxMuEta     (iConfig.getUntrackedParameter<double>("MuonMaximumEta",2.1)),
    _elVetoEt     (iConfig.getUntrackedParameter<double>("ElectronVetoET",15.)),
    _elVetoEta    (iConfig.getUntrackedParameter<double>("ElectronVetoEta",2.5)),
    _elVetoIso    (iConfig.getUntrackedParameter<double>("ElectronVetoIsolation",0.2)),
    _usePUCorrIso (iConfig.getUntrackedParameter<bool>  ("UsePUCorrectedIsolation",false)),
    _usePFIso     (iConfig.getUntrackedParameter<bool>  ("UsePFIsolation",true)),
    _jets(&jet),
    _minJetPt     (iConfig.getUntrackedParameter<double>("JetMinimumPT",30.)),
    _maxJetEta    (iConfig.getUntrackedParameter<double>("JetMaximumEta",2.4)),
    _useJetID     (iConfig.getUntrackedParameter<bool>  ("JetUseID",true)),
    _minNOD       (iConfig.getUntrackedParameter<int>   ("JetMinimumConstituents",1)),
    _minNCH       (iConfig.getUntrackedParameter<int>   ("JetMinimumChargedMultiplicity",0)),
    _maxCEF       (iConfig.getUntrackedParameter<double>("JetMaximumCEF",0.99)),
    _maxNHF       (iConfig.getUntrackedParameter<double>("JetMaximumNHF",0.99)),
    _maxNEF       (iConfig.getUntrackedParameter<double>("JetMaximumNEF",0.99)),
    _minCHF       (iConfig.getUntrackedParameter<double>("JetMinimumCHF",0.)),
    _jetEtaSwitch (iConfig.getUntrackedParameter<double>("JetEtaSwitch",2.4)),
    _vertices(&vtx),
    _firstVtxOnly (iConfig.getUntrackedParameter<bool>  ("FirstVertexOnly",false)),
    _minNDOF      (iConfig.getUntrackedParameter<double>("VertexMinimumNDOF",4.)),
    _maxZ         (iConfig.getUntrackedParameter<double>("VertexMaximumZ",24.)),
    _maxRho       (iConfig.getUntrackedParameter<double>("VertexMaximumRho",2.)),
    _debug        (iConfig.getUntrackedParameter<bool>("Debug",false))
{
}


bool objectSelector::isGoodElectron(const int lepIdx, const int vtxIdx) const
{
  if(_debug) printf("isGoodElectron: lepIdx %i, vtxIdx %i type %i\n", lepIdx,vtxIdx,_leptons->LeptonType[lepIdx]);
  if(ELECTRON != _leptons->LeptonType[lepIdx]) return false; //can't be good electron if not an electron
  if(!(qualityLep(lepIdx,vtxIdx) &TIGHT)) return false;
  if(!(isolatedLep(lepIdx) &TIGHT)) return false;
  if(!(fiducialLep(lepIdx) &TIGHT)) return false;
  return true;
}

bool objectSelector::isGoodMuon(const int lepIdx, const int vtxIdx) const
{
  if(_debug) printf("isGoodMuon: lepIdx %i, vtxIdx %i type %i\n", lepIdx,vtxIdx,_leptons->LeptonType[lepIdx]);
  if(MUON != _leptons->LeptonType[lepIdx]) return false; //can't be good muon if not a muon
  if(!(qualityLep(lepIdx,vtxIdx) &TIGHT)) return false;
  if(!(isolatedLep(lepIdx) &TIGHT)) return false;
  if(!(fiducialLep(lepIdx) &TIGHT)) return false;
  return true;
}

bool objectSelector::isLooseElectron(const int lepIdx, const int vtxIdx, int channel) const
{
  if(_debug) printf("isLooseElectron: lepIdx %i channel %i\n",lepIdx,channel);
  if(ELECTRON != _leptons->LeptonType[lepIdx]) return false; //can't be loose electron if not an electron
  int q = qualityLep(lepIdx,vtxIdx);
  int i = isolatedLep(lepIdx);
  int f = fiducialLep(lepIdx);
  if(ELECTRON==channel) {
    if(!(q&LOOSE_EL)) return false;
    if(!(i&LOOSE_EL)) return false;
    if(!(f&LOOSE_EL)) return false;
  }
  else {
    if(!(q&LOOSE_MU)) return false;
    if(!(i&LOOSE_MU)) return false;
    if(!(f&LOOSE_MU)) return false;
  }
  return true;
}

bool objectSelector::isLooseMuon(const int lepIdx, const int vtxIdx, int channel) const
{
  if(_debug) printf("isLooseMuon: lepIdx %i channel %i\n",lepIdx,channel);
  if(MUON != _leptons->LeptonType[lepIdx]) return false;
  int q = qualityLep(lepIdx,vtxIdx);
  int i = isolatedLep(lepIdx);
  int f = fiducialLep(lepIdx);
  if(ELECTRON==channel) {
    if(!(q&LOOSE_EL)) return false;
    if(!(i&LOOSE_EL)) return false;
    if(!(f&LOOSE_EL)) return false;
  }
  else {
    if(!(q&LOOSE_MU)) return false;
    if(!(i&LOOSE_MU)) return false;
    if(!(f&LOOSE_MU)) return false;
  }
  return true;
}

int objectSelector::selectionCode(const int lepIdx, const int vtxIdx) const
{
  int code = qualityLep(lepIdx,vtxIdx);
  code    &= isolatedLep(lepIdx);
  code    &= fiducialLep(lepIdx);

  return code;
}

int objectSelector::qualityLep(const int lepIdx, const int vtxIdx) const
{
  int  code  = FAIL;
  bool novtx = false;
  if(lepIdx < 0 || lepIdx > _leptons->Size) {
    std::cout << "ERROR: Invalid lepton index.\n";
    return code;
  }
  if(vtxIdx < 0 || vtxIdx > _vertices->Size) {
    std::cout << "WARNING: Invalid vertex index.  Can't check lepton against vertex.\n";
    novtx = true;
    //return code;
  }

  if(_debug) std::cout << "qualityLep: lepIdx " << lepIdx << " vtxIdx " << vtxIdx << std::endl;

  if(ELECTRON == _leptons->LeptonType[lepIdx]) {
    if(fabs(_leptons->ElTrackDxy_PV[lepIdx]) < _maxElDxy)
      if(!_leptons->ElhasConv[lepIdx])
	if(_leptons->EgammaMVATrig[lepIdx] >= _minElMVA && _leptons->EgammaMVATrig[lepIdx] <= _maxElMVA)
	  if(!_useElID || _leptons->EgammaCutBasedEleIdTIGHT[lepIdx])
	    code |= TIGHT;

    if(_leptons->EgammaMVATrig[lepIdx] >= _minElMVA && _leptons->EgammaMVATrig[lepIdx] <= _maxElMVA)
      if(!_useElID || _leptons->EgammaCutBasedEleIdVETO[lepIdx]) {
	code |= LOOSE_EL;
	code |= LOOSE_MU;
      }

//    if(_debug) std::cout << "\telectron: dz=" << _leptons->vertexZ[lepIdx]-(novtx ? 0.:_vertices->z[vtxIdx]) << " d0=" << _leptons->ElTrackDxy_BS[lepIdx] << " simpleEleId70,95=" << (((int)_leptons->simpleEleId70cIso[lepIdx]) & 0x1) << "," << (((int)_leptons->simpleEleId95cIso[lepIdx]) & 0x1) << " code= " << code << std::endl;
  }

  if(MUON == _leptons->LeptonType[lepIdx]) {
    if(!(0 == (_leptons->MuType[lepIdx]&0x02)))                 //global muon
      //if(!(0 == (_leptons->MuType[lepIdx]&0x04)))               //tracker muon
      if(!(0 == (_leptons->MuType[lepIdx]&0x20)))               //PF muon
	if(_leptons->MuGlobalNormalizedChi2[lepIdx] < _maxMuChi2)
	  if(_leptons->MuNTrackLayersWMeasurement[lepIdx] > _minTrNHit)
	    if(_leptons->MuNMuonhits[lepIdx] > _minMuHits)
	      //if(_leptons->MuInnerTrackNHits[lepIdx] > _minTrNHit)
	      if(fabs(_leptons->MuInnerTrackDxy_PV[lepIdx]) < _maxMuDxy)    
		  //if(fabs(_leptons->MuInnerTrackDxy_BS[lepIdx]) < _maxMuD0BS)    
		  if(novtx || (fabs(_leptons->vertexZ[lepIdx]-_vertices->z[vtxIdx]) < _maxMuDZ))
		    if(_leptons->MuNPixelLayers[lepIdx] > _minMuPixHit) 
		      //if(_leptons->MuNPixelLayersWMeasurement[lepIdx] >= _minMuPixHit) 
		      if(_leptons->MuNMatchedStations[lepIdx] > _minMatch)
			//if(_leptons->MuNChambersMatchesSegment[lepIdx] > _minMatch)
			//if(_leptons->MuIDGlobalMuonPromptTight[lepIdx])
			code |= TIGHT;

    if(!(0 == (_leptons->MuType[lepIdx]&0x20)))               //PF muon
    if(!(0 == (_leptons->MuType[lepIdx]&0x02)) || !(0 == (_leptons->MuType[lepIdx]&0x04))) {  //global muon or tracker muon
      code |= LOOSE_MU;
      code |= LOOSE_EL;
    }

    if(_debug) printf("\tmuon: type=%i chi2=%2.3f muhits=%i tkhits=%i d0=%2.3f dz=%2.3f pixlayers=%i matches=%i code=%i\n",_leptons->MuType[lepIdx],_leptons->MuGlobalNormalizedChi2[lepIdx],_leptons->MuNMuonhits[lepIdx],_leptons->MuInnerTrackNHits[lepIdx],_leptons->MuInnerTrackDxy_BS[lepIdx],_leptons->vertexZ[lepIdx]-(novtx ? 0.:_vertices->z[vtxIdx]),_leptons->MuNPixelLayersWMeasurement[lepIdx],_leptons->MuNChambersMatchesSegment[lepIdx],code);
  }

  return code;
}

int objectSelector::isolatedLep(const int lepIdx) const
{
  int code = FAIL;
  if(lepIdx < 0 || lepIdx > _leptons->Size) {
    std::cout << "WARNING: Invalid lepton index\n";
    return code;
  }

  if(_debug) printf("isolatedLep: lepIdx %i\n",lepIdx);

  if(ELECTRON == _leptons->LeptonType[lepIdx]) {
    double relIso = 999.;
    if(_leptons->Pt[lepIdx] > 0.) {
      relIso = _usePFIso ?
	//( chIso03 + max(0.0, nhIso03 + phIso03 - rhoIso*AEff03) )/ electron.pt();
	_leptons->IsoRhoCorrR03[lepIdx]/_leptons->Pt[lepIdx] :
	(_leptons->TrackIso[lepIdx]+_leptons->EcalIso[lepIdx]+_leptons->HcalIso[lepIdx])/_leptons->Pt[lepIdx];
    }

    if(relIso < _maxElIso) code |= TIGHT;

    if(relIso < _maxLooseElIso) code |= LOOSE_EL;

    if(relIso < _elVetoIso) code |= LOOSE_MU;

    if(_debug) printf("\telectron: reliso=%2.3f code=%i\n",relIso,code);
  }

  if(MUON == _leptons->LeptonType[lepIdx]) {
    double relIso = 999.;
    if(_leptons->Pt[lepIdx] > 0.) {
      relIso = _usePFIso ? 
	(_leptons->ChargedHadronIsoR04[lepIdx]+std::max(0.,_leptons->NeutralHadronIsoR04[lepIdx]+_leptons->PhotonIsoR04[lepIdx]-0.5*_leptons->sumPUPtR04[lepIdx]))/_leptons->Pt[lepIdx] :
	//(_leptons->ChargedHadronIso[lepIdx]+_leptons->NeutralHadronIso[lepIdx]+_leptons->PhotonIso[lepIdx])/_leptons->Pt[lepIdx] :
	(_leptons->TrackIso[lepIdx]+_leptons->EcalIso[lepIdx]+_leptons->HcalIso[lepIdx])/_leptons->Pt[lepIdx];
    }

    if(relIso < _maxMuIso) code |= TIGHT;

    if(relIso < _muVetoIso) {
      code |= LOOSE_MU;
      code |= LOOSE_EL;
    }

    if(_debug) printf("\tmuon: reliso=%2.3f code=%i\n",relIso,code);
  }

  return code;
}


int objectSelector::fiducialLep(const int lepIdx) const
{
  int code = FAIL;
  if(lepIdx < 0 || lepIdx > _leptons->Size) {
    std::cout << "WARNING: Invalid lepton index\n";
    return code;
  }

  if(_debug) printf("fiducialLep: lepIdx %i\n",lepIdx);

  if(ELECTRON == _leptons->LeptonType[lepIdx]) {
    double et = _leptons->Et[lepIdx];
    double eta = _leptons->Eta[lepIdx];

    if(et > _minElPt) 
      if(fabs(eta) < _maxElEta) 
	if(fabs(eta) <= _minElEtaCrack || 
	   fabs(eta) >= _maxElEtaCrack) 
	  code |= TIGHT;

    if(et > _minLooseElPt) 
      if(fabs(eta) < _maxLooseElEta) 
	if(fabs(eta) <= _minElEtaCrack || 
	   fabs(eta) >= _maxElEtaCrack) 
	  code |= LOOSE_EL;

    if(et > _elVetoEt) 
      if(fabs(eta) < _elVetoEta)
	code |= LOOSE_MU;

    if(_debug) printf("\telectron: et=%2.3f eta=%2.3f code=%i\n",et,eta,code);
  }

  if(MUON == _leptons->LeptonType[lepIdx]) {
    double pt = _leptons->Pt[lepIdx];
    double eta = _leptons->Eta[lepIdx];

    if(pt > _minMuPt) 
      if(fabs(eta) < _maxMuEta)
	code |= TIGHT;

    if(pt > _muVetoPt) 
      if(fabs(eta) < _muVetoEta) {
	code |= LOOSE_MU;
	code |= LOOSE_EL;
      }

    if(_debug) printf("\tmuon: pt=%2.3f eta=%2.3f code=%i\n",pt,eta,code);
  }

  return code;
}

int objectSelector::passesConversionVeto(const int lepIdx) const
{
  if(ELECTRON != _leptons->LeptonType[lepIdx]) return TIGHT;

  if(_leptons->NumberOfExpectedInnerHits[lepIdx] > _convNHits ) return FAIL;
  if(fabs(_leptons->Eldist[lepIdx]) < _convDist &&
     fabs(_leptons->Eldcot[lepIdx]) < _convDcot) return FAIL;

  return TIGHT;
}

bool objectSelector::isGoodJet(const int jetIdx) const
{
  if(_debug) printf("isGoodJet: jetIdx %i\n", jetIdx);
  if(!(qualityJet(jetIdx) &TIGHT)) return false;
  if(!(fiducialJet(jetIdx) &TIGHT)) return false;
  return true;
}

int  objectSelector::qualityJet  (const int jetIdx) const
{
  int code = FAIL;
  if(jetIdx < 0 || jetIdx > _jets->Size) {
    std::cout << "WARNING: Invalid jet index\n";
    return code;
  }

  if(_debug) printf("qualityJet: jetIdx %i\n", jetIdx);

  if(_useJetID) {
    if(_jets->JetIDLOOSE[jetIdx])
      code |= TIGHT;
  }
  else {
    if(_jets->NConstituents[jetIdx] > _minNOD)
      if(_jets->NHF[jetIdx] < _maxNHF)
	if(_jets->NEF[jetIdx] < _maxNEF)
	  if((fabs(_jets->Eta[jetIdx]) >= _jetEtaSwitch) || 
	     ((_jets->CEF[jetIdx] < _maxCEF) &&
	      (_jets->CHF[jetIdx] > _minCHF) &&
	      (_jets->NCH[jetIdx] > _minNCH)))
	    code |= TIGHT;
  }

  if(_debug) printf("\tJetIDLOOSE=%i code=%i\n", _jets->JetIDLOOSE[jetIdx],code);

  return code;
}

int  objectSelector::fiducialJet (const int jetIdx) const
{
  int code = FAIL;
  if(jetIdx < 0 || jetIdx > _jets->Size) {
    std::cout << "WARNING: Invalid jet index\n";
    return code;
  }

  if(_debug) printf("fiducialJet: jetIdx %i\n", jetIdx);

  if(_jets->Pt[jetIdx] > _minJetPt) 
    if(fabs(_jets->Eta[jetIdx]) < _maxJetEta)
      code |= TIGHT;

  if(_debug) printf("\tpt=%2.3f eta=%2.3f code=%i\n",_jets->Pt[jetIdx],_jets->Eta[jetIdx],code);

  return code;
}

bool objectSelector::isPrimaryVertex(const int vtxIdx) const
{
  if(vtxIdx < 0 || vtxIdx > _vertices->Size) {
    std::cout << "WARNING: Invalid vertex index\n";
    return false;
  }

  if(_debug) printf("isPrimaryVertex: vtxIdx %i\n", vtxIdx);
  if(_debug) printf("\tfake=%i ndof=%2.3f z=%2.3f rho=%2.3f\n",_vertices->isFake[vtxIdx],_vertices->Ndof[vtxIdx],_vertices->z[vtxIdx],_vertices->Rho[vtxIdx]);

  if(_vertices->Type[vtxIdx] != 0)           return false;//only primary vertices, not with bs constraint
  if(_firstVtxOnly && vtxIdx!=0)             return false;
  if(_vertices->isFake[vtxIdx])              return false;
  if(_vertices->Ndof[vtxIdx] < _minNDOF)     return false;
  if(fabs(_vertices->z[vtxIdx]) > _maxZ)     return false;
  if(fabs(_vertices->Rho[vtxIdx]) > _maxRho) return false;
  return true;
}

std::ostream& operator<<(std::ostream& s, const objectSelector& c)
{
  s << "Selection criteria:\n";
  s << "\tPrimary Muon:\n";
  s << "\t\t_minMuPt=" << c._minMuPt << std::endl;
  s << "\t\t_maxMuEta=" << c._maxMuEta << std::endl;
  s << "\t\t_maxMuDxy=" << c._maxMuDxy << std::endl;
  //s << "\t\t_maxMuD0BS=" << c._maxMuD0BS << std::endl;
  s << "\t\t_maxMuDZ=" << c._maxMuDZ << std::endl;
  s << "\t\t_minTrNHit=" << c._minTrNHit << std::endl;
  s << "\t\t_minMuPixHit=" << c._minMuPixHit << std::endl;
  s << "\t\t_minMatch=" << c._minMatch << std::endl;
  s << "\t\t_maxMuIso=" << c._maxMuIso << std::endl;
  s << "\t\t_usePUCorrIso=" << c._usePUCorrIso << std::endl;

  s << "\n\tVeto Electron:\n";
  s << "\t\t_elVetoEt=" << c._elVetoEt << std::endl;
  s << "\t\t_elVetoEta=" << c._elVetoEta << std::endl;
  s << "\t\t_elVetoIso=" << c._elVetoIso << std::endl;

  s << "\n\tVeto Muonn:\n";
  s << "\t\t_muVetoPt=" << c._muVetoPt << std::endl;
  s << "\t\t_muVetoEta=" << c._muVetoEta << std::endl;
  s << "\t\t_muVetoIso=" << c._muVetoIso << std::endl;

  s << "\n\tPrimary Electron:\n";
  s << "\t\t_minElPt=" << c._minElPt << std::endl;
  s << "\t\t_maxElEta=" << c._maxElEta << std::endl;
  s << "\t\t_maxElDZ=" << c._maxElDZ << std::endl;
  s << "\t\t_maxElD0BS=" << c._maxElD0BS << std::endl;
  s << "\t\t_maxElIso=" << c._maxElIso << std::endl;
  s << "\t\t_convNHits=" << c._convNHits << std::endl;
  s << "\t\t_convDist=" << c._convDist << std::endl;
  s << "\t\t_convDcot=" << c._convDcot << std::endl;
  s << "\t\t_minElEtaCrack=" << c._minElEtaCrack << std::endl;
  s << "\t\t_maxElEtaCrack=" << c._maxElEtaCrack << std::endl;

  s << "\n\tLoose Electron:\n";
  s << "\t\t_minLooseElPt=" << c._minLooseElPt << std::endl;
  s << "\t\t_maxLooseElEta=" << c._maxLooseElEta << std::endl;
  s << "\t\t_maxLooseElIso=" << c._maxLooseElIso << std::endl;

  s << "\n\tJet:\n";
  s << "\t\t_minJetPt=" << c._minJetPt << std::endl;
  s << "\t\t_maxJetEta=" << c._maxJetEta << std::endl;
  s << "\t\t_useJetID=" << c._useJetID << std::endl;

  s << "\n\tVertex:\n";
  s << "\t\t_firstVtxOnly=" << c._firstVtxOnly << std::endl;
  s << "\t\t_minNDOF=" << c._minNDOF << std::endl;
  s << "\t\t_maxZ=" << c._maxZ << std::endl;
  s << "\t\t_maxRho=" << c._maxRho << std::endl;

  return s;
}
