#-------------------------------------------------------------------------------
#
#   File        : common.sh
#   Descriptions: Functions used to hide certain implementations
#
#-------------------------------------------------------------------------------

function makeName(){
   echo $1 | awk -F "/" '{print $2"_"$3 }' 
}

function getLHELabel(){
   if [[ $dataset == *"MINIAODSIM" ]]; then
      echo "externalLHEProducer"
   else
      echo ""
   fi
}

function getDataProcess() {
   local data_set=$1
   if [[ $data_set == *"RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"* ]]; then
      echo "MC25ns_MiniAODv2"
   elif [[ $data_set == *"RunIISpring15MiniAODv2-Asympt50ns_74X_mcRun2_asymptotic50ns_v0-v1"* ]]; then
      echo "MC50ns_MiniAODv2"
   elif [[ $data_set == *"Run2015D-PromptReco-v4"* ]]; then
      echo "Data25ns_PromptRecov4"
   elif [[ $data_set == *"Run2015D-05Oct2015"* ]]; then
      echo "Data25ns_MiniAODv2"
   elif [[ $data_set == *"Run2015C_25ns-05Oct2015"* ]]; then
      echo "Data25ns_ReReco"
   elif [[ $data_set == *"Run2015"*"05Oct2015"* ]]; then
      echo "Data50ns_MiniAODv2"
   elif [[ $data_set == *"RunIISpring15DR74"* ]]; then
      echo "MC25ns_MiniAODv1"
   else
      echo ""
   fi
}

