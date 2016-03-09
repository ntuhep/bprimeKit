#!/bin/bash
#-------------------------------------------------------------------------------
#
#   File        : Submit_Sample.sh
#   Descriptions: Submit a single data sample
#   Usage       : ./Submit_Sample.sh [sample name]
#
#-------------------------------------------------------------------------------
source $PWD/scripts/common.sh

Config_dir=$PWD/crab_config/

#--------------------------------------------------------------------------------
#  Pre-requisites
#--------------------------------------------------------------------------------
if [[ $# != 1 ]]; then
   echo "Error! Expect exactly one command!"
   exit 1
fi

if [[ ! -d $Config_dir ]] ; then
   mkdir $Config_dir
fi

#--------------------------------------------------------------------------------
#  Main control flow
#--------------------------------------------------------------------------------
dataset=$1

## Calling functions defined in scripts/common.sh
name=$( makeName $dataset )
process=$( getDataProcess $dataset )
crab_file=${Config_dir}/${name}.py
template=""

if [[ $process == *"Data"* ]]; then
   template=${PWD}/crab_data_template.py
else
   template=${PWD}/crab_mc_template.py
fi
## Making crab configuration file
cat $template |
   sed "s@DATASET@${dataset}@"      |
   sed "s@PROCESS@${process}@"      |
   sed "s@NAME@${name}@" > $crab_file

## Submitting crab job
# export SCRAM_ARCH=slc6_amd64_gcc491
# eval `scramv1 runtime -sh`
# source /cvmfs/cms.cern.ch/crab3/crab.sh
# voms-proxy-init -voms cms -valid 192:0
# crab submit -c $crab_file
