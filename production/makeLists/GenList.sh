#!/bin/bash 

source ./scripts/colors.sh 

function getSection(){
   local file=$1
   local section=$2   
   local sedcmd="sed '/\[${2}\]/,/\[.*\]/!d'"

   cat $file | eval $sedcmd | head -n -1 | tail -n +2
}

#--------------------------------------------------------------------------------  
#  Setting up constants
#--------------------------------------------------------------------------------  

if [[ $1 == "MC" ]] ; then 
   echo -e ${GREEN}Running generation for Monte Carlo!${NC}
   CFGFILE="./cfgFiles/MC_Files.cfg"
   LISTFILE="./MC_dataset"
   SIZEFILE="./sizeList_MC.txt"
elif [[ $1 == "Data" ]] ; then 
   echo -e ${GREEN}Running generation for Data!${NC}
   CFGFILE="./cfgFiles/Data_Files.cfg"
   LISTFILE="./Data_dataset"
   SIZEFILE="./sizeList_Data.txt"
else
   echo "Usage: ./GenList.sh [Data] or ./GenList.sh [MC]"
   exit -1;
fi

RUNLIST=`getSection $CFGFILE RUN`
TAGLIST=`getSection $CFGFILE TAG`
FORMAT=`getSection $CFGFILE FORMAT`
SETLIST=`getSection $CFGFILE DATASET`
USERLIST=( `getSection ./cfgFiles/User.cfg USER` )

userCount=${#USERLIST[@]}
echo $userCount Users: ${USERLIST[@]}

#--------------------------------------------------------------------------------  
#  Checking existence of files
#--------------------------------------------------------------------------------  
if [[ -e $LISTFILE ]] ; then 
   rm -r $LISTFILE
fi 
if [[ -e $SIZEFILE ]] ; then 
   rm -r $SIZEFILE 
fi
for user in ${USERLIST[@]} ; do
   if [[ -e ${LISTFILE}_${user} ]] ; then 
      rm -r ${LISTFILE}_${user}
   fi
done

#--------------------------------------------------------------------------------  
#  Begin main control sequence
#-------------------------------------------------------------------------------- 

#-----  Getting Data sets from wild cards  ---------------------------------------------------------
for run in $RUNLIST ; do 
   for tag in  $TAGLIST ; do 
      for dataSet in $SETLIST ; do

         queryFormat="/${dataSet}/*${run}*${tag}*/$FORMAT"
         queryCommand="das_client --query=\"dataset=$queryFormat\""
         queryCommand=${queryCommand}" --limit=10000"
         #Special case for QCD 
         if [[ $dataSet == "QCD_*_TuneCUETP8M1*" ]] ; then 
            queryCommand=${queryCommand}" | grep \"QCD_Pt_[0-9]*to[0-9a-zA-Z]*_TuneCUE\""
         fi

         echo -e "Making query for datset: ${WHITE}$queryFormat${NC}"
         eval $queryCommand | grep ${FORMAT} >> $LISTFILE

      done 
   done 
done

#-----  Getting data set total file size  ----------------------------------------------------------
totalSize=0
for dataSet in `cat $LISTFILE` ; do
   printf "Getting filesize for ${WHITE}$dataSet${NC}..."
   com="das_client --query=\"file dataset=$dataSet | sum(file.size)\""
   size=`eval $com | grep file | awk 'NR==1'  `
   size=${size##*=}
   totalSize=$((totalSize + size))

   echo $dataSet $size >> $SIZEFILE
   printf "$size\n"
done

#-----  Generation data set for user based on file size  -------------------------------------------
userIndex=0
user=${USERLIST[$userIndex]}
userFilesize=0
targetFilesize=$(( totalSize / userCount ))  

while read -r line ; do
   dataSet=` echo $line  | awk '{print $1}' `
   dataSize=` echo $line  | awk '{print $2}' `
   echo $dataSet >> ${LISTFILE}_${user}
   userFilesize=$(( userFilesize + dataSize ))
   
   if [[ $userFilesize -gt $targetFilesize ]]; then
      userIndex=$((userIndex + 1 ))
      user=${USERLIST[$userIndex]}
      userFilesize=0
   fi
done < "$SIZEFILE"
