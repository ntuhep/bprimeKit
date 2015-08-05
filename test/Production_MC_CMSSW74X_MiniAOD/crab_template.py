from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'CRAB_JOB_NAME'
config.General.workArea = 'bprimekit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'BPK_PYTHONFILE'
config.JobType.inputFiles = [
    '../dataEIDMVA' ] 

config.Data.inputDataset = 'CRAB_DATA_SET'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 20
config.Data.outLFNDirBase = 'LFN_DIR' 
#config.Data.publication = True
#config.Data.publishDataName = 'CRAB3_tutorial_May2015_MC_analysis'

config.Site.storageSite = 'SITE'
