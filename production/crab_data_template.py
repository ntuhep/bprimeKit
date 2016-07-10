from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_80X_NAME'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './bprimeKit_cfg.py'

## Input parameters
config.JobType.pyCfgParams = [
   'DataProcessing=PROCESS',
   'maxEvents=-1',
   ]
config.JobType.inputFiles = [
"Fall15_25nsV1_MC.db",
"Fall15_25nsV2_DATA.db",
"Fall15_25nsV2_MC.db",
"Spring16_25nsFastSimMC_V1.db",
"Spring16_25nsV3_DATA.db",
"Spring16_25nsV3_MC.db",
"Spring16_25nsV6_DATA.db",
"Spring16_25nsV6_DATA_JER.db",
"Spring16_25nsV6_MC.db",
"Spring16_25nsV6_MC_JER.db",
"Summer15_25nsV2_DATA.db",
"Summer15_25nsV2_MC.db",
"Summer15_25nsV5_DATA.db",
"Summer15_25nsV5_MC.db",
"Summer15_25nsV6_DATA.db",
"Summer15_25nsV6_MC.db",
"Summer15_25nsV7_DATA.db",
"Summer15_25nsV7_MC.db",
"Summer15_50nsV2_MC.db",
"Summer15_50nsV4_DATA.db",
"Summer15_50nsV4_MC.db",
"Summer15_50nsV5_DATA.db",
"Summer15_50nsV5_MC.db"
]

config.Data.inputDataset = 'DATASET'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/group/phys_b2g/BprimeKit_Ntuples_CMSSW_80X/'

config.Site.storageSite = 'T2_CH_CERN'
