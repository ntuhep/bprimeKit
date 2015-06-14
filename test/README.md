#`test` Directory

This directory is the main operation directory for bprimeKit ntuple production.

## Local production
```
cmsenv
cmsRun bprimeKit_miniAOD.py
```
Should be sufficient to run a local production. please define input and output sources manually 


## CRAB3 for MC
Navigate to the `MC_CRAB` directory first and you could either:

1. Alter the existing `crab_example.py` file and run the command:
```
crab submit -c crab_MC_Example.py
```
2. Define the required datasets in the file `MC_datasets_` and run the command 
```
./MC_Crab.sh [username]
```
for an automated CRAB job submission. 
DO NOT edit the all-caps parameters in the `crab.py` file. This would break the script.
