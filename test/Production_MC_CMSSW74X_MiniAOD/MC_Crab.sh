#!/bin/bash

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 
WORKINGPATH=`pwd`

if [ "$1" == "" ] ; then 
   echo "Please input your afs ID like : "
   echo "./SUBMIT_MC_Summer12.csh [afs ID]"
   exit -1
fi

if [ ! -e MC_datasets_$1 ] ;  then 
   echo "Please check if $WORKINGPATH/MC_datasets_$1 exists or not."
   exit
fi

echo -e "\n================= SOURCE CRAB ENVIRONMENT ========================"
export SCRAM_ARCH=slc6_amd64_gcc491
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
eval `scramv1 runtime -sh`
scramv1 runtime -sh
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -valid 192:0


echo -e "\n================= Begin scanning MC data sets defined in MC_datasets_$1 ===================="
for DATA in $(cat $WORKINGPATH/MC_datasets_$1 ) ;  do
   DATALABEL=`echo $DATA | awk -F "/" '{print $2"_"$3 }'`
   echo $DATALABEL
   cp ./crab.py "crab-$DATALABEL".py

   sed -i "s@CRAB_JOB_NAME@$DATALABEL@" "crab-$DATALABEL".py
   sed -i "s@CRAB_DATA_SET@$DATA@" "crab-$DATALABEL".py

   crab submit -c "crab-$DATALABEL".py
done


