#List of issues

## Event information

   - Isolation information
      - [X] float RhoPU: Recipe found, but is it used?
      - [X] float Rho: replaced RhoPU with since RHO value.
      - [X] float SigmaPU: Not used any more?
   - High level trigger information
      - [ ] Untested: Multiple errors.. format update?


## Lepton information

   - Common variable issues:
      - [X] Isolation information: Duplicates and inconsistance format
      - [X] Ip3d information: Required vertex infromation

   - Muon information:
      - [X] Isolation information: For all muons
      - [X] Ip3d informattoin: in inner muon

   - Electron information
      - [X] Isolation related: Should be replaced with pre-computed cuts?
         - [X] float *R03
         - [ ] float *R04: Check is still exists
         - [X] float ElhcalOverEcalBc
         - [X] float Eldr03HcalDepth1TowerSumEtBc
         - [X] float Eldr03HcalDepth2TowerSumEtBc
         - [X] float Eldr04HcalDepth1TowerSumEtBc
         - [X] float Eldr04HcalDepth2TowerSumEtBc
         - [X] float EgammaMVANonTrig
         - [ ] float EgammaMVATrig: AOD only?
         - [X] bool  EgammaCutBasedEleIdTRIGGERTIGHT
         - [X] bool  EgammaCutBasedEleIdTRIGGERWP70
         - [X] float ElEcalE
         - [X] bool  ElhasConv
      - [ ] Conversion rejection related: / new recipe found for trackless 
         - [ ] float Eldist
         - [ ] float Eldcot
         - [ ] float Elconvradius
         - [ ] float ElConvPoint_x
         - [ ] float ElConvPoint_y
         - [ ] float ElConvPoint_z
      - [ ] Unimplemented variables.
         - [ ] int   ElTrackNHits: 
         - [ ] int   ChargeCtf: 
         - [ ] float ElSharedHitsFraction
         - [ ] float dR_gsf_ctfTrack
         - [ ] float dPt_gsf_ctfTrack
         - [ ] float TrgPt
         - [ ] float TrgEta
         - [ ] float TrgPhi
         - [ ] int   TrgID

   - Tau information:
      - [X] Isolation related: New Identification variables used

## Jet information
   - [ ] General issue
      - [ ] QuarkGluonTagger: cannot be used for by `slimmedJetsAK8` collection
      - [X] Subjet info extraction.
      - [X] bTag completed 

## Photon information 
   - [X] General information
      - [X] Added identification?

   - [X] Isolation information: No DR03 and DR04 distinctions anymore?

## Vertex information
  - [ ] No "offlinePrimaryVerticesWithBS" 
