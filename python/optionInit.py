##**************************************************************************************************
##
##    FileName   :  optionInit.py
##    Description:  Function for initializing options. To be in compliance with B2GAnaFW
##
##**************************************************************************************************
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts


def initB2GOptions( options ):
   options.register('sample',
         'file:///store/yichen/MiniAODS/MC_80X/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root',
         opts.VarParsing.multiplicity.list,
         opts.VarParsing.varType.string,
         'Sample to analyze')

   options.register('outputLabel',
         'bpk_ntuple.root',
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.string,
         'Output label')

   options.register('DataProcessing',
         "",
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.string,
         'Data processing types. Options are: MC25ns_MiniAODv2, Data25ns_MiniAODv2, Data25ns_PromptRecov4')

   options.register('lheLabel',
         "",
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.string,
         'LHE module label, MC sample specific. Can be: externalLHEProducer')

   #----- Expert options, don't change -----------------------------------------------------------------
   options.register('useNoHFMET',
         True,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         'Adding met without HF and relative jets')

   options.register('usePrivateSQLite',
         True,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         'Take Corrections from private SQL file')

   options.register('forceResiduals',
         True,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         'Whether to force residuals to be applied')

   options.register('globalTag',
         '',
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.string,
         'Global Tag')

   options.register('wantSummary',
         True,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         'Want summary report')


def initBPKOptions( options ):
   options.register('Debug',
         0,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.int,
         'Debugging output level' )

   options.register('RunMuonJetClean',
         False,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         "Whether to run Muon-Jet Cleaning")

   options.register('b2gPreprocess',
         False,
         opts.VarParsing.multiplicity.singleton,
         opts.VarParsing.varType.bool,
         'Whether to use the filters and producers defined by b2g group')

   options.register('runJetToolKit',
        True,
        opts.VarParsing.multiplicity.singleton,
        opts.VarParsing.varType.bool,
        'Whether to use the jet tool kit to recluster jets'
    )

   options.setDefault('maxEvents', 100 )
