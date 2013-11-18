#!/bin/tcsh

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 
set T3_folder=CMSSW5_3_11_MC_AOD_v3

set WORKINGPATH=`pwd`

if ( "$1" == "" ) then
echo "Please input your afs ID like : "
echo "./SUBMIT_MC_Summer12.csh [afs ID]"
exit
endif
if ( ! -e MC_datasets_$1 ) then 
echo "Please check if $WORKINGPATH/MC_datasets_$1 exists or not."
exit
endif

#### SOURCE CRAB ENVIRONMENT ####
#source /uscmst1/prod/sw/cms/cshrc prod
setenv SCRAM_ARCH slc5_amd64_gcc462
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
cmsenv
scramv1 runtime -csh
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh
voms-proxy-init -voms cms -valid 192:0	


set datasets_=`cat MC_datasets_$1`
#### CREATE SUBMITTING FOLDER : DATASET+RUN RANGE and SUBMIT CRAB JOBS ####
foreach lt($datasets_)
   set lt_t1=`echo $lt | awk -F "/" '{print $2"_"$3}'`
   set lt_t=$lt_t1
   if ( -e ./SUBMIT_MC/$lt_t ) then
   continue
   endif
   echo $lt_t1
   mkdir -p ./SUBMIT_MC/$lt_t

   if ( !(-e ../bprimeKit.py )) then
   echo "No ../bprimeKit.py"
   echo "Please execute this script under MyAna/bprimeKit/test/Production_Data_DCSONLY_CMSSW52X_AOD"
   exit
   endif
   sed 's/dataEIDMVA\///g' ../bprimeKit.py | sed 's/START53_V7E/START53_V27/g' >& ./SUBMIT_MC/$lt_t/bprimeKit.py

   set lt_c=`echo $lt | sed 's/\//\\\//g'`
   sed "s/datasetpath=DataSet/datasetpath=$lt_c/g" crab_MC.cfg | \
       sed "s/name_directory_you_want/$lt_t \ncheck_user_remote_dir = 0/g" | \
       sed "s/T3_folder/$T3_folder/g" > crab_test.cfg

   mv crab_test.cfg ./SUBMIT_MC/$lt_t/crab.cfg

   #### START TO SUBMIT JOBS #### 
   cd ./SUBMIT_MC/$lt_t ; crab -create -cfg crab.cfg >& log_$lt_t
   set JobNumber_=`grep "crab:  Total" log_$lt_t | grep "jobs created"| awk '{print $4}'|head -n 1`
   set checkJobNumber_=`grep "crab:  Total" log_$lt_t | grep "jobs created"| awk '{print $4}'|head -n 1|wc|awk '{print $1}' `

   if ( $checkJobNumber_ != "0" ) then
      if ( $JobNumber_ < 500 ) then
      crab -submit 
      else
   	set counter_=1
           set counterEnd_=0
           while ( $counter_ < $JobNumber_ ) 
              @ counterEnd_=$counterEnd_ + 500
              if ( $counterEnd_ > $JobNumber_ ) then
   	      @ counterEnd_=$JobNumber_
   	   endif
              crab -submit ${counter_}-${counterEnd_} 
              @ counter_=$counter_ + 500
   	end
      endif
   else
      echo "[WARNING] No matched site for $lt_t"
   endif

   cd -
end ## for datasets_

#### CRAB INFORMATION ####
## crab -forceResubmit job	// once you can not resubmit job
## crab -report		// for calculate lumi
