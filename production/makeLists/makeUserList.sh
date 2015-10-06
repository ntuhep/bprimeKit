#!/bin/bash

function getSection(){
   local file=$1
   local section=$2   
   local sedcmd="sed '/\[${2}\]/,/\[.*\]/!d'"

   cat $file | eval $sedcmd | head -n -1 | tail -n +2
}

USERLIST=( `getSection ./cfgFiles/User.cfg USER `)

for user in ${USERLIST[@]} ; do 
   userfile=MC_dataset_${user}
   if [[ -e $userfile ]]; then
      rm -rf $userfile
   fi

   datasetList=`grep $user ./MC_Twiki.txt | sed "s@|@@" | awk '{print $1}'`
   for dataset in $datasetList ; do 
      echo $dataset >> $userfile
   done
   
   
   userfile=Data_dataset_${user}
   if [[ -e $userfile ]]; then
      rm -rf $userfile
   fi

   datasetList=`grep $user ./Data_Twiki.txt | sed "s@|@@" | awk '{print $1}'`
   for dataset in $datasetList ; do 
      echo $dataset >> $userfile
   done
done
