#!/bin/bash

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 

WORKINGPATH=`pwd`
DATASET_FILE=MC_dataset_${USER}

if [ ! -e $DATASET_FILE ] ;  then 
   echo "Error: $DATASET_FILE doesn't exists!"
   exit
fi

#-----  Helper functions  --------------------------------------------------------------------------
function getDataLabel () {
   echo $1 | awk -F "/" '{print $2 }' 
}

function getSection(){
   local file=$1
   local section=$2   
   local sedcmd="sed '/\[${2}\]/,/\[.*\]/!d'"

   cat $file | eval $sedcmd | head -n -1 | tail -n +2
}

#-----  Setting up crab environment  ---------------------------------------------------------------
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -valid 192:0

if [[ ! -d config_files ]] ; then 
   mkdir config_files
fi 

#-----  Setting up individual configuration files  -------------------------------------------------
for DATA in $( cat $DATASET_FILE ) ;  do
   DATALABEL=` getDataLabel $DATA `
   echo $DATALABEL

   BPK_PYTHONFILE=../bprimeKit_miniAOD_MC.py
   CRAB_FILE=config_files/"crab-$DATALABEL".py

   cp ./crab_template.py                      $CRAB_FILE 
   sed -i "s@CRAB_JOB_NAME@$DATALABEL@"       $CRAB_FILE
   sed -i "s@CRAB_DATA_SET@$DATA@"            $CRAB_FILE 
   sed -i "s@BPK_PYTHONFILE@$BPK_PYTHONFILE@" $CRAB_FILE

   site=`getSection ./Sites.cfg SITE`
   lfn_dir=`getSection ./Sites.cfg LFN`
   
   sed -i "s@SITE@${site}@"       $CRAB_FILE 
   sed -i "s@LFN_DIR@${lfn_dir}@" $CRAB_FILE

   #crab submit -c $CRAB_FILE
done


