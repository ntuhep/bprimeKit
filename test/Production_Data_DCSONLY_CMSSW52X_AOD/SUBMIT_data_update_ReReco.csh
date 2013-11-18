#!/bin/tcsh

#### SET FOLDER IN Tier3 FOR SAVING OUTPUT FILES like :/dpm/phys.ntu.edu.tw/home/cms/store/user/USERID/T3_folder #### 
set T3_folder=CMSSW_5_3_11_data_8TeV_22Jan2013ReReco_AOD_v3

#### SET RUN RANGE BY HAND (LIKE run_rangeByhand="161234-164321"). IF NOT SET BY HAND, IT WILL BE SET AUTOMATICALLY ####
set run_rangeByhand=""      ## for default
set category_=""    ## [Ele, Mu, Jet]

set SUBMIT_dataFolder_=SUBMIT_data

if ( "$1" == "" ) then
   echo "##################################################################################################"
   echo "##   PLEASE INPUT THE DATASET GATEGORY like                                                     ##"
   echo "##      ./SUBMIT_data_update_prompt_DCSONLY.csh [GATEGORY]                                      ##"
   echo "##   [GATEGORY] : DoubleElectron, DoubleMuParked, DoublePhoton, ElectronHad, Jet, JetHT, MuEG,  ##"
   echo "##                MuHad, MultiJet, Photon, PhotonHad, SingleElectron, SingleMu, SinglePhoton,   ##"
   echo "##                SinglePhotonParked                                                            ##"     
   echo "##################################################################################################"
   exit
endif


set category_=$1

#### SOURCE CRAB ENVIRONMENT ####
setenv SCRAM_ARCH slc5_amd64_gcc462
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
cmsenv
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh
voms-proxy-init -voms cms -valid 192:0	

set GlobalTag_=FT_53_V21_AN5  ## 0 : GR_P_V40_AN3 for prompt , 1 : FT_53_V6_AN3 for 13Jul2012, 2 : FT_53_V10_AN3 for 24Aug2012
#### SET JSON FILE LIKE /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/JSONFILE ####
set JSONFILE=json_DCSONLY.txt

if ( ! -e run.list ) then
echo "Please check if run.list exists or not."
endif

set run_list=`awk '{print $2}' run.list`

foreach run_range($run_list)
   set RUNPERIOD=`cat run.list|grep $run_range | awk '{print $1}'`    
   set lt=`cat Data_datasets | grep $RUNPERIOD | grep "/$1/"`
   set ltStatus_=`cat Data_datasets | grep $RUNPERIOD | grep "/$1/"|wc|awk '{print $1}'`

   if ( $ltStatus_ != "1" ) then
   echo "Please check dataset keyword... [Status : $ltStatus_]"
   echo "cat Data_datasets | grep $RUNPERIOD | grep /$1/"
   continue
   endif

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

end ## for runlist

#### CRAB INFORMATION ####
## crab -forceResubmit job	// once you can not resubmit job
## crab -report		// for calculate lumi

## ls | awk '{print "ls "$1"/crab_0*/share/arguments.xml"}' | csh | awk -F"/" '{print "mkdir -p ../SAVED/"$1"/"$2"/"$3"; cp "$1"/"$2"/"$3"/arguments.xml ../SAVED/"$1"/"$2"/"$3}' | csh
