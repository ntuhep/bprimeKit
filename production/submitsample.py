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

config.General.requestName = 'BPK_102X_{0}'
config.General.workArea = 'bprimeKit_crab'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './bprimeKit_cfg.py'

## Input parameters
config.JobType.pyCfgParams = [
   'DataProcessing={1}',
   'maxEvents=-1',
   ]
config.JobType.inputFiles = [
]

config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime

config.Data.inputDataset  = '{2}'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = {3}
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
    parser.add_option('-l','--lfndir',dest='lfndir',help='the storage lfn directory' , default='/store/group/phys_b2g/BprimeKit_Ntuples_CMSSW_102X', type='string')

    opt, args = parser.parse_args( argv )

    if not opt.dataset :
        print "Error! Data set argument is required!"
        print parser.print_help()
        return 1


    ### Constant factors
    EventsPerLumi   = 3400 if myparser.isdata( opt.dataset ) else 200
    RuntimePerEvent = 0.9
    TargetTime      = 8 * 60 * 60

    content = crabcfgformat.format(
        myparser.makename( opt.dataset ),
        myparser.getdataprocess( opt.dataset ),
        opt.dataset,
        int(TargetTime / RuntimePerEvent / EventsPerLumi),
        opt.lfndir,
        opt.site
    )

    filename = './crab_config/' + myparser.makename( opt.dataset ) + '.py'
    if not os.path.isdir('./crab_config'):
        os.system('mkdir ./crab_config')

    cfgfile = open(filename, 'w')
    cfgfile.write(content)
    cfgfile.close()

    if not os.environ.get('CRABCLIENT_ROOT'):
        os.system('source /cvmfs/cms.cern.ch/crab3/crab.sh')
    # os.system('crab submit ' +filename)
    print "Writting to file {}. Do not sumbit crab jobs directly using this version! At most use crab submit --dryrun!".format( filename )


if __name__ == '__main__':
    sys.exit(submitsample(sys.argv[1:]))
