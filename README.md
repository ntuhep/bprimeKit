# The bprimeKit

The bprimeKit aims to strip the miniAOD format used by CMSSW into a standalone ROOT Ntuple for offline analysis needs. 

## Setup and running
Below is the standard setup for the brpimeKit running under CMSSW_7_4_2 
```
cmsrel CMSSW_7_4_2
cd CMSSW_7_4_2/src
cmsenv 

git cms-merge-topic ikrav:egm_id_74X_v0
git clone https://github.com/cmsb2g/B2GAnaFW Analysis/B2GAnaFW 
git clone https://github.com/ETHZ/sixie-Muon-MuonAnalysisTools.git UserCode/sixie/Muon/MuonAnalysisTools
git clone https://github.com/amarini/QuarkGluonTagger.git
git clone -b CMSSW_7_4_2 https://github.com/enochnotsocool/bprimeKit MyAna/bprimeKit

scram b -j 10
```
To run the bprimeKit and generate ntuples, under the CMS environment:
```
cd MyAna/bprimeKit/test
cmsRun bprimeKit_miniAOD_74X.py  
```
please manually change the input and output files for you needs.

=========

b prime Kit 
