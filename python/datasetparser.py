#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):

    if re.match( r'/.*/Run2018D.*-PromptReco.*/MINIAOD', dataset ):
        return 'DataRun2018_PromptReco'
    elif re.match( r'/.*/Run2018.*-17Sep2018.*/MINIAOD', dataset ):
        return 'Data17Sep2018'
    elif re.match( r'/.*/Run2017.*31Mar2018.*/MINIAOD', dataset ):
        return 'Data31Mar2018'
    elif re.match( r'/.*/Run2016.*17Jul2018.*/MINIAOD', dataset ):
        return 'Data17Jul2018'
    elif re.match( r'/.*/RunIIAutumn18MiniAOD.*/MINIAODSIM', dataset ):
        return 'MC_RunIIAutumn18'
    elif re.match( r'/.*/RunIIFall17.*12Apr2018.*/MINIAODSIM', dataset ):
        return 'MC_RunIIFall17'
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
