from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_76X_NAME'
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

config.Data.inputDataset = 'DATASET'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/group/phys_b2g/BprimeKit_Ntuples_CMSSW_76X/'

config.Site.storageSite = 'T2_CH_CERN'
