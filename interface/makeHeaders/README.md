# bprimeKit `interface` management 

## `format.h` management
The script  `makeFormat.sh` automaticlly generates the `format.h` required by the bprimeKit to ensure that every data type is in sync. 
The variables store could be found in the [`variableList`](./variableList) directory. 

Standard storage format:
  - The class is stored in the filename as: `[BranchName]_List.cc`
  - Each branchs has a default name, stored in the filename as: `[DefaultName]Branches_List.cc`
  - All should use ROOT flavour variable types rather than standard C++ types to unified type conversion rules.
  - Specical cases for the vector variables are used.

The scripts `makeTwiki.sh` generates the documentation of the scripts based on the comments of the variables in question.
Notice that multiline commenting is not get supported.

## `TriggerBooking.h` management

The script `./makeTriggerBooking.sh` makes the TriggerBooking.h file out of the existing [`variableList/HLTList.cc`](./variableList/HLTList.cc) file,
the TriggerBooking is essentially two hard-coded objects, the `string` list of the HLT names and a `enum` for interface unification.

The script `makeHLTList.sh` gets available High Level Trigger names out of the list of selected miniAODs files.
This scripts also calls for the `TriggerBooking.h` and `format.h` file to be updated by their respective scripts 
The information is then passed to the `makeTriggerBooking.sh` script to make it into a the stardard bprimeKit `TriggerBooking.h` file required.

## Miscellenous 

The scripts directory is a handleful of common variable to make the output of the scripts for uniformed.

The `copyFiles.sh` is to copy the output file to the bprimeKit, if this package was deployed in the CMS src directory that is the same as the bprimeKit.

