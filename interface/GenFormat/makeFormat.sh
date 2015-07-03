#!/bin/bash

BRANCH_LIST=( "LepInfoBranches" "EvtInfoBranches" "JetInfoBranches" "PairInfoBranches" "GenInfoBranches" "PhotonInfoBranches" );
HEAD_FILE="./format_head.h"
OUTPUT_FILE="./format_test.h"
TEMP_FILE="./.temp"

cat $HEAD_FILE > $OUTPUT_FILE 

for BRANCH in ${BRANCH_LIST[@]} ; do 
   INPUT_FILE="$BRANCH""_List.cc"
   if [[ ! -e $INPUT_FILE ]] ; then 
      echo "*** Warning! Skipping branch $BRANCH,file $INPUT_FILE, doesn't exist!"
      continue
   fi
   
   echo "class $BRANCH { public: " >> $OUTPUT_FILE 
   cat $INPUT_FILE >> $OUTPUT_FILE 
   
   echo "Parsing $BRANCH from file $INPUT_FILE"
   sed 's@//.*@@' $INPUT_FILE |  ## Stripping comments 
   sed 's@\[\s*@ [@'          |  ## Ensuring array declaration spacing 
   sed 's@\s*\]@]@'           |  ## Ensuring array declaration spacing
   sed 's@;@@'                |  ## Stripping semicolons
   sed '/^\s*$/d'             |  ## Stripping blank lines
   cat > $TEMP_FILE

   DEFAULT_BRANCH_NAME=${BRANCH%%Branches} 
   
   #-------------------------  Making RegisterTree functions  -------------------------
   echo ">>> Making RegisterTree function for $BRANCH..." 
   echo "void RegisterTree( TTree* root , std::string name=\"$DEFAULT_BRANCH_NAME\" ) { " >> $OUTPUT_FILE
   
   while read LINE ; do 
      if [[ $LINE == \#* ]] ; then  ## Skipping over bprimeKit specific variables 
         break;
      fi 
      WORD_LIST=( $LINE )
      VTYPE=${WORD_LIST[0]} ; VNAME=${WORD_LIST[1]} ; VSIZE=${WORD_LIST[2]}
      
      ## Getting character flag
      if [[ $VTYPE == "int" ]] ; then
         LEAF_TYPE=I
      elif [[ $VTYPE == "float" ]] ; then   
         LEAF_TYPE=F
      elif [[ $VTYPE == "bool" ]] ; then
         LEAF_TYPE=C
      elif [[ $VTYPE == "char" ]] ; then 
         LEAF_TYPE=C
      elif [[ $VTYPE == "size_t" ]] ; then 
         LEAF_TYPE=l
      elif [[ $VTYPE == *"vector"*"*" ]] ; then
         echo "   >>> Skipping over vector pointer..."
         continue 
      elif [[ $VTYPE == *"vector"* ]] ; then
         LEAF_TYPE=V
      else 
         echo "*** Warning! Skipping unknown data type $VTYPE: Found on the line: $LINE"
         continue 
      fi
      
      BRANCH_NAME="(name+\".${VNAME%%_w}\").c_str()";
      
      if [[ ${VSIZE} == "" ]] ; then
         if [[ $VTYPE != V ]] ; then ## Special case for vectors 
            LEAF_NAME="(name+\".$VAME/$LEAF_TYPE\").c_str()";
            echo "root->Branch( $BRANCH_NAME , &$VNAME ) ;" >> $OUTPUT_FILE
         else
            echo "root->Branch( $BRANCH_NAME , &$VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
         fi 
      elif [[ $VSIZE == "[MAX_BX]" ]] ; then    ## Special case for nBX 
         LEAF_NAME="(name+\".$VNAME[\"+name+\".$nBX]/$LEAF_TYPE\").c_str()";
         echo "root->Branch( $BRANCH_NAME , $VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
      elif [[ $VNAME == "HLT"* ]] ; then        ## Special case for HLT 
         LEAF_NAME="(name+\".$VNAME[\"+name+\".nHLT]/$LEAF_TYPE\").c_str()";
         echo "root->Branch( $BRANCH_NAME , $VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
      elif [[ $VNAME == "TrgBook" ]] ; then    ## Special case for Tiggerbooking
         LEAF_NAME="(name+\".$VNAME[\"+name+\".nTrgBook]/$LEAF_TYPE\").c_str()";
         echo "root->Branch( $BRANCH_NAME , $VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
      elif [[ $VSIZE == "["[1-9]*"]" ]] ; then  ## Special case for fixed size array 
         LEAF_NAME="(name+\".$VNAME$VSIZE/$LEAF_TYPE\").c_str()";
         echo "root->Branch( $BRANCH_NAME , $VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
      else 
         LEAF_NAME="(name+\".$VNAME[\"+name+\".Size]/$LEAF_TYPE\").c_str()";
         echo "root->Branch( $BRANCH_NAME , $VNAME , $LEAF_NAME ) ;" >> $OUTPUT_FILE
      fi 
   done < $TEMP_FILE 

   echo "}" >> $OUTPUT_FILE ## Done register function

   #---------------------------  Making Register function  ----------------------------
   echo ">>> Making register functions for $BRANCH..."
   echo "void Register( TTree* root , std::string name=\"$DEFAULT_BRANCH_NAME\" ) { " >> $OUTPUT_FILE 

   while read LINE ; do 
      if [[ $LINE == \#* ]] ; then  ## Skipping over bprimeKit specific variables 
         break;
      fi 
      WORD_LIST=( $LINE )
      VTYPE=${WORD_LIST[0]} ; VNAME=${WORD_LIST[1]} ; VSIZE=${WORD_LIST[2]}
      
      if [[ $VTYPE == *"vector"*"*" ]] ; then 
         LEAF_TYPE=VP 
      elif [[ $VTYPE == *"vector"* ]] ; then 
         echo "   >>> Skipping over vectors...."
         continue; 
      fi 

      BRANCH_NAME="(name+\".$VNAME\").c_str()";
      
      if [[ ${VSIZE} == "" ]] ; then
         echo "root->SetBranchAddress( $BRANCH_NAME , &$VNAME ) ;" >> $OUTPUT_FILE
         if [[ $LEAF_TYPE = VP ]] ; then  ## Special case for vector pointer
            echo "$VNAME = 0 ; " >> $OUTPUT_FILE   
         fi
      else
         echo "root->SetBranchAddress( $BRANCH_NAME , $VNAME ) ;" >> $OUTPUT_FILE
      fi

   done < $TEMP_FILE 

   echo "}" >> $OUTPUT_FILE
   
   echo "}" >> $OUTPUT_FILE   ## End of class 
   printf "Done with $BRANCH!\n\n" 
done
astyle --suffix=none $OUTPUT_FILE 
