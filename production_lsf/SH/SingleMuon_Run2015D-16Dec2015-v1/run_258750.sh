#!/bin/bash
cd /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf
eval `scramv1 runtime -sh`
cmsRun /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/bprimeKit_cfg.py DataProcessing=Data25ns_76X sample=/store/data/Run2015D/SingleMuon/MINIAOD/16Dec2015-v1/10000/02AB974A-E0A8-E511-BCB4-002618943896.root RunRange=258750 outputLabel=/wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258750.root maxEvents=-1 &> /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/LOG/SingleMuon_Run2015D-16Dec2015-v1/run_{run}.txt
scp /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258750.root /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/STORE/SingleMuon_Run2015D-16Dec2015-v1/bpk_ntuple_258750.root
rm /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258750.root
