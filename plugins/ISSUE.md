#List of issues

## Event information

   - Isolation information
      - [ ] float RhoPU: Recipe found, but is it used?
      - [X] float Rho: replaced RhoPU with since RHO value.
      - [ ] float SigmaPU: Not used any more?
   - High level trigger information
      - [ ] Untested


## Lepton information

   - General problem
      - [ ] Particle Flow information (what classifies as PF information?)
      - [ ] GsfElectron and RecoElectron comparison

   - Common variable issues:
      - [ ] Isolation information: Duplicates and inconsistance format
      - [ ] Ip3d information: Required vertex infromation

   - Muon information:
      - [ ] Isolation information: For all muons
      - [ ] Ip3d informattoin: in inner muon

   - Electron information
      - [ ] Isolation related: Should be replaced with pre-computed cuts?
         - [ ] Common isolation information
         - [ ] float ElhcalOverEcalBc
         - [ ] float Eldr03HcalDepth1TowerSumEtBc
         - [ ] float Eldr03HcalDepth2TowerSumEtBc
         - [ ] float Eldr04HcalDepth1TowerSumEtBc
         - [ ] float Eldr04HcalDepth2TowerSumEtBc
         - [ ] float EgammaMVANonTrig: AOD only?
         - [ ] float EgammaMVATrig: AOD only?
         - [ ] bool  EgammaCutBasedEleIdTRIGGERTIGHT
         - [ ] bool  EgammaCutBasedEleIdTRIGGERWP70
         - [ ] float ElEcalE
         - [ ] bool  ElhasConv
      - [ ] Conversion rejection related:
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
      - [ ] No particle flow information
      - [ ] QuarkGluonTagger: cannot be used for by `slimmedJetsAK8` collection
      - [ ] Subjet info extraction.
      - [X] bTag completed 

## Photon information 
   - [ ] General information
      - [ ] Particle flow information
      - [X] Added identification?

   - [X] Isolation information: No DR03 and DR04 distinctions anymore?
      - [ ] float phoPFChIsoDR03
      - [ ] float phoPFNeuIsoDR03
      - [ ] float phoPFPhoIsoDR03
      - [ ] float phoPFChIsoDR04
      - [ ] float phoPFNeuIsoDR04
      - [ ] float phoPFPhoIsoDR04


## Vertex information
  - [ ] No "offlinePrimaryVerticesWithBS" 
