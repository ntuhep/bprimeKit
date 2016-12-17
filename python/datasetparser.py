#*******************************************************************************
 #
 #  Filename    : datasetparser.py
 #  Description : Attempting to determine DataProcessing from dataset format
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
import re

def getdataprocess( dataset ):
    if re.match( '/.*/.*Run2016.*-PromptReco-v.*/.*',dataset):
        return 'Data2016_80X'
    elif re.match( '/.*/Run2016.*-23Sep2016-v.*/MINIAOD', dataset ):
        return 'Data2016_SepRepro'
    elif re.match('/.*/RunIISpring16MiniAODv2.*reHLT.*/MINIAODSIM' , dataset):
        return 'MC_MiniAOD_80X'
    elif re.match("/.*/.*RunIISummer16MiniAODv2.*/MINIAODSIM" , dataset ):
        return 'MC_Summer16'
    else:
        print "Error! Dataset pattern is not recognized!"
        raise Exception("Unrecognized data")

def makename( dataset ):
    entries = dataset.split('/')
    if 'Data' in getdataprocess( dataset ):
        return entries[1]+'_'+entries[2]
    else:
        return entries[1]

def isdata( dataset ):
    if re.match( '/.*/./*Run[0-9]{4}.*/.*' , dataset ):
        return True;
    else:
        return False
