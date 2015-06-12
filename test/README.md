#bprimeKit Ntuple production guide

## Local production
If you have already finished the setup procedure described in the directory above, 
then the local production could be ran simply the commands
```
cmsenv
cmsRun bprimeKit_miniAOD_74X.py
```
in this directory. Please manually alter the input and output sources.


## CRAB3 Production
You could alter the existing `crab_MC_Example.py` file and run the command:
```
crab submit -c crab_MC_Example.py
```

Alternatively, you could define the required datasets in the file `MC_datasets_` and run the command 
```
./MC_Crab.sh [username]
```
for an automated submittion.
DO NOT edit the all-caps parameters in the `crab.py` file. This would break the script.
