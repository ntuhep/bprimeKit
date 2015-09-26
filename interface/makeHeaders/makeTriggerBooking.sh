##**************************************************************************************************
##
##  A script for generating the TriggerBooking.h required by the bprimeKit
##  Options: 
##    --refresh: runs the script that refreshes the list found in varaibleList/HLTList.cc
##
##**************************************************************************************************


DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
HEAD_FILE=$DIR/variableHead/TriggerBooking_head.h
TAIL_FILE=$DIR/variableHead/TriggerBooking_tail.h
VAR_LIST=$DIR/variableList/HLTList.cc
OUTPUT_FILE=$DIR/TriggerBooking.h

source $DIR/scripts/colors.sh 
source $DIR/scripts/common.sh

if [[ -e $OUTPUT_FILE ]] ; then 
   rm $OUTPUT_FILE;
fi

## Getting number of High level triggers
numOfHLT=`cat $VAR_LIST | wc -l`
echo "$numOfHLT"

#--------------------------------------------------------------------------------  
#  Begin main control flow
#--------------------------------------------------------------------------------  

fileToFile $OUTPUT_FILE $HEAD_FILE

#-----  Making the string array  -------------------------------------------------------------------
printToFile $OUTPUT_FILE "static const std::string TriggerBooking[N_TRIGGER_BOOKINGS] = { "
counter=0;
for hltLabel in `cat $VAR_LIST` ; do 
   if [[ $counter != $((numOfHLT-1)) ]] ; then 
      printToFile $OUTPUT_FILE "\"$hltLabel\" ,"
   else 
      printToFile $OUTPUT_FILE "\"$hltLabel\""
   fi 
   counter=$((counter+1))
done 
printToFile $OUTPUT_FILE "};"


#-----  Making enum list  --------------------------------------------------------------------------
printToFile $OUTPUT_FILE "enum TriggerBitNumber { "
counter=0;
for hltLabel in `cat $VAR_LIST` ; do 
   if [[ $counter != $((numOfHLT-1)) ]] ; then 
      printToFile $OUTPUT_FILE " $hltLabel = $counter ,"
   else 
      printToFile $OUTPUT_FILE " $hltLabel = $counter"
   fi 
   counter=$((counter+1))
done
printToFile $OUTPUT_FILE "};"

#-----  Appending tail  ----------------------------------------------------------------------------
fileToFile $OUTPUT_FILE $TAIL_FILE
astyle --suffix=none $OUTPUT_FILE

sed -i "s/#define N_TRIGGER_BOOKINGS.*/#define N_TRIGGER_BOOKINGS $numOfHLT/" ./variableHead/format_head.h

