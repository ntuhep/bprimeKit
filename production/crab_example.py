from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'name_2'
config.General.workArea = 'projects_2'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'bprimeKit_miniAOD.py'

config.Data.inputDataset = '/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 64
config.Data.outLFNDirBase = '/store/user/%s/BPRIMEKIT_PERSONAL_TESTING' % (getUsernameFromSiteDB())

config.Site.storageSite = 'T3_TW_NTU_HEP'
