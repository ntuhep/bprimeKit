#!/bin/tcsh

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 
set T3_folder=CMSSW_5_3_11_data_DCSONLY_AOD

#### SET RUN RANGE BY HAND (LIKE run_rangeByhand="161234-164321"). IF NOT SET BY HAND, IT WILL BE SET AUTOMATICALLY ####
set run_rangeByhand=""      ## for default
set category_=""    ## [Ele, Mu, Jet]

#set Run2012AEndNumber_=193686
#set Run2012BEndNumber_=196531
set Run2012AReRecoRunRange_=190456-193686
set Run2012BReRecoRunRange_=193752-196531
set Run2012CReRecoRunRange_=198022-198523
set Run2012CPromptRunRange_=198934-203002
set Run2012DPromptRunRange_=203773-208551
set Run2012DPromptRunRangeRest_=208552-208686
set Run2012CNormalRunNumber_=999999

set SUBMIT_dataFolder_=SUBMIT_data

if ( "$1" == "" || "$2" == "" ) then
   echo "##################################################################################################"
   echo "##   PLEASE INPUT THE DATASET GATEGORY like                                                     ##"
   echo "##      ./SUBMIT_data_update_prompt_DCSONLY.csh [GATEGORY] [RUNRANGE]                           ##"
   echo "##   [GATEGORY] : Ele, Mu, Jet, or other keywords                                               ##"     
   echo "##   [RUNRANGE] :                                                                               ##"     
   echo "##                $Run2012AReRecoRunRange_ for Run2012AReRecoRunRange                                      ##"
   echo "##                $Run2012BReRecoRunRange_ for Run2012BReRecoRunRange                                      ##"
   echo "##                $Run2012CReRecoRunRange_ for Run2012CReRecoRunRange                                      ##"
   echo "##                $Run2012CPromptRunRange_ for Run2012CPromptRunRange                                      ##"
   echo "##                $Run2012DPromptRunRange_ for Run2012DPromptRunRange                                      ##"
   echo "##                $Run2012DPromptRunRangeRest_ for Run2012DPromptRunRange                                      ##"
   echo "##                If using 190456-198523, it will include the first three run-ranges.           ##"
   echo "##                Or the run should be larger and equal to $Run2012CNormalRunNumber_                               ##"
   echo "##################################################################################################"
   exit
else if ( "$2" != "" ) then
   set check1=`echo $2 | sed 's/-/ /g' |awk '{print $1}' | wc | awk '{print $3}'` 
   if ( "${check1}" != "7" ) then
   echo "##################################################################################################"
   echo "##   $2 is not in right format                                                                  ##"     
   echo "##   [RUNRANGE] : 193752-195016                                                                 ##"     
   echo "##################################################################################################"
   exit
   endif

   set check1=`echo $2 | sed 's/-/ /g' |awk '{print $2}' | wc | awk '{print $3}'` 
   if ( "${check1}" != "7" ) then
   echo "##################################################################################################"
   echo "##   $2 is not in right format                                                                  ##"     
   echo "##   [RUNRANGE] : 193752-195016                                                                 ##"     
   echo "##################################################################################################"
   exit
   endif
   set run_rangeByhand=$2
endif


set category_=$1

#### SOURCE CRAB ENVIRONMENT ####
setenv SCRAM_ARCH slc5_amd64_gcc462
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
cmsenv
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh
voms-proxy-init -voms cms -valid 192:0	

#### START TO SET RUN RANGE   ####
#### ACCORDING TO GOLDEN JSON ####
if ( -e .runlist ) then
rm .runlist*
endif

set run_rangeByhandStart_=`echo $run_rangeByhand | awk -F"-" '{print $1}' `
set run_rangeByhandEnd_=`echo $run_rangeByhand | awk -F"-" '{print $2}' `

echo "run_rangeByhandStart_ ${run_rangeByhandStart_} and run_rangeByhandEnd_ ${run_rangeByhandEnd_} "

if ( !(( ${run_rangeByhandStart_} == 190456 || ${run_rangeByhandStart_} == 193752 || ${run_rangeByhandStart_} == 198022 || ${run_rangeByhandStart_} == 198934 || ${run_rangeByhandStart_} == 203773 || ${run_rangeByhandStart_} == 208552 ) && ($run_rangeByhandEnd_ == 193686 || $run_rangeByhandEnd_ == 196531 || $run_rangeByhandEnd_ == 198523 || $run_rangeByhandEnd_ == 198523 || $run_rangeByhandEnd_ == 203002 || $run_rangeByhandEnd_ == 208551 || ${run_rangeByhandEnd_} == 208686 ) )) then

   if ( ${run_rangeByhandStart_} < ${Run2012CNormalRunNumber_} || $run_rangeByhandEnd_ < ${Run2012CNormalRunNumber_}) then
      echo "Please consider the run-ranges like :  "
      echo " Run2012AReRecoRunRange for $Run2012AReRecoRunRange_"
      echo " Run2012BReRecoRunRange for $Run2012BReRecoRunRange_"
      echo " Run2012CReRecoRunRange for $Run2012CReRecoRunRange_"
      echo " Run2012CPromptRunRange for $Run2012CPromptRunRange_"
      echo " Run2012DPromptRunRange for $Run2012DPromptRunRange_"
      echo " Run2012DPromptRunRangeRest for $Run2012DPromptRunRangeRest_"
      echo " If using 190456-198523, it will include the first three run-ranges."
      echo " Or the run should be larger and equal to $Run2012CNormalRunNumber_ "
      exit
   endif

   echo $run_rangeByhand >& .runlist

else 

echo $Run2012AReRecoRunRange_ >& .runlist1
echo $Run2012BReRecoRunRange_ >> .runlist1
echo $Run2012CReRecoRunRange_ >> .runlist1
echo $Run2012CPromptRunRange_ >> .runlist1
echo $Run2012DPromptRunRange_ >> .runlist1
echo $Run2012DPromptRunRangeRest_ >> .runlist1
grep -B 1000 $run_rangeByhandEnd_ .runlist1 | grep -A 1000 ${run_rangeByhandStart_} >& .runlist

endif

#ls -lt /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/ | grep Collisions12_JSON | grep -v MuonPhys | awk '{print $9}' | sed 's/-/_/g' | awk -F"_" '{print $2}' | sort -u >& .runlist1
#ls -lt /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/ | grep Collisions12_JSON | grep -v MuonPhys | awk '{print $9}' | sed 's/-/_/g' | awk -F"_" '{print $3}' | sort -u | awk 'BEGIN{ra1=-1;ra2=-1;}{ if( $1 > '${Run2012AEndNumber_}'){ra1=1;} if((ra1+ra2)==0){print '${Run2012AEndNumber_}';} ra2=ra1; print $1;}' | awk 'BEGIN{ra1=-1;ra2=-1;}{ if( $1 > '${Run2012BEndNumber_}'){ra1=1;} if((ra1+ra2)==0){print '${Run2012BEndNumber_}';} ra2=ra1; print $1;}' >& .runlist2
#awk '{print $1+1}' .runlist2 >& .runlist2-1
#set Nrun_=`wc .runlist2-1 | awk '{print $1}'`
#cat .runlist1 .runlist2-1 | head -n ${Nrun_} >& .runlist3
#paste .runlist3 .runlist2 | awk '{print $1"-"$2}' >& .runlist

#if ( "${run_rangeByhand}" != "" ) then
#rm .runlist
#echo ${run_rangeByhand} >& .runlist
#endif

set runlist_=`cat .runlist`
foreach rl_(${runlist_})
set startrun=`echo ${rl_}|awk -F"-" '{print $1}'`
set endrun=`echo ${rl_}|awk -F"-" '{print $2}'`

echo "startrun : "$startrun" endrun : "$endrun" CATEGORY : "$category_

if ( -e .dataset_log ) then
rm .dataset_log
endif

touch .dataset_log
set checkSize_=`wc .dataset_log | awk '{print $1}'`

if ( -e .exclude_list ) then
rm .exclude_list
endif

touch .exclude_list
if ( "$1" == "Mu" ) then
echo "Commissioning\nTOP\nHZZ\nBUNNIES\nBJet\nJetMon\nL1Jets\nMuOnia\nMultiJet" >> .exclude_list
else
echo "Commissioning\nTOP\nHZZ\nBUNNIES\nBJet\nJetMon\nL1Jets\nMuOnia" >> .exclude_list
endif

#while ( "${checkSize_}" == "0" )
#dbsql find dataset where run '>=' $startrun and run '<=' $endrun and dataset = '*Run20*AOD' |grep ${category_} | grep -v -f .exclude_list >> .dataset_log
#set checkSize_=`wc .dataset_log | awk '{print $1}'`
#end

set GlobalTag_=GR_P_V40_AN3  ## 0 : GR_P_V40_AN3 for prompt , 1 : FT_53_V6_AN3 for 13Jul2012, 2 : FT_53_V10_AN3 for 24Aug2012

if ( "${rl_}" == "$Run2012AReRecoRunRange_" ) then
grep "Run2012A-13Jul2012" Data_datasets | grep ${category_} | grep -v -f .exclude_list >> .dataset_log
set GlobalTag_=FT_53_V6_AN3
else if ( "${rl_}" == "$Run2012BReRecoRunRange_" ) then
grep "Run2012B-13Jul2012" Data_datasets | grep ${category_} | grep -v -f .exclude_list >> .dataset_log
set GlobalTag_=FT_53_V6C_AN3
else if ( "${rl_}" == "$Run2012CReRecoRunRange_" ) then
grep "Run2012C-24Aug2012" Data_datasets | grep ${category_} | grep -v -f .exclude_list >> .dataset_log
set GlobalTag_=FT53_V10A_AN3
else if ( "${rl_}" == "$Run2012CPromptRunRange_" ) then
grep "Run2012C-PromptReco" Data_datasets | grep ${category_} | grep -v -f .exclude_list >> .dataset_log
set GlobalTag_=FT_P_V42C_AN3
else if ( "${rl_}" == "$Run2012DPromptRunRange_" || "${rl_}" == "$Run2012DPromptRunRangeRest_" ) then
grep "Run2012D-PromptReco" Data_datasets | grep ${category_} | grep -v -f .exclude_list >> .dataset_log
set GlobalTag_=FT_P_V42_AN3
endif

set datasets_=`cat .dataset_log`

set run_range=$rl_

#### SET JSON FILE LIKE /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/JSONFILE ####
set JSONFILE=json_DCSONLY.txt

#### CREATE SUBMITTING FOLDER : DATASET+RUN RANGE and SUBMIT CRAB JOBS ####
foreach lt($datasets_)
   set lt_t1=`echo $lt | awk -F "/" '{print $2"_"$3}'`
   set lt_t=$lt_t1\_$run_range
   if ( -e ./$SUBMIT_dataFolder_/$lt_t ) then
   continue
   endif
   echo $lt_t1\_$run_range
   mkdir -p ./$SUBMIT_dataFolder_/$lt_t

   if ( !(-e ../bprimeKit.py )) then
   echo "No ../bprimeKit.py"
   echo "Please execute this script under MyAna/bprimeKit/test/Production_Data_DCSONLY_CMSSW52X_AOD"
   exit
   endif
   sed 's/dataEIDMVA\///g' ../bprimeKit.py | sed "s/GR_P_V40_AN2/${GlobalTag_}/g" >& ./$SUBMIT_dataFolder_/$lt_t/bprimeKit.py

   set lt_c=`echo $lt | sed 's/\//\\\//g'`
   sed "s/datasetpath=DataSet/datasetpath=$lt_c/g" crab_data.cfg | \
       sed "s/JSONFILE/$JSONFILE/g" | \
       sed "s/name_directory_you_want/$lt_t \ncheck_user_remote_dir = 0/g" | \
       sed "s/runselection/runselection \nrunselection = $run_range \n# /g"| \
       sed "s/T3_folder/$T3_folder/g" > crab_test.cfg

   mv crab_test.cfg ./$SUBMIT_dataFolder_/$lt_t/crab.cfg

   #### START TO SUBMIT JOBS #### 
   #cd ./$SUBMIT_dataFolder_/$lt_t ; crab -create -submit -cfg crab.cfg ; cd -

   cd ./$SUBMIT_dataFolder_/$lt_t ; crab -create -cfg crab.cfg >& log_$lt_t
   set JobNumber_=`grep "crab:  Total" log_$lt_t | grep "jobs created"| awk '{print $4}'|head -n 1`
   set checkJobNumber_=`grep "crab:  Total" log_$lt_t | grep "jobs created"| awk '{print $4}'|head -n 1|wc|awk '{print $1}' `

   if ( $checkJobNumber_ != "0" ) then
      if ( $JobNumber_ < 500 ) then
         echo "crab -submit "
         crab -submit 
         crab -status 
      else
         set counter_=1
         set counterEnd_=0
         while ( $counter_ < $JobNumber_ ) 
            @ counterEnd_=$counterEnd_ + 500
            if ( $counterEnd_ > $JobNumber_ ) then
            @ counterEnd_=$JobNumber_
            endif
            echo "crab -submit ${counter_}-${counterEnd_} "
            crab -submit ${counter_}-${counterEnd_} 
            crab -status 
            @ counter_=$counter_ + 500
         end
      endif
   else
      echo "[WARNING] No matched site for $lt_t"
   endif

   cd -

end ## for datasets_

end ## for runlist

#### CRAB INFORMATION ####
## crab -forceResubmit job	// once you can not resubmit job
## crab -report		// for calculate lumi

## ls | awk '{print "ls "$1"/crab_0*/share/arguments.xml"}' | csh | awk -F"/" '{print "mkdir -p ../SAVED/"$1"/"$2"/"$3"; cp "$1"/"$2"/"$3"/arguments.xml ../SAVED/"$1"/"$2"/"$3}' | csh
