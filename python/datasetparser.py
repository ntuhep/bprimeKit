#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( r'/.*/Run2018.*12Nov2019_UL2018.*/MINIAOD', dataset ):
        return 'Data12Nov2019_RunII_UL2018'
    elif re.match( r'/.*/Run2017.*09Aug2019_UL2017.*/MINIAOD', dataset ):
        return 'Data09Aug2019_RunII_UL2017'
    elif re.match( r'/.*/Run2016.*17Jul2018.*/MINIAOD', dataset ):
        return 'Data17Jul2018'
    elif re.match( r'/.*/RunIISummer19UL18.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer19UL18'
    elif re.match( r'/.*/RunIISummer19UL17.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer19UL17'
    elif re.match( r'/.*/RunIISummer16MiniAODv3.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer16'
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
