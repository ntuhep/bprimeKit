#`test` Directory

This directory is the main operation directory for bprimeKit ntuple production.

## Local production
```
cmsenv
cmsRun bprimeKit_miniAOD.py <options>
```
Should be sufficient to run a local production. Please define input and output sources manually. 

## Crab production
```
source /cvmfs/cms.cern.ch/crab3/crab.sh
crab submit -c crab_example.py
```
Edit the parameters in `crab_example.py` according to your own needs


