
#!/bin/bash

#--------------------------------------------------------------------------------  
#  Environment setup
#--------------------------------------------------------------------------------  
eval `scramv1 runtime -sh`
EOS="/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"

source ./scripts/common.sh 

#--------------------------------------------------------------------------------  
#  Constant variables definitions
#--------------------------------------------------------------------------------  
BASE_DIR="/eos/cms/store/data"
RUN_PERIOD=Run2015B
DATA_SET_LIST=HLTPhysics
MinRunNumber=0
OUTPUT_DIR="./TriggerPathLog"
CONFIG_TEMPLATE="./triggerDemo_eos.py"
CONFIG_DIR="./configAndTemp"
LIST_FILE="./variableList/HLTList.cc"

#--------------------------------------------------------------------------------  
#  Helper functions
#--------------------------------------------------------------------------------  

descendEOS () 
{
   ## descending into eos file system recursively
   local currentDir=$1
   local level=$2

   if [[ $level -eq 0 ]] ; then 
      local fileList=`$EOS ls -l $currentDir | awk '{print $9}'`
      for file in $fileList ; do 
         echo $currentDir/$file
      done
   else
      local nextLevelList=`$EOS ls -l $currentDir | awk '{print $9}'`
      for nextLevel in $nextLevelList ; do 
         descendEOS "$currentDir/$nextLevel" $((level-1))
      done 
   fi 
}

makeLabel() 
{
   ## Make a filename label from full path name
   local filePath=$1
   local dataSet=$2
   local runLabel=${filePath%/*/*}
   runLabel=${runLabel#/*/*/*/*/*/*/*/*/}
   runLabel=${runLabel//\//_}
   runLabel=${dataSet}_${runLabel}
   echo $runLabel
}


#--------------------------------------------------------------------------------  
#  Beginning the main control flow
#--------------------------------------------------------------------------------  

refreshHLTList () 
{
   checkDir $CONFIG_DIR 
   checkDir $OUTPUT_DIR 

   for dataSet in $DATA_SET_LIST ; do 
      filepathList=`descendEOS $BASE_DIR/$RUN_PERIOD/$dataSet 6`
      for filePath in $filepathList ; do 
         label=`makeLabel $filePath $dataSet`
         configFile=$CONFIG_DIR/triggerCif_${label}.py
         logFile=$CONFIG_DIR/triggerCif_${label}.log
         outputFile=$OUTPUT_DIR/${label}.log

         if [[ -e $logFile ]] ; then 
            printf "${RED}** Warning${NC}: Removing previous existence of $configFile\n"
            rm $configFile
         fi
         
         if [[ -e $configFile ]] ; then 
            printf "${RED}** Warning${NC}: Removing previous existence of $configFile\n"
            rm $configFile
         fi

         printf "> Running with input file $label\n"
         sed "s@FILEPATH@$filePath@" $CONFIG_TEMPLATE >& $configFile
         cmsRun $configFile &> $logFile
         
         grep "TriggerPath=" $logFile >& $outputFile 
      done 
   done

   ## Stripping to final output
   cat ${OUTPUT_DIR}/*log |
   awk '{print $2}'       |
   sort --unique          |
   grep --invert-match triggerDemo >> $LIST_FILE
}
