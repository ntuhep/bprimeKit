#!/bin/bash
cd /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf
eval `scramv1 runtime -sh`
cmsRun /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/bprimeKit_cfg.py DataProcessing=Data25ns_76X sample=/store/data/Run2015D/SingleMuon/MINIAOD/16Dec2015-v1/10000/00006301-CAA8-E511-AD39-549F35AD8BC9.root RunRange=259811 outputLabel=/wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_259811.root maxEvents=-1 &> /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/LOG/SingleMuon_Run2015D-16Dec2015-v1/run_{run}.txt
scp /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_259811.root /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/STORE/SingleMuon_Run2015D-16Dec2015-v1/bpk_ntuple_259811.root
rm /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_259811.root
