from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'CRAB_JOB_NAME'
config.General.workArea = 'bprime_crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../bprimeKit_miniAOD.py'
config.JobType.inputFiles = [
    '../dataEIDMVA' ] 

config.Data.inputDataset = 'CRAB_DATA_SET'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits  =  100
config.Data.outLFNDirBase = '/store/user/%s/CMSSW_74X_BPRIMEKIT' % (getUsernameFromSiteDB())
#config.Data.publication = True
#config.Data.publishDataName = 'CRAB3_tutorial_May2015_MC_analysis'

config.Site.storageSite = 'T3_TW_NTU_HEP'
