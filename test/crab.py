from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'CRAB_JOB_NAME'
config.General.workArea = 'bprime_crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'bprimeKit_miniAOD_74X.py'
config.JobType.inputFiles = [ 
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat1.weights.xml',
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat2.weights.xml',
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat3.weights.xml',
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat4.weights.xml',
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat5.weights.xml',
    'dataEIDMVA/Electrons_BDTG_NonTrigV0_Cat6.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat1.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat2.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat3.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat4.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat5.weights.xml',
    'dataEIDMVA/Electrons_BDTG_TrigV0_Cat6.weights.xml'
    ]

config.Data.inputDataset = 'CRAB_DATA_SET'

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits  = config.Data.unitsPerJob * 1
config.Data.outLFNDirBase = '/store/user/%s/CRAB_OUTPUT' % (getUsernameFromSiteDB())
#config.Data.publication = True
#config.Data.publishDataName = 'CRAB3_tutorial_May2015_MC_analysis'

config.Site.storageSite = 'T3_TW_NTU_HEP'
