##**************************************************************************************************
##
##    FileName   :  OptionParser.py
##    Description:  Various functions for parsing option input into variables
##
##**************************************************************************************************
import sys 

def getGlobalTag( dataProcess ):
   if dataProcess == "MC25ns_MiniAODv2":
      return "74X_mcRun2_asymptotic_v2"
   elif dataProcess =="MC25ns_MiniAODv2_FastSim":
     return "74X_mcRun2_asymptotic_v2"
   elif dataProcess=="Data25ns_ReReco":
      return "74X_dataRun2_v4"
   elif dataProcess=="Data25ns_MiniAODv2":
     return "74X_dataRun2_reMiniAOD_v0"
   elif dataProcess=="Data25ns_PromptRecov4":
     return "74X_dataRun2_Prompt_v4"
   elif dataProcess=="MC50ns_MiniAODv2":
     return "74X_mcRun2_asymptotic50ns_v0"
   elif dataProcess=="Data50ns_MiniAODv2":
     return "74X_dataRun2_reMiniAOD_v0"
   elif dataProcess=="MC25ns_MiniAODv1":
      print( "Warning!! Running a unrecommended version of MC miniAOD, global tag MCRUN2_74_V9\n" ) 
      return "MCRUN2_74_V9"
   else:
     sys.exit("!!!!Error: Wrong DataProcessing option. Choose any of the following options:\n"
           "'MC25ns_MiniAODv2', 'MC25ns_MiniAODv2_FastSim',\n"
           "'Data25ns_ReReco' , 'Data25ns_MiniAODv2', 'Data25ns_PromptRecov4',\n"
           "'MC50ns_MiniAODv2', 'Data50ns_MiniAODv2'\n")

def getElectronIDLabel( workingPoint , dataProcess ):
   if "50ns" in dataProcess:
      if workingPoint == "veto":
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-veto"
      elif workingPoint == "loose" : 
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-loose"
      elif workingPoint == "medium" :
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-medium"
      elif workingPoint == "tight" :
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-tight"
      elif workingPoint == "heep" :
         return "egmGsfElectronIDs:heepElectronID-HEEPV60"
      else:
         return ""
   elif "25ns" in dataProcess:
      if workingPoint == "veto" : 
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"
      elif workingPoint == "loose" :
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"
      elif workingPoint == "medium" : 
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"
      elif workingPoint == "tight" :
         return "egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"
      elif workingPoint == "heep" :
         return "egmGsfElectronIDs:heepElectronID-HEEPV60"
      else:
         return ""
   else:
      return ""

def getElectronIDModule( workingPoint , dataProcess ):
   if workingPoint == "heep":
      return "RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff"
   elif "25ns" in dataProcess:
      return "RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff"
   elif "50ns" in dataProcess:
      return "RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_50ns_V2_cff"
   else :
      return ""

def getPileUpLabel( dataProcess ):
   if dataProcess=="MC25ns_MiniAODv1":
      return "addPileupInfo"
   elif "Data" in dataProcess :
      return ""
   else:
      return "slimmedAddPileupInfo"



