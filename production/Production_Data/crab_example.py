from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_CMSSW_7_4_14_NAME'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../bprimeKit_cfg.py'
config.JobType.pyCfgParams = [ 'DataProcessing=Data25ns' ]
config.JobType.inputFiles = [ '../Summer15_25nsV2_DATA.db' ] 

config.Data.inputDataset = 'DATASET'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 64
config.Data.outLFNDirBase = '/store/group/phys_b2g/BprimeKit_Ntuples_Data/'

config.Site.storageSite = 'T2_CH_CERN'
