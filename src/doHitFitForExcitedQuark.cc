#include "DataFormats/Math/interface/LorentzVector.h"

#include "TopQuarkAnalysis/TopHitFit/interface/Top_Decaykin.h"

#include "MyAna/bprimeKit/interface/doHitFitForExcitedQuark.h"
#include "MyAna/bprimeKit/interface/bpkUtils.h"
#include "MyAna/bprimeKit/interface/format.h"
#include "MyAna/bprimeKit/interface/HitFitInfoBranches.h"
#include "TLorentzVector.h"


doHitFitForExcitedQuark::doHitFitForExcitedQuark(const edm::ParameterSet& iConfig, const EvtInfoBranches &evt, 
			 const LepInfoBranches &lep, //const int lepIdx,
			 const JetInfoBranches &jets, const GenInfoBranches &gens)://, const std::vector<int> jetsIdx) :
  EvtInfo(&evt),
  LepInfo(&lep),
  JetInfo(&jets),
  GenInfo(&gens),
  debug_(iConfig.getUntrackedParameter<bool>("Debug",false)),
  Default_(iConfig.getUntrackedParameter<edm::FileInPath>("Default",
							  edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/setting/RunHitFitConfiguration.txt")))),
  ElectronResolution_(iConfig.getUntrackedParameter<edm::FileInPath>(std::string("ElectronResolution"),
								     edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt")))),
  ElectronObjRes_ (iConfig.getUntrackedParameter<bool>("ElectronObjRes", bool(false))),
  MuonResolution_(iConfig.getUntrackedParameter<edm::FileInPath>("MuonResolution",
								 edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt")))),
  MuonObjRes_(iConfig.getUntrackedParameter<bool>("MuonObjRes", bool(false))),
  UdscJetResolution_(iConfig.getUntrackedParameter<edm::FileInPath>("UdscJetResolution",
								    edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt")))),
  BJetResolution_(iConfig.getUntrackedParameter<edm::FileInPath>("BJetResolution",
								 edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt")))),
  JetObjRes_(iConfig.getUntrackedParameter<bool>("JetObjRes", bool(false))),
  JetCorrectionLevel_(iConfig.getUntrackedParameter<std::string>("JetCorrectionLevel","L7Parton")),
  UdscJES_(iConfig.getUntrackedParameter<double>("UdscJES", 1.0)),
  BJES_(iConfig.getUntrackedParameter<double>("BJES", 1.0)),
  METResolution_(iConfig.getUntrackedParameter<edm::FileInPath>("METResolution",
								edm::FileInPath(std::string("TopQuarkAnalysis/TopHitFit/data/resolution/tqafKtResolution.txt")))),
  METObjRes_(iConfig.getUntrackedParameter<bool>("METsObjRes", bool(false))),
  LepWMass_(iConfig.getUntrackedParameter<double>("LepWMass",80.4)),
  HadWMass_(iConfig.getUntrackedParameter<double>("HadWMass",80.4)),
  //TopMass_(iConfig.getUntrackedParameter<double>("TopMass",0.0)),
  TopMass_(iConfig.getUntrackedParameter<double>("TopMass",172.0)),
  nuSolution_(iConfig.getUntrackedParameter<int>("NuSolution",2)),
  LeptonTranslator_(ElectronResolution_.fullPath(), MuonResolution_.fullPath()),
  JetTranslator_(UdscJetResolution_.fullPath(), BJetResolution_.fullPath(),JetCorrectionLevel_,UdscJES_,BJES_),
  METTranslator_(METResolution_.fullPath()),
  MinLeptonPt_(iConfig.getUntrackedParameter<double>("MinLeptonPt",15.0)),
  MinJetPt_(iConfig.getUntrackedParameter<double>("MinJetPt",15.0)),
  MinMET_(iConfig.getUntrackedParameter<double>("MinMET",0.0)),
  MaxNJet_(iConfig.getUntrackedParameter<unsigned>("MaxNJet",unsigned(4)))
{

  if (MaxNJet_ < kMIN_HITFIT_JET_TSTAR) {
    std::cout << "Error, MaxNJet is smaller than kMIN_HITFIT_JET_TSTAR ! Exiting !\n"  ;
    exit(1);
  }
  if (MaxNJet_ > kMAX_HITFIT_JET) {
    std::cout << "Error, MaxNJet is larger than kMAX_HITFIT_JET ! Exiting !\n"  ;
    exit(1);
  }

  HitFit = new hitfit::bpkRunHitFit(LeptonTranslator_,
				    JetTranslator_,
				    METTranslator_,
				    Default_.fullPath(),
				    LepWMass_,
				    HadWMass_,
				    TopMass_,
                    nuSolution_);

  HitFit->clear();

  _ranHitFit = false;
  if(debug_) std::cout << "Created doHitFitForExcitedQuark\n";
}

doHitFitForExcitedQuark::~doHitFitForExcitedQuark()
{
}

bool doHitFitForExcitedQuark::runHitFit(const int lepIdx, const std::vector<int> jetsIdx, std::vector<bool> jetisbtag)
{
  if(debug_) std::cout << "runHitFit: Lep index " << lepIdx << ", number of jets " << jetsIdx.size() << std::endl;
  // We haven't run HitFit yet, so we set the flag to false
  _ranHitFit = false ;
  HitFit->clear();

  _lepIdx = lepIdx;
  _jetsIdx = jetsIdx;

  if(_lepIdx < 0 || _lepIdx >= LepInfo->Size) {
    std::cout << "Lepton index " << _lepIdx << " invalid, can't run HitFit!\n";
    return _ranHitFit;
  }

  for(unsigned i=0; i<_jetsIdx.size(); i++) {
    if(_jetsIdx[i] < 0 || _jetsIdx[i] >= JetInfo->Size) {
      std::cout << "Jet index " << _jetsIdx[i] << " invalid, can't run HitFit!\n";
      return _ranHitFit;
    }
  }

  //set some other variables to be set later as well.
  _nHitFit = 0;
  _nHitFitJet = 0;

  // Insert the lepton into HitFit, check if the lepton
  // is within acceptance of the eta-dependent resolution.
  if (LeptonTranslator_.CheckEta(*LepInfo,_lepIdx) &&
      LepInfo->Pt[_lepIdx] > MinLeptonPt_) {
    if(11==LepInfo->LeptonType[_lepIdx]) {
      HitFit->AddLepton(*LepInfo,_lepIdx,ElectronObjRes_);
    }
    else {
      HitFit->AddLepton(*LepInfo,_lepIdx,MuonObjRes_);
    }
    if(debug_) std::cout << "  Added Lepton to HitFit\n";
  }
  else {
    if(debug_) std::cout << "  Lepton doesn't pass HitFit requirements\n";
    return _ranHitFit;
  }

  hitfitJet.clear();

  // copy jet which pass the minimal criteria as input to HitFit
  // There may more jets which pass the minimal criteria as input
  // to HitFit
  for (std::vector<int>::const_iterator jet = _jetsIdx.begin() ; jet != _jetsIdx.end() ; ++jet) {
    if (JetInfo->Pt[*jet] > MinJetPt_ && JetTranslator_.CheckEta(*JetInfo,*jet)) {
      hitfitJet.push_back(*jet);
      if(debug_) std::cout << "  Jet " << *jet << " passes HitFit requirements\n";
    }
  }

  // if there is not enough jets, return
  if (hitfitJet.size() < kMIN_HITFIT_JET_TSTAR) {
    if(debug_) std::cout << "  Only " << hitfitJet.size() << " jets---not enough\n";
    return _ranHitFit;
  }

  // Force the max number of jets included in the fit to be
  // kMAX_HITFIT_JET.  While there is no requirement to use only
  // the first N leading jets, we don't want HitFit to fit too many
  // permutations.
  _nHitFitJet =   hitfitJet.size() > MaxNJet_ ? MaxNJet_ : hitfitJet.size() ;
  for (size_t j = 0 ; j != _nHitFitJet ; ++j) {
    HitFit->AddJet(hitfitJet[j],JetObjRes_);
    if(debug_) std::cout <<  "  Added jet " << hitfitJet[j] << " to HitFit\n";
  }

  // Since we have determined how many jets have been inserted to HitFit,
  // we now remove the remaining jets in order not to mess up ourselves later !
  hitfitJet.erase(hitfitJet.begin() + _nHitFitJet, hitfitJet.end());

  HitFit->SetMet(*EvtInfo,METObjRes_);

  // Run HitFit and see how many permutations do we get ?
  _nHitFit = HitFit->FitAllPermutation(*JetInfo, jetisbtag);

  // We've run HitFit !
  _ranHitFit   = true ;

  if(debug_) std::cout << (_ranHitFit ? "Ran HitFit" : "HitFit not run") << std::endl;

  return _ranHitFit;


} // void doHitFitForExcitedQuark::runHitFit()

void doHitFitForExcitedQuark::fillHitFitInfo(HitFitInfoBranches& HitFitInfo)
{
  if(debug_) std::cout << "fillHitFitInfo\n";
  isMC = EvtInfo->McFlag;
  HitFitInfo.clear(); //make sure that old information isn't kept if hitfit is false
  if(!_ranHitFit) {
    HitFitInfo.hitfit = false;
    return;
  }

  double minChi2        = 1000000.0 ;
  double maxChi2        = 0.0       ;

  int _nHitFit_tmp = _nHitFit; // save only one permutation with minChi2
  if(_nHitFit>0) _nHitFit = 1; // save only one permutation with minChi2

  HitFitInfo.hitfit = true;
  HitFitInfo.nHitFit = _nHitFit;
  HitFitInfo.nHitFitJet = _nHitFitJet;
  HitFitInfo.nHitFitXnHitFitJet = _nHitFit*_nHitFitJet;
  HitFitInfo.LepInfoIndex = _lepIdx;

  std::vector<hitfit::Lepjets_Event>
    unfittedEvent                               = HitFit->GetUnfittedEvent();
  std::vector<hitfit::Fit_Result> hitfitResult  = HitFit->GetFitAllPermutation();


  for(size_t k=0; k!=HitFitInfo.nHitFitJet; ++k) {
    HitFitInfo.JetInfoIndex[k] = hitfitJet[k];
  }

  int unfittedLeptonCharge = LepInfo->Charge[_lepIdx];

/* modify to save only one permutation */
  double minChi2_        = 1000000.0 ;
  int minChi2Index_        = -1 ;
  for (int h = 0 ; h!= _nHitFit_tmp ; ++h) {

	  if (hitfitResult[h].chisq()  < minChi2_ && hitfitResult[h].chisq()  > 0.0) {
		  minChi2_                     = hitfitResult[h].chisq() ;
		  minChi2Index_          = h ;
	  }
  }

/* modify to save only one permutation */
if(minChi2Index_!=-1){

  if(isMC) {//isMC

    for (size_t i = 0, saw_hadw1 = 0 ; i != HitFitInfo.nHitFitJet  ; ++i) {

	int JetPartonPdgId            = JetInfo->GenPdgID[hitfitJet[i]];

	// Set the default value to be unknown, will be overwritten later
	HitFitInfo.JetPartonType[i]             = hitfit::unknown_label;

	// There is correlation between the lepton charge
	// and which b-jet (coming from b or bbar) is assigned
	// to the leptonic top / hadronicc top.
	//
	// Illustration, let's take a t tbar event decaying
	// to l (lepton) + jets
	// t tbar -> W+    b W-    bbar
	//
	//        -> l+ nu b qq'   bbar (W+ -> l+ nu, W- -> qq')
	//        -> qq'   b l- nu bbar (W- -> l- nu, W+ -> qq')
	//
	// For l+, bbar (PDG ID: -5) is the leptonic b-jet
	// For l+, b    (PDG ID:  5) is the hadronic b-jet
	// For l-, bbar (PDG ID: -5) is the hadronic b-jet
	// For l-, b    (PDG ID:  5) is the leptonic b-jet
	//
	// This simple analysis ignore the decay of W boson to
	// ub or cb.
	// The relevant CKM matrix elements (V_ub and V_cb)
	// are very small.
	//
	// |V_ub| = 0.00359  +/- 0.00016
	// |V_cb| = 0.0415  (+0.0010 - 0.0011)
	//
	// The hitfit jet code in ttbar events are reproduced here
	//
	// 0  ISR / FSR (hitfit::isr_label)
	// 11 b-jet from leptonic top (hitfit::lepb_label)
	// 12 b-jet from hadronic top (hitfit::hadb_label)
	// 13 q-jet from hadronic W, the higher pt one (hitfit::hadw1_label)
	// 14 q-jet from hadronic W, the higher pt one (hitfit::hadw2_label)
	// 15 b-jet from Higgs, (hitfit::higgs_label)
	// 20 unknown (hitfit::unknown_label)

	if (unfittedLeptonCharge > 0) {

	  if (JetPartonPdgId ==  5) {
	    HitFitInfo.JetPartonType[i]      = hitfit::lepb_label;
	  }
	  if (JetPartonPdgId == -5) {
	    HitFitInfo.JetPartonType[i]      = hitfit::hadb_label;
	  }
	  if (abs(JetPartonPdgId) == 4 ||
	      abs(JetPartonPdgId) == 3 ||
	      abs(JetPartonPdgId) == 2 ||
	      abs(JetPartonPdgId) == 1 ) {
	    if (bool(saw_hadw1)) {
	      HitFitInfo.JetPartonType[i]  = hitfit::hadw2_label;
	    } else {
	      HitFitInfo.JetPartonType[i]  = hitfit::hadw1_label;
	      saw_hadw1               = 1;
	    }

	  }
	  if (abs(JetPartonPdgId) == 21) {
	    HitFitInfo.JetPartonType[i]      = hitfit::isr_label;
	  }

	}

	if (unfittedLeptonCharge < 0) {


	  if (JetPartonPdgId == -5) {
	    HitFitInfo.JetPartonType[i]  = hitfit::lepb_label;
	  }
	  if (JetPartonPdgId ==  5) {
	    HitFitInfo.JetPartonType[i]  = hitfit::hadb_label;
	  }
	  if (abs(JetPartonPdgId) == 4 ||
	      abs(JetPartonPdgId) == 3 ||
	      abs(JetPartonPdgId) == 2 ||
	      abs(JetPartonPdgId) == 1 ) {
	    if (bool(saw_hadw1)) {
	      HitFitInfo.JetPartonType[i]  = hitfit::hadw2_label;
	    } else {
	      HitFitInfo.JetPartonType[i]  = hitfit::hadw1_label;
	      saw_hadw1               = 1;
	    }

	  }
	  if (abs(JetPartonPdgId) == 21) {
	    HitFitInfo.JetPartonType[i]      = hitfit::isr_label;
	  }

	}

	// The Permutation is a string (char) encoded representation
	// of the jet
	// hitfit::isr_label     : b
	// hitfit::lepb_label    : B
	// hitfit::hadb_label    : w
	// hitfit::hadw1_label   : w
	// hitfit::hadw2_label   : h
	// hitfit::higgs_label   : g
	// hitfit::unknown_label : ?
	// Not set               : *

	HitFitInfo.JetPartonPermutation[i]       = hitfit::jetTypeChar(HitFitInfo.JetPartonType[i]);
    }

    bool saw_hadw1(false);

    for (size_t i = 0 ; i != HitFitInfo.nHitFitJet ; ++i) {

	int JetGenJetPdgId             = JetInfo->GenPdgID[hitfitJet[i]];
	int JetMCTag 				   = JetInfo->GenMCTag[hitfitJet[i]];
	//Obtaining the Gen Gluons
	int gengluon = 0;
	int gengluon_posi = -1;
	int gengluon_nega = -1;
	TLorentzVector gen_lepgluon, gen_hadgluon;
	for(int j = 0; j < GenInfo->Size; j++){
	    if(GenInfo->PdgID[j] == 21) {
        //if gluon has mom 8, it must be mom 1
        int mo1 = GenInfo->Mo1[j];
        //if a gluon has mom = 8, it will not has mom2. and number of mom will be just 1
        	if(mo1 != -1) { 
    	    	if (GenInfo->PdgID[mo1] == 8){ 
	                gengluon_posi = j;gengluon++;
    	        }
        	    if (GenInfo->PdgID[mo1] == -8){
	            	gengluon_nega = j;gengluon++;
    	        }
        	}
        }
	}

	HitFitInfo.JetGenJetType[i]              = hitfit::unknown_label;
	

	if (unfittedLeptonCharge > 0) {
	  if(gengluon_posi != -1) gen_lepgluon.SetPtEtaPhiM(GenInfo->Pt[gengluon_posi], GenInfo->Eta[gengluon_posi], GenInfo->Phi[gengluon_posi], GenInfo->Mass[gengluon_posi]);
	  if(gengluon_nega != -1) gen_hadgluon.SetPtEtaPhiM(GenInfo->Pt[gengluon_nega], GenInfo->Eta[gengluon_nega], GenInfo->Phi[gengluon_nega], GenInfo->Mass[gengluon_nega]);


	  if (JetGenJetPdgId ==  5 && JetMCTag == 20) {
	    HitFitInfo.JetGenJetType[i]      = hitfit::lepb_label;
	  }
	  if (JetGenJetPdgId == -5 && JetMCTag == 20) {
	    HitFitInfo.JetGenJetType[i]      = hitfit::hadb_label;
	  }
	  if ((abs(JetGenJetPdgId) == 4 ||
	      abs(JetGenJetPdgId) == 3 ||
	      abs(JetGenJetPdgId) == 2 ||
	      abs(JetGenJetPdgId) == 1 ||
	      abs(JetGenJetPdgId) == 5 ) && JetMCTag == 21) {
	    if (bool(saw_hadw1)) {
	      HitFitInfo.JetGenJetType[i]  = hitfit::hadw2_label;
	    } else {
	      HitFitInfo.JetGenJetType[i]  = hitfit::hadw1_label;
	      saw_hadw1               = 1;
	    }

	  }
	  if (abs(JetGenJetPdgId) == 21 && JetMCTag == 20) {
		TLorentzVector gluon;
		gluon.SetPtEtaPhiE(JetInfo->Pt[hitfitJet[i]], JetInfo->Eta[hitfitJet[i]], JetInfo->Phi[hitfitJet[i]], JetInfo->Energy[hitfitJet[i]]);
	  	if(gengluon_posi != -1 && gengluon_nega != -1) {
			HitFitInfo.JetGenJetType[i] = (gluon.DeltaR(gen_lepgluon) < gluon.DeltaR(gen_hadgluon)) ? hitfit::gluon1_label : hitfit::gluon2_label;
		}
		if(gengluon_posi != -1 && gengluon_nega == -1) {if (gluon.DeltaR(gen_lepgluon) < 0.4) HitFitInfo.JetGenJetType[i] = hitfit::gluon1_label;}
		if(gengluon_posi == -1 && gengluon_nega != -1) {if (gluon.DeltaR(gen_hadgluon) < 0.4) HitFitInfo.JetGenJetType[i] = hitfit::gluon2_label;}
	    //HitFitInfo.JetGenJetType[i]      = hitfit::isr_label;
	  }

	}

	if (unfittedLeptonCharge < 0) {
	  if(gengluon_posi != -1) gen_hadgluon.SetPtEtaPhiM(GenInfo->Pt[gengluon_posi], GenInfo->Eta[gengluon_posi], GenInfo->Phi[gengluon_posi], GenInfo->Mass[gengluon_posi]);
	  if(gengluon_nega != -1) gen_lepgluon.SetPtEtaPhiM(GenInfo->Pt[gengluon_nega], GenInfo->Eta[gengluon_nega], GenInfo->Phi[gengluon_nega], GenInfo->Mass[gengluon_nega]);

	  if (JetGenJetPdgId == -5 && JetMCTag == 20) {
	    HitFitInfo.JetGenJetType[i]      = hitfit::lepb_label;
	  }
	  if (JetGenJetPdgId ==  5 && JetMCTag == 20) {
	    HitFitInfo.JetGenJetType[i]      = hitfit::hadb_label;
	  }
	  if ((abs(JetGenJetPdgId) == 4 ||
	      abs(JetGenJetPdgId) == 3 ||
	      abs(JetGenJetPdgId) == 2 ||
	      abs(JetGenJetPdgId) == 1 ||
	      abs(JetGenJetPdgId) == 5 ) && JetMCTag == 21) {
	    if (bool(saw_hadw1)) {
	      HitFitInfo.JetGenJetType[i]  = hitfit::hadw2_label;
	    } else {
	      HitFitInfo.JetGenJetType[i]  = hitfit::hadw1_label;
	      saw_hadw1               = 1;
	    }

	  }
	  if (abs(JetGenJetPdgId) == 21 && JetMCTag == 20) {
		TLorentzVector gluon;
		gluon.SetPtEtaPhiE(JetInfo->Pt[hitfitJet[i]], JetInfo->Eta[hitfitJet[i]], JetInfo->Phi[hitfitJet[i]], JetInfo->Energy[hitfitJet[i]]);
	  	if(gengluon_posi != -1 && gengluon_nega != -1) {
			HitFitInfo.JetGenJetType[i] = (gluon.DeltaR(gen_lepgluon) < gluon.DeltaR(gen_hadgluon)) ? hitfit::gluon1_label : hitfit::gluon2_label;
		}
		if(gengluon_posi == -1 && gengluon_nega != -1) {if (gluon.DeltaR(gen_lepgluon) < 0.4) HitFitInfo.JetGenJetType[i] = hitfit::gluon1_label;}
		if(gengluon_posi != -1 && gengluon_nega == -1) {if (gluon.DeltaR(gen_hadgluon) < 0.4) HitFitInfo.JetGenJetType[i] = hitfit::gluon2_label;}
	    //HitFitInfo.JetGenJetType[i]      = hitfit::isr_label;
	  }

	}

	HitFitInfo.JetGenJetPermutation[i]       = hitfit::jetTypeChar(HitFitInfo.JetGenJetType[i]);

    }
  }

  for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {

    for (size_t i = 0 ; i != HitFitInfo.nHitFitJet ; ++i) {

      // Casting to (UInt_t) is necessary to avoid compilation warning.
      HitFitInfo.JetType[h*HitFitInfo.nHitFitJet+i]           = (UInt_t) hitfitResult[h+minChi2Index_].ev().jet(i).type();
      HitFitInfo.Permutation[h*HitFitInfo.nHitFitJet+i]       = hitfit::jetTypeChar(HitFitInfo.JetType[h*HitFitInfo.nHitFitJet+i]);

    }

    // first set the correct jet permutation flag to false
    HitFitInfo.CorrectPermutation[h]                 = false ;

    if (isMC) {

      // Only check for MC event

      // loop over all jets to see if the parton-level jet type and assumed jet type in
      // the fit are identical
      // Count the number of correctPermutaion
      for (size_t j = 0 ; j != HitFitInfo.nHitFitJet ; ++j) {
	if (HitFitInfo.JetPartonType[j] == HitFitInfo.JetType[h*HitFitInfo.nHitFitJet+j]) {
	  ++HitFitInfo.NJetCorrectPermutation[h];
	}
      }
    }

    // If the number of correct jet in the permutation is equal to the
    // minimum number of jet put into HitFit, means that the permutation is correct.

    if (HitFitInfo.NJetCorrectPermutation[h] == kMIN_HITFIT_JET_TSTAR) {
      if (h % 2 == 0) {
	HitFitInfo.CorrectPermutationIndex[0]    = h ;
      }
      else {
	HitFitInfo.CorrectPermutationIndex[1]    = h ;
      }
      HitFitInfo.CorrectPermutation[h]             = true ;
    }

    HitFitInfo.hitfitNX                                    = (size_t) hitfitResult[h+minChi2Index_].pullx().num_row();
    HitFitInfo.hitfitNY                                    = (size_t) hitfitResult[h+minChi2Index_].pully().num_row();

    HitFitInfo.Chi2[h] = hitfitResult[h+minChi2Index_].chisq();
    if (HitFitInfo.Chi2[h] > 0.0) {
      HitFitInfo.Converge[h]                       = true ;
    } else {
      HitFitInfo.Converge[h]                       = false ;
    }

    HitFitInfo.nHitFitXnX                                  = HitFitInfo.nHitFit*HitFitInfo.hitfitNX;
    HitFitInfo.nHitFitXnY                                  = HitFitInfo.nHitFit*HitFitInfo.hitfitNY;

    for (size_t i = 0 ; i != (size_t) hitfitResult[h+minChi2Index_].pullx().num_row() ; ++i) {
      HitFitInfo.PullX[h*HitFitInfo.hitfitNX+i]               = hitfitResult[h+minChi2Index_].pullx()[i];
    }

    for (size_t i = 0 ; i != (size_t) hitfitResult[h+minChi2Index_].pully().num_row() ; ++i) {
      HitFitInfo.PullY[h*HitFitInfo.hitfitNY+i]               = hitfitResult[h+minChi2Index_].pully()[i];
    }

  }

  for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {
    if (HitFitInfo.NJetCorrectPermutation[h] > HitFitInfo.MaxNJetCorrectPermutation) {
      HitFitInfo.MaxNJetCorrectPermutation = HitFitInfo.NJetCorrectPermutation[h];
    }
  }

  hitfit::Resolution leptonPRes                   = unfittedEvent.front().lep(0).res().p_res();
  hitfit::Resolution leptonEtaRes                 = unfittedEvent.front().lep(0).res().eta_res();
  hitfit::Resolution leptonPhiRes                 = unfittedEvent.front().lep(0).res().phi_res();

  HitFitInfo.unfittedLeptonPResC                             = leptonPRes.C();
  HitFitInfo.unfittedLeptonPResR                             = leptonPRes.R();
  HitFitInfo.unfittedLeptonPResN                             = leptonPRes.N();
  HitFitInfo.unfittedLeptonPResInverse                       = leptonPRes.inverse();

  HitFitInfo.unfittedLeptonEtaResC                           = leptonEtaRes.C();
  HitFitInfo.unfittedLeptonEtaResR                           = leptonEtaRes.R();
  HitFitInfo.unfittedLeptonEtaResN                           = leptonEtaRes.N();
  HitFitInfo.unfittedLeptonEtaResInverse                     = leptonEtaRes.inverse();

  HitFitInfo.unfittedLeptonPhiResC                           = leptonPhiRes.C();
  HitFitInfo.unfittedLeptonPhiResR                           = leptonPhiRes.R();
  HitFitInfo.unfittedLeptonPhiResN                           = leptonPhiRes.N();
  HitFitInfo.unfittedLeptonPhiResInverse                     = leptonPhiRes.inverse();

  HitFitInfo.unfittedLeptonResPtFlag                         = unfittedEvent.front().lep(0).res().use_et();

  for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {
    for (size_t j = 0 ; j != HitFitInfo.nHitFitJet ; ++j) {

      math::XYZTLorentzVector unfittedJet(unfittedEvent[h+minChi2Index_].jet(j).p());

      HitFitInfo.unfittedJetMass[h*HitFitInfo.nHitFitJet+j]         = unfittedJet.mass();
      HitFitInfo.unfittedJetE[h*HitFitInfo.nHitFitJet+j]            = unfittedJet.e();
      HitFitInfo.unfittedJetP[h*HitFitInfo.nHitFitJet+j]            = unfittedJet.P();
      HitFitInfo.unfittedJetPx[h*HitFitInfo.nHitFitJet+j]           = unfittedJet.px();
      HitFitInfo.unfittedJetPy[h*HitFitInfo.nHitFitJet+j]           = unfittedJet.py();
      HitFitInfo.unfittedJetPz[h*HitFitInfo.nHitFitJet+j]           = unfittedJet.pz();
      HitFitInfo.unfittedJetPt[h*HitFitInfo.nHitFitJet+j]           = unfittedJet.pt();
      HitFitInfo.unfittedJetEta[h*HitFitInfo.nHitFitJet+j]          = unfittedJet.eta();
      HitFitInfo.unfittedJetTheta[h*HitFitInfo.nHitFitJet+j]        = unfittedJet.theta();
      HitFitInfo.unfittedJetPhi[h*HitFitInfo.nHitFitJet+j]          = unfittedJet.phi();

      hitfit::Resolution jetPRes              = unfittedEvent[h+minChi2Index_].jet(j).res().p_res();
      hitfit::Resolution jetEtaRes            = unfittedEvent[h+minChi2Index_].jet(j).res().eta_res();
      hitfit::Resolution jetPhiRes            = unfittedEvent[h+minChi2Index_].jet(j).res().phi_res();

      HitFitInfo.unfittedJetPResC[h*HitFitInfo.nHitFitJet+j]        = jetPRes.C();
      HitFitInfo.unfittedJetPResR[h*HitFitInfo.nHitFitJet+j]        = jetPRes.R();
      HitFitInfo.unfittedJetPResN[h*HitFitInfo.nHitFitJet+j]        = jetPRes.N();
      HitFitInfo.unfittedJetPResInverse[h*HitFitInfo.nHitFitJet+j]  = jetPRes.inverse();

      HitFitInfo.unfittedJetEtaResC[h*HitFitInfo.nHitFitJet+j]      = jetEtaRes.C();
      HitFitInfo.unfittedJetEtaResR[h*HitFitInfo.nHitFitJet+j]      = jetEtaRes.R();
      HitFitInfo.unfittedJetEtaResN[h*HitFitInfo.nHitFitJet+j]      = jetEtaRes.N();
      HitFitInfo.unfittedJetEtaResInverse[h*HitFitInfo.nHitFitJet+j]= jetEtaRes.inverse();

      HitFitInfo.unfittedJetPhiResC[h*HitFitInfo.nHitFitJet+j]      = jetPhiRes.C();
      HitFitInfo.unfittedJetPhiResR[h*HitFitInfo.nHitFitJet+j]      = jetPhiRes.R();
      HitFitInfo.unfittedJetPhiResN[h*HitFitInfo.nHitFitJet+j]      = jetPhiRes.N();
      HitFitInfo.unfittedJetPhiResInverse[h*HitFitInfo.nHitFitJet+j]= jetPhiRes.inverse();

      HitFitInfo.unfittedJetResPtFlag[h*HitFitInfo.nHitFitJet+j]    = unfittedEvent[h+minChi2Index_].jet(j).res().use_et();

      // Need to take the information from the original pat::Jet
      // unfittedJetIsBTagged
      // unfittedJetBTagProbability
    }
  }

  HitFitInfo.unfittedKtResC                                  = unfittedEvent.front().kt_res().C();
  HitFitInfo.unfittedKtResR                                  = unfittedEvent.front().kt_res().R();
  HitFitInfo.unfittedKtResN                                  = unfittedEvent.front().kt_res().N();
  HitFitInfo.unfittedKtResInverse                            = unfittedEvent.front().kt_res().inverse();

  for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {

    // Copy the jet permutation from the fit results

    hitfit::Lepjets_Event uEvent         = unfittedEvent[h+minChi2Index_];

    // Solve the neutrino pz for both real and complex solutions
    double re_nuz1          = 0.0 ;
    double im_nuz1          = 0.0 ;
    double re_nuz2          = 0.0 ;
    double im_nuz2          = 0.0 ;
    bool real_nuz_sol       = false ;

    //if (HitFit->GetTopFit().args().solve_nu_tmass()) {
    if (HitFit->GetTopGluonFit().args().solve_nu_tmass()) {
      double hadtmass         = hitfit::Top_Decaykin::hadt(uEvent).m();
      real_nuz_sol            = hitfit::Top_Decaykin::solve_nu_tmass(uEvent,
								     hadtmass,
								     re_nuz1,
								     im_nuz1,
								     re_nuz2,
								     im_nuz2);
    }
    else {
      double wmass            = LepWMass_;
      real_nuz_sol            = hitfit::Top_Decaykin::solve_nu(uEvent,
							       wmass,
							       re_nuz1,
							       im_nuz1,
							       re_nuz2,
							       im_nuz2);
    }

    double re_nuz           = 0.0 ;
    double im_nuz           = 0.0 ;

    // In the internal working of mechanism of RunHitFit
    // the results are ordered w.r.t. to their jet permutation,
    // starting from (leptonic b, hadronic b, first hadronic w, second hadronic w).
    // In HitFit jet type code, this correspond to (11,12,13,14)
    // For each jet permutation, there are two neutrino solutions from
    // solving the quadratic equation coming from requiring the leptonic side
    // and hadronic side to have equal mass.
    // Note: No dependence on W and top mass here !
    //
    // Real solution:
    // Even index: for smaller absolute value of neutrino pz.
    // Odd  index: for larger absolute value of neutrino pz.
    //
    // Complex solution:
    // Even index: for imaginary component in the lower-half of the complex plane.
    // Odd index : for imaginary component in the upper-half of the complex plane.

    if (h % 2 == 0) {
      HitFitInfo.NeutrinoSol[h] = false ;
      re_nuz      = re_nuz1;
      im_nuz      = im_nuz1;
    } else {
      HitFitInfo.NeutrinoSol[h] = true  ;
      re_nuz      = re_nuz2 ;
      im_nuz      = im_nuz2 ;
    }

    // if we get real neutrino pz, flag this permutation.

    HitFitInfo.RealNeutrinoSol[h]                    = real_nuz_sol ;

    // The neutrino pz in the event is still zero, we set it to
    // the solution of neutrino_pz we have choose above.

    uEvent.met().setZ(re_nuz);

    // We have calculate the solutions for neutrino pz and set the neutrino pz for
    // one of the two solution.  But we haven't adjust the neutrino energy yet, the
    // neutrino energy was originally equal to sqrt(nu_px*nu_px + nu_py*nu_py)
    // Calculating the invariant mass of the neutrino (or combination of other
    // momenta with the neutrino momenta) may give suprising results.
    // Therefore we adjust the energy of the neutrino to be equal to
    // sqrt(nu_px*nu_px + nu_py*nu_py + nu_pz*nu_pz), equivalently giving the neutrino
    // a mass of zero

    hitfit::adjust_e_for_mass(uEvent.met(),0);

    // Collect all the four-momenta
    math::XYZTLorentzVector unfittedLepton      (uEvent.lep(0).p());
    math::XYZTLorentzVector unfittedNeutrino    (uEvent.met());
    math::XYZTLorentzVector unfittedLepW        (hitfit::Top_Decaykin::lepw(uEvent));
    math::XYZTLorentzVector unfittedLepTop      (hitfit::Top_Decaykin::lept(uEvent));
    math::XYZTLorentzVector unfittedHadW        (hitfit::Top_Decaykin::hadw(uEvent));
    math::XYZTLorentzVector unfittedHadW1       (hitfit::Top_Decaykin::hadw1(uEvent));
    math::XYZTLorentzVector unfittedHadW2       (hitfit::Top_Decaykin::hadw2(uEvent));
    math::XYZTLorentzVector unfittedHadTop      (hitfit::Top_Decaykin::hadt(uEvent));
    math::XYZTLorentzVector unfittedKt          (uEvent.kt());
    math::XYZTLorentzVector unfittedTt          (unfittedLepTop + unfittedHadTop);

    HitFitInfo.unfittedNeutrinoE[h]                        = unfittedNeutrino.e();
    HitFitInfo.unfittedNeutrinoP[h]                        = unfittedNeutrino.P();
    HitFitInfo.unfittedNeutrinoPx[h]                       = unfittedNeutrino.px();
    HitFitInfo.unfittedNeutrinoPy[h]                       = unfittedNeutrino.py();
    HitFitInfo.unfittedNeutrinoPz[h]                       = unfittedNeutrino.pz();
    HitFitInfo.unfittedNeutrinoPzRe[h]                     = re_nuz;
    HitFitInfo.unfittedNeutrinoPzIm[h]                     = im_nuz;
    HitFitInfo.unfittedNeutrinoPt[h]                       = unfittedNeutrino.pt();
    HitFitInfo.unfittedNeutrinoEta[h]                      = unfittedNeutrino.eta();
    HitFitInfo.unfittedNeutrinoTheta[h]                    = unfittedNeutrino.theta();
    HitFitInfo.unfittedNeutrinoPhi[h]                      = unfittedNeutrino.phi();

    HitFitInfo.unfittedLepWMass[h]                         = unfittedLepW.mass();
    HitFitInfo.unfittedLepWE[h]                            = unfittedLepW.e();
    HitFitInfo.unfittedLepWP[h]                            = unfittedLepW.P();
    HitFitInfo.unfittedLepWPx[h]                           = unfittedLepW.px();
    HitFitInfo.unfittedLepWPy[h]                           = unfittedLepW.py();
    HitFitInfo.unfittedLepWPz[h]                           = unfittedLepW.pz();
    HitFitInfo.unfittedLepWPt[h]                           = unfittedLepW.pt();
    HitFitInfo.unfittedLepWEta[h]                          = unfittedLepW.eta();
    HitFitInfo.unfittedLepWTheta[h]                        = unfittedLepW.theta();
    HitFitInfo.unfittedLepWPhi[h]                          = unfittedLepW.phi();

    HitFitInfo.unfittedLepTopMass[h]                       = unfittedLepTop.mass();
    HitFitInfo.unfittedLepTopE[h]                          = unfittedLepTop.e();
    HitFitInfo.unfittedLepTopP[h]                          = unfittedLepTop.P();
    HitFitInfo.unfittedLepTopPx[h]                         = unfittedLepTop.px();
    HitFitInfo.unfittedLepTopPy[h]                         = unfittedLepTop.py();
    HitFitInfo.unfittedLepTopPz[h]                         = unfittedLepTop.pz();
    HitFitInfo.unfittedLepTopPt[h]                         = unfittedLepTop.pt();
    HitFitInfo.unfittedLepTopEta[h]                        = unfittedLepTop.eta();
    HitFitInfo.unfittedLepTopTheta[h]                      = unfittedLepTop.theta();
    HitFitInfo.unfittedLepTopPhi[h]                        = unfittedLepTop.phi();

    HitFitInfo.unfittedHadWMass[h]                         = unfittedHadW.mass();
    HitFitInfo.unfittedHadWE[h]                            = unfittedHadW.e();
    HitFitInfo.unfittedHadWP[h]                            = unfittedHadW.P();
    HitFitInfo.unfittedHadWPx[h]                           = unfittedHadW.px();
    HitFitInfo.unfittedHadWPy[h]                           = unfittedHadW.py();
    HitFitInfo.unfittedHadWPz[h]                           = unfittedHadW.pz();
    HitFitInfo.unfittedHadWPt[h]                           = unfittedHadW.pt();
    HitFitInfo.unfittedHadWEta[h]                          = unfittedHadW.eta();
    HitFitInfo.unfittedHadWTheta[h]                        = unfittedHadW.theta();
    HitFitInfo.unfittedHadWPhi[h]                          = unfittedHadW.phi();

    HitFitInfo.unfittedHadTopMass[h]                       = unfittedHadTop.mass();
    HitFitInfo.unfittedHadTopE[h]                          = unfittedHadTop.e();
    HitFitInfo.unfittedHadTopP[h]                          = unfittedHadTop.P();
    HitFitInfo.unfittedHadTopPx[h]                         = unfittedHadTop.px();
    HitFitInfo.unfittedHadTopPy[h]                         = unfittedHadTop.py();
    HitFitInfo.unfittedHadTopPz[h]                         = unfittedHadTop.pz();
    HitFitInfo.unfittedHadTopPt[h]                         = unfittedHadTop.pt();
    HitFitInfo.unfittedHadTopEta[h]                        = unfittedHadTop.eta();
    HitFitInfo.unfittedHadTopTheta[h]                      = unfittedHadTop.theta();
    HitFitInfo.unfittedHadTopPhi[h]                        = unfittedHadTop.phi();

    HitFitInfo.unfittedTopMass[h]                          = hitfitResult[h+minChi2Index_].utmass();
    HitFitInfo.unfittedTopMassSigma[h]                     = fabs(HitFitInfo.unfittedLepTopMass[h] - HitFitInfo.unfittedHadTopMass[h]) / 2.0 ;

    HitFitInfo.unfittedKtMass[h]                           = unfittedKt.mass();
    HitFitInfo.unfittedKtE[h]                              = unfittedKt.e();
    HitFitInfo.unfittedKtP[h]                              = unfittedKt.P();
    HitFitInfo.unfittedKtPx[h]                             = unfittedKt.px();
    HitFitInfo.unfittedKtPy[h]                             = unfittedKt.py();
    HitFitInfo.unfittedKtPz[h]                             = unfittedKt.pz();
    HitFitInfo.unfittedKtPt[h]                             = unfittedKt.pt();
    HitFitInfo.unfittedKtEta[h]                            = unfittedKt.eta();
    HitFitInfo.unfittedKtTheta[h]                          = unfittedKt.theta();
    HitFitInfo.unfittedKtPhi[h]                            = unfittedKt.phi();

    HitFitInfo.unfittedTtMass[h]                           = unfittedTt.mass();
    HitFitInfo.unfittedTtE[h]                              = unfittedTt.e();
    HitFitInfo.unfittedTtP[h]                              = unfittedTt.P();
    HitFitInfo.unfittedTtPx[h]                             = unfittedTt.px();
    HitFitInfo.unfittedTtPy[h]                             = unfittedTt.py();
    HitFitInfo.unfittedTtPz[h]                             = unfittedTt.pz();
    HitFitInfo.unfittedTtPt[h]                             = unfittedTt.pt();
    HitFitInfo.unfittedTtEta[h]                            = unfittedTt.eta();
    HitFitInfo.unfittedTtTheta[h]                          = unfittedTt.theta();
    HitFitInfo.unfittedTtPhi[h]                            = unfittedTt.phi();

    math::XYZTLorentzVector fittedLepton        (hitfitResult[h+minChi2Index_].ev().lep(0).p());
    math::XYZTLorentzVector fittedNeutrino      (hitfitResult[h+minChi2Index_].ev().met());

    std::vector<math::XYZTLorentzVector> fittedJet;
    for (size_t j = 0 ; j!= HitFitInfo.nHitFitJet ; ++j) {
      fittedJet.push_back(math::XYZTLorentzVector(hitfitResult[h+minChi2Index_].ev().jet(j).p()));
    }

    math::XYZTLorentzVector fittedLepW          (hitfit::Top_Decaykin::lepw(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedLepTop        (hitfit::Top_Decaykin::lept(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedHadW          (hitfit::Top_Decaykin::hadw(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedHadW1         (hitfit::Top_Decaykin::hadw1(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedHadW2         (hitfit::Top_Decaykin::hadw2(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedHadTop        (hitfit::Top_Decaykin::hadt(hitfitResult[h+minChi2Index_].ev()));
    math::XYZTLorentzVector fittedKt            (hitfitResult[h+minChi2Index_].ev().kt());
    math::XYZTLorentzVector fittedTt            (fittedLepTop + fittedHadTop);

    HitFitInfo.fittedLeptonE[h]                            = fittedLepton.e();
    HitFitInfo.fittedLeptonP[h]                            = fittedLepton.P();
    HitFitInfo.fittedLeptonPx[h]                           = fittedLepton.px();
    HitFitInfo.fittedLeptonPy[h]                           = fittedLepton.py();
    HitFitInfo.fittedLeptonPz[h]                           = fittedLepton.pz();
    HitFitInfo.fittedLeptonPt[h]                           = fittedLepton.pt();
    HitFitInfo.fittedLeptonEta[h]                          = fittedLepton.eta();
    HitFitInfo.fittedLeptonTheta[h]                        = fittedLepton.theta();
    HitFitInfo.fittedLeptonPhi[h]                          = fittedLepton.phi();

    for (size_t j = 0 ; j != HitFitInfo.nHitFitJet ; ++j) {

      HitFitInfo.fittedJetE[h*HitFitInfo.nHitFitJet+j]              = fittedJet[j].e();
      HitFitInfo.fittedJetP[h*HitFitInfo.nHitFitJet+j]              = fittedJet[j].P();
      HitFitInfo.fittedJetPx[h*HitFitInfo.nHitFitJet+j]             = fittedJet[j].px();
      HitFitInfo.fittedJetPy[h*HitFitInfo.nHitFitJet+j]             = fittedJet[j].py();
      HitFitInfo.fittedJetPz[h*HitFitInfo.nHitFitJet+j]             = fittedJet[j].pz();
      HitFitInfo.fittedJetPt[h*HitFitInfo.nHitFitJet+j]             = fittedJet[j].pt();
      HitFitInfo.fittedJetEta[h*HitFitInfo.nHitFitJet+j]            = fittedJet[j].eta();
      HitFitInfo.fittedJetTheta[h*HitFitInfo.nHitFitJet+j]          = fittedJet[j].theta();
      HitFitInfo.fittedJetPhi[h*HitFitInfo.nHitFitJet+j]            = fittedJet[j].phi();

    }

    HitFitInfo.fittedNeutrinoE[h]                          = fittedNeutrino.e();
    HitFitInfo.fittedNeutrinoP[h]                          = fittedNeutrino.P();
    HitFitInfo.fittedNeutrinoPx[h]                         = fittedNeutrino.px();
    HitFitInfo.fittedNeutrinoPy[h]                         = fittedNeutrino.py();
    HitFitInfo.fittedNeutrinoPz[h]                         = fittedNeutrino.pz();
    HitFitInfo.fittedNeutrinoPt[h]                         = fittedNeutrino.pt();
    HitFitInfo.fittedNeutrinoEta[h]                        = fittedNeutrino.eta();
    HitFitInfo.fittedNeutrinoTheta[h]                      = fittedNeutrino.theta();
    HitFitInfo.fittedNeutrinoPhi[h]                        = fittedNeutrino.phi();

    HitFitInfo.fittedLepWMass[h]                           = fittedLepW.mass();
    HitFitInfo.fittedLepWE[h]                              = fittedLepW.e();
    HitFitInfo.fittedLepWP[h]                              = fittedLepW.P();
    HitFitInfo.fittedLepWPx[h]                             = fittedLepW.px();
    HitFitInfo.fittedLepWPy[h]                             = fittedLepW.py();
    HitFitInfo.fittedLepWPz[h]                             = fittedLepW.pz();
    HitFitInfo.fittedLepWPt[h]                             = fittedLepW.pt();
    HitFitInfo.fittedLepWEta[h]                            = fittedLepW.eta();
    HitFitInfo.fittedLepWTheta[h]                          = fittedLepW.theta();
    HitFitInfo.fittedLepWPhi[h]                            = fittedLepW.phi();

    HitFitInfo.fittedLepTopMass[h]                         = fittedLepTop.mass();
    HitFitInfo.fittedLepTopE[h]                            = fittedLepTop.e();
    HitFitInfo.fittedLepTopP[h]                            = fittedLepTop.P();
    HitFitInfo.fittedLepTopPx[h]                           = fittedLepTop.px();
    HitFitInfo.fittedLepTopPy[h]                           = fittedLepTop.py();
    HitFitInfo.fittedLepTopPz[h]                           = fittedLepTop.pz();
    HitFitInfo.fittedLepTopPt[h]                           = fittedLepTop.pt();
    HitFitInfo.fittedLepTopEta[h]                          = fittedLepTop.eta();
    HitFitInfo.fittedLepTopTheta[h]                        = fittedLepTop.theta();
    HitFitInfo.fittedLepTopPhi[h]                          = fittedLepTop.phi();

    HitFitInfo.fittedHadWMass[h]                           = fittedHadW.mass();
    HitFitInfo.fittedHadWE[h]                              = fittedHadW.e();
    HitFitInfo.fittedHadWP[h]                              = fittedHadW.P();
    HitFitInfo.fittedHadWPx[h]                             = fittedHadW.px();
    HitFitInfo.fittedHadWPy[h]                             = fittedHadW.py();
    HitFitInfo.fittedHadWPz[h]                             = fittedHadW.pz();
    HitFitInfo.fittedHadWPt[h]                             = fittedHadW.pt();
    HitFitInfo.fittedHadWEta[h]                            = fittedHadW.eta();
    HitFitInfo.fittedHadWTheta[h]                          = fittedHadW.theta();
    HitFitInfo.fittedHadWPhi[h]                            = fittedHadW.phi();

    HitFitInfo.fittedHadTopMass[h]                         = fittedHadTop.mass();
    HitFitInfo.fittedHadTopE[h]                            = fittedHadTop.e();
    HitFitInfo.fittedHadTopP[h]                            = fittedHadTop.P();
    HitFitInfo.fittedHadTopPx[h]                           = fittedHadTop.px();
    HitFitInfo.fittedHadTopPy[h]                           = fittedHadTop.py();
    HitFitInfo.fittedHadTopPz[h]                           = fittedHadTop.pz();
    HitFitInfo.fittedHadTopPt[h]                           = fittedHadTop.pt();
    HitFitInfo.fittedHadTopEta[h]                          = fittedHadTop.eta();
    HitFitInfo.fittedHadTopTheta[h]                        = fittedHadTop.theta();
    HitFitInfo.fittedHadTopPhi[h]                          = fittedHadTop.phi();

    HitFitInfo.fittedKtMass[h]                             = fittedKt.mass();
    HitFitInfo.fittedKtE[h]                                = fittedKt.e();
    HitFitInfo.fittedKtP[h]                                = fittedKt.P();
    HitFitInfo.fittedKtPx[h]                               = fittedKt.px();
    HitFitInfo.fittedKtPy[h]                               = fittedKt.py();
    HitFitInfo.fittedKtPz[h]                               = fittedKt.pz();
    HitFitInfo.fittedKtPt[h]                               = fittedKt.pt();
    HitFitInfo.fittedKtEta[h]                              = fittedKt.eta();
    HitFitInfo.fittedKtTheta[h]                            = fittedKt.theta();
    HitFitInfo.fittedKtPhi[h]                              = fittedKt.phi();

    HitFitInfo.fittedTtMass[h]                             = fittedTt.mass();
    HitFitInfo.fittedTtE[h]                                = fittedTt.e();
    HitFitInfo.fittedTtP[h]                                = fittedTt.P();
    HitFitInfo.fittedTtPx[h]                               = fittedTt.px();
    HitFitInfo.fittedTtPy[h]                               = fittedTt.py();
    HitFitInfo.fittedTtPz[h]                               = fittedTt.pz();
    HitFitInfo.fittedTtPt[h]                               = fittedTt.pt();
    HitFitInfo.fittedTtEta[h]                              = fittedTt.eta();
    HitFitInfo.fittedTtTheta[h]                            = fittedTt.theta();
    HitFitInfo.fittedTtPhi[h]                              = fittedTt.phi();

    HitFitInfo.fittedExcitedQuarkMass[h]                            = hitfitResult[h+minChi2Index_].mt();
    HitFitInfo.fittedExcitedQuarkMassSigma[h]                       = hitfitResult[h+minChi2Index_].sigmt();

  } // for (size_t h = 0 ; h!= nHitFit_ ; ++h) {


  for (size_t h = 0 ; h!= HitFitInfo.nHitFit ; ++h) {

    if (HitFitInfo.Converge[h]) {
      ++HitFitInfo.nHitFitConverge;
    }

  }

  // if the number of converging fits is equal to the total number
  // of fit results, then all fits converge.

  if (HitFitInfo.nHitFitConverge == HitFitInfo.nHitFit) {
    HitFitInfo.AllConverge               = true;
  }

  for (size_t h = 0 ; h!= HitFitInfo.nHitFit ; ++h) {

    if (HitFitInfo.Chi2[h]  < minChi2 && HitFitInfo.Chi2[h]  > 0.0) {
      minChi2                     = HitFitInfo.Chi2[h] ;
      HitFitInfo.MinChi2Index          = h ;
    }

    if (HitFitInfo.Chi2[h]  >= maxChi2 && HitFitInfo.Chi2[h]  > 0.0) {
      maxChi2                     = HitFitInfo.Chi2[h]  ;
      HitFitInfo.MaxChi2Index          = h ;
    }

  }

  if(HitFitInfo.MinChi2Index!=-1)
	  std::cout<<"HitFitInfo.fittedExcitedQuarkMass[HitFitInfo.MinChi2Index] : "<<HitFitInfo.fittedExcitedQuarkMass[HitFitInfo.MinChi2Index]<<std::endl;

  if (HitFitInfo.nHitFit > 0) {
    std::vector<std::pair <size_t,double> > hitfitIndexChi2;
    for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {
      hitfitIndexChi2.push_back(std::pair<size_t,double>(h,HitFitInfo.Chi2[h]));
    }

    std::stable_sort(hitfitIndexChi2.begin(),
		     hitfitIndexChi2.end(),
		     ::IndexedQuantityAbsLessThan<double>);

    for (size_t h = 0 ; h != HitFitInfo.nHitFit ; ++h) {
      HitFitInfo.SortedChi2Index[h]    = hitfitIndexChi2[h].first;
    }
  }

  for (size_t h = 0 ; h!= HitFitInfo.nHitFit  ; ++h) {
    if (HitFitInfo.Converge[h]) {
      HitFitInfo.ExpHalfChi2[h]        =  exp(-0.5*HitFitInfo.Chi2[h]);
      HitFitInfo.SumExpHalfChi2        += exp(-0.5*HitFitInfo.Chi2[h]);
    } else {
      HitFitInfo.ExpHalfChi2[h]        =  0.0;
    }
  }

  // Chi2Probability
  for (size_t h = 0 ; h!= HitFitInfo.nHitFit  ; ++h) {
    if (HitFitInfo.Converge[h]) {
      HitFitInfo.Chi2Probability[h]    =  HitFitInfo.ExpHalfChi2[h];
      HitFitInfo.SumChi2Probability    += HitFitInfo.ExpHalfChi2[h];
    } else {
      HitFitInfo.Chi2Probability[h]    =  0.0;
    }
  }


  // BTagProbability
  for (size_t h = 0 ; h!= HitFitInfo.nHitFit  ; ++h) {
    // do nothing for now
  }


  // Chi2BTagProbability, so far only Chi2BTag
  for (size_t h = 0 ; h!= HitFitInfo.nHitFit  ; ++h) {
    if (HitFitInfo.Converge[h]) {
      HitFitInfo.Chi2Probability[h]    =  HitFitInfo.ExpHalfChi2[h]/HitFitInfo.SumExpHalfChi2;
      HitFitInfo.SumChi2Probability    += HitFitInfo.Chi2Probability[h];
    }
  }

  // Chi2 Weight
  for (size_t h = 0 ; h!= HitFitInfo.nHitFit ; ++h) {
    if (HitFitInfo.Converge[h]) {
      HitFitInfo.Chi2Weight[h]         =  HitFitInfo.Chi2Probability[h]/HitFitInfo.SumChi2Probability;
    } else {
      HitFitInfo.Chi2Weight[h]         =  0.0;
    }
  }

  // BTag Weight

  // Chi2BTag Weight

  // so far only chi2
  // combination of Chi2 and BTag to be implemented
  for (size_t h = 0 ; h!= HitFitInfo.nHitFit ; ++h) {
    if (HitFitInfo.Converge[h]) {
      HitFitInfo.Weight[h]             =  HitFitInfo.Chi2Weight[h];
    } else {
      HitFitInfo.Weight[h]             =  0.0;
    }
  }

  if(debug_) std::cout << " Done with fillHitFitInfo\n";
  return ;

}

} // void doHitFitForExcitedQuark::fillHitFitInfo()

