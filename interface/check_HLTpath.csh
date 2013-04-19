#!/bin/tcsh

cmsenv
rm format.h TriggerBooking.h
cvs update

set TriggerSize=`grep "define N_TRIGGER_BOOKINGS" format.h | awk '{print $3}'`
set WORKSPACE=`pwd`

cd $WORKSPACE
cp /afs/cern.ch/user/y/ymtzeng/public/html/HLT_PRESCALE/HLT_list .

# from database
if ( -e .hlt_sum_ck ) then
rm .hlt_sum_ck
endif
cat HLT_list | awk '{print "@"$1"@"}' | sort -u >& .hlt_sum_ck 

# from NTU
if ( -e .hlt_ntu_ck ) then
rm .hlt_ntu_ck
endif
grep -A 99999 "TriggerBitNumber" TriggerBooking.h | grep -v TriggerBitNumber | grep -v "}" | sed 's/,//g' | awk '{print "@"$1"@"}' >& .hlt_ntu_ck 

# Make a different hlt file 
if ( -e .hlt_differ_ck ) then
rm .hlt_differ_ck
endif
grep -v -f .hlt_ntu_ck .hlt_sum_ck | sed 's/@//g' >& .hlt_differ_ck


if ( -e hltP1 ) then
rm hltP1
endif
awk '{print "\""$1"\","}' .hlt_differ_ck | awk '{i++;for(j=1;j<=NR;j++){if(j==i){printf("   %-    88s //%i\n",$1,i+"'$TriggerSize'"-1);} }}' >& hltP1

if ( -e hltP2 ) then
rm hltP2
endif
awk '{print "   "$1","}' .hlt_differ_ck  >& hltP2

set addNewTriggerPaths=`wc hltP1 | awk '{print $1}'`
set TriggerSizeNew=$TriggerSize
@ TriggerSizeNew+=$addNewTriggerPaths

if( "$addNewTriggerPaths" != "0" ) then

#1). In format.h, please change "#define N_TRIGGER_BOOKINGS    '$TriggerSize'" to "#define N_TRIGGER_BOOKINGS    '$TriggerSizeNew'" 
sed "s/N_TRIGGER_BOOKINGS\t$TriggerSize/N_TRIGGER_BOOKINGS\t$TriggerSizeNew/g" format.h >& format.h123_
mv format.h123_ format.h
#2). In TriggerBooking.h, please paste the "hltP1" and "hleP2" file'
set TriggerSize_=$TriggerSize
@ TriggerSize_-=1

set lastOnePrevious=`grep $TriggerSize_ TriggerBooking.h | awk -F'"' '{print $2}'`
sed "s/$lastOnePrevious/${lastOnePrevious},/g" TriggerBooking.h | sed 's/,"/"/g' | sed 's/'$lastOnePrevious'"/'$lastOnePrevious'",/g' >& TriggerBooking.h123_
mv TriggerBooking.h123_ TriggerBooking.h123

set hltpart1_=`grep -n $lastOnePrevious TriggerBooking.h123 | awk -F: '{print $1}' | head -n 1`
set hltpart2_=`grep -n $lastOnePrevious TriggerBooking.h123 | awk -F: '{print $1}' | tail -n 1`
set hltpart3_=`wc TriggerBooking.h123 | awk '{print $1}'`
@ hltpart21_=$hltpart2_
@ hltpart21_-=$hltpart1_ 

@ hltpart32_=$hltpart3_
@ hltpart32_-=$hltpart2_

if ( -e hltpart1 ) then
rm hltpart1
endif

if ( -e hltpart2 ) then
rm hltpart2
endif

if ( -e hltpart3 ) then
rm hltpart3
endif

cat TriggerBooking.h123 | head -n $hltpart1_ >& hltpart1
cat TriggerBooking.h123 | head -n $hltpart2_ | tail -n $hltpart21_ >& hltpart2
cat TriggerBooking.h123 | head -n $hltpart3_ | tail -n $hltpart32_ >& hltpart3

rm TriggerBooking.h123
touch TriggerBooking.h123
cat hltpart1 >> TriggerBooking.h123
cat hltP1 >> TriggerBooking.h123
cat hltpart2 >> TriggerBooking.h123
cat hltP2 >> TriggerBooking.h123
cat hltpart3 >> TriggerBooking.h123

set lastOneNew=`cat hltP1 |tail -n 1| awk -F'"' '{print $2}'`
sed "s/$lastOneNew,/$lastOneNew/g" TriggerBooking.h123 | sed 's/'$lastOneNew'",/'$lastOneNew'"/g' >& TriggerBooking.h123_
mv TriggerBooking.h123_ TriggerBooking.h
rm TriggerBooking.h123 hltpart1 hltpart2 hltpart3

echo "########################## Message ################################"
echo "##                                                               ##"
echo "##   Please commit format.h and TriggerBooking.h to CVS like:    ##"
echo "##   cvs commit format.h                                         ##"
echo "##   cvs commit TriggerBooking.h                                 ##"
echo "##                                                               ##"
echo "########################## Message ################################"

endif

