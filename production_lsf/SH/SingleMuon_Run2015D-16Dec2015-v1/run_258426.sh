#!/bin/bash
cd /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf
eval `scramv1 runtime -sh`
cmsRun /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/bprimeKit_cfg.py DataProcessing=Data25ns_76X sample=/store/data/Run2015D/SingleMuon/MINIAOD/16Dec2015-v1/10000/00E42A7E-BAA8-E511-B23B-0CC47A4C8E86.root RunRange=258426 outputLabel=/wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258426.root maxEvents=-1 &> /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/LOG/SingleMuon_Run2015D-16Dec2015-v1/run_{run}.txt
scp /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258426.root /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/STORE/SingleMuon_Run2015D-16Dec2015-v1/bpk_ntuple_258426.root
rm /wk_cms/yichen/BprimeKit_testing/CMSSW_7_6_3_patch2/src/bpkFrameWork/bprimeKit/production_lsf/NTP/SingleMuon_Run2015D-16Dec2015-v1/run_258426.root
