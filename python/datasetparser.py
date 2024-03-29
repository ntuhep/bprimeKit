#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( r'/.*/Run2018.*UL2018.*MiniAODv2.*/MINIAOD', dataset ):
        return 'Data_RunII_UL2018_MiniAODv2'
    elif re.match( r'/.*/Run2018.*12Nov2019.*UL2018.*/MINIAOD', dataset ):
        return 'Data12Nov2019_RunII_UL2018'
    elif re.match( r'/.*/Run2017.*09Aug2019.*UL2017.*/MINIAOD', dataset ):
        return 'Data09Aug2019_RunII_UL2017'
    elif re.match( r'/.*/Run2016.*21Feb2020.*UL2016.*/MINIAOD', dataset ):
        return 'Data21Feb2020_RunII_UL2016'
    elif re.match( r'/.*/RunIISummer19UL18.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer19UL18'
    elif re.match( r'/.*/RunIISummer19UL17.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer19UL17'
    elif re.match( r'/.*/RunIISummer20UL18MiniAOD.*-106X.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer20UL18'
    elif re.match( r'/.*/RunIISummer20UL17MiniAOD.*-106X.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer20UL17'
    elif re.match( r'/.*/RunIISummer20UL16MiniAOD.*-106X.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer20UL16'
    elif re.match( r'/.*/RunIISummer20UL16MiniAODAPV.*-106X.*/MINIAODSIM', dataset ):
        return 'MC_RunIISummer20UL16APV'
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
