##**************************************************************************************************
##
##    FileName   :  OptionParser.py
##    Description:  Various functions for parsing option input into variables
##
##**************************************************************************************************
import os, os.path
import re #Regular expression library
import glob
import sys
import ConfigParser

#-------------------------------------------------------------------------------
#   Helper functions and global variables
#-------------------------------------------------------------------------------
cmssw_base = os.environ['CMSSW_BASE']
cfg_dir   = cmssw_base + "/src/bpkFrameWork/bprimeKit/data/"
all_cfg_files = glob.glob( cfg_dir+"/Process*.cfg")

def ListAllProcesses():
   for cfg_file in all_cfg_files :
      if 'Recommended' in cfg_file:
         continue
      processList = cfg_file.replace( cfg_dir , '' )
      processList = processList.replace( 'Process_' , '' )
      processList = processList.replace( '.cfg' , '' )
      print "ProcessList=", processList, ", File: $CMSSW_BASE/%s" % cfg_file.replace( cmssw_base , '' )
      cfg = ConfigParser.ConfigParser()
      cfg.optionxform=str
      cfg.read( cfg_file )
      for section in cfg.sections():
         if cfg.get(section,'Recommended') == 'Yes' :
            print '\t[V] %-30s: %s' % ( section , cfg.get( section, 'GlobalTag' ) )
         else:
            print '\t[ ] %-30s: %s' % ( section , cfg.get( section, 'GlobalTag' ) )
      print ""

#-------------------------------------------------------------------------------
#   Begin class definition
#-------------------------------------------------------------------------------
class OptionParser:
   def __init__( self, options ):
      self.DataProcessing = ''
      self.cfgSettings = {}

      if options.DataProcessing == '' :
         print "Error! No Data Processing number input! Look at candidates in bprimeKit/data/Process*.cfg"
         ListAllProcesses()
         sys.exit()

      self.DataProcessing = options.DataProcessing.strip()
      for processfile in all_cfg_files :
         cfg = ConfigParser.ConfigParser()
         cfg.optionxform = str
         cfg.read( processfile )
         if cfg.has_section( options.DataProcessing ):
            self.cfgSettings = dict( cfg.items( options.DataProcessing ) )
            break

      if len(self.cfgSettings) == 0 :
         print "Error! Illegal Data processing options (",options.DataProcessing, ")! Look at candidates in bprimeKit/data/Process*.cfg"
         ListAllProcesses()
         sys.exit()

      if self.cfgSettings['Recommended'] != 'Yes' :
         print "Warning! Un-recommended data processing ", options.DataProcessing, " used!"

      if options.lheLabel != '' :
         print "Warning! Overriding default lhelabel to ", options.lheLabel
         self.cfgSettings['LHELabel'] = options.lheLabel

      if options.globalTag != '':
         print "Warning! Overriding default global tag to ", options.globalTag
         self.cfgSettings['GlobalTag'] = options.globalTag

   def IsData(self):
      return 'Data' in self.DataProcessing

   def IsMC(self):
      return 'MC' in self.DataProcessing

   def GetProcess(self):
      return self.DataProcessing

   def GetSetting( self, x ):
      if x in self.cfgSettings:
         return self.cfgSettings[x]
      else:
         print "Warning! Setting (",x,") not found!"
         return ''

   def GetMultiSetting( self,x ):
      if x in self.cfgSettings:
         tmp = self.cfgSettings[x]
         ret = [ y.strip() for y in tmp.split(',') ]
         return ret
      else:
         return []
