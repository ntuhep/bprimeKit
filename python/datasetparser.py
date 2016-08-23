#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( '/.*/.*Run2016.*/.*',dataset):
        return 'Data2016_80X'
    elif re.match('/.*/RunIISpring16MiniAODv2.*reHLT.*/.*' , dataset):
        return 'MC_MiniAOD_80X'
    else:
        print "Error! Dataset pattern is not recognized!"
        raise Exception("Unrecognized data")

def makename( dataset ):
    entries = dataset.split('/')
    if 'Data' in getdataprocess( dataset ):
        return entries[0]+'_'+entries[1]
    else:
        return entries[0]
