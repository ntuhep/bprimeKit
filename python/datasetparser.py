#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( r'/.*/Run2017.*-31Mar2018.*/MINIAOD', dataset ):
        return 'Data31Mar2018'
    elif re.match( r'/.*/RunIIFall17MiniAODv2-PU2017.*/MINIAODSIM', dataset ):
        return 'MC_RunIIFall17'
    else:
        print 'Error! Dataset pattern is not recognized!'
        raise Exception('Unrecognized data')

def makename( dataset ):
    entries = dataset.split('/')
    if 'Data' in getdataprocess( dataset ):
        return entries[1]+'_'+entries[2]
    else:
        return entries[1]

def isdata( dataset ):
    if re.match( r'/.*/.*Run[0-9]{4}.*/.*' , dataset ):
        return True;
    else:
        return False
