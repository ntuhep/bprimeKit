#!/bin/bash 

#--------------------------------------------------------------------------------  
#  Defining content variables
#-------------------------------------------------------------------------------- 
DATA_SET_LIST=( "`cat ./MC_dataset`" )
LOCAL_STORAGE="`pwd`/BprimeKit_Ntuples_MC"
LOG_FILE=$LOCAL_STORAGE/log.txt

#--------------------------------------------------------------------------------  
#  Helper functions
#--------------------------------------------------------------------------------  
function getDataLabel () {
echo $1 | awk -F "/" '{print $2}' 
}


function listDataSets () {
local dataSetList="$1"
printf "  %80s | %s\n" "[Data label]" "[Data set full name]"
for dataSet in $dataSetList ; do 
   local dataSetLabel=`getDataLabel $dataSet`
   printf "  %80s | %s\n" $dataSetLabel $dataSet
done
}

function descendxrdfs () {
local presentDir=$1
local level=$2
if [[ $level -eq  0 ]]; then
   echo "$presentDir"
else 
   nextDirList=`xrdfs $REMOTE_SERVER ls $presentDir`
   for nextDir in $nextDirList ; do 
      descendxrdfs $nextDir $(( level - 1 ))
   done 
fi
}

function getSubmitTime () {
local fullPath=$1
local submitTime=`expr "$fullPath" : '.*\(/[0-9]*_[0-9]*/.*\)'`
local submitTime=${submitTime%/*/*}
local submitTime=${submitTime#/}
local submitTime=${submitTime/_/}
echo $submitTime
}

function getFileSize () {
local fullPath=$1
local fileSize=`xrdfs $REMOTE_SERVER stat $fullPath | grep Size |  awk '{print $2}'`
echo $fileSize
}

function remotecp() {
local remoteFile=$1
local localFile=$2
echo "  >> $remoteFile"
xrdcp -f xroot://$REMOTE_SERVER/$remoteFile $localFile &> /dev/null
}

function addLogEntry(){
local submitTime=$1
local targetLabel=$2
local fileName=$3
local fileSize=$4
echo "$1 $2 $3 $4" >> $LOG_FILE
}

function changeLogEntry() {
local submitTime=$1
local targetLabel=$2
local fileName=$3
local fileSize=$4
sed -i "/$targetLabel $fileName/d" $LOG_FILE
addLogEntry $1 $2 $3 $4
}

function getSection(){
   local file=$1
   local section=$2   
   local sedcmd="sed '/\[${2}\]/,/\[.*\]/!d'"

   cat $file | eval $sedcmd | head -n -1 | tail -n +2
}

#--------------------------------------------------------------------------------  
#  Main control flow begin
#--------------------------------------------------------------------------------  


#-----  Setting up constants  ----------------------------------------------------------------------
REMOTE_SITE=`getSection ./Sites.cfg SITE`
if [[ $REMOTE_SITE == "T2_CH_CERN" ]] ; then
   REMOTE_SERVER="eoscms.cern.ch"
fi
LFN_DIR_LIST=( `getSection ./Sites.cfg LFN` )


#-----  Argument parsing  --------------------------------------------------------------------------
if [[ $1 == "" ]] ; then  
   echo "---------------------------------  Instructions  ----------------------------------"
   echo "  - To get individual data sets run: ./GetNtuples.sh [DATA_LABEL_1] [DATA_LABEL_2] .... "
   echo "  - To get   \"all\"    data sets run: ./Get_Ntuples_inrne4.sh  all"
   echo ""
   echo "  Available data sets:"
   listDataSets  "$DATA_SET_LIST"
   echo "-------------------------------  End Instructions  --------------------------------"
   exit 0
fi

#-----  Getting target list  -----------------------------------------------------------------------
targetLabelList=();
dataLabelList=();   ## All labels
for data in ${DATA_SET_LIST[@]} ; do 
   dataLabelList+=( "`getDataLabel $data`" )
done

if [[ "all" =~ $@ ]] ; then
   targetLabelList=( "${dataLabelList[@]}" ) 
else
   for targetLabel in $@ ; do
      if [[ " ${dataLabelList[*]} " == *" $targetLabel "* ]] ; then
         targetLabelList+=("$targetLabel")
      else
         echo "Warning! Skipping over illegal label \"$targetLabel\"..."
      fi 
   done
fi

#-----  Checking file dependencies  ----------------------------------------------------------------
for targetLabel in ${targetLabelList[@]} ; do 
   if [[ ! -d $LOCAL_STORAGE/$targetLabel ]] ; then 
      mkdir -p $LOCAL_STORAGE/$targetLabel
   fi
done

if [[ ! -e $LOG_FILE ]] ; then
   echo "# submit_time  filename   Size" > $LOG_FILE 
fi

#-----  Descending into xrd file system  -----------------------------------------------------------
# path/to/file=/cms/store/user/[userid]/[crabdir]/[dataLabel]/[crabName]/[submitTime]/0000/[rootfile]

for targetLabel in ${targetLabelList[@]} ; do
   for lfn_dir in ${LFN_DIR_LIST[@]} ; do 
   echo "Finding files for $targetLabel...."
   targetFileList=()
   labelPath=$lfn_dir/$targetLabel 
   targetFileList+=("`descendxrdfs $labelPath 4`")

   for targetFile in ${targetFileList[@]} ; do

      if [[ $targetFile != *".root" ]]; then continue ; fi

      fileName=${targetFile##*/}
      submitTime=`getSubmitTime $targetFile`
      fileSize=`getFileSize $targetFile`
      localFilePath=$LOCAL_STORAGE/$targetLabel/$fileName

      previousTime=`grep $fileName $LOG_FILE | awk '{print $1}'`
      previousSize=`grep $fileName $LOG_FILE | awk '{print $3}'`

      if [[ $previousTime == "" ]] ; then 
         echo ">>> Inserting new file $fileName"
         addLogEntry $submitTime $targetLabel $fileName $fileSize
         remotecp $targetFile $localFilePath

      elif [[ $submitTime -gt $previousTime ]] ; then 
         echo ">>> New submission for file $fileName found!"
         changeLogEntry $submitTime $targetLabel $fileName $fileSize
         remotecp $targetFile $localFilePath

      elif [[ $fileSize -gt $previousSize ]]; then
         echo ">>> New update for file $fileName found!"
         changeLogEntry $submitTime $targetLabel $fileName $fileSize
         remotecp $targetFile $localFilePath
      fi
   done
done
done
