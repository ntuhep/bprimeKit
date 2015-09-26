##**************************************************************************************************
##
##  Common functions used by scripts
##
##**************************************************************************************************

source $DIR/scripts/colors.sh

checkDir()
{
   ## Creates directory if it doesn't exist.
   if [[ ! -d $1 ]] ; then 
      if [[ -e $1 ]] ; then 
         echo "${RED}ERROR!${NC}: A file exists for ${WHITE}$1${NC}"
      else 
         mkdir $1
      fi
   fi
}

printToFile () {
   echo "$2" >> $1
}

fileToFile () {
   cat $2 >> $1
}

