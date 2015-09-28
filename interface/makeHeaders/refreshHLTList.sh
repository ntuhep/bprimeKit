#!/bin/bash

#-----  Defining constant variables  ---------------------------------------------------------------
INPUT_FILE_LIST=(
   "/store/data/Run2015B/Charmonium/AOD/PromptReco-v1/000/251/168/00000/1602316B-CF26-E511-BCBA-02163E011BF3.root"
   "/store/data/Run2015C/SingleMuon/MINIAOD/PromptReco-v1/000/253/952/00000/C6F1C99F-4241-E511-A877-02163E01439D.root"
   "/store/data/Run2015D/SingleMuon/MINIAOD/PromptReco-v3/000/256/843/00000/201CB0C7-E560-E511-BB1F-02163E013425.root"
   "/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/70000/F884E560-CE09-E511-95DB-00266CF3DE70.root"
)

INPUT_FILE_PREFIX="root://eoscms//eos/cms/"
DATA_GLOBAL_TAG="74X_dataRun2_Prompt_v0"
MC_GLOBAL_TAG="MCRUN2_74_V9"
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
PYTHON_FILE="${DIR}/hltConfig/getHLTNames.py"
CPP_FILE="${DIR}/hltConfig/printLeaf.cc"
LIST_FILE="${DIR}/variableList/HLTList.cc"
TEMP_FILE="${DIR}/temp.txt"
#-----  sourcing constant  -------------------------------------------------------------------------
source $DIR/scripts/colors.sh

#-----  Setting up environments  -------------------------------------------------------------------
eval `scramv1 runtime -sh`
cd ${DIR}/hltConfig

for inputFile in ${INPUT_FILE_LIST[@]} ; do  
   gtag=""
   if [[ $inputFile == "/store/data"* ]] ; then 
      gtag=$DATA_GLOBAL_TAG
   elif [[ $inputFile == "/store/mc"* ]] ; then 
      gtag=$MC_GLOBAL_TAG
   else 
      printf "${RED}**Warning!${NC} Skipping input file ${WHITE}$inputFile${NC}\n"
      continue
   fi
   
   echo Running $inputFile with $gtag
   options="sample=$INPUT_FILE_PREFIX$inputFile globalTag=$gtag"
   cmsRun $PYTHON_FILE $options &> /dev/null 
   root -l -b -q $CPP_FILE | ## Flushing leaf names
   grep Br                 | ## Stripping to leaf names lines only
   awk '{print $3}'        | ## Stripping to leaf names column only
   sed 's/://'             | ## Stripping odd characters
   sed '/Event/d'          | ## Removing bad leaf name
   sed '/LumiBlock/d'      |
   sed '/Run/d'            |
   sed '/Bx/d'             |
   sed '/Orbit/d'          |
   sed '/AvgInstDelLumi/d' |
   sort --unique >> $TEMP_FILE


   printf  "${GREEN}Done   ${NC} $inputFile with $gtag\n"
done


sort --unique $TEMP_FILE > temp
cat temp > $TEMP_FILE
rm temp

for hltname in `cat $LIST_FILE` ; do 
   echo removing $hltname from $TEMP_FILE ...
   cmd="sed -i '/$hltname/d' $TEMP_FILE"
   eval $cmd
done

cat $TEMP_FILE >> $LIST_FILE
rm $TEMP_FILE


cd -

#-----  Refreshing format.h and TriggerBooking.h  --------------------------------------------------
$DIR/makeTriggerBooking.sh
$DIR/makeFormat.sh
