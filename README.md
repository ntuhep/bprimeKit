# The bprime Analysis Kit

The bprime analysis Kit aims to strip the miniAOD format used by CMSSW into a standalone ROOT Ntuple for offline analysis needs. 

## Setup and running
The curent version of the bprime analysis kit is meant to be ran under CMSSW_7_4_5_patch5.
```
cmsrel CMSSW_7_4_5_patch1
cd CMSSW_7_4_5_patch1/src
cmsenv 

git cms-merge-topic ikrav:egm_id_74X_v2
git clone https://github.com/cmsb2g/B2GAnaFW Analysis/B2GAnaFW 
git clone https://github.com/ETHZ/sixie-Muon-MuonAnalysisTools.git UserCode/sixie/Muon/MuonAnalysisTools
git clone https://github.com/ntuhep/bprimeKit.git MyAna/bprimeKit

scram b
```
To run the bprimeKit and generate ntuples, see the README in the [`test`](test/) directory


## Structural overview

  * [`interface`](interface/) The directory defining the classes required for the bprimeKit
  * [`plugin`](plugins/) Implementation of the methods defined in `interface`
  * [`python`](python/) defines the extra model configuration python files for processing the data
  * [`test`](test/) the directory where the operation of bprimeKit is meant to take place
  

