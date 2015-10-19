#-------------------------------------------------------------------------------
#
#   File        : Submit_crab_jobs.sh
#   Descriptions: Submits crab jobs for data
#   Usage       : ./Submit_crab_jobs.sh
#   Requirements: MC_Dataset_${USER}.txt
#
#-------------------------------------------------------------------------------

#!/bin/bash

Config_dir="./config/"

function main() {
   RunPrequisites 
   for data_set in $(cat Data_dataset.txt ) ; do
      DataProcess=$( getDataProcess $data_set )
      DataDBFile=$( getDBFile $data_set )
      name=$( makeName $data_set )
      echo $name

      crab_file=$Config_dir/${name}.py
      cat "./crab_example.py" |
         sed "s@DATASET@$data_set@" |
         sed "s@NAME@$name@" |
         sed "s@DP@$DataProcess@" |
         sed "s@DB_FILE@$DataDBFile@"  > $crab_file 
      crab submit -c $crab_file
   done
}

#--------------------------------------------------------------------------------  
#  Helper functions
#--------------------------------------------------------------------------------  
function makeName(){
   echo $1 | awk -F "/" '{print $2"_"$3 }' 
}

function getDataProcess() {
   local data_set=$1
   if [[ $data_set == *"25ns"* ]]; then
      echo "Data25nsv2"
   elif [[ $data_set == *"Run2015D"* ]]; then
      echo "Data25ns"
   else
      echo "Data50ns"
   fi
}

function getDBFile(){
   local data_set=$1 
   if [[ $data_set == *"25ns"* ]]; then
      echo "Summer15_25nsV2_DATA.db" 
   elif [[ $data_set == *"Run2015D"* ]]; then
      echo "Summer15_25nsV2_DATA.db"
   else 
      echo "Summer15_50nsV5_DATA.db" 
   fi
}

function RunPrequisites(){
   export SCRAM_ARCH=slc6_amd64_gcc491
   eval `scramv1 runtime -sh`
   source /cvmfs/cms.cern.ch/crab3/crab.sh
   voms-proxy-init -voms cms -valid 192:0
   if [[ ! -d $Config_dir ]] ; then 
      mkdir $Config_dir
   fi 
}
main 
