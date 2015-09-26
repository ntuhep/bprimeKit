##**************************************************************************************************
##
##  Simple script for generating format.h required for the bprime Analysis Kit
##  The variables lists are defined in the directory ./variableList
##  The macros are defined in the directory ./variableHead
##  Custom functions are defined in the directory ./include 
##
##**************************************************************************************************

#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
#-----  Including custom functions  ----------------------------------------------------------------
source $DIR/scripts/common.sh     ## For color output
source $DIR/scripts/makeNames.sh
source $DIR/scripts/colors.sh  ## For make branch/leaf name generation

#-----  Declaring Global constant variables  -------------------------------------------------------
BRANCH_LIST=( "EvtInfoBranches" "LepInfoBranches" "JetInfoBranches" "PairInfoBranches" "GenInfoBranches" "PhotonInfoBranches" "VertexInfoBranches");
HEAD_FILE="$DIR/variableHead/format_head.h"
TAIL_FILE="$DIR/variableHead/format_tail.h"
OUTPUT_FILE="./format.h"

#--------------------------------------------------------------------------------  
#  The Main control flow
#--------------------------------------------------------------------------------

rm $OUTPUT_FILE                    ## Removing previous output 
fileToFile $OUTPUT_FILE $HEAD_FILE ## Inserting headers

for BRANCH in ${BRANCH_LIST[@]} ; do 
   INPUT_FILE=$DIR/variableList/${BRANCH}_List.cc
   TEMP_FILE="./.${BRANCH}_TEMP.txt"
   if [[ ! -e $INPUT_FILE ]] ; then 
      printf "${RED}*** Warning!${NC} Skipping branch ${WHITE}$BRANCH${NC},file ${WHITE}$INPUT_FILE${NC}, doesn't exist!\n"
      continue
   fi
   
   printToFile $OUTPUT_FILE "class $BRANCH { public: "
   fileToFile  $OUTPUT_FILE $INPUT_FILE 
   
   printf "Parsing ${WHITE}$BRANCH${NC} from file ${WHITE}$INPUT_FILE${NC}\n"
   
   sed 's|/\*|\n&|g;s|*/|&\n|g' $INPUT_FILE |
   sed '/\/\*/,/*\//d'  |  ## Removing multi-line comments
   sed 's@//.*@@'       |  ## Remove single line comment
   sed 's@\[\s*@ [@'    |  ## Ensuring array declaration spacing
   sed 's@\s*\]@]@'     |  ## Ensuring array declaration spacing
   sed 's@;@@'          |  ## Stripping semicolons
   sed '/^\s*$/d'       |  ## Stripping blank lines
   cat > $TEMP_FILE

   DEFAULT_BRANCH_NAME=${BRANCH%%Branches} 
   
   #-------------------------  Making RegisterTree functions  -------------------------
   printf ">>> Making RegisterTree function for ${WHITE}$BRANCH${NC}...\n" 
   printToFile $OUTPUT_FILE "void RegisterTree( TTree* root , std::string name=\"$DEFAULT_BRANCH_NAME\" ) {"
   
   while read LINE ; do 
      if [[ $LINE == "#"* ]] ; then  ## Skipping over bprimeKit specific variables 
         break;
      fi 
      WORD_LIST=( $LINE )
      VTYPE=${WORD_LIST[0]} ; VNAME=${WORD_LIST[1]} ; VSIZE=${WORD_LIST[2]}
      
      BRANCH_NAME=`makeBranchName $VTYPE $VNAME $VSIZE`
      ADDRESS=`    makeAddress    $VTYPE $VNAME $VSIZE`
      LEAF_NAME=`  makeLeafName   $VTYPE $VNAME $VSIZE`

      if [[ $LEAF_NAME == $EXIT_STRING  ]]; then
         printf "   >>>Skipping variables: ${WHITE}$VTYPE $VNAME $VSIZE${NC}\n"
      elif [[ $LEAF_NAME == $UNKNOWN_STRING ]]; then
         printf "   ***${RED}Warning!${NC} skipping unknown variable ${WHITE}$VTYPE $VNAME $VSIZE${NC}\n"
      elif [[ $LEAF_NAME == "" ]]; then
         printToFile $OUTPUT_FILE "root->Branch($BRANCH_NAME,$ADDRESS);"
      else 
         printToFile $OUTPUT_FILE "root->Branch($BRANCH_NAME,$ADDRESS,$LEAF_NAME);"
      fi
     

   done < $TEMP_FILE 
   printToFile $OUTPUT_FILE "}"

   #---------------------------  Making Register function  ----------------------------
   printf ">>> Making register functions for ${WHITE}$BRANCH${NC}...\n"
   printToFile $OUTPUT_FILE "void Register( TTree* root , std::string name=\"$DEFAULT_BRANCH_NAME\" ) {"

   while read LINE ; do 
      if [[ $LINE == "#"* ]] ; then  ## Skipping over bprimeKit specific variables 
         break;
      fi 
      WORD_LIST=( $LINE )
      VTYPE=${WORD_LIST[0]} ; VNAME=${WORD_LIST[1]} ; VSIZE=${WORD_LIST[2]}
      
      BRANCH_NAME=`makeBranchName $VTYPE $VNAME $VSIZE`
      ADDRESS=`    makeAddress $VTYPE $VNAME $VSIZE   `

      ## Special case for vector and vector pointer
      if [[ $VTYPE == *"vector"*"*" ]] ; then  
         printToFile $OUTPUT_FILE "$VNAME = 0 ; "
      elif [[ $VTYPE == *"vector"* ]] ; then 
         printf "   >>> Skipping over variables: ${WHITE}$VTYPE $VNAME $VSIZE${NC}\n"
         continue; 
      fi  
      
      printToFile $OUTPUT_FILE "root->SetBranchAddress( $BRANCH_NAME , $ADDRESS ) ;"
   done < $TEMP_FILE 
   printToFile $OUTPUT_FILE "}"  
   
   printToFile $OUTPUT_FILE "};" ## End of class 
   printf "${GREEN}Done${NC} with ${WHITE}$BRANCH${NC}!\n\n"

   rm $TEMP_FILE 
   
done

fileToFile $OUTPUT_FILE $TAIL_FILE

astyle --suffix=none $OUTPUT_FILE 
