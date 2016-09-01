#-------------------------------------------------------------------------------
#
#   File        : common.sh
#   Descriptions: Functions used to hide certain implementations
#
#-------------------------------------------------------------------------------

function makeName(){
   input=$1
   geometry=$(echo $input | awk -F "/" '{print $2}')
   tag=$(echo $input | awk -F "/" '{print $3}')
   process=$(getDataProcess $input)
   name=""
   if [[ $process == *"Data"* ]]; then
      name="$geometry"_"$tag"
   else
      name=$geometry
   fi
   echo $name
}

function getDataProcess() {
   local data_set=$1
   if [[ $data_set == *"RunIISpring16MiniAODv2"*"reHLT"* ]]; then
      echo "MC_MiniAOD_80X_reHLT"
   elif [[ $data_set == *"RunIISpring16MiniAODv2"* ]]; then
      echo "MC_MiniAOD_80X"
   elif [[ $data_set == *"Run2016"*"-PromptReco-v"* ]]; then
      echo  "Data2016_80X"
   elif [[ $data_set == *"16Dec2015"* ]]; then
      echo "Data25ns_76X"
   elif [[ $data_set ==  *"RunIIFall15MiniAODv2-PU25nsData2015v1"* ]]; then
      echo "MC25ns_MiniAOD_76X"
   elif [[ $data_set == *"RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1"* ]]; then
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
