#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu 'Enoch' Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( r'/.*/Run2023.*-22Sep2023.*/MINIAOD', dataset ):
        return 'DataRun2023_22Sep2023'
    elif re.match( r'/.*/Run2022[C-E]-22Sep2023.*/MINIAOD', dataset ):
        return 'DataRun2022CDE_22Sep2023'
    elif re.match( r'/.*/Run2022[FG]-22Sep2023.*/MINIAOD', dataset ):
        return 'DataRun2022FG_22Sep2023'
    elif re.match( r'/.*/Run3Summer23MiniAODv4.*/MINIAODSIM', dataset ):
        return 'MC_Run3Summer23'
    elif re.match( r'/.*/Run3Summer23BPixMiniAODv4.*/MINIAODSIM', dataset ):
        return 'MC_Run3Summer23BPix'
    elif re.match( r'/.*/Run3Summer22EEMiniAODv4.*/MINIAODSIM', dataset ):
        return 'MC_Run3Summer22EE'
    elif re.match( r'/.*/Run3Summer22MiniAODv4.*/MINIAODSIM', dataset ):
        return 'MC_Run3Summer22'
    else:
        print ('Error! Dataset pattern is not recognized!')
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
