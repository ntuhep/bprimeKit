from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_CMSSW_7_4_15_Tprime_RunClean'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './bprimeKit_cfg.py'

## Input parameters 
config.JobType.pyCfgParams = [ 
   'DataProcessing=MC25ns_MiniAODv1',
   'Debug=10',
   'forceResiduals=false'
   ]

## Input files, including all files for simplicity 
config.JobType.inputFiles = [ 
   './Summer15_25nsV2_DATA.db',
   './Summer15_25nsV2_MC.db',
   './Summer15_25nsV5_DATA.db', 
   './Summer15_25nsV5_MC.db',   
   './Summer15_25nsV6_DATA.db', 
   './Summer15_25nsV6_MC.db',   
   './Summer15_50nsV2_MC.db',   
   './Summer15_50nsV4_DATA.db', 
   './Summer15_50nsV4_MC.db',   
   './Summer15_50nsV5_DATA.db', 
   './Summer15_50nsV5_MC.db',
   './JECs'
   ] 


config.Data.inputDataset = '/TprimeTprime_M-1000_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 16
config.Data.outLFNDirBase = '/store/user/yichen/BprimeKit_Ntuples_CMSSW_7_4_15/'

config.Site.storageSite = 'T3_TW_NTU_HEP'
