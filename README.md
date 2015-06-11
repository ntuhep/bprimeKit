#bprimeKit

## Setup Commands 
```
cmsrel CMSSW_7_4_2
cd CMSSW_7_4_2/src
cmsenv 
git cms-merge-topic ikrav:egm_id_74X_v0 
mkdir Analysis
cd Analysis
git clone https://github.com/cmsb2g/B2GAnaFW
cd ..
git clone https://github.com/enochnotsocool/bprimeKit.git -b CMSSW_7_4_X MyAna/bprimeKit
git clone https://github.com/ETHZ/sixie-Muon-MuonAnalysisTools.git UserCode/sixie/Muon/MuonAnalysisTools
git clone https://github.com/amarini/QuarkGluonTagger.git
scram b -j 10
```

=========

b prime Kit 
