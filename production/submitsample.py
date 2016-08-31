#!/usr/bin/env python
#*******************************************************************************
 #
 #  Filename    : submitsample.py
 #  Description : python file for submitting single dataset to crab job
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
crabcfgformat="""
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'BPK_80X_{1}'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './bprimeKit_cfg.py'

## Input parameters
config.JobType.pyCfgParams = [
   'DataProcessing={2}',
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

config.Data.inputDataset = '{3}'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '{4}'

config.Site.storageSite = '{5}'
"""

import bpkFrameWork.bprimeKit.datasetparser as myparser
import optparse
import os
import sys

def submitsample(argv):
    parser=optparse.OptionParser()
    parser.add_option("-d","--dataset",dest='dataset',help='which dataset to run', default=None, type='string')
    parser.add_option('-s','--site',dest='site',help='which site to store output', default='T2_CH_CERN', type='string')
    parser.add_option('-l','--lfndir',dest='lfndir',help='the storage lfn directory' , default='/store/group/phys_b2g/BprimeKit_Ntuples_CMSSW_80X', type='string')

    opt, args = parser.parse_args( argv )

    if not opt.dataset :
        print "Error! Data set argument is required!"
        return 1

    content = crabcfgformat.format(
        myparser.makename( opt.dataset ),
        myparser.getdataprocess( opt.dataset ),
        opt.dataset,
        opt.lfndir,
        opt.site
    )

    filename = './crab_config/' + makename( opt.dataset )
    if not os.path.isdir('./crab_config'):
        os.sys('mkdir ./crab_config')

    cfgfile = open(filename, 'w')
    cfgfile.write(content)
    cfgfile.close()

    if not os.environ.get('CRABCLIENT_ROOT'):
        os.sys('source /cvmfs/cms.cern.ch/crab3/crab.sh')
    os.sys('crab submit ' +filename)


if __name__ == '__main__':
    sys.exit(submitsample(sys.argv[1:]))
