#ifndef __DOHITFIT__
#define __DOHITFIT__

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MyAna/bpkHitFitForExcitedQuark/interface/bpkRunHitFit.h"
#include "MyAna/bpkHitFitForExcitedQuark/interface/HitFitTranslator.h"

class LepInfoBranches;
class JetInfoBranches;
class VertexInfoBranches;
class HitFitInfoBranches;
class GenInfoBranches;

class doHitFitForExcitedQuark {

 public:

  doHitFitForExcitedQuark(const edm::ParameterSet&,
	      const EvtInfoBranches &evt, const LepInfoBranches &lep, //const int lepIdx, 
	      const JetInfoBranches &jets, const GenInfoBranches &gens);//, const std::vector<int> jetsIdx);

  ~doHitFitForExcitedQuark();

  bool runHitFit(const int lepIdx, const std::vector<int> jetsIdx, std::vector<bool> jetisbtag);
  void fillHitFitInfo(HitFitInfoBranches& HitFitInfo);

 private:

  const EvtInfoBranches* EvtInfo;
  const LepInfoBranches* LepInfo;
  int _lepIdx;
  const JetInfoBranches* JetInfo;
  const GenInfoBranches* GenInfo;
  std::vector<int> _jetsIdx;

  std::vector<int> hitfitJet;

  bool _ranHitFit;
  bool isMC;

  unsigned int _nHitFit;
  unsigned int _nHitFitJet;

  bool debug_;

    //Path of text file containing the default settings to HitFit.
    edm::FileInPath Default_;

    //Path of text file containing the electron resolution.
    edm::FileInPath ElectronResolution_;

    //If TRUE, use the resolution embedded in the electron physics object and NOT the resolution in the file hitfitElectronResolution_.
    //If FALSE, use the resolution in the file hitfitElectronResolution_.
    bool        ElectronObjRes_;

    //Path of text file containing the muon resolution.
    edm::FileInPath MuonResolution_;

    //If TRUE, use the resolution embedded in the muon physics object and NOT the resolution in the file hitfitMuonResolution_.
    //If FALSE, use the resolution in the file hitfitMuonResolution_.
    bool        MuonObjRes_;

    //Path of text file containing the udsc jet resolution.
    edm::FileInPath UdscJetResolution_;

    //Path of text file containing the b jet resolution.
    edm::FileInPath BJetResolution_;

    //If TRUE, use the resolution embedded in the jet physics object and NOT the resolution in the file hitfitUdscJetResolution_ and hitfitBJetResolution_.
    //If FALSE, use the resolution in the hitfitUdscJetResolution_ and hitfitBJetResolution_.
    bool        JetObjRes_;

    //level to correct jets to in HitFit
    std::string JetCorrectionLevel_;

    //factor to scale jets by in HitFit
    double      UdscJES_;
    double      BJES_;

    //Path of text file containing the missing transverse energy resolution.
    edm::FileInPath METResolution_;

    //If TRUE, use the resolution embedded in the MET physics object and NOT the resolution in the file hitfitMETResolution_.
    //If FALSE, use the resolution in the file hitfitMETResolution_.
    bool        METObjRes_;

    //The values to which HitFit must constrain the leptonic/hadronic W boson mass, in units of GeV.
    double      LepWMass_;
    double      HadWMass_;

    //The value to which HitFit must constrain the top quark mass, in units of GeV. 0.0 is unconstrained.
    double      TopMass_;

    //Select which neutrino solution to use 0=smaller pz, 1=larger pz, otherwise use both
    int nuSolution_;

    hitfit::LeptonTranslator LeptonTranslator_;
    hitfit::JetTranslator    JetTranslator_;
    hitfit::METTranslator    METTranslator_;

    hitfit::bpkRunHitFit* HitFit;

    double      MinLeptonPt_;
    double      MinJetPt_;
    double      MinMET_;

    //	    The maximum number of of jets used in HitFit.
   unsigned    MaxNJet_;

    /**
        NOT READY: If TRUE, make HitFit prefer bjets.
     */
    bool        PreferBJet_;
};

#endif //doHitFitForExcitedQuark_h
