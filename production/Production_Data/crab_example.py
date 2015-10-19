from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_CMSSW_7_4_14_NAME'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../bprimeKit_cfg.py'
config.JobType.pyCfgParams = [ 'DataProcessing=DP' ]
config.JobType.inputFiles = [ '../DB_FILE' ] 

config.Data.inputDataset = 'DATASET'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 32
config.Data.outLFNDirBase = '/store/user/%s/bpkNtuples_CMSSW_7_4_14' % (getUsernameFromSiteDB())

config.Site.storageSite = 'T3_TW_NTU_HEP'
