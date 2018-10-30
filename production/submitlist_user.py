#!/bin/env python
#*******************************************************************************
 #
 #  Filename    : submitlist.py
 #  Description : submitting from a list file
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import submitsample_user as mysubmit
import optparse
import os,sys

def submitlist( argv ):
    parser = optparse.OptionParser()
    parser.add_option( '-i', '--inputfile', dest='input', help='inputfile to read', default=None, type='string')
    parser.add_option('-s','--site',dest='site',help='which site to store output', default='T2_CH_CERN', type='string')
    parser.add_option('-l','--lfndir',dest='lfndir',help='the storage lfn directory' , default='/store/user/' + os.environ.get('USER') + '/BprimeKit_Ntuples_CMSSW_102X', type='string')

    opt, args = parser.parse_args( argv )

    if not opt.input:
        print "Error! input file required!"
        return -1

    with open(opt.input) as inputfile :
        datasetlist=inputfile.readlines()
        datasetlist=[x.strip() for x in datasetlist ]
        for dataset in datasetlist:
            arglist=['-d',dataset]
            if opt.site:
                arglist.extend( ['-s',opt.site] )
            if opt.lfndir:
                arglist.extend( ['-l',opt.lfndir] )
            mysubmit.submitsample( arglist )


if __name__ == '__main__':
    sys.exit( submitlist(sys.argv[1:]) )
