#!/bin/bash
#*******************************************************************************
 #
 #  Filename    : submit_dataset.sh
 #  Description : Running a whole dataset with just one run
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
source $PWD/scripts/common.sh

SH_DIR=${PWD}/SH
LOG_DIR=${PWD}/LOG
OUT_TEMP=${PWD}/NTP
OUT_STORE=${PWD}/STORE


function main()
{
   dataset=$1
   name=$( makeName $dataset )
   process=$(getDataProcess $dataset )

   echo "Running on dataset $dataset ($name) ($process)"

   mkdir -p $SH_DIR/${name}
   mkdir -p $LOG_DIR/${name}
   mkdir -p $OUT_TEMP/${name}

   file_list=$( das_client  --limit=1000000 --query="file dataset=$dataset" | grep "store" )
   for file in $file_list ; do
      run_list=$(das_client --limit=1000000 --query="run file=$file" | grep --invert-match "Showing" )
      run_list=${run_list#*\[}
      run_list=${run_list%\]*}
      run_list=${run_list//,/}

      for run in $run_list ; do
         echo $file $run
         script_file=${SH_DIR}/${name}/run_${run}.sh
         log_file=${LOG_DIR}/${name}/run_${run}.txt
         ntp_file=${OUT_TEMP}/${name}/run_${run}.root
         store_file=${OUT_STORE}/${name}/bpk_ntuple_${run}.root

         cmd="cmsRun $PWD/bprimeKit_cfg.py"
         cmd="${cmd} DataProcessing=${process}"
         cmd="${cmd} sample=${file}"
         cmd="${cmd} RunRange=${run}"
         cmd="${cmd} outputLabel=${ntp_file}"
         cmd="${cmd} maxEvents=-1"

         echo "#!/bin/bash" > ${script_file}
         echo "cd $PWD"                      >> $script_file
         echo "eval \`scramv1 runtime -sh\`" >> $script_file
         echo "${cmd} &> ${log_file}"        >> $script_file
         echo "scp ${ntp_file} ${store_file}">> $script_file
         echo "rm ${ntp_file}"               >> $script_file

         chmod +x $script_file

      done
   done

}

main $@
