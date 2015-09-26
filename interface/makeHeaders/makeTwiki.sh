#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

#-----  Including custom functions  ----------------------------------------------------------------
source ./scripts/common.sh     ## For color output
source ./scripts/makeNames.sh
source ./scripts/colors.sh  ## For make branch/leaf name generation

#-----  Declaring Global constant variables  -------------------------------------------------------
BRANCH_LIST=( "EvtInfoBranches" "LepInfoBranches" "JetInfoBranches" "PairInfoBranches" "GenInfoBranches" "PhotonInfoBranches" "VertexInfoBranches");
HEAD_FILE="./variableHead/format_head.h"
TAIL_FILE="./variableHead/format_tail.h"

#--------------------------------------------------------------------------------  
#  Main control flow
#--------------------------------------------------------------------------------

for branch in ${BRANCH_LIST[@]}  ; do 
   inputFile=./variableList/${branch}_List.cc
   tempFile=${branch}.txt
   outputFile=./${branch}.twiki
  
   if [[ -e $outputFile ]] ; then 
      rm $outputFile
   fi 
   printToFile $outputFile "---+ *$branch*"
   printToFile $outputFile "List of variables store in class $branch"
   printToFile $outputFile "| *type* | *variable name* | *Additional description* | "

   sed "s@\(^[a-zA-Z_:\<\>\*]\+\)@\|\1\|@" $inputFile |  ## Encapsulating variable types
   sed "s@^\/\/-*@|*@"                                |  ## Making sections part 1
   sed "s@----*@*||@"                                 |  ## Making sections part 2
   sed "s@;@|@"                                       |  ## Ending variable name
   sed "s@\/\/@@"                                     |  ## Stripping remaining comments
   sed "s@|@ | @g"                                    |  ## Padding labels
   sed "s@\(.*\)\$@\1 |@"                             |  ## Ending additional descriptions
   sed "s@  *@ @g"                                    |  ## Removing left spaces
   sed "s@|\s\+|\s\+|@|||@g"  > $tempFile ## Contracting empty shell part 2

   fileToFile $outputFile $tempFile
   rm $tempFile
done
