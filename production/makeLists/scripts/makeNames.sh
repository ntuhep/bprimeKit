##**************************************************************************************************
##
##  Functions for making the strings required for TTree intreactions
##
##**************************************************************************************************

EXIT_STRING="EXIT"
UNKNOWN_STRING="UNKNOWN"

makeBranchName() 
{
   VARIABLE_TYPE=$1
   VARIABLE_NAME=$2
   VARIABLE_SIZE=$3
   
   echo "(name+\".${VARIABLE_NAME%%_w}\").c_str()"; 
}

makeAddress()
{
   VARIABLE_TYPE=$1
   VARIABLE_NAME=$2
   VARIABLE_SIZE=$3

   if [[ $VARIABLE_SIZE == ""  ]] ; then 
      echo "&$VARIABLE_NAME"; 
   else
      echo "$VARIABLE_NAME"
   fi 
}

makeLeafName()
{
   VARIABLE_TYPE=$1
   VARIABLE_NAME=$2
   VARIABLE_SIZE=$3

   ## Getting character flag
   if [[ $VTYPE == "Int_t" ]] ; then
      TYPE_TOKEN=I
   elif [[ $VTYPE == "Float_t" ]] ; then   
      TYPE_TOKEN=F
   elif [[ $VTYPE == "Bool_t" ]] ; then
      TYPE_TOKEN=O
   elif [[ $VTYPE == "Char_t" ]] ; then 
      TYPE_TOKEN=C
   elif [[ $VTYPE == "ULong64_t" ]] ; then 
      TYPE_TOKEN=l
   elif [[ $VTYPE == *"vector"*"*" ]] ; then
      echo $EXIT_STRING
      exit -1;
   elif [[ $VTYPE == *"vector"* ]] ; then
      TYPE_TOKEN=V
   else 
      echo $UNKNOWN_STRING
      exit -1;
   fi
   
   if [[ $VARIABLE_SIZE == "" ]]; then
      if [[ $TYPE_TOKEN == "V" ]]; then
         exit 0; ## No Leaf name 
      else 
         echo "(name+\".$VARIABLE_NAME/$TYPE_TOKEN\").c_str()"
      fi
   elif [[ $VARIABLE_SIZE == "[MAX_BX]" ]] ; then    ## Special case for nBX 
      echo "(name+\".$VNAME[\"+name+\".nBX]/$TYPE_TOKEN\").c_str()";
   elif [[ $VARIABLE_NAME == "HLT"* ]] ; then        ## Special case for HLT 
      echo "(name+\".$VNAME[\"+name+\".nHLT]/$TYPE_TOKEN\").c_str()";
   elif [[ $VARIABLE_NAME == "TrgBook" ]] ; then    ## Special case for Tiggerbooking
      echo "(name+\".$VNAME[\"+name+\".nTrgBook]/$TYPE_TOKEN\").c_str()";
   elif [[ $VARIABLE_SIZE == "["[1-9]*"]" ]] ; then  ## Special case for fixed size array 
      echo "(name+\".$VNAME$VSIZE/$TYPE_TOKEN\").c_str()";
   else 
      echo "(name+\".$VNAME[\"+name+\".Size]/$TYPE_TOKEN\").c_str()";
   fi 
}
