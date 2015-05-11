#!/bin/bash

CMSSW_VERSION="CMSSW_7_3_1_patch2"

scramv1 project CMSSW ${CMSSW_VERSION}
cd ${CMSSW_VERSION}/src

echo "Begin BPRIME_KIT requirement setup"
git cms-merge-topic rappoccio:RecoJetsFromMiniAOD
mkdir Analysis
cd Analysis
git clone https://github.com/cmsb2g/B2GAnaFW
cd ..
scram b -j 10


git clone https://github.com/ntuhep/bprimeKit MyAna/bprimeKit
cd MyAna/bprimeKit
git checkout CMSSW_7_3_x.v3


cd ../..
git clone https://github.com/ETHZ/sixie-Muon-MuonAnalysisTools.git UserCode/sixie/Muon/MuonAnalysisTools
git clone https://github.com/amarini/QuarkGluonTagger.git
scram b -j 10

echo "Begin Pythia requirement setup"
curl https://raw.githubusercontent.com/cms-sw/genproductions/master/python/ThirteenTeV/Hadronizer_TuneCUEP8S1CTEQ6L1_13TeV_generic_LHE_pythia8_Tauola_cff.py \
   --create-dirs -o Configuration/GenProduction/python/ThirteenTeV/Hadronizer_TuneCUEP8S1CTEQ6L1_13TeV_generic_LHE_pythia8_Tauola_cff.py
scram b

echo "Duplication configuration files..."
cmsDriver.py step1 \
   --filein file:/eos/uscms/store/user/lpcmbja/noreplica/boostedGen/Zh/lhe/unweighted_events.1.lhe \
   --fileout file:step1.root --mc --eventcontent LHE --datatier GEN --conditions PHYS14_50_V2 --step NONE \
   --python_filename step1.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 10000
cmsRun step1.py 

cmsDriver.py \
   Configuration/GenProduction/python/ThirteenTeV/Hadronizer_TuneCUEP8S1CTEQ6L1_13TeV_generic_LHE_pythia8_Tauola_cff.py \
   --filein "file:step1.root" --fileout file:step2.root --mc --eventcontent RAWSIM \
   --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring \
   --datatier GEN-SIM --conditions PHYS14_50_V2 --step GEN,SIM --magField 38T_PostLS1 --geometry Extended2015 \
   --python_filename step2.py --no_exec -n 2
cmsRun step2.py

cmsDriver.py step3 \
   --filein "file:step2.root" --fileout file:step3.root \
   --pileup_input "dbs:/MinBias_TuneA2MB_13TeV-pythia8/Fall13-POSTLS162_V1-v1/GEN-SIM" --mc --eventcontent RAWSIM --inputEventContent REGEN \
   --pileup AVE_40_BX_50ns --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring \
   --datatier GEN-SIM-RAW --conditions PHYS14_50_V2 --step GEN:fixGenInfo,DIGI,L1,DIGI2RAW,HLT:GRun --magField 38T_PostLS1 \
   --python_filename step3.py --no_exec -n 2
cmsRun step3.py

cmsDriver.py step4 \
   --filein file:step3.root --fileout file:step4.root \
   --mc --eventcontent AODSIM,DQM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring \
   --datatier AODSIM,DQMIO --conditions PHYS14_50_V2 --step RAW2DIGI,L1Reco,RECO,EI,DQM:DQMOfflinePOGMC --magField 38T_PostLS1 \
   --python_filename step4.py --no_exec -n 2
cmsRun step4.py

cmsDriver.py step5 \
   --filein file:step4.root --fileout file:step5.root \
   --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM --conditions PHYS14_50_V2 --step PAT \
   --python_filename step5.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 2
cmsRun step5.py
