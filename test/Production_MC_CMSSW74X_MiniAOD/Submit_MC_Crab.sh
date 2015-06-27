#!/bin/bash

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 

source "/afs/cern.ch/user/y/yichen/public/NtupleControl/Master.sh"
WORKINGPATH=`pwd`


if [ ! -e $MASTER_DIR/MC_datasets_$USER ] ;  then 
   echo "Error: MC_datasets_$USER doesn't exists in $MASTER_DIR"
   exit
fi

echo -e "\n================= SOURCE CRAB ENVIRONMENT ========================"
export SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -valid 192:0

if [[ ! -d config_files ]] ; then 
   mkdir config_files
fi 

echo -e "\n================= Begin scanning MC data sets defined in MC_datasets_$USER ===================="
for DATA in $( cat $MASTER_DIR/MC_datasets_$USER ) ;  do
   DATALABEL=` getDataLabel $DATA `
   echo $DATALABEL

   BPK_PYTHONFILE=config_files/"bprimeKit-$DATALABEL".py
   CRAB_FILE=config_files/"crab-$DATALABEL".py
   OUTPUT_FILE="results-$DATALABEL".root

   cp ../bprimeKit_miniAOD.py            $BPK_PYTHONFILE
   sed -i "s@results.root@$OUTPUT_FILE@" $BPK_PYTHONFILE

   cp $MASTER_DIR/crab_template.py            $CRAB_FILE 
   sed -i "s@CRAB_JOB_NAME@$DATALABEL@"       $CRAB_FILE
   sed -i "s@CRAB_DATA_SET@$DATA@"            $CRAB_FILE 
   sed -i "s@BPK_PYTHONFILE@$BPK_PYTHONFILE@" $CRAB_FILE

   # crab submit -c $CRAB_FILE
done


