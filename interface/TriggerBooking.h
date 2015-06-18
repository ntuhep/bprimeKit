#ifndef __TriggerBooking__
#define __TriggerBooking__

#include "MyAna/bprimeKit/interface/format.h"

//Also Remember to change # inside EvtInfo.TrgBook[123]/I
//char TriggerBooking[N_TRIGGER_BOOKINGS][48] = {
static const std::string TriggerBooking[N_TRIGGER_BOOKINGS] = {
   "generation_step"                                                                  , // 0
   "digitisation_step"                                                                , // 1
   "L1simulation_step"                                                                , // 2
   "digi2raw_step"                                                                    , // 3
   "HLTriggerFirstPath"                                                               , // 4
   "HLT_Mu40_v1"                                                                      , // 5
   "HLT_IsoMu20_eta2p1_IterTrk02_v1"                                                  , // 6
   "HLT_IsoTkMu20_eta2p1_IterTrk02_v1"                                                , // 7
   "HLT_IsoMu24_eta2p1_IterTrk02_v1"                                                  , // 8
   "HLT_IsoTkMu24_eta2p1_IterTrk02_v1"                                                , // 9
   "HLT_IsoMu24_IterTrk02_v1"                                                         , // 10
   "HLT_IsoTkMu24_IterTrk02_v1"                                                       , // 11
   "HLT_Mu17_Mu8_v1"                                                                  , // 12
   "HLT_Mu17_TkMu8_v1"                                                                , // 13
   "HLT_Mu30_TkMu11_v1"                                                               , // 14
   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v1"                                              , // 15
   "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v1"                                            , // 16
   "HLT_DoubleMu4_3_Bs_v1"                                                            , // 17
   "HLT_DoubleMu4_3_Jpsi_Displaced_v1"                                                , // 18
   "HLT_Dimuon20_Jpsi_v1"                                                             , // 19
   "HLT_Dimuon13_PsiPrime_v1"                                                         , // 20
   "HLT_Dimuon13_Upsilon_v1"                                                          , // 21
   "HLT_Mu25_TkMu0_dEta18_Onia_v1"                                                    , // 22
   "HLT_DoubleMu4_JpsiTrk_Displaced_v1"                                               , // 23
   "HLT_DoubleMu4_PsiPrimeTrk_Displaced_v1"                                           , // 24
   "HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v1"                                 , // 25
   "HLT_DoubleMu33NoFiltersNoVtx_v1"                                                  , // 26
   "HLT_DoubleMu38NoFiltersNoVtx_v1"                                                  , // 27
   "HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v1"                                       , // 28
   "HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v1"                                       , // 29
   "HLT_Ele27_eta2p1_WP85_Gsf_v1"                                                     , // 30
   "HLT_Ele32_eta2p1_WP85_Gsf_v1"                                                     , // 31
   "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v1"                                            , // 32
   "HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass95_v1" , // 33
   "HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_v1"        , // 34
   "HLT_PFJet260_v1"                                                                  , // 35
   "HLT_AK8PFJet360TrimMod_Mass30_v1"                                                 , // 36
   "HLT_L2Mu10_NoVertex_NoBPTX_v1"                                                    , // 37
   "HLT_L2Mu10_NoVertex_NoBPTX3BX_NoHalo_v1"                                          , // 38
   "HLT_L2Mu20_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"                                     , // 39
   "HLT_L2Mu30_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1"                                     , // 40
   "HLT_JetE30_NoBPTX_v1"                                                             , // 41
   "HLT_JetE30_NoBPTX3BX_NoHalo_v1"                                                   , // 42
   "HLT_JetE50_NoBPTX3BX_NoHalo_v1"                                                   , // 43
   "HLT_JetE70_NoBPTX3BX_NoHalo_v1"                                                   , // 44
   "HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1"                                            , // 45
   "HLT_Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1"                                     , // 46
   "HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"                                    , // 47
   "HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1"                                       , // 48
   "HLT_ReducedIterativeTracking_v1"                                                  , // 49
   "HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1"                          , // 50
   "HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet40_v1"                                , // 51
   "HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"                           , // 52
   "HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1"                          , // 53
   "HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet40_v1"                                , // 54
   "HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1"                           , // 55
   "HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"                                   , // 56
   "HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"                             , // 57
   "HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"                              , // 58
   "HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1"                                   , // 59
   "HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1"                             , // 60
   "HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1"                              , // 61
   "HLT_Mu40_eta2p1_PFJet200_PFJet50_v1"                                              , // 62
   "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v1"                                 , // 63
   "HLT_Ele23_Ele12_CaloId_TrackId_Iso_v1"                                            , // 64
   "HLT_Ele17_Ele12_Ele10_CaloId_TrackId_v1"                                          , // 65
   "HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v1"                      , // 66
   "HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v1"                       , // 67
   "HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"                                 , // 68
   "HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"                                 , // 69
   "HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"                                 , // 70
   "HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"                                 , // 71
   "HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1"                                 , // 72
   "HLT_Photon135_PFMET40_v1"                                                         , // 73
   "HLT_Photon150_PFMET40_v1"                                                         , // 74
   "HLT_Photon160_PFMET40_v1"                                                         , // 75
   "HLT_Photon250_NoHE_PFMET40_v1"                                                    , // 76
   "HLT_Photon300_NoHE_PFMET40_v1"                                                    , // 77
   "HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1"                                     , // 78
   "HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1"                                     , // 79
   "HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1"                                     , // 80
   "HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1"                                     , // 81
   "HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1"                                     , // 82
   "HLT_Photon135_VBF_v1"                                                             , // 83
   "HLT_Photon150_VBF_v1"                                                             , // 84
   "HLT_Photon160_VBF_v1"                                                             , // 85
   "HLT_Photon250_NoHE_VBF_v1"                                                        , // 86
   "HLT_Photon300_NoHE_VBF_v1"                                                        , // 87
   "HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v1"                                         , // 88
   "HLT_Ele95_CaloIdVT_GsfTrkIdT_v1"                                                  , // 89
   "HLT_DoublePhoton85_v1"                                                            , // 90
   "HLT_Photon155_v1"                                                                 , // 91
   "HLT_Ele20WP60_Ele8_Mass55_v1"                                                     , // 92
   "HLT_Ele25WP60_SC4_Mass55_v1"                                                      , // 93
   "HLT_L2DoubleMu23_NoVertex_v1"                                                     , // 94
   "HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v1"                                , // 95
   "HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v1"                                , // 96
   "HLT_PFMET170_NoiseCleaned_v1"                                                     , // 97
   "HLT_PFMET120_NoiseCleaned_BTagCSV07_v1"                                           , // 98
   "HLT_PFHT350_PFMET120_NoiseCleaned_v1"                                             , // 99
   "HLT_PFHT900_v1"                                                                   , // 100
   "HLT_ZeroBias_v1"                                                                  , // 101
   "HLT_Physics_v1"                                                                   , // 102
   "HLTriggerFinalPath" // 103
};

enum TriggerBitNumber {
   generation_step                                                                  ,
   digitisation_step                                                                ,
   L1simulation_step                                                                ,
   digi2raw_step                                                                    ,
   HLTriggerFirstPath                                                               ,
   HLT_Mu40_v1                                                                      ,
   HLT_IsoMu20_eta2p1_IterTrk02_v1                                                  ,
   HLT_IsoTkMu20_eta2p1_IterTrk02_v1                                                ,
   HLT_IsoMu24_eta2p1_IterTrk02_v1                                                  ,
   HLT_IsoTkMu24_eta2p1_IterTrk02_v1                                                ,
   HLT_IsoMu24_IterTrk02_v1                                                         ,
   HLT_IsoTkMu24_IterTrk02_v1                                                       ,
   HLT_Mu17_Mu8_v1                                                                  ,
   HLT_Mu17_TkMu8_v1                                                                ,
   HLT_Mu30_TkMu11_v1                                                               ,
   HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v1                                              ,
   HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v1                                            ,
   HLT_DoubleMu4_3_Bs_v1                                                            ,
   HLT_DoubleMu4_3_Jpsi_Displaced_v1                                                ,
   HLT_Dimuon20_Jpsi_v1                                                             ,
   HLT_Dimuon13_PsiPrime_v1                                                         ,
   HLT_Dimuon13_Upsilon_v1                                                          ,
   HLT_Mu25_TkMu0_dEta18_Onia_v1                                                    ,
   HLT_DoubleMu4_JpsiTrk_Displaced_v1                                               ,
   HLT_DoubleMu4_PsiPrimeTrk_Displaced_v1                                           ,
   HLT_DoubleMu4_LowMassNonResonantTrk_Displaced_v1                                 ,
   HLT_DoubleMu33NoFiltersNoVtx_v1                                                  ,
   HLT_DoubleMu38NoFiltersNoVtx_v1                                                  ,
   HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v1                                       ,
   HLT_Mu42NoFiltersNoVtx_Photon42_CaloIdL_v1                                       ,
   HLT_Ele27_eta2p1_WP85_Gsf_v1                                                     ,
   HLT_Ele32_eta2p1_WP85_Gsf_v1                                                     ,
   HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v1                                            ,
   HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon18_AND_HE10_R9Id65_Mass95_v1 ,
   HLT_Photon42_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_v1        ,
   HLT_PFJet260_v1                                                                  ,
   HLT_AK8PFJet360TrimMod_Mass30_v1                                                 ,
   HLT_L2Mu10_NoVertex_NoBPTX_v1                                                    ,
   HLT_L2Mu10_NoVertex_NoBPTX3BX_NoHalo_v1                                          ,
   HLT_L2Mu20_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1                                     ,
   HLT_L2Mu30_NoVertex_3Sta_NoBPTX3BX_NoHalo_v1                                     ,
   HLT_JetE30_NoBPTX_v1                                                             ,
   HLT_JetE30_NoBPTX3BX_NoHalo_v1                                                   ,
   HLT_JetE50_NoBPTX3BX_NoHalo_v1                                                   ,
   HLT_JetE70_NoBPTX3BX_NoHalo_v1                                                   ,
   HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1                                            ,
   HLT_Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1                                     ,
   HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1                                    ,
   HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1                                       ,
   HLT_ReducedIterativeTracking_v1                                                  ,
   HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1                          ,
   HLT_IsoMu20_eta2p1_IterTrk02_TriCentralPFJet40_v1                                ,
   HLT_IsoMu20_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1                           ,
   HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet60_50_35_v1                          ,
   HLT_IsoMu24_eta2p1_IterTrk02_TriCentralPFJet40_v1                                ,
   HLT_IsoMu24_eta2p1_IterTrk02_CentralPFJet30_BTagCSV_v1                           ,
   HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet40_v1                                   ,
   HLT_Ele27_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1                             ,
   HLT_Ele27_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1                              ,
   HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet40_v1                                   ,
   HLT_Ele32_eta2p1_WP85_Gsf_TriCentralPFJet60_50_35_v1                             ,
   HLT_Ele32_eta2p1_WP85_Gsf_CentralPFJet30_BTagCSV_v1                              ,
   HLT_Mu40_eta2p1_PFJet200_PFJet50_v1                                              ,
   HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v1                                 ,
   HLT_Ele23_Ele12_CaloId_TrackId_Iso_v1                                            ,
   HLT_Ele17_Ele12_Ele10_CaloId_TrackId_v1                                          ,
   HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v1                      ,
   HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v1                       ,
   HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1                                 ,
   HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1                                 ,
   HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1                                 ,
   HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1                                 ,
   HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1                                 ,
   HLT_Photon135_PFMET40_v1                                                         ,
   HLT_Photon150_PFMET40_v1                                                         ,
   HLT_Photon160_PFMET40_v1                                                         ,
   HLT_Photon250_NoHE_PFMET40_v1                                                    ,
   HLT_Photon300_NoHE_PFMET40_v1                                                    ,
   HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v1                                     ,
   HLT_Photon36_R9Id90_HE10_Iso40_EBOnly_VBF_v1                                     ,
   HLT_Photon50_R9Id90_HE10_Iso40_EBOnly_VBF_v1                                     ,
   HLT_Photon75_R9Id90_HE10_Iso40_EBOnly_VBF_v1                                     ,
   HLT_Photon90_R9Id90_HE10_Iso40_EBOnly_VBF_v1                                     ,
   HLT_Photon135_VBF_v1                                                             ,
   HLT_Photon150_VBF_v1                                                             ,
   HLT_Photon160_VBF_v1                                                             ,
   HLT_Photon250_NoHE_VBF_v1                                                        ,
   HLT_Photon300_NoHE_VBF_v1                                                        ,
   HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v1                                         ,
   HLT_Ele95_CaloIdVT_GsfTrkIdT_v1                                                  ,
   HLT_DoublePhoton85_v1                                                            ,
   HLT_Photon155_v1                                                                 ,
   HLT_Ele20WP60_Ele8_Mass55_v1                                                     ,
   HLT_Ele25WP60_SC4_Mass55_v1                                                      ,
   HLT_L2DoubleMu23_NoVertex_v1                                                     ,
   HLT_L2DoubleMu28_NoVertex_2Cha_Angle2p5_Mass10_v1                                ,
   HLT_L2DoubleMu38_NoVertex_2Cha_Angle2p5_Mass10_v1                                ,
   HLT_PFMET170_NoiseCleaned_v1                                                     ,
   HLT_PFMET120_NoiseCleaned_BTagCSV07_v1                                           ,
   HLT_PFHT350_PFMET120_NoiseCleaned_v1                                             ,
   HLT_PFHT900_v1                                                                   ,
   HLT_ZeroBias_v1                                                                  ,
   HLT_Physics_v1                                                                   ,
   HLTriggerFinalPath
};


#endif
