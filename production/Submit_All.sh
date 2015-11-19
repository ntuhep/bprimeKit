#!/bin/bash
#-------------------------------------------------------------------------------
#
#   File        : Submit_All.sh
#   Descriptions: Submit all data samples designated to user 
#   Usage       : ./Submit_All.sh
#
#-------------------------------------------------------------------------------

sampleList="Dataset_${USER}.txt"

if [[ ! -e $sampleList ]]; then
   echo "Error! File $sampleList not found!" 
   exit 1
fi

for sample in $(cat $sampleList) ; do
   echo "Working on ${sample}..."
   ./Submit_Sample.sh $sample
done
