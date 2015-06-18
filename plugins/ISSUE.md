#List of issues

## Event information

   - Isolation information
      - [ ] float RhoPU: Recipe found, but is it used?
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
      - [ ] Isolation related:
         - [ ] Common isolation information
         - [ ] float ElhcalOverEcalBc
         - [ ] float Eldr03HcalDepth1TowerSumEtBc
         - [ ] float Eldr03HcalDepth2TowerSumEtBc
         - [ ] float Eldr04HcalDepth1TowerSumEtBc
         - [ ] float Eldr04HcalDepth2TowerSumEtBc
         - [ ] float EgammaMVANonTrig
         - [ ] float EgammaMVATrig
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
      - [ ] Missing variables
         - [ ] int   ElTrackNHits: No longer Used?
         - [ ] int ChargeCtf: No longer used?
         - [ ] float ElSharedHitsFraction: No longer used?
         - [ ] float dR_gsf_ctfTrack
         - [ ] float dPt_gsf_ctfTrack
         - [ ] float TrgPt
         - [ ] float TrgEta
         - [ ] float TrgPhi
         - [ ] int TrgID

   - Tau information:
      - [ ] Isolation related:
         - [ ] float byVLooseCombinedIsolationDeltaBetaCorr
         - [ ] float byLooseCombinedIsolationDeltaBetaCorr
         - [ ] float byMediumCombinedIsolationDeltaBetaCorr
         - [ ] float byTightCombinedIsolationDeltaBetaCorr
         - [ ] float againstElectronLoose
         - [ ] float againstElectronMedium
         - [ ] float againstElectronTight
         - [ ] float againstElectronMVA
         - [ ] float againstMuonLoose
         - [ ] float againstMuonMedium
         - [ ] float againstMuonTight

## Jet information
   - [ ] General issue
      - [ ] No particle flow information
      - [ ] AK8BosonJetInfo (selectedPatJetsAK8PFCHSPrunedPacked, not use CA8)
      - [ ] CA8TopJetInfo (patJetsCMSTopTagCHSPacked)
      - [ ] QuarkGluonTagger is not up to date.
      - [ ] bTag isn't ready. 

## Photon information 
   - [ ] General information
      - [ ] Particle flow information
      - [ ] Add additional isolation information?

   - [ ] Isolation information 
      - [ ] float phoPFChIsoDR03
      - [ ] float phoPFNeuIsoDR03
      - [ ] float phoPFPhoIsoDR03
      - [ ] float phoPFChIsoDR04
      - [ ] float phoPFNeuIsoDR04
      - [ ] float phoPFPhoIsoDR04


## Vertex information
  - [ ] No "offlinePrimaryVerticesWithBS" 
