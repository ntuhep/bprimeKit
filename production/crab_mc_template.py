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
    "Fall15_25nsV2_DATA_PtResolution_AK4PFchs.txt",
    "Fall15_25nsV2_DATA_SF_AK4PFchs.txt",
    "Fall15_25nsV2_MC_PtResolution_AK4PFchs.txt",
    "Fall15_25nsV2_MC_SF_AK4PFchs.txt",
    "Fall15_25nsV2_DATA_PtResolution_AK4PFPuppi.txt",
    "Fall15_25nsV2_DATA_SF_AK4PFPuppi.txt",
    "Fall15_25nsV2_MC_PtResolution_AK4PFPuppi.txt",
    "Fall15_25nsV2_MC_SF_AK4PFPuppi.txt",
    "Fall15_25nsV2_DATA_PtResolution_AK8PFchs.txt",
    "Fall15_25nsV2_DATA_SF_AK8PFchs.txt",
    "Fall15_25nsV2_MC_PtResolution_AK8PFchs.txt",
    "Fall15_25nsV2_MC_SF_AK8PFchs.txt",
    "Fall15_25nsV2_DATA_PtResolution_AK8PFPuppi.txt",
    "Fall15_25nsV2_DATA_SF_AK8PFPuppi.txt",
    "Fall15_25nsV2_MC_PtResolution_AK8PFPuppi.txt",
    "Fall15_25nsV2_MC_SF_AK8PFPuppi.txt",
]

config.Data.inputDataset = 'DATASET'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/group/phys_b2g/BprimeKit_Ntuples_CMSSW_80X/'

config.Site.storageSite = 'T2_CH_CERN'
