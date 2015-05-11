#!/bin/bash 

## Defining running variables
MaxLabel="100"
EventCount="300"
Channel="Tstar900GeV-ggChannel"
EventDir="$HOME/Events/$Channel/"
WorkDir="/afs/cern.ch/work/y/yichen/CMSSW_7_3_1_patch2/src/"
ScriptDir="/afs/cern.ch/work/y/yichen/sripts/"
StoreDir="/tmp/"
AnaDir="$WorkDir/MyAna/bprimeKit/test/"

##Setting up working environment
echo "cmsenv"
eval `scramv1 runtime -sh`


echo "Creating directories..."
if [ -d "$ScriptDir$Channel" ] ; then
   echo "Directory Already exists! Exiting..."
   exit -1
else
   echo "Making directory $ScriptDir$Channel"
   mkdir -p "$ScriptDir$Channel"
fi

echo "Begin incremental processes"
Label="0"
while [[ $Label -lt MaxLabel ]] ; do 
   EventLabel=$((Label))
   EventLabel=$Channel-$EventLabel
   RunFileDir="$ScriptDir$Channel/$EventLabel"
   inputFile="$EventDir./$EventLabel.lhe"
   echo "Input files: $inputFile" 
   output_1=$StoreDir./$EventLabel-step1.root
   output_2=$StoreDir./$EventLabel-step2.root
   output_3=$StoreDir./$EventLabel-step3.root
   output_4=$StoreDir./$EventLabel-step4.root
   output_4_5=$StoreDir./$EvenLabel-step4_inDQM.root
   output_5=$StoreDir./$EventLabel-step5.root
   
   ## Setting up working directory
   if [ -d "$RunFileDir" ] ; then
      echo "Warning! over-writing previous existence of $RunFileDir"
   else
      echo "Creating working directory $RunFileDir"
      mkdir -p "$RunFileDir"
   fi

   ## Duplicating runfiles
   cp $WorkDir/step1.py $RunFileDir/step1.py
   cp $WorkDir/step2.py $RunFileDir/step2.py
   cp $WorkDir/step3.py $RunFileDir/step3.py
   cp $WorkDir/step4.py $RunFileDir/step4.py
   cp $WorkDir/step5.py $RunFileDir/step5.py

   ####Editing standard recipe files
   sed -i "s@fileNames = cms.untracked.vstring('file:.*')@fileNames = cms.untracked.vstring('file:$inputFile')@"               "$RunFileDir/step1.py"
   sed -i "s@input = cms.untracked.int32(10000)@input = cms.untracked.int32($EventCount)@"                                     "$RunFileDir/step1.py"
   sed -i "s@fileName = cms.untracked.string('file:.*'),@fileName = cms.untracked.string('file:$output_1'),@"                  "$RunFileDir/step1.py"
   sed -i "s@fileNames = cms.untracked.vstring('file:.*')@fileNames = cms.untracked.vstring('file:$output_1')@"                "$RunFileDir/step2.py"
   sed -i "s@input = cms.untracked.int32(2)@input = cms.untracked.int32($EventCount)@"                                         "$RunFileDir/step2.py" 
   sed -i "s@fileName = cms.untracked.string('file:.*'),@fileName = cms.untracked.string('file:$output_2'),@"                  "$RunFileDir/step2.py"
   sed -i "s@fileNames = cms.untracked.vstring('file:.*')@fileNames = cms.untracked.vstring('file:$output_2')@"                "$RunFileDir/step3.py"
   sed -i "s@fileName = cms.untracked.string('file:.*'),@fileName = cms.untracked.string('file:$output_3'),@"                  "$RunFileDir/step3.py"
   sed -i "s@input = cms.untracked.int32(2)@input = cms.untracked.int32($EventCount)@"                                         "$RunFileDir/step3.py" 
   sed -i "s@fileNames = cms.untracked.vstring('file:.*')@fileNames = cms.untracked.vstring('file:$output_3')@"                "$RunFileDir/step4.py"
   sed -i "s@fileName = cms.untracked.string('file:step4\.root'),@fileName = cms.untracked.string('file:$output_4'),@"         "$RunFileDir/step4.py"
   sed -i "s@fileName = cms.untracked.string('file:step4_inDQM\.root'),@fileName = cms.untracked.string('file:$output_4_5'),@" "$RunFileDir/step4.py"
   sed -i "s@input = cms.untracked.int32(2)@input = cms.untracked.int32($EventCount)@"                                         "$RunFileDir/step4.py" 
   sed -i "s@fileNames = cms.untracked.vstring('file:.*')@fileNames = cms.untracked.vstring('file:$output_4')@"                "$RunFileDir/step5.py"
   sed -i "s@input = cms.untracked.int32(2)@input = cms.untracked.int32($EventCount)@"                                         "$RunFileDir/step5.py" 
   sed -i "s@fileName = cms.untracked.string('file:.*'),@fileName = cms.untracked.string('file:$output_5'),@"                  "$RunFileDir/step5.py"
   
   outputFile="$StoreDir./yichen-$EventLabel-results.root"
   echo "outputFile is $outputFile" ;
   ### Ntuples file editing 
   cp "$AnaDir./bprimeKit_miniAOD.py"                         "$RunFileDir/bprimeKit_miniAOD-$EventLabel.py"
   sed -i "s@'file:.*'@'file:$output_5'@"                     "$RunFileDir/bprimeKit_miniAOD-$EventLabel.py"
   sed -i "s@resultsFile = '.*'@resultsFile = '$outputFile'@" "$RunFileDir/bprimeKit_miniAOD-$EventLabel.py"
   
   echo "Making script files...."
   ### Creating execution script 
   echo "#!/bin/bash"                                                      >> $RunFileDir/run_local.sh
   echo "cd $WorkDir"                                                      >> $RunFileDir/run_local.sh
   echo "eval \`scramv1 runtime -sh\`"                                     >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/step1.py &> /dev/null "                            >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/step2.py &> /dev/null "                            >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/step3.py &> /dev/null "                            >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/step4.py &> /dev/null "                            >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/step5.py &> /dev/null "                            >> $RunFileDir/run_local.sh
   echo "cd $AnaDir"                                                       >> $RunFileDir/run_local.sh
   echo "cmsRun $RunDir/bprimeKit_miniAOD-$EventLabel.py &> /dev/null"     >> $RunFileDir/run_local.sh
   echo "scp $outputFile ensc@ntugrid5.phys.ntu.edu.tw:~/Results/$Channel" >> $RunFileDir/run_local.sh
   echo "rm $output_1 $output_2"                                           >> $RunFileDir/run_local.sh
   echo "rm $output_3 $output_4"                                           >> $RunFileDir/run_local.sh
   echo "rm $output_4_5 $output_5"                                         >> $RunFileDir/run_local.sh
   echo "rm $outputFile"                                                   >> $RunFileDir/run_local.sh
   chmod +x    $RunFileDir/run_local.sh
   bsub -q 1nw $RunFileDir/run_local.sh 
   
   let Label+="1"
done
