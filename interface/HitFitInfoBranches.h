#include <TTree.h>

static const unsigned int kMIN_HITFIT_JET=4;
static const unsigned int kMIN_HITFIT_JET_TSTAR=6;
static const unsigned int kMAX_HITFIT_JET=8;
static const unsigned int kMAX_HITFIT=10080;
//static const unsigned int kMAX_HITFIT=1680;
static const unsigned int kMAX_HITFIT_VAR=24;
static const unsigned int kMAX_HITFIT_DISCRIMINANT=8;

class HitFitInfoBranches {
 public:
    //__________________________________________________________________
    //
    // HitFit analysis variables

    // Naming convention:

    // Variables whose name starts with 'hitfit' are:
    // - MC quantities which are not related to the reconstructed event.
    // - Reconstructed quantities which remain constant before and after fit.

    // Variables whose name starts with 'unfitted' and 'fitted'
    // are:
    // - reconstructed quantities which MAY changes before and after fit
    // - reconstructed quantities which are depend on specific permutation
    //   hypothesis
    // Example: for jets, we assign the parton level corrections and
    // resolution after a particular flavor hypothesis is assigned.
    // Consequently, a jet's properties may change after a specific
    // permutation is deciced.

    Bool_t      hitfit;
    UInt_t      nHitFit;
    UInt_t      nHitFitJet;
    UInt_t      nHitFitXnHitFitJet;
    UInt_t      hitfitNX;
    UInt_t      hitfitNY;
    UInt_t      nHitFitXnX;
    UInt_t      nHitFitXnY;

    // HitFit requires the complete jet information, as the
    // kinematic fit's results strongly depend on jet information
    // The 'hitfitJet' leaves contain jet information BEFORE a specific
    // jet permutation is made.
    // The 'unfittedJet' leaves contain jet information FOR a specific
    // jet permutation, as the parton level corrections and jet resolution
    // is dependent on the parton type assignment.

    Int_t       JetIndex                        [kMAX_HITFIT_JET];
    Int_t       JetInfoIndex                    [kMAX_HITFIT_JET];

    Int_t       JetPartonType                   [kMAX_HITFIT_JET];
    char        JetPartonPermutation            [kMAX_HITFIT_JET];

    Int_t       JetGenJetType                   [kMAX_HITFIT_JET];
    char        JetGenJetPermutation            [kMAX_HITFIT_JET];

    // A few definitions here:
    //
    // Hypothetically assigned jet types: For each jet considered in the fit,
    // a specific hypothesis is made for the jet types which evolve
    // into that specific jet.
    // HitFit recognizes the following jet types (see the header file for
    // Lepjets_Event_Jet class in HitFit package).
    //
    // 0: ISR, gluon
    // 11: leptonic b, b jet from the leptonic top quark
    // 12: hadronic b, b jet from the hadronic top quark
    // 13: hadronic W with higher pT, hadronic jet from W boson
    // 14: hadronic W with lower pT, hadronic jet from W boson
    // 15: b jet from Higgs decay
    // 20: unknown, can also be gluon
    //
    // A permutation hypothesis is written in the following notation:
    // a series of integer (in an array or vector), starting from the highest
    // pT jet.
    //
    // Parton-level assigned jet types: For each jet considered in the fit,
    // try to find a specific parton matched with the jet.  Based on the flavor
    // of the parton matched with the jet, a jet type for that jet is deduced.
    // However, it is not guaranteed that:
    // - A match will always be found for each jet
    // - The set of parton matched with the jets will contain all the
    //   quarks coming from ttbar decay.
    // Both situation will cause mismatch between the hypothetical jet types
    // and the parton-level jet types.
    //
    // Correct: The hypothetical jet types and the parton-level jet types
    // are identical.
    // Identical to: CorrectLepB && CorrectHadTop
    //
    // SemiCorrect:
    // Matched: Leptonic b and one of the hadronic W jets
    // Mismatch: hadronic b and the other hadronic W jet are swapped.
    // In this permutation, the hypothesis assigned the set of jets
    // to both leptonic top and hadronic top, but the hypothetical permutation
    // of jets from hadronic top doesn't match the parton-level permutation.
    // Identical to: CorrectLepB && SemiCorrectHadTop
    //
    // CorrectLepB:
    // The jet which is hypothesized to be b-jet from leptonic top quark,
    // is matched to the parton-level b quark coming from parton-level leptonic
    // top quark.
    // No statement is made about the other jets.

    // CorrectHadB:
    // The jet which is hypothesized to be b-jet from hadronic top quark,
    // is matched to the parton-level b quark coming from parton-level hadronic
    // top quark.
    // No statement is made about the other jets.
    //
    // CorrectHadW:
    // A set of two jets which are hypothesized to come from hadronic W are
    // matched to a set of two parton-level quarks coming from W decay coming
    // from hadronic top quark.
    // No statement is made about the other jets.
    //
    // CorrectHadTop:
    // Three jets which are hypothesized to come from hadronic top are matched
    // to three parton-level quarks which come from hadronic top, and the
    // permutation
    // is correct.
    // Identical to: CorrectHadB && CorrectHadW
    // No statement is made about the other jets.
    //
    // SemiCorrectHadTop:
    // The set of three jets which are hypothesized to come from
    // hadronic top, are matched to three parton-level quarks which come
    // from hadronic top, but the permutation/type assignment is incorrect.
    // No statement is made about the other jets.
    //
    // CorrectTwoJet:
    // A set of two jets amoung four which are hypothesized to come from ttbar,
    // are matched one-to-one with two parton-level quarks which come
    // from ttbar.
    // The two others are NOT matched to any quark from ttbar.
    //
    // SemiCorrectTwoJet:
    // A set of two jets amoung four which are hypothesized to come from ttbar,
    // are matched to a set of two parton-level quarks which come from ttbar,
    // but
    // the permutation is incorrect.
    // The two others are NOT matched to any quark from ttbar.
    //
    // CorrectThreeJet:
    // A set of three jets amoung four which are hypothesized to come from
    // ttbar,
    // are matched one-to-one with three parton-level quarks which come
    // from ttbar.
    // The one other jet is NOT matched to any quark from ttbar.
    //
    // SemiCorrectThreeJet:
    // A set of three jets amoung four which are hypothesized to come from
    // ttbar,
    // are matched to a set of three parton-level quarks which come from
    // ttbar, but
    // the permutation is incorrect.
    // The one other jet is NOT matched to any quark from ttbar.
    //
    // SemiCorrectFourJet:
    // The hypothesized b jets are actually coming from two hadronic W, while
    // the hypothesized hadronic W jets are actually coming from parton-level
    // b.
    //

    Int_t       JetType             [kMAX_HITFIT*kMAX_HITFIT_JET];

    char        Permutation         [kMAX_HITFIT*kMAX_HITFIT_JET];
    Bool_t      CorrectPermutation                  [kMAX_HITFIT];
    Bool_t      SemiCorrectPermutation              [kMAX_HITFIT];
    Bool_t      CorrectLepB                         [kMAX_HITFIT];
    Bool_t      CorrectHadB                         [kMAX_HITFIT];
    Bool_t      CorrectHadW                         [kMAX_HITFIT];
    Bool_t      CorrectHadTop                       [kMAX_HITFIT];
    Bool_t      SemiCorrectHadTop                   [kMAX_HITFIT];
    Bool_t      CorrectTwoJet                       [kMAX_HITFIT];
    Bool_t      SemiCorrectTwoJet                   [kMAX_HITFIT];
    Bool_t      CorrectThreeJet                     [kMAX_HITFIT];
    Bool_t      SemiCorrectThreeJet                 [kMAX_HITFIT];
    Bool_t      SemiCorrectFourJet                  [kMAX_HITFIT];

    UInt_t      NJetCorrectPermutation              [kMAX_HITFIT];
    UInt_t      MaxNJetCorrectPermutation;
    Int_t       CorrectPermutationIndex                       [2];

    Int_t       LepInfoIndex;

    Double_t    unfittedLeptonPResC;
    Double_t    unfittedLeptonPResR;
    Double_t    unfittedLeptonPResN;
    Bool_t      unfittedLeptonPResInverse;

    Double_t    unfittedLeptonEtaResC;
    Double_t    unfittedLeptonEtaResR;
    Double_t    unfittedLeptonEtaResN;
    Bool_t      unfittedLeptonEtaResInverse;

    Double_t    unfittedLeptonPhiResC;
    Double_t    unfittedLeptonPhiResR;
    Double_t    unfittedLeptonPhiResN;
    Bool_t      unfittedLeptonPhiResInverse;

    Bool_t      unfittedLeptonResPtFlag;

    // The unfitted jet kinematics for each permutation may be different
    // for each permutation. The reason is: the parton-level
    // jet energy correction depends on the type of jet assumed for
    // each permutation!
    Double_t    unfittedJetMass           [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetE              [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetP              [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPx             [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPy             [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPz             [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPt             [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetEta            [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetTheta          [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPhi            [kMAX_HITFIT*kMAX_HITFIT_JET];

    Double_t    unfittedJetPResC          [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPResR          [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPResN          [kMAX_HITFIT*kMAX_HITFIT_JET];
    Bool_t      unfittedJetPResInverse    [kMAX_HITFIT*kMAX_HITFIT_JET];

    Double_t    unfittedJetEtaResC        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetEtaResR        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetEtaResN        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Bool_t      unfittedJetEtaResInverse  [kMAX_HITFIT*kMAX_HITFIT_JET];

    Double_t    unfittedJetPhiResC        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPhiResR        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetPhiResN        [kMAX_HITFIT*kMAX_HITFIT_JET];
    Bool_t      unfittedJetPhiResInverse  [kMAX_HITFIT*kMAX_HITFIT_JET];

    Bool_t      unfittedJetResPtFlag      [kMAX_HITFIT*kMAX_HITFIT_JET];

    Bool_t      unfittedJetIsBTagged      [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    unfittedJetBTagProbability[kMAX_HITFIT*kMAX_HITFIT_JET];

   // real solution    : false=smaller solution, true=larger solution
    // complex solution : false=lower-half plane, true=upper-half plane
    Bool_t      NeutrinoSol                         [kMAX_HITFIT];
    // false=complex solution, true=real solution
    Bool_t      RealNeutrinoSol                     [kMAX_HITFIT];

    Double_t    unfittedNeutrinoE                         [kMAX_HITFIT];
    Double_t    unfittedNeutrinoP                         [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPx                        [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPy                        [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPz                        [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPzRe                      [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPzIm                      [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPt                        [kMAX_HITFIT]; //
    Double_t    unfittedNeutrinoEta                       [kMAX_HITFIT];
    Double_t    unfittedNeutrinoTheta                     [kMAX_HITFIT];
    Double_t    unfittedNeutrinoPhi                       [kMAX_HITFIT]; //

    Double_t    unfittedLepWMass                          [kMAX_HITFIT];
    Double_t    unfittedLepWE                             [kMAX_HITFIT];
    Double_t    unfittedLepWP                             [kMAX_HITFIT];
    Double_t    unfittedLepWPx                            [kMAX_HITFIT];
    Double_t    unfittedLepWPy                            [kMAX_HITFIT];
    Double_t    unfittedLepWPz                            [kMAX_HITFIT];
    Double_t    unfittedLepWPt                            [kMAX_HITFIT];
    Double_t    unfittedLepWEta                           [kMAX_HITFIT];
    Double_t    unfittedLepWTheta                         [kMAX_HITFIT];
    Double_t    unfittedLepWPhi                           [kMAX_HITFIT];

    Double_t    unfittedLepTopMass                        [kMAX_HITFIT];
    Double_t    unfittedLepTopE                           [kMAX_HITFIT];
    Double_t    unfittedLepTopP                           [kMAX_HITFIT];
    Double_t    unfittedLepTopPx                          [kMAX_HITFIT];
    Double_t    unfittedLepTopPy                          [kMAX_HITFIT];
    Double_t    unfittedLepTopPz                          [kMAX_HITFIT];
    Double_t    unfittedLepTopPt                          [kMAX_HITFIT];
    Double_t    unfittedLepTopEta                         [kMAX_HITFIT];
    Double_t    unfittedLepTopTheta                       [kMAX_HITFIT];
    Double_t    unfittedLepTopPhi                         [kMAX_HITFIT];

    Double_t    unfittedHadWMass                          [kMAX_HITFIT];
    Double_t    unfittedHadWE                             [kMAX_HITFIT];
    Double_t    unfittedHadWP                             [kMAX_HITFIT];
    Double_t    unfittedHadWPx                            [kMAX_HITFIT];
    Double_t    unfittedHadWPy                            [kMAX_HITFIT];
    Double_t    unfittedHadWPz                            [kMAX_HITFIT];
    Double_t    unfittedHadWPt                            [kMAX_HITFIT];
    Double_t    unfittedHadWEta                           [kMAX_HITFIT];
    Double_t    unfittedHadWTheta                         [kMAX_HITFIT];
    Double_t    unfittedHadWPhi                           [kMAX_HITFIT];

    Double_t    unfittedHadTopMass                        [kMAX_HITFIT];
    Double_t    unfittedHadTopE                           [kMAX_HITFIT];
    Double_t    unfittedHadTopP                           [kMAX_HITFIT];
    Double_t    unfittedHadTopPx                          [kMAX_HITFIT];
    Double_t    unfittedHadTopPy                          [kMAX_HITFIT];
    Double_t    unfittedHadTopPz                          [kMAX_HITFIT];
    Double_t    unfittedHadTopPt                          [kMAX_HITFIT];
    Double_t    unfittedHadTopEta                         [kMAX_HITFIT];
    Double_t    unfittedHadTopTheta                       [kMAX_HITFIT];
    Double_t    unfittedHadTopPhi                         [kMAX_HITFIT];

    Double_t    unfittedTopMass                           [kMAX_HITFIT];
    Double_t    unfittedTopMassSigma                      [kMAX_HITFIT];

    Double_t    unfittedKtMass                            [kMAX_HITFIT];
    Double_t    unfittedKtE                               [kMAX_HITFIT];
    Double_t    unfittedKtP                               [kMAX_HITFIT];
    Double_t    unfittedKtPx                              [kMAX_HITFIT];
    Double_t    unfittedKtPy                              [kMAX_HITFIT];
    Double_t    unfittedKtPz                              [kMAX_HITFIT];
    Double_t    unfittedKtPt                              [kMAX_HITFIT];
    Double_t    unfittedKtEta                             [kMAX_HITFIT];
    Double_t    unfittedKtTheta                           [kMAX_HITFIT];
    Double_t    unfittedKtPhi                             [kMAX_HITFIT];

    Double_t    unfittedKtResC;
    Double_t    unfittedKtResR;
    Double_t    unfittedKtResN;
    Bool_t      unfittedKtResInverse;

    Double_t    unfittedTtMass                            [kMAX_HITFIT];
    Double_t    unfittedTtE                               [kMAX_HITFIT];
    Double_t    unfittedTtP                               [kMAX_HITFIT];
    Double_t    unfittedTtPx                              [kMAX_HITFIT];
    Double_t    unfittedTtPy                              [kMAX_HITFIT];
    Double_t    unfittedTtPz                              [kMAX_HITFIT];
    Double_t    unfittedTtPt                              [kMAX_HITFIT];
    Double_t    unfittedTtEta                             [kMAX_HITFIT];
    Double_t    unfittedTtTheta                           [kMAX_HITFIT];
    Double_t    unfittedTtPhi                             [kMAX_HITFIT];

    Double_t    fittedLeptonE                             [kMAX_HITFIT];
    Double_t    fittedLeptonP                             [kMAX_HITFIT];
    Double_t    fittedLeptonPx                            [kMAX_HITFIT];
    Double_t    fittedLeptonPy                            [kMAX_HITFIT];
    Double_t    fittedLeptonPz                            [kMAX_HITFIT];
    Double_t    fittedLeptonPt                            [kMAX_HITFIT];
    Double_t    fittedLeptonEta                           [kMAX_HITFIT];
    Double_t    fittedLeptonTheta                         [kMAX_HITFIT];
    Double_t    fittedLeptonPhi                           [kMAX_HITFIT];

    Double_t    fittedJetMass             [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetE                [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetP                [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetPx               [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetPy               [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetPz               [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetPt               [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetEta              [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetTheta            [kMAX_HITFIT*kMAX_HITFIT_JET];
    Double_t    fittedJetPhi              [kMAX_HITFIT*kMAX_HITFIT_JET];

    Double_t    fittedNeutrinoE                           [kMAX_HITFIT];
    Double_t    fittedNeutrinoP                           [kMAX_HITFIT];
    Double_t    fittedNeutrinoPx                          [kMAX_HITFIT];
    Double_t    fittedNeutrinoPy                          [kMAX_HITFIT];
    Double_t    fittedNeutrinoPz                          [kMAX_HITFIT];
    Double_t    fittedNeutrinoPt                          [kMAX_HITFIT];
    Double_t    fittedNeutrinoEta                         [kMAX_HITFIT];
    Double_t    fittedNeutrinoTheta                       [kMAX_HITFIT];
    Double_t    fittedNeutrinoPhi                         [kMAX_HITFIT];

    Double_t    fittedLepWMass                            [kMAX_HITFIT];
    Double_t    fittedLepWE                               [kMAX_HITFIT];
    Double_t    fittedLepWP                               [kMAX_HITFIT];
    Double_t    fittedLepWPx                              [kMAX_HITFIT];
    Double_t    fittedLepWPy                              [kMAX_HITFIT];
    Double_t    fittedLepWPz                              [kMAX_HITFIT];
    Double_t    fittedLepWPt                              [kMAX_HITFIT];
    Double_t    fittedLepWEta                             [kMAX_HITFIT];
    Double_t    fittedLepWTheta                           [kMAX_HITFIT];
    Double_t    fittedLepWPhi                             [kMAX_HITFIT];

    Double_t    fittedLepTopMass                          [kMAX_HITFIT];
    Double_t    fittedLepTopE                             [kMAX_HITFIT];
    Double_t    fittedLepTopP                             [kMAX_HITFIT];
    Double_t    fittedLepTopPx                            [kMAX_HITFIT];
    Double_t    fittedLepTopPy                            [kMAX_HITFIT];
    Double_t    fittedLepTopPz                            [kMAX_HITFIT];
    Double_t    fittedLepTopPt                            [kMAX_HITFIT];
    Double_t    fittedLepTopEta                           [kMAX_HITFIT];
    Double_t    fittedLepTopTheta                         [kMAX_HITFIT];
    Double_t    fittedLepTopPhi                           [kMAX_HITFIT];

    Double_t    fittedHadWMass                            [kMAX_HITFIT];
    Double_t    fittedHadWE                               [kMAX_HITFIT];
    Double_t    fittedHadWP                               [kMAX_HITFIT];
    Double_t    fittedHadWPx                              [kMAX_HITFIT];
    Double_t    fittedHadWPy                              [kMAX_HITFIT];
    Double_t    fittedHadWPz                              [kMAX_HITFIT];
    Double_t    fittedHadWPt                              [kMAX_HITFIT];
    Double_t    fittedHadWEta                             [kMAX_HITFIT];
    Double_t    fittedHadWTheta                           [kMAX_HITFIT];
    Double_t    fittedHadWPhi                             [kMAX_HITFIT];

    Double_t    fittedHadTopMass                          [kMAX_HITFIT];
    Double_t    fittedHadTopE                             [kMAX_HITFIT];
    Double_t    fittedHadTopP                             [kMAX_HITFIT];
    Double_t    fittedHadTopPx                            [kMAX_HITFIT];
    Double_t    fittedHadTopPy                            [kMAX_HITFIT];
    Double_t    fittedHadTopPz                            [kMAX_HITFIT];
    Double_t    fittedHadTopPt                            [kMAX_HITFIT];
    Double_t    fittedHadTopEta                           [kMAX_HITFIT];
    Double_t    fittedHadTopTheta                         [kMAX_HITFIT];
    Double_t    fittedHadTopPhi                           [kMAX_HITFIT];

    Double_t    fittedTopMass                             [kMAX_HITFIT];
    Double_t    fittedTopMassSigma                        [kMAX_HITFIT];

    Double_t    fittedExcitedQuarkMass                             [kMAX_HITFIT];
    Double_t    fittedExcitedQuarkMassSigma                        [kMAX_HITFIT];

    Double_t    fittedKtMass                              [kMAX_HITFIT];
    Double_t    fittedKtE                                 [kMAX_HITFIT];
    Double_t    fittedKtP                                 [kMAX_HITFIT];
    Double_t    fittedKtPx                                [kMAX_HITFIT];
    Double_t    fittedKtPy                                [kMAX_HITFIT];
    Double_t    fittedKtPz                                [kMAX_HITFIT];
    Double_t    fittedKtPt                                [kMAX_HITFIT];
    Double_t    fittedKtEta                               [kMAX_HITFIT];
    Double_t    fittedKtTheta                             [kMAX_HITFIT];
    Double_t    fittedKtPhi                               [kMAX_HITFIT];

    Double_t    fittedTtMass                              [kMAX_HITFIT];
    Double_t    fittedTtE                                 [kMAX_HITFIT];
    Double_t    fittedTtP                                 [kMAX_HITFIT];
    Double_t    fittedTtPx                                [kMAX_HITFIT];
    Double_t    fittedTtPy                                [kMAX_HITFIT];
    Double_t    fittedTtPz                                [kMAX_HITFIT];
    Double_t    fittedTtPt                                [kMAX_HITFIT];
    Double_t    fittedTtEta                               [kMAX_HITFIT];
    Double_t    fittedTtTheta                             [kMAX_HITFIT];
    Double_t    fittedTtPhi                               [kMAX_HITFIT];

    Double_t    Chi2                                [kMAX_HITFIT];
    Bool_t      Converge                            [kMAX_HITFIT];
    Double_t    PullX               [kMAX_HITFIT*kMAX_HITFIT_VAR];
    Double_t    PullY               [kMAX_HITFIT*kMAX_HITFIT_VAR];

    UInt_t      nHitFitConverge;
    Bool_t      AllConverge;
    Int_t       MinChi2Index;
    Int_t       MaxChi2Index;
    Int_t       SortedChi2Index                     [kMAX_HITFIT];

    Double_t    ExpHalfChi2                         [kMAX_HITFIT];
    Double_t    SumExpHalfChi2;

    Double_t    Chi2Probability                     [kMAX_HITFIT];
    Double_t    BTagProbability                     [kMAX_HITFIT];
    Double_t    Chi2BTagProbability                 [kMAX_HITFIT];

    Double_t    SumChi2Probability;
    Double_t    SumBTagProbability;
    Double_t    SumChi2BTagProbability;

    Double_t    Chi2Weight                          [kMAX_HITFIT];
    Double_t    BTagWeight                          [kMAX_HITFIT];
    Double_t    Chi2BTagWeight                      [kMAX_HITFIT];

    Double_t    Weight                              [kMAX_HITFIT];

    void RegisterTree(TTree *root) {
      root->Branch("HitFitInfo.hitfit"	     		 , &hitfit	  		 , "HitFitInfo.hitfit/O"     );
      root->Branch("HitFitInfo.nHitFit"	     		 , &nHitFit	  		 , "HitFitInfo.nHitFit/i"    );
      root->Branch("HitFitInfo.nHitFitJet"	     	 , &nHitFitJet	  		 , "HitFitInfo.nHitFitJet/i" );
      root->Branch("HitFitInfo.nHitFitXnHitFitJet"     	 , &nHitFitXnHitFitJet 		 , "HitFitInfo.nHitFitXnHitFitJet/i" );
      root->Branch("HitFitInfo.hitfitNX"     		 , &hitfitNX	  		 , "HitFitInfo.hitFitNX/i"    );
      root->Branch("HitFitInfo.hitfitNY"     		 , &hitfitNY	  		 , "HitFitInfo.hitFitNY/i"    );
      root->Branch("HitFitInfo.nHitFitXnX"     		 , &nHitFitXnX	  		 , "HitFitInfo.nHitFitXnX/i"    );
      root->Branch("HitFitInfo.nHitFitXnY"     		 , &nHitFitXnY	  		 , "HitFitInfo.nHitFitXnY/i"    );


      root->Branch("HitFitInfo.JetIndex", &JetIndex[0]	, "HitFitInfo.JetIndex[HitFitInfo.nHitFitJet]/I"	);
      root->Branch("HitFitInfo.JetInfoIndex", &JetInfoIndex[0]	, "HitFitInfo.JetInfoIndex[HitFitInfo.nHitFitJet]/I"	);

      root->Branch("HitFitInfo.JetPartonType" , &JetPartonType[0]	, "HitFitInfo.JetPartonType[HitFitInfo.nHitFitJet]/I"   );
      root->Branch("HitFitInfo.JetPartonPermutation" , &JetPartonPermutation[0]	, "HitFitInfo.JetPartonPermutation[HitFitInfo.nHitFitJet]/B"   );

      root->Branch("HitFitInfo.JetGenJetType" , &JetGenJetType[0]	, "HitFitInfo.JetGenJetType[HitFitInfo.nHitFitJet]/I"   );
      root->Branch("HitFitInfo.JetGenJetPermutation" , &JetGenJetPermutation[0]	, "HitFitInfo.JetGenJetPermutation[HitFitInfo.nHitFitJet]/B"   );

      root->Branch("HitFitInfo.JetType" , &JetType[0]	, "HitFitInfo.JetType[HitFitInfo.nHitFitXnHitFitJet]/I"   );
      root->Branch("HitFitInfo.Permutation" , &Permutation[0]	, "HitFitInfo.Permutation[HitFitInfo.nHitFitXnHitFitJet]/B"   );
      root->Branch("HitFitInfo.CorrectPermutation" , &CorrectPermutation[0]	, "HitFitInfo.CorrectPermutation[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.SemiCorrectPermutation" , &SemiCorrectPermutation[0]	, "HitFitInfo.SemiCorrectPermutation[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectLepB" , &CorrectLepB[0]	, "HitFitInfo.CorrectLepB[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectHadB" , &CorrectHadB[0]	, "HitFitInfo.CorrectHadB[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectHadW" , &CorrectHadW[0]	, "HitFitInfo.CorrectHadW[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectHadTop" , &CorrectHadTop[0]	, "HitFitInfo.CorrectHadTop[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.SemiCorrectHadTop" , &SemiCorrectHadTop[0]	, "HitFitInfo.SemiCorrectHadTop[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectTwoJet" , &CorrectTwoJet[0]	, "HitFitInfo.CorrectTwoJet[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.SemiCorrectTwoJet" , &SemiCorrectTwoJet[0]	, "HitFitInfo.SemiCorrectTwoJet[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.CorrectThreeJet" , &CorrectThreeJet[0]	, "HitFitInfo.CorrectThreeJet[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.SemiCorrectThreeJet" , &SemiCorrectThreeJet[0]	, "HitFitInfo.SemiCorrectThreeJet[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.SemiCorrectFourJet" , &SemiCorrectFourJet[0]	, "HitFitInfo.SemiCorrectFourJet[HitFitInfo.nHitFit]/O"   );

      root->Branch("HitFitInfo.NJetCorrectPermutation" , &NJetCorrectPermutation[0]	, "HitFitInfo.NJetCorrectPermutation[HitFitInfo.nHitFit]/i"   );
      root->Branch("HitFitInfo.MaxNJetCorrectPermutation" , &MaxNJetCorrectPermutation	 , "HitFitInfo.MaxNJetCorrectPermutation/i"    );
      root->Branch("HitFitInfo.CorrectPermutationIndex" , &CorrectPermutationIndex[0]	, "HitFitInfo.CorrectPermutationIndex[2]/I"   );

      root->Branch("HitFitInfo.LepInfoIndex"  , &LepInfoIndex  , "HitFitInfo.LepInfoIndex/I" );

      root->Branch("HitFitInfo.unfittedLeptonPResC"      , &unfittedLeptonPResC      , "HitFitInfo.unfittedLeptonPResC/D" );
      root->Branch("HitFitInfo.unfittedLeptonPResR"      , &unfittedLeptonPResR      , "HitFitInfo.unfittedLeptonPResR/D" );
      root->Branch("HitFitInfo.unfittedLeptonPResN"      , &unfittedLeptonPResN      , "HitFitInfo.unfittedLeptonPResN/D" );
      root->Branch("HitFitInfo.unfittedLeptonPResInverse", &unfittedLeptonPResInverse, "HitFitInfo.unfittedLeptonPResInverse/O" );

      root->Branch("HitFitInfo.unfittedLeptonEtaResC"      , &unfittedLeptonEtaResC      , "HitFitInfo.unfittedLeptonEtaResC/D" );
      root->Branch("HitFitInfo.unfittedLeptonEtaResR"      , &unfittedLeptonEtaResR      , "HitFitInfo.unfittedLeptonEtaResR/D" );
      root->Branch("HitFitInfo.unfittedLeptonEtaResN"      , &unfittedLeptonEtaResN      , "HitFitInfo.unfittedLeptonEtaResN/D" );
      root->Branch("HitFitInfo.unfittedLeptonEtaResInverse", &unfittedLeptonEtaResInverse, "HitFitInfo.unfittedLeptonEtaResInverse/O" );

      root->Branch("HitFitInfo.unfittedLeptonPhiResC"      , &unfittedLeptonPhiResC      , "HitFitInfo.unfittedLeptonPhiResC/D" );
      root->Branch("HitFitInfo.unfittedLeptonPhiResR"      , &unfittedLeptonPhiResR      , "HitFitInfo.unfittedLeptonPhiResR/D" );
      root->Branch("HitFitInfo.unfittedLeptonPhiResN"      , &unfittedLeptonPhiResN      , "HitFitInfo.unfittedLeptonPhiResN/D" );
      root->Branch("HitFitInfo.unfittedLeptonPhiResInverse", &unfittedLeptonPhiResInverse, "HitFitInfo.unfittedLeptonPhiResInverse/O" );

      root->Branch("HitFitInfo.unfittedLeptonResPtFlag", &unfittedLeptonResPtFlag, "HitFitInfo.unfittedLeptonResPtFlag/O" );

      root->Branch("HitFitInfo.unfittedJetMass"	, &unfittedJetMass[0]	, "HitFitInfo.unfittedJetMass[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetE"	, &unfittedJetE[0]	, "HitFitInfo.unfittedJetE[HitFitInfo.nHitFitXnHitFitJet]/D"		);
      root->Branch("HitFitInfo.unfittedJetP"	, &unfittedJetP[0]	, "HitFitInfo.unfittedJetP[HitFitInfo.nHitFitXnHitFitJet]/D"		);
      root->Branch("HitFitInfo.unfittedJetPx"	, &unfittedJetPx[0]	, "HitFitInfo.unfittedJetPx[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPy"	, &unfittedJetPy[0]	, "HitFitInfo.unfittedJetPy[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPz"	, &unfittedJetPz[0]	, "HitFitInfo.unfittedJetPz[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPt"	, &unfittedJetPt[0]	, "HitFitInfo.unfittedJetPt[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetEta"	, &unfittedJetEta[0]	, "HitFitInfo.unfittedJetEta[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetTheta", &unfittedJetTheta[0]	, "HitFitInfo.unfittedJetTheta[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPhi"	, &unfittedJetPhi[0]	, "HitFitInfo.unfittedJetPhi[HitFitInfo.nHitFitXnHitFitJet]/D"	);

      root->Branch("HitFitInfo.unfittedJetPResC", &unfittedJetPResC[0]	, "HitFitInfo.unfittedJetPResC[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPResR", &unfittedJetPResR[0]	, "HitFitInfo.unfittedJetPResR[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPResN", &unfittedJetPResN[0]	, "HitFitInfo.unfittedJetPResN[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPResInverse", &unfittedJetPResInverse[0]	, "HitFitInfo.unfittedJetPResInverse[HitFitInfo.nHitFitXnHitFitJet]/O"	);

      root->Branch("HitFitInfo.unfittedJetEtaResC", &unfittedJetEtaResC[0]	, "HitFitInfo.unfittedJetEtaResC[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetEtaResR", &unfittedJetEtaResR[0]	, "HitFitInfo.unfittedJetEtaResR[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetEtaResN", &unfittedJetEtaResN[0]	, "HitFitInfo.unfittedJetEtaResN[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetEtaResInverse", &unfittedJetEtaResInverse[0]	, "HitFitInfo.unfittedJetEtaResInverse[HitFitInfo.nHitFitXnHitFitJet]/O"	);

      root->Branch("HitFitInfo.unfittedJetPhiResC", &unfittedJetPhiResC[0]	, "HitFitInfo.unfittedJetPhiResC[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPhiResR", &unfittedJetPhiResR[0]	, "HitFitInfo.unfittedJetPhiResR[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPhiResN", &unfittedJetPhiResN[0]	, "HitFitInfo.unfittedJetPhiResN[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.unfittedJetPhiResInverse", &unfittedJetPhiResInverse[0]	, "HitFitInfo.unfittedJetPhiResInverse[HitFitInfo.nHitFitXnHitFitJet]/O"	);

      root->Branch("HitFitInfo.unfittedJetResPtFlag", &unfittedJetResPtFlag[0]	, "HitFitInfo.unfittedJetResPtFlag[HitFitInfo.nHitFitXnHitFitJet]/O"	);

      root->Branch("HitFitInfo.unfittedJetIsBTagged", &unfittedJetIsBTagged[0]	, "HitFitInfo.unfittedJetIsBTagged[HitFitInfo.nHitFitXnHitFitJet]/O"	);
      root->Branch("HitFitInfo.unfittedJetBTagProbability", &unfittedJetBTagProbability[0]	, "HitFitInfo.unfittedJetBTagProbability[HitFitInfo.nHitFitXnHitFitJet]/D"	);

      root->Branch("HitFitInfo.NeutrinoSol" , &NeutrinoSol[0]	, "HitFitInfo.NeutrinoSol[HitFitInfo.nHitFit]/O"   );
      root->Branch("HitFitInfo.RealNeutrinoSol" , &RealNeutrinoSol[0]	, "HitFitInfo.RealNeutrinoSol[HitFitInfo.nHitFit]/O"   );

      root->Branch("HitFitInfo.unfittedNeutrinoE"	, &unfittedNeutrinoE[0]	        , "HitFitInfo.unfittedNeutrinoE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoP"	, &unfittedNeutrinoP[0]	        , "HitFitInfo.unfittedNeutrinoP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoPx"	, &unfittedNeutrinoPx[0]	, "HitFitInfo.unfittedNeutrinoPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoPy"	, &unfittedNeutrinoPy[0]	, "HitFitInfo.unfittedNeutrinoPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoPz"	, &unfittedNeutrinoPz[0]	, "HitFitInfo.unfittedNeutrinoPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoPzRe"	, &unfittedNeutrinoPzRe[0]	, "HitFitInfo.unfittedNeutrinoPzRe[HitFitInfo.nHitFit]/D"  );
      root->Branch("HitFitInfo.unfittedNeutrinoPzIm"	, &unfittedNeutrinoPzIm[0]	, "HitFitInfo.unfittedNeutrinoPzIm[HitFitInfo.nHitFit]/D"  );
      root->Branch("HitFitInfo.unfittedNeutrinoPt"	, &unfittedNeutrinoPt[0]	, "HitFitInfo.unfittedNeutrinoPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedNeutrinoEta"	, &unfittedNeutrinoEta[0]	, "HitFitInfo.unfittedNeutrinoEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedNeutrinoTheta"   , &unfittedNeutrinoTheta[0]	, "HitFitInfo.unfittedNeutrinoTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedNeutrinoPhi"	, &unfittedNeutrinoPhi[0]	, "HitFitInfo.unfittedNeutrinoPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedLepWMass", &unfittedLepWMass[0]  , "HitFitInfo.unfittedLepWMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWE"	, &unfittedLepWE[0]	, "HitFitInfo.unfittedLepWE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWP"	, &unfittedLepWP[0]	, "HitFitInfo.unfittedLepWP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWPx"	, &unfittedLepWPx[0]	, "HitFitInfo.unfittedLepWPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWPy"	, &unfittedLepWPy[0]	, "HitFitInfo.unfittedLepWPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWPz"	, &unfittedLepWPz[0]	, "HitFitInfo.unfittedLepWPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWPt"	, &unfittedLepWPt[0]	, "HitFitInfo.unfittedLepWPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepWEta"	, &unfittedLepWEta[0]	, "HitFitInfo.unfittedLepWEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedLepWTheta"   , &unfittedLepWTheta[0]	, "HitFitInfo.unfittedLepWTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedLepWPhi"	, &unfittedLepWPhi[0]	, "HitFitInfo.unfittedLepWPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedLepTopMass", &unfittedLepTopMass[0]  , "HitFitInfo.unfittedLepTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopE"	, &unfittedLepTopE[0]	, "HitFitInfo.unfittedLepTopE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopP"	, &unfittedLepTopP[0]	, "HitFitInfo.unfittedLepTopP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopPx"	, &unfittedLepTopPx[0]	, "HitFitInfo.unfittedLepTopPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopPy"	, &unfittedLepTopPy[0]	, "HitFitInfo.unfittedLepTopPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopPz"	, &unfittedLepTopPz[0]	, "HitFitInfo.unfittedLepTopPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopPt"	, &unfittedLepTopPt[0]	, "HitFitInfo.unfittedLepTopPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedLepTopEta"	, &unfittedLepTopEta[0]	, "HitFitInfo.unfittedLepTopEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedLepTopTheta"   , &unfittedLepTopTheta[0]	, "HitFitInfo.unfittedLepTopTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedLepTopPhi"	, &unfittedLepTopPhi[0]	, "HitFitInfo.unfittedLepTopPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedHadWMass", &unfittedHadWMass[0]  , "HitFitInfo.unfittedHadWMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWE"	, &unfittedHadWE[0]	, "HitFitInfo.unfittedHadWE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWP"	, &unfittedHadWP[0]	, "HitFitInfo.unfittedHadWP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWPx"	, &unfittedHadWPx[0]	, "HitFitInfo.unfittedHadWPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWPy"	, &unfittedHadWPy[0]	, "HitFitInfo.unfittedHadWPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWPz"	, &unfittedHadWPz[0]	, "HitFitInfo.unfittedHadWPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWPt"	, &unfittedHadWPt[0]	, "HitFitInfo.unfittedHadWPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadWEta"	, &unfittedHadWEta[0]	, "HitFitInfo.unfittedHadWEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedHadWTheta"   , &unfittedHadWTheta[0]	, "HitFitInfo.unfittedHadWTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedHadWPhi"	, &unfittedHadWPhi[0]	, "HitFitInfo.unfittedHadWPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedHadTopMass", &unfittedHadTopMass[0]  , "HitFitInfo.unfittedHadTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopE"	, &unfittedHadTopE[0]	, "HitFitInfo.unfittedHadTopE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopP"	, &unfittedHadTopP[0]	, "HitFitInfo.unfittedHadTopP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopPx"	, &unfittedHadTopPx[0]	, "HitFitInfo.unfittedHadTopPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopPy"	, &unfittedHadTopPy[0]	, "HitFitInfo.unfittedHadTopPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopPz"	, &unfittedHadTopPz[0]	, "HitFitInfo.unfittedHadTopPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopPt"	, &unfittedHadTopPt[0]	, "HitFitInfo.unfittedHadTopPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedHadTopEta"	, &unfittedHadTopEta[0]	, "HitFitInfo.unfittedHadTopEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedHadTopTheta"   , &unfittedHadTopTheta[0]	, "HitFitInfo.unfittedHadTopTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedHadTopPhi"	, &unfittedHadTopPhi[0]	, "HitFitInfo.unfittedHadTopPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedTopMass", &unfittedTopMass[0]  , "HitFitInfo.unfittedTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTopMassSigma", &unfittedTopMassSigma[0]  , "HitFitInfo.unfittedTopMassSigma[HitFitInfo.nHitFit]/D"	   );

      root->Branch("HitFitInfo.unfittedKtMass", &unfittedKtMass[0]  , "HitFitInfo.unfittedKtMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtE"	, &unfittedKtE[0]	, "HitFitInfo.unfittedKtE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtP"	, &unfittedKtP[0]	, "HitFitInfo.unfittedKtP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtPx"	, &unfittedKtPx[0]	, "HitFitInfo.unfittedKtPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtPy"	, &unfittedKtPy[0]	, "HitFitInfo.unfittedKtPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtPz"	, &unfittedKtPz[0]	, "HitFitInfo.unfittedKtPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtPt"	, &unfittedKtPt[0]	, "HitFitInfo.unfittedKtPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedKtEta"	, &unfittedKtEta[0]	, "HitFitInfo.unfittedKtEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedKtTheta"   , &unfittedKtTheta[0]	, "HitFitInfo.unfittedKtTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedKtPhi"	, &unfittedKtPhi[0]	, "HitFitInfo.unfittedKtPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.unfittedKtResC"	, &unfittedKtResC	, "HitFitInfo.unfittedKtResC/D"	   );
      root->Branch("HitFitInfo.unfittedKtResR"	, &unfittedKtResR	, "HitFitInfo.unfittedKtResR/D"	   );
      root->Branch("HitFitInfo.unfittedKtResN"	, &unfittedKtResN	, "HitFitInfo.unfittedKtResN/D"	   );
      root->Branch("HitFitInfo.unfittedKtResInverse"	, &unfittedKtResInverse	, "HitFitInfo.unfittedKtResInverse/O"	   );

      root->Branch("HitFitInfo.unfittedTtMass", &unfittedTtMass[0]  , "HitFitInfo.unfittedTtMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtE"	, &unfittedTtE[0]	, "HitFitInfo.unfittedTtE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtP"	, &unfittedTtP[0]	, "HitFitInfo.unfittedTtP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtPx"	, &unfittedTtPx[0]	, "HitFitInfo.unfittedTtPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtPy"	, &unfittedTtPy[0]	, "HitFitInfo.unfittedTtPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtPz"	, &unfittedTtPz[0]	, "HitFitInfo.unfittedTtPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtPt"	, &unfittedTtPt[0]	, "HitFitInfo.unfittedTtPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.unfittedTtEta"	, &unfittedTtEta[0]	, "HitFitInfo.unfittedTtEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.unfittedTtTheta"   , &unfittedTtTheta[0]	, "HitFitInfo.unfittedTtTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.unfittedTtPhi"	, &unfittedTtPhi[0]	, "HitFitInfo.unfittedTtPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedLeptonE"	, &fittedLeptonE[0]	, "HitFitInfo.fittedLeptonE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonP"	, &fittedLeptonP[0]	, "HitFitInfo.fittedLeptonP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonPx"	, &fittedLeptonPx[0]	, "HitFitInfo.fittedLeptonPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonPy"	, &fittedLeptonPy[0]	, "HitFitInfo.fittedLeptonPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonPz"	, &fittedLeptonPz[0]	, "HitFitInfo.fittedLeptonPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonPt"	, &fittedLeptonPt[0]	, "HitFitInfo.fittedLeptonPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLeptonEta"	, &fittedLeptonEta[0]	, "HitFitInfo.fittedLeptonEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedLeptonTheta"   , &fittedLeptonTheta[0]	, "HitFitInfo.fittedLeptonTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedLeptonPhi"	, &fittedLeptonPhi[0]	, "HitFitInfo.fittedLeptonPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedJetMass"	, &fittedJetMass[0]	, "HitFitInfo.fittedJetMass[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetE"	, &fittedJetE[0]	, "HitFitInfo.fittedJetE[HitFitInfo.nHitFitXnHitFitJet]/D"		);
      root->Branch("HitFitInfo.fittedJetP"	, &fittedJetP[0]	, "HitFitInfo.fittedJetP[HitFitInfo.nHitFitXnHitFitJet]/D"		);
      root->Branch("HitFitInfo.fittedJetPx"	, &fittedJetPx[0]	, "HitFitInfo.fittedJetPx[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetPy"	, &fittedJetPy[0]	, "HitFitInfo.fittedJetPy[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetPz"	, &fittedJetPz[0]	, "HitFitInfo.fittedJetPz[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetPt"	, &fittedJetPt[0]	, "HitFitInfo.fittedJetPt[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetEta"	, &fittedJetEta[0]	, "HitFitInfo.fittedJetEta[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetTheta", &fittedJetTheta[0]	, "HitFitInfo.fittedJetTheta[HitFitInfo.nHitFitXnHitFitJet]/D"	);
      root->Branch("HitFitInfo.fittedJetPhi"	, &fittedJetPhi[0]	, "HitFitInfo.fittedJetPhi[HitFitInfo.nHitFitXnHitFitJet]/D"	);

      root->Branch("HitFitInfo.fittedNeutrinoE"	, &fittedNeutrinoE[0]	, "HitFitInfo.fittedNeutrinoE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoP"	, &fittedNeutrinoP[0]	, "HitFitInfo.fittedNeutrinoP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoPx"	, &fittedNeutrinoPx[0]	, "HitFitInfo.fittedNeutrinoPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoPy"	, &fittedNeutrinoPy[0]	, "HitFitInfo.fittedNeutrinoPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoPz"	, &fittedNeutrinoPz[0]	, "HitFitInfo.fittedNeutrinoPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoPt"	, &fittedNeutrinoPt[0]	, "HitFitInfo.fittedNeutrinoPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedNeutrinoEta"	, &fittedNeutrinoEta[0]	, "HitFitInfo.fittedNeutrinoEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedNeutrinoTheta"   , &fittedNeutrinoTheta[0]	, "HitFitInfo.fittedNeutrinoTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedNeutrinoPhi"	, &fittedNeutrinoPhi[0]	, "HitFitInfo.fittedNeutrinoPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedLepWMass", &fittedLepWMass[0]  , "HitFitInfo.fittedLepWMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWE"	, &fittedLepWE[0]	, "HitFitInfo.fittedLepWE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWP"	, &fittedLepWP[0]	, "HitFitInfo.fittedLepWP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWPx"	, &fittedLepWPx[0]	, "HitFitInfo.fittedLepWPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWPy"	, &fittedLepWPy[0]	, "HitFitInfo.fittedLepWPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWPz"	, &fittedLepWPz[0]	, "HitFitInfo.fittedLepWPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWPt"	, &fittedLepWPt[0]	, "HitFitInfo.fittedLepWPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepWEta"	, &fittedLepWEta[0]	, "HitFitInfo.fittedLepWEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedLepWTheta"   , &fittedLepWTheta[0]	, "HitFitInfo.fittedLepWTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedLepWPhi"	, &fittedLepWPhi[0]	, "HitFitInfo.fittedLepWPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedLepTopMass", &fittedLepTopMass[0]  , "HitFitInfo.fittedLepTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopE"	, &fittedLepTopE[0]	, "HitFitInfo.fittedLepTopE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopP"	, &fittedLepTopP[0]	, "HitFitInfo.fittedLepTopP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopPx"	, &fittedLepTopPx[0]	, "HitFitInfo.fittedLepTopPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopPy"	, &fittedLepTopPy[0]	, "HitFitInfo.fittedLepTopPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopPz"	, &fittedLepTopPz[0]	, "HitFitInfo.fittedLepTopPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopPt"	, &fittedLepTopPt[0]	, "HitFitInfo.fittedLepTopPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedLepTopEta"	, &fittedLepTopEta[0]	, "HitFitInfo.fittedLepTopEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedLepTopTheta"   , &fittedLepTopTheta[0]	, "HitFitInfo.fittedLepTopTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedLepTopPhi"	, &fittedLepTopPhi[0]	, "HitFitInfo.fittedLepTopPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedHadWMass", &fittedHadWMass[0]  , "HitFitInfo.fittedHadWMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWE"	, &fittedHadWE[0]	, "HitFitInfo.fittedHadWE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWP"	, &fittedHadWP[0]	, "HitFitInfo.fittedHadWP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWPx"	, &fittedHadWPx[0]	, "HitFitInfo.fittedHadWPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWPy"	, &fittedHadWPy[0]	, "HitFitInfo.fittedHadWPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWPz"	, &fittedHadWPz[0]	, "HitFitInfo.fittedHadWPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWPt"	, &fittedHadWPt[0]	, "HitFitInfo.fittedHadWPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadWEta"	, &fittedHadWEta[0]	, "HitFitInfo.fittedHadWEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedHadWTheta"   , &fittedHadWTheta[0]	, "HitFitInfo.fittedHadWTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedHadWPhi"	, &fittedHadWPhi[0]	, "HitFitInfo.fittedHadWPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedHadTopMass", &fittedHadTopMass[0]  , "HitFitInfo.fittedHadTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopE"	, &fittedHadTopE[0]	, "HitFitInfo.fittedHadTopE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopP"	, &fittedHadTopP[0]	, "HitFitInfo.fittedHadTopP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopPx"	, &fittedHadTopPx[0]	, "HitFitInfo.fittedHadTopPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopPy"	, &fittedHadTopPy[0]	, "HitFitInfo.fittedHadTopPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopPz"	, &fittedHadTopPz[0]	, "HitFitInfo.fittedHadTopPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopPt"	, &fittedHadTopPt[0]	, "HitFitInfo.fittedHadTopPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedHadTopEta"	, &fittedHadTopEta[0]	, "HitFitInfo.fittedHadTopEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedHadTopTheta"   , &fittedHadTopTheta[0]	, "HitFitInfo.fittedHadTopTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedHadTopPhi"	, &fittedHadTopPhi[0]	, "HitFitInfo.fittedHadTopPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedTopMass", &fittedTopMass[0]  , "HitFitInfo.fittedTopMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTopMassSigma", &fittedTopMassSigma[0]  , "HitFitInfo.fittedTopMassSigma[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedExcitedQuarkMass", &fittedExcitedQuarkMass[0]  , "HitFitInfo.fittedExcitedQuarkMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedExcitedQuarkMassSigma", &fittedExcitedQuarkMassSigma[0]  , "HitFitInfo.fittedExcitedQuarkMassSigma[HitFitInfo.nHitFit]/D"	   );

      root->Branch("HitFitInfo.fittedKtMass", &fittedKtMass[0]  , "HitFitInfo.fittedKtMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtE"	, &fittedKtE[0]	, "HitFitInfo.fittedKtE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtP"	, &fittedKtP[0]	, "HitFitInfo.fittedKtP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtPx"	, &fittedKtPx[0]	, "HitFitInfo.fittedKtPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtPy"	, &fittedKtPy[0]	, "HitFitInfo.fittedKtPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtPz"	, &fittedKtPz[0]	, "HitFitInfo.fittedKtPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtPt"	, &fittedKtPt[0]	, "HitFitInfo.fittedKtPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedKtEta"	, &fittedKtEta[0]	, "HitFitInfo.fittedKtEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedKtTheta"   , &fittedKtTheta[0]	, "HitFitInfo.fittedKtTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedKtPhi"	, &fittedKtPhi[0]	, "HitFitInfo.fittedKtPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.fittedTtMass", &fittedTtMass[0]  , "HitFitInfo.fittedTtMass[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtE"	, &fittedTtE[0]	, "HitFitInfo.fittedTtE[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtP"	, &fittedTtP[0]	, "HitFitInfo.fittedTtP[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtPx"	, &fittedTtPx[0]	, "HitFitInfo.fittedTtPx[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtPy"	, &fittedTtPy[0]	, "HitFitInfo.fittedTtPy[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtPz"	, &fittedTtPz[0]	, "HitFitInfo.fittedTtPz[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtPt"	, &fittedTtPt[0]	, "HitFitInfo.fittedTtPt[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.fittedTtEta"	, &fittedTtEta[0]	, "HitFitInfo.fittedTtEta[HitFitInfo.nHitFit]/D"   );
      root->Branch("HitFitInfo.fittedTtTheta"   , &fittedTtTheta[0]	, "HitFitInfo.fittedTtTheta[HitFitInfo.nHitFit]/D" );
      root->Branch("HitFitInfo.fittedTtPhi"	, &fittedTtPhi[0]	, "HitFitInfo.fittedTtPhi[HitFitInfo.nHitFit]/D"   );

      root->Branch("HitFitInfo.Chi2", &Chi2[0]  , "HitFitInfo.Chi2[HitFitInfo.nHitFit]/D"	   );
      root->Branch("HitFitInfo.Converge", &Converge[0]  , "HitFitInfo.Converge[HitFitInfo.nHitFit]/O"	   );
      root->Branch("HitFitInfo.PullX", &PullX[0]  , "HitFitInfo.PullX[HitFitInfo.nHitFitXnX]/D"	   );
      root->Branch("HitFitInfo.PullY", &PullY[0]  , "HitFitInfo.PullY[HitFitInfo.nHitFitXnY]/D"	   );

      root->Branch("HitFitInfo.nHitFitConverge"	 , &nHitFitConverge	 , "HitFitInfo.nHitFitConverge/i"    );
      root->Branch("HitFitInfo.AllConverge"	 , &AllConverge	 , "HitFitInfo.AllConverge/O"    );
      root->Branch("HitFitInfo.MinChi2Index"	 , &MinChi2Index	 , "HitFitInfo.MinChi2Index/I"    );
      root->Branch("HitFitInfo.MaxChi2Index"	 , &MaxChi2Index	 , "HitFitInfo.MaxChi2Index/I"    );
      root->Branch("HitFitInfo.SortedChi2Index"	 , &SortedChi2Index[0]	 , "HitFitInfo.SortedChi2Index[HitFitInfo.nHitFit]/I"    );

      root->Branch("HitFitInfo.ExpHalfChi2"	 , &ExpHalfChi2[0]	 , "HitFitInfo.ExpHalfChi2[HitFitInfo.nHitFit]/D"    );
      root->Branch("HitFitInfo.SumExpHalfChi2"	 , &SumExpHalfChi2	 , "HitFitInfo.SumExpHalfChi2/D"    );

      root->Branch("HitFitInfo.Chi2Probability"	 , &Chi2Probability[0]	 , "HitFitInfo.Chi2Probability[HitFitInfo.nHitFit]/D"    );
      root->Branch("HitFitInfo.BTagProbability"	 , &BTagProbability[0]	 , "HitFitInfo.BTagProbability[HitFitInfo.nHitFit]/D"    );
      root->Branch("HitFitInfo.Chi2BTagProbability"	 , &Chi2BTagProbability[0]	 , "HitFitInfo.Chi2BTagProbability[HitFitInfo.nHitFit]/D"    );

      root->Branch("HitFitInfo.SumChi2Probability"	 , &SumChi2Probability	 , "HitFitInfo.SumChi2Probability/D"    );
      root->Branch("HitFitInfo.SumBTagProbability"	 , &SumBTagProbability	 , "HitFitInfo.SumBTagProbability/D"    );
      root->Branch("HitFitInfo.SumChi2BTagProbability"	 , &SumChi2BTagProbability , "HitFitInfo.SumChi2BTagProbability/D"    );

      root->Branch("HitFitInfo.Chi2Weight"	 , &Chi2Weight[0]	 , "HitFitInfo.Chi2Weight[HitFitInfo.nHitFit]/D"    );
      root->Branch("HitFitInfo.BTagWeight"	 , &BTagWeight[0]	 , "HitFitInfo.BTagWeight[HitFitInfo.nHitFit]/D"    );
      root->Branch("HitFitInfo.Chi2BTagWeight"	 , &Chi2BTagWeight[0]	 , "HitFitInfo.Chi2BTagWeight[HitFitInfo.nHitFit]/D"    );

      root->Branch("HitFitInfo.Weight"	 , &Weight[0]	 , "HitFitInfo.Weight[HitFitInfo.nHitFit]/D"    );
    }

    void Register(TTree *root) {
      root->SetBranchAddress("HitFitInfo.hitfit"	     		 , &hitfit	 );
      root->SetBranchAddress("HitFitInfo.nHitFit"	     		 , &nHitFit	 );
      root->SetBranchAddress("HitFitInfo.nHitFitJet"	     	 , &nHitFitJet	  );
      root->SetBranchAddress("HitFitInfo.nHitFitXnHitFitJet"     	 , &nHitFitXnHitFitJet 	);
      root->SetBranchAddress("HitFitInfo.hitfitNX"     		 , &hitfitNX);
      root->SetBranchAddress("HitFitInfo.hitfitNY"     		 , &hitfitNY);
      root->SetBranchAddress("HitFitInfo.nHitFitXnX"     		 , &nHitFitXnX);
      root->SetBranchAddress("HitFitInfo.nHitFitXnY"     		 , &nHitFitXnY);

      root->SetBranchAddress("HitFitInfo.JetIndex"	, &JetIndex[0]	);
      root->SetBranchAddress("HitFitInfo.JetInfoIndex"	, &JetInfoIndex[0]	);

      root->SetBranchAddress("HitFitInfo.JetPartonType" , &JetPartonType[0]	   );
      root->SetBranchAddress("HitFitInfo.JetPartonPermutation" , &JetPartonPermutation[0] );

      root->SetBranchAddress("HitFitInfo.JetGenJetType" , &JetGenJetType[0] );
      root->SetBranchAddress("HitFitInfo.JetGenJetPermutation" , &JetGenJetPermutation[0]  );

      root->SetBranchAddress("HitFitInfo.JetType" , &JetType[0] );
      root->SetBranchAddress("HitFitInfo.Permutation" , &Permutation[0]  );
      root->SetBranchAddress("HitFitInfo.CorrectPermutation" , &CorrectPermutation[0]  );
      root->SetBranchAddress("HitFitInfo.SemiCorrectPermutation" , &SemiCorrectPermutation[0] );
      root->SetBranchAddress("HitFitInfo.CorrectLepB" , &CorrectLepB[0]	 );
      root->SetBranchAddress("HitFitInfo.CorrectHadB" , &CorrectHadB[0]	 );
      root->SetBranchAddress("HitFitInfo.CorrectHadW" , &CorrectHadW[0]	 );
      root->SetBranchAddress("HitFitInfo.CorrectHadTop" , &CorrectHadTop[0]	   );
      root->SetBranchAddress("HitFitInfo.SemiCorrectHadTop" , &SemiCorrectHadTop[0]	   );
      root->SetBranchAddress("HitFitInfo.CorrectTwoJet" , &CorrectTwoJet[0]	   );
      root->SetBranchAddress("HitFitInfo.SemiCorrectTwoJet" , &SemiCorrectTwoJet[0]	   );
      root->SetBranchAddress("HitFitInfo.CorrectThreeJet" , &CorrectThreeJet[0]	 );
      root->SetBranchAddress("HitFitInfo.SemiCorrectThreeJet" , &SemiCorrectThreeJet[0]  );
      root->SetBranchAddress("HitFitInfo.SemiCorrectFourJet" , &SemiCorrectFourJet[0]   );

      root->SetBranchAddress("HitFitInfo.NJetCorrectPermutation" , &NJetCorrectPermutation[0]   );
      root->SetBranchAddress("HitFitInfo.MaxNJetCorrectPermutation"	 , &MaxNJetCorrectPermutation   );
      root->SetBranchAddress("HitFitInfo.CorrectPermutationIndex" , &CorrectPermutationIndex[0]  );

      root->SetBranchAddress("HitFitInfo.LepInfoIndex"  , &LepInfoIndex  );

      root->SetBranchAddress("HitFitInfo.unfittedLeptonPResC"      , &unfittedLeptonPResC       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPResR"      , &unfittedLeptonPResR       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPResN"      , &unfittedLeptonPResN       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPResInverse", &unfittedLeptonPResInverse );

      root->SetBranchAddress("HitFitInfo.unfittedLeptonEtaResC"      , &unfittedLeptonEtaResC       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonEtaResR"      , &unfittedLeptonEtaResR       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonEtaResN"      , &unfittedLeptonEtaResN       );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonEtaResInverse", &unfittedLeptonEtaResInverse );

      root->SetBranchAddress("HitFitInfo.unfittedLeptonPhiResC"      , &unfittedLeptonPhiResC      );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPhiResR"      , &unfittedLeptonPhiResR      );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPhiResN"      , &unfittedLeptonPhiResN      );
      root->SetBranchAddress("HitFitInfo.unfittedLeptonPhiResInverse", &unfittedLeptonPhiResInverse);

      root->SetBranchAddress("HitFitInfo.unfittedLeptonResPtFlag", &unfittedLeptonResPtFlag );

      root->SetBranchAddress("HitFitInfo.unfittedJetMass"	, &unfittedJetMass[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetE"	, &unfittedJetE[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetP"	, &unfittedJetP[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetPx"	, &unfittedJetPx[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetPy"	, &unfittedJetPy[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetPz"	, &unfittedJetPz[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetPt"	, &unfittedJetPt[0]		);
      root->SetBranchAddress("HitFitInfo.unfittedJetEta"	, &unfittedJetEta[0]);
      root->SetBranchAddress("HitFitInfo.unfittedJetTheta", &unfittedJetTheta[0]    );
      root->SetBranchAddress("HitFitInfo.unfittedJetPhi"	, &unfittedJetPhi[0]);

      root->SetBranchAddress("HitFitInfo.unfittedJetPResC", &unfittedJetPResC[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPResR", &unfittedJetPResR[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPResN", &unfittedJetPResN[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPResInverse", &unfittedJetPResInverse[0]	);

      root->SetBranchAddress("HitFitInfo.unfittedJetEtaResC", &unfittedJetEtaResC[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetEtaResR", &unfittedJetEtaResR[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetEtaResN", &unfittedJetEtaResN[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetEtaResInverse", &unfittedJetEtaResInverse[0]);

      root->SetBranchAddress("HitFitInfo.unfittedJetPhiResC", &unfittedJetPhiResC[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPhiResR", &unfittedJetPhiResR[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPhiResN", &unfittedJetPhiResN[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedJetPhiResInverse", &unfittedJetPhiResInverse[0]);

      root->SetBranchAddress("HitFitInfo.unfittedJetResPtFlag", &unfittedJetResPtFlag[0]);

      root->SetBranchAddress("HitFitInfo.unfittedJetIsBTagged", &unfittedJetIsBTagged[0]);
      root->SetBranchAddress("HitFitInfo.unfittedJetBTagProbability", &unfittedJetBTagProbability[0]);

      root->SetBranchAddress("HitFitInfo.NeutrinoSol" , &NeutrinoSol[0] );
      root->SetBranchAddress("HitFitInfo.RealNeutrinoSol" , &RealNeutrinoSol[0]  );

      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoE"	, &unfittedNeutrinoE[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoP"	, &unfittedNeutrinoP[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPx"	, &unfittedNeutrinoPx[0]     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPy"	, &unfittedNeutrinoPy[0]     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPz"	, &unfittedNeutrinoPz[0]     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPzRe"	, &unfittedNeutrinoPzRe[0]   );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPzIm"	, &unfittedNeutrinoPzIm[0]   );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPt"	, &unfittedNeutrinoPt[0]     );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoEta"	, &unfittedNeutrinoEta[0]    );
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoTheta"   , &unfittedNeutrinoTheta[0]);
      root->SetBranchAddress("HitFitInfo.unfittedNeutrinoPhi"	, &unfittedNeutrinoPhi[0]    );

      root->SetBranchAddress("HitFitInfo.unfittedLepWMass", &unfittedLepWMass[0]  );
      root->SetBranchAddress("HitFitInfo.unfittedLepWE"	, &unfittedLepWE[0]	  );
      root->SetBranchAddress("HitFitInfo.unfittedLepWP"	, &unfittedLepWP[0]	  );
      root->SetBranchAddress("HitFitInfo.unfittedLepWPx"	, &unfittedLepWPx[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedLepWPy"	, &unfittedLepWPy[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedLepWPz"	, &unfittedLepWPz[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedLepWPt"	, &unfittedLepWPt[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedLepWEta"	, &unfittedLepWEta[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedLepWTheta"   , &unfittedLepWTheta[0]	);
      root->SetBranchAddress("HitFitInfo.unfittedLepWPhi"	, &unfittedLepWPhi[0]	);

      root->SetBranchAddress("HitFitInfo.unfittedLepTopMass", &unfittedLepTopMass[0]   );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopE"	, &unfittedLepTopE[0]  );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopP"	, &unfittedLepTopP[0]  );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopPx"	, &unfittedLepTopPx[0] );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopPy"	, &unfittedLepTopPy[0] );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopPz"	, &unfittedLepTopPz[0] );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopPt"	, &unfittedLepTopPt[0] );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopEta"	, &unfittedLepTopEta[0]	  );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopTheta"   , &unfittedLepTopTheta[0] );
      root->SetBranchAddress("HitFitInfo.unfittedLepTopPhi"	, &unfittedLepTopPhi[0]	  );

      root->SetBranchAddress("HitFitInfo.unfittedHadWMass", &unfittedHadWMass[0]  );
      root->SetBranchAddress("HitFitInfo.unfittedHadWE"	, &unfittedHadWE[0]	   );
      root->SetBranchAddress("HitFitInfo.unfittedHadWP"	, &unfittedHadWP[0]	   );
      root->SetBranchAddress("HitFitInfo.unfittedHadWPx"	, &unfittedHadWPx[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedHadWPy"	, &unfittedHadWPy[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedHadWPz"	, &unfittedHadWPz[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedHadWPt"	, &unfittedHadWPt[0]	     );
      root->SetBranchAddress("HitFitInfo.unfittedHadWEta"	, &unfittedHadWEta[0]	 );
      root->SetBranchAddress("HitFitInfo.unfittedHadWTheta"   , &unfittedHadWTheta[0]	 );
      root->SetBranchAddress("HitFitInfo.unfittedHadWPhi"	, &unfittedHadWPhi[0]	 );

      root->SetBranchAddress("HitFitInfo.unfittedHadTopMass", &unfittedHadTopMass[0]    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopE"	, &unfittedHadTopE[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopP"	, &unfittedHadTopP[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopPx"	, &unfittedHadTopPx[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopPy"	, &unfittedHadTopPy[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopPz"	, &unfittedHadTopPz[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopPt"	, &unfittedHadTopPt[0]	    );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopEta"	, &unfittedHadTopEta[0]	  );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopTheta"   , &unfittedHadTopTheta[0] );
      root->SetBranchAddress("HitFitInfo.unfittedHadTopPhi"	, &unfittedHadTopPhi[0]	  );

      root->SetBranchAddress("HitFitInfo.unfittedTopMass", &unfittedTopMass[0]  	   );
      root->SetBranchAddress("HitFitInfo.unfittedTopMassSigma", &unfittedTopMassSigma[0]   );

      root->SetBranchAddress("HitFitInfo.unfittedKtMass", &unfittedKtMass[0]         );
      root->SetBranchAddress("HitFitInfo.unfittedKtE"	, &unfittedKtE[0]            );
      root->SetBranchAddress("HitFitInfo.unfittedKtP"	, &unfittedKtP[0]            );
      root->SetBranchAddress("HitFitInfo.unfittedKtPx"	, &unfittedKtPx[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedKtPy"	, &unfittedKtPy[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedKtPz"	, &unfittedKtPz[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedKtPt"	, &unfittedKtPt[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedKtEta"	, &unfittedKtEta[0]    );
      root->SetBranchAddress("HitFitInfo.unfittedKtTheta"   , &unfittedKtTheta[0] );
      root->SetBranchAddress("HitFitInfo.unfittedKtPhi"	, &unfittedKtPhi[0]   );

      root->SetBranchAddress("HitFitInfo.unfittedKtResC"	, &unfittedKtResC	   );
      root->SetBranchAddress("HitFitInfo.unfittedKtResR"	, &unfittedKtResR	   );
      root->SetBranchAddress("HitFitInfo.unfittedKtResN"	, &unfittedKtResN	   );
      root->SetBranchAddress("HitFitInfo.unfittedKtResInverse"	, &unfittedKtResInverse	   );

      root->SetBranchAddress("HitFitInfo.unfittedTtMass", &unfittedTtMass[0]         );
      root->SetBranchAddress("HitFitInfo.unfittedTtE"	, &unfittedTtE[0]            );
      root->SetBranchAddress("HitFitInfo.unfittedTtP"	, &unfittedTtP[0]            );
      root->SetBranchAddress("HitFitInfo.unfittedTtPx"	, &unfittedTtPx[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedTtPy"	, &unfittedTtPy[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedTtPz"	, &unfittedTtPz[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedTtPt"	, &unfittedTtPt[0]           );
      root->SetBranchAddress("HitFitInfo.unfittedTtEta"	, &unfittedTtEta[0]    );
      root->SetBranchAddress("HitFitInfo.unfittedTtTheta"   , &unfittedTtTheta[0] );
      root->SetBranchAddress("HitFitInfo.unfittedTtPhi"	, &unfittedTtPhi[0]       );

      root->SetBranchAddress("HitFitInfo.fittedLeptonE"	, &fittedLeptonE[0]		   );
      root->SetBranchAddress("HitFitInfo.fittedLeptonP"	, &fittedLeptonP[0]		   );
      root->SetBranchAddress("HitFitInfo.fittedLeptonPx"	, &fittedLeptonPx[0]	    );
      root->SetBranchAddress("HitFitInfo.fittedLeptonPy"	, &fittedLeptonPy[0]	    );
      root->SetBranchAddress("HitFitInfo.fittedLeptonPz"	, &fittedLeptonPz[0]	    );
      root->SetBranchAddress("HitFitInfo.fittedLeptonPt"	, &fittedLeptonPt[0]	    );
      root->SetBranchAddress("HitFitInfo.fittedLeptonEta"	, &fittedLeptonEta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedLeptonTheta"   , &fittedLeptonTheta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedLeptonPhi"	, &fittedLeptonPhi[0]	);

      root->SetBranchAddress("HitFitInfo.fittedJetMass"	, &fittedJetMass[0]		);
      root->SetBranchAddress("HitFitInfo.fittedJetE"	, &fittedJetE[0]		);
      root->SetBranchAddress("HitFitInfo.fittedJetP"	, &fittedJetP[0]		);
      root->SetBranchAddress("HitFitInfo.fittedJetPx"	, &fittedJetPx[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetPy"	, &fittedJetPy[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetPz"	, &fittedJetPz[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetPt"	, &fittedJetPt[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetEta"	, &fittedJetEta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetTheta", &fittedJetTheta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedJetPhi"	, &fittedJetPhi[0]	);

      root->SetBranchAddress("HitFitInfo.fittedNeutrinoE"	, &fittedNeutrinoE[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoP"	, &fittedNeutrinoP[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoPx"	, &fittedNeutrinoPx[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoPy"	, &fittedNeutrinoPy[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoPz"	, &fittedNeutrinoPz[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoPt"	, &fittedNeutrinoPt[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoEta"	, &fittedNeutrinoEta[0]	 );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoTheta"   , &fittedNeutrinoTheta[0] );
      root->SetBranchAddress("HitFitInfo.fittedNeutrinoPhi"	, &fittedNeutrinoPhi[0]	  );

      root->SetBranchAddress("HitFitInfo.fittedLepWMass", &fittedLepWMass[0]         );
      root->SetBranchAddress("HitFitInfo.fittedLepWE"	, &fittedLepWE[0]            );
      root->SetBranchAddress("HitFitInfo.fittedLepWP"	, &fittedLepWP[0]            );
      root->SetBranchAddress("HitFitInfo.fittedLepWPx"	, &fittedLepWPx[0]           );
      root->SetBranchAddress("HitFitInfo.fittedLepWPy"	, &fittedLepWPy[0]           );
      root->SetBranchAddress("HitFitInfo.fittedLepWPz"	, &fittedLepWPz[0]           );
      root->SetBranchAddress("HitFitInfo.fittedLepWPt"	, &fittedLepWPt[0]           );
      root->SetBranchAddress("HitFitInfo.fittedLepWEta"	, &fittedLepWEta[0]    );
      root->SetBranchAddress("HitFitInfo.fittedLepWTheta"   , &fittedLepWTheta[0] );
      root->SetBranchAddress("HitFitInfo.fittedLepWPhi"	, &fittedLepWPhi[0]  );

      root->SetBranchAddress("HitFitInfo.fittedLepTopMass", &fittedLepTopMass[0]   );
      root->SetBranchAddress("HitFitInfo.fittedLepTopE"	, &fittedLepTopE[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedLepTopP"	, &fittedLepTopP[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedLepTopPx"	, &fittedLepTopPx[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedLepTopPy"	, &fittedLepTopPy[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedLepTopPz"	, &fittedLepTopPz[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedLepTopPt"	, &fittedLepTopPt[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedLepTopEta"	, &fittedLepTopEta[0]	 );
      root->SetBranchAddress("HitFitInfo.fittedLepTopTheta"   , &fittedLepTopTheta[0]	 );
      root->SetBranchAddress("HitFitInfo.fittedLepTopPhi"	, &fittedLepTopPhi[0]	 );

      root->SetBranchAddress("HitFitInfo.fittedHadWMass", &fittedHadWMass[0]        );
      root->SetBranchAddress("HitFitInfo.fittedHadWE"	, &fittedHadWE[0]           );
      root->SetBranchAddress("HitFitInfo.fittedHadWP"	, &fittedHadWP[0]           );
      root->SetBranchAddress("HitFitInfo.fittedHadWPx"	, &fittedHadWPx[0]          );
      root->SetBranchAddress("HitFitInfo.fittedHadWPy"	, &fittedHadWPy[0]          );
      root->SetBranchAddress("HitFitInfo.fittedHadWPz"	, &fittedHadWPz[0]          );
      root->SetBranchAddress("HitFitInfo.fittedHadWPt"	, &fittedHadWPt[0]          );
      root->SetBranchAddress("HitFitInfo.fittedHadWEta"	, &fittedHadWEta[0]   );
      root->SetBranchAddress("HitFitInfo.fittedHadWTheta"   , &fittedHadWTheta[0] );
      root->SetBranchAddress("HitFitInfo.fittedHadWPhi"	, &fittedHadWPhi[0]	   );

      root->SetBranchAddress("HitFitInfo.fittedHadTopMass", &fittedHadTopMass[0]);
      root->SetBranchAddress("HitFitInfo.fittedHadTopE"	, &fittedHadTopE[0]	);
      root->SetBranchAddress("HitFitInfo.fittedHadTopP"	, &fittedHadTopP[0]	);
      root->SetBranchAddress("HitFitInfo.fittedHadTopPx"	, &fittedHadTopPx[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedHadTopPy"	, &fittedHadTopPy[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedHadTopPz"	, &fittedHadTopPz[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedHadTopPt"	, &fittedHadTopPt[0]	     );
      root->SetBranchAddress("HitFitInfo.fittedHadTopEta"	, &fittedHadTopEta[0]	 );
      root->SetBranchAddress("HitFitInfo.fittedHadTopTheta"   , &fittedHadTopTheta[0]	 );
      root->SetBranchAddress("HitFitInfo.fittedHadTopPhi"	, &fittedHadTopPhi[0]	 );

      root->SetBranchAddress("HitFitInfo.fittedTopMass", &fittedTopMass[0]  	   );
      root->SetBranchAddress("HitFitInfo.fittedTopMassSigma", &fittedTopMassSigma[0]  	   );

      root->SetBranchAddress("HitFitInfo.fittedExcitedQuarkMass", &fittedExcitedQuarkMass[0]       );   
      root->SetBranchAddress("HitFitInfo.fittedExcitedQuarkMassSigma", &fittedExcitedQuarkMassSigma[0]         );

      root->SetBranchAddress("HitFitInfo.fittedKtMass", &fittedKtMass[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedKtE"	, &fittedKtE[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtP"	, &fittedKtP[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtPx"	, &fittedKtPx[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtPy"	, &fittedKtPy[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtPz"	, &fittedKtPz[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtPt"	, &fittedKtPt[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtEta"	, &fittedKtEta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtTheta"   , &fittedKtTheta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedKtPhi"	, &fittedKtPhi[0]	);

      root->SetBranchAddress("HitFitInfo.fittedTtMass", &fittedTtMass[0]	   );
      root->SetBranchAddress("HitFitInfo.fittedTtE"	, &fittedTtE[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtP"	, &fittedTtP[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtPx"	, &fittedTtPx[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtPy"	, &fittedTtPy[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtPz"	, &fittedTtPz[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtPt"	, &fittedTtPt[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtEta"	, &fittedTtEta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtTheta"   , &fittedTtTheta[0]	);
      root->SetBranchAddress("HitFitInfo.fittedTtPhi"	, &fittedTtPhi[0]	);

      root->SetBranchAddress("HitFitInfo.Chi2", &Chi2[0]  	   );
      root->SetBranchAddress("HitFitInfo.Converge", &Converge[0]    );
      root->SetBranchAddress("HitFitInfo.PullX", &PullX[0]    );
      root->SetBranchAddress("HitFitInfo.PullY", &PullY[0]    );

      root->SetBranchAddress("HitFitInfo.nHitFitConverge"	 , &nHitFitConverge	 );
      root->SetBranchAddress("HitFitInfo.AllConverge"	 , &AllConverge	   );
      root->SetBranchAddress("HitFitInfo.MinChi2Index"	 , &MinChi2Index	    );
      root->SetBranchAddress("HitFitInfo.MaxChi2Index"	 , &MaxChi2Index	    );
      root->SetBranchAddress("HitFitInfo.SortedChi2Index"	 , &SortedChi2Index[0]);

      root->SetBranchAddress("HitFitInfo.ExpHalfChi2"	 , &ExpHalfChi2[0]	);
      root->SetBranchAddress("HitFitInfo.SumExpHalfChi2"	 , &SumExpHalfChi2        );

      root->SetBranchAddress("HitFitInfo.Chi2Probability"	 , &Chi2Probability[0]	    );
      root->SetBranchAddress("HitFitInfo.BTagProbability"	 , &BTagProbability[0]	    );
      root->SetBranchAddress("HitFitInfo.Chi2BTagProbability"	 , &Chi2BTagProbability[0]   );

      root->SetBranchAddress("HitFitInfo.SumChi2Probability"	 , &SumChi2Probability	 );
      root->SetBranchAddress("HitFitInfo.SumBTagProbability"	 , &SumBTagProbability	 );
      root->SetBranchAddress("HitFitInfo.SumChi2BTagProbability"	 , &SumChi2BTagProbability     );

      root->SetBranchAddress("HitFitInfo.Chi2Weight"	 , &Chi2Weight[0]	     );
      root->SetBranchAddress("HitFitInfo.BTagWeight"	 , &BTagWeight[0]	     );
      root->SetBranchAddress("HitFitInfo.Chi2BTagWeight"	 , &Chi2BTagWeight[0]	    );

      root->SetBranchAddress("HitFitInfo.Weight"	 , &Weight[0]       );
    }

    void clear() {
      hitfit = false;
      nHitFit = 0;
      nHitFitJet = 0;
      nHitFitXnHitFitJet = 0;
      hitfitNX = 0;
      hitfitNY = 0;
      nHitFitXnX = 0;
      nHitFitXnY = 0;

      for(unsigned int i=0; i<kMAX_HITFIT_JET; i++) {
	JetIndex            [i] = -1;
	JetInfoIndex        [i] = -1;

	JetPartonType       [i] = 0;
	JetPartonPermutation[i] = 'a';

	JetGenJetType       [i] = 0;
	JetGenJetPermutation[i] = 'a';
      }

      for(unsigned int i=0; i<kMAX_HITFIT*kMAX_HITFIT_JET; i++) {
	JetType             [i] = 0;

	Permutation         [i] = 'a';
	unfittedJetEtaResInverse  [i] = false;
	unfittedJetPhiResInverse  [i] = false;
	unfittedJetResPtFlag      [i] = false;
	unfittedJetIsBTagged      [i] = false;
	unfittedJetPResInverse    [i] = false;

	unfittedJetMass           [i] = 0.;
	unfittedJetE              [i] = 0.;
	unfittedJetP              [i] = 0.;
	unfittedJetPx             [i] = 0.;
	unfittedJetPy             [i] = 0.;
	unfittedJetPz             [i] = 0.;
	unfittedJetPt             [i] = 0.;
	unfittedJetEta            [i] = 0.;
	unfittedJetTheta          [i] = 0.;
	unfittedJetPhi            [i] = 0.;

	unfittedJetPResC          [i] = 0.;
	unfittedJetPResR          [i] = 0.;
	unfittedJetPResN          [i] = 0.;

	unfittedJetEtaResC        [i] = 0.;
	unfittedJetEtaResR        [i] = 0.;
	unfittedJetEtaResN        [i] = 0.;

	unfittedJetPhiResC        [i] = 0.;
	unfittedJetPhiResR        [i] = 0.;
	unfittedJetPhiResN        [i] = 0.;
	unfittedJetBTagProbability[i] = 0.;
	fittedJetMass             [i] = 0.;
	fittedJetE                [i] = 0.;
	fittedJetP                [i] = 0.;
	fittedJetPx               [i] = 0.;
	fittedJetPy               [i] = 0.;
	fittedJetPz               [i] = 0.;
	fittedJetPt               [i] = 0.;
	fittedJetEta              [i] = 0.;
	fittedJetTheta            [i] = 0.;
	fittedJetPhi              [i] = 0.;
      }

      for(unsigned int i=0; i<kMAX_HITFIT; i++) {
	CorrectPermutation    [i] = false;
	SemiCorrectPermutation[i] = false;
	CorrectLepB           [i] = false;
	CorrectHadB           [i] = false;
	CorrectHadW           [i] = false;
	CorrectHadTop         [i] = false;
	SemiCorrectHadTop     [i] = false;
	CorrectTwoJet         [i] = false;
	SemiCorrectTwoJet     [i] = false;
	CorrectThreeJet       [i] = false;
	SemiCorrectThreeJet   [i] = false;
	SemiCorrectFourJet    [i] = false;
	NeutrinoSol           [i] = false;
	RealNeutrinoSol       [i] = false;
	Converge              [i] = false;

	SortedChi2Index    [i] = -1;
	NJetCorrectPermutation[i] = 0;

	unfittedNeutrinoE     [i] = 0.;
	unfittedNeutrinoP     [i] = 0.;
	unfittedNeutrinoPx    [i] = 0.;
	unfittedNeutrinoPy    [i] = 0.;
	unfittedNeutrinoPz    [i] = 0.;
	unfittedNeutrinoPzRe  [i] = 0.;
	unfittedNeutrinoPzIm  [i] = 0.;
	unfittedNeutrinoPt    [i] = 0.; //
	unfittedNeutrinoEta   [i] = 0.;
	unfittedNeutrinoTheta [i] = 0.;
	unfittedNeutrinoPhi   [i] = 0.; //

	unfittedLepWMass      [i] = 0.;
	unfittedLepWE         [i] = 0.;
	unfittedLepWP         [i] = 0.;
	unfittedLepWPx        [i] = 0.;
	unfittedLepWPy        [i] = 0.;
	unfittedLepWPz        [i] = 0.;
	unfittedLepWPt        [i] = 0.;
	unfittedLepWEta       [i] = 0.;
	unfittedLepWTheta     [i] = 0.;
	unfittedLepWPhi       [i] = 0.;

	unfittedLepTopMass    [i] = 0.;
	unfittedLepTopE       [i] = 0.;
	unfittedLepTopP       [i] = 0.;
	unfittedLepTopPx      [i] = 0.;
	unfittedLepTopPy      [i] = 0.;
	unfittedLepTopPz      [i] = 0.;
	unfittedLepTopPt      [i] = 0.;
	unfittedLepTopEta     [i] = 0.;
	unfittedLepTopTheta   [i] = 0.;
	unfittedLepTopPhi     [i] = 0.;

	unfittedHadWMass      [i] = 0.;
	unfittedHadWE         [i] = 0.;
	unfittedHadWP         [i] = 0.;
	unfittedHadWPx        [i] = 0.;
	unfittedHadWPy        [i] = 0.;
	unfittedHadWPz        [i] = 0.;
	unfittedHadWPt        [i] = 0.;
	unfittedHadWEta       [i] = 0.;
	unfittedHadWTheta     [i] = 0.;
	unfittedHadWPhi       [i] = 0.;

	unfittedHadTopMass    [i] = 0.;
	unfittedHadTopE       [i] = 0.;
	unfittedHadTopP       [i] = 0.;
	unfittedHadTopPx      [i] = 0.;
	unfittedHadTopPy      [i] = 0.;
	unfittedHadTopPz      [i] = 0.;
	unfittedHadTopPt      [i] = 0.;
	unfittedHadTopEta     [i] = 0.;
	unfittedHadTopTheta   [i] = 0.;
	unfittedHadTopPhi     [i] = 0.;

	unfittedTopMass       [i] = 0.;
	unfittedTopMassSigma  [i] = 0.;

	unfittedKtMass        [i] = 0.;
	unfittedKtE           [i] = 0.;
	unfittedKtP           [i] = 0.;
	unfittedKtPx          [i] = 0.;
	unfittedKtPy          [i] = 0.;
	unfittedKtPz          [i] = 0.;
	unfittedKtPt          [i] = 0.;
	unfittedKtEta         [i] = 0.;
	unfittedKtTheta       [i] = 0.;
	unfittedKtPhi         [i] = 0.;

	unfittedTtMass        [i] = 0.;
	unfittedTtE           [i] = 0.;
	unfittedTtP           [i] = 0.;
	unfittedTtPx          [i] = 0.;
	unfittedTtPy          [i] = 0.;
	unfittedTtPz          [i] = 0.;
	unfittedTtPt          [i] = 0.;
	unfittedTtEta         [i] = 0.;
	unfittedTtTheta       [i] = 0.;
	unfittedTtPhi         [i] = 0.;

	fittedLeptonE         [i] = 0.;
	fittedLeptonP         [i] = 0.;
	fittedLeptonPx        [i] = 0.;
	fittedLeptonPy        [i] = 0.;
	fittedLeptonPz        [i] = 0.;
	fittedLeptonPt        [i] = 0.;
	fittedLeptonEta       [i] = 0.;
	fittedLeptonTheta     [i] = 0.;
	fittedLeptonPhi       [i] = 0.;
	fittedNeutrinoE       [i] = 0.;
	fittedNeutrinoP       [i] = 0.;
	fittedNeutrinoPx      [i] = 0.;
	fittedNeutrinoPy      [i] = 0.;
	fittedNeutrinoPz      [i] = 0.;
	fittedNeutrinoPt      [i] = 0.;
	fittedNeutrinoEta     [i] = 0.;
	fittedNeutrinoTheta   [i] = 0.;
	fittedNeutrinoPhi     [i] = 0.;

	fittedLepWMass    [i] = 0.;
	fittedLepWE       [i] = 0.;
	fittedLepWP       [i] = 0.;
	fittedLepWPx      [i] = 0.;
	fittedLepWPy      [i] = 0.;
	fittedLepWPz      [i] = 0.;
	fittedLepWPt      [i] = 0.;
	fittedLepWEta     [i] = 0.;
	fittedLepWTheta   [i] = 0.;
	fittedLepWPhi     [i] = 0.;

	fittedLepTopMass  [i] = 0.;
	fittedLepTopE     [i] = 0.;
	fittedLepTopP     [i] = 0.;
	fittedLepTopPx    [i] = 0.;
	fittedLepTopPy    [i] = 0.;
	fittedLepTopPz    [i] = 0.;
	fittedLepTopPt    [i] = 0.;
	fittedLepTopEta   [i] = 0.;
	fittedLepTopTheta [i] = 0.;
	fittedLepTopPhi   [i] = 0.;

	fittedHadWMass    [i] = 0.;
	fittedHadWE       [i] = 0.;
	fittedHadWP       [i] = 0.;
	fittedHadWPx      [i] = 0.;
	fittedHadWPy      [i] = 0.;
	fittedHadWPz      [i] = 0.;
	fittedHadWPt      [i] = 0.;
	fittedHadWEta     [i] = 0.;
	fittedHadWTheta   [i] = 0.;
	fittedHadWPhi     [i] = 0.;

	fittedHadTopMass  [i] = 0.;
	fittedHadTopE     [i] = 0.;
	fittedHadTopP     [i] = 0.;
	fittedHadTopPx    [i] = 0.;
	fittedHadTopPy    [i] = 0.;
	fittedHadTopPz    [i] = 0.;
	fittedHadTopPt    [i] = 0.;
	fittedHadTopEta   [i] = 0.;
	fittedHadTopTheta [i] = 0.;
	fittedHadTopPhi   [i] = 0.;

	fittedTopMass     [i] = 0.;
	fittedTopMassSigma[i] = 0.;

	fittedExcitedQuarkMass     [i] = 0.;
	fittedExcitedQuarkMassSigma[i] = 0.;

	fittedKtMass      [i] = 0.;
	fittedKtE         [i] = 0.;
	fittedKtP         [i] = 0.;
	fittedKtPx        [i] = 0.;
	fittedKtPy        [i] = 0.;
	fittedKtPz        [i] = 0.;
	fittedKtPt        [i] = 0.;
	fittedKtEta       [i] = 0.;
	fittedKtTheta     [i] = 0.;
	fittedKtPhi       [i] = 0.;

	fittedTtMass      [i] = 0.;
	fittedTtE         [i] = 0.;
	fittedTtP         [i] = 0.;
	fittedTtPx        [i] = 0.;
	fittedTtPy        [i] = 0.;
	fittedTtPz        [i] = 0.;
	fittedTtPt        [i] = 0.;
	fittedTtEta       [i] = 0.;
	fittedTtTheta     [i] = 0.;
	fittedTtPhi       [i] = 0.;

	Chi2               [i] = 0.;
	ExpHalfChi2        [i] = 0.;
	SumExpHalfChi2 = 0.;
	Chi2Probability    [i] = 0.;
	BTagProbability    [i] = 0.;
	Chi2BTagProbability[i] = 0.;
	Chi2Weight         [i] = 0.;
	BTagWeight         [i] = 0.;
	Chi2BTagWeight     [i] = 0.;
	Weight             [i] = 0.;
      }

      unfittedLeptonPResInverse = false;
      unfittedLeptonEtaResInverse = false;
      unfittedLeptonPhiResInverse = false;
      unfittedLeptonResPtFlag = false;
      unfittedKtResInverse = false;
      AllConverge = false;

      nHitFitConverge = 0;
      MaxNJetCorrectPermutation = 0;

      CorrectPermutationIndex[0] = -1;
      CorrectPermutationIndex[1] = -1;
      LepInfoIndex = -1;
      MinChi2Index = -1;
      MaxChi2Index = -1;

      SumChi2Probability = 0.;
      SumBTagProbability = 0.;
      SumChi2BTagProbability = 0.;
      unfittedLeptonPResC = 0.;
      unfittedLeptonPResR = 0.;
      unfittedLeptonPResN = 0.;
      unfittedLeptonEtaResC = 0.;
      unfittedLeptonEtaResR = 0.;
      unfittedLeptonEtaResN = 0.;
      unfittedLeptonPhiResC = 0.;
      unfittedLeptonPhiResR = 0.;
      unfittedLeptonPhiResN = 0.;
      unfittedKtResC = 0.;
      unfittedKtResR = 0.;
      unfittedKtResN = 0.;

      for(unsigned int i=0; i<kMAX_HITFIT*kMAX_HITFIT_VAR; i++) {
	PullX               [i] = 0.;
	PullY               [i] = 0.;
      }
    }

};
