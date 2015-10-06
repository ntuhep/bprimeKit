# Production management 

The scripts `GenList.sh` will generate the list of files defined in the `./cfgFiles/*_Files.cfg`, and separate the files using the file size in formation. 
It will also generate a twiki.txt file to be copied onto the bprimeKit production twiki.

The scripts `makeUserList.sh` will generate the uselist files based on the twiki.txt files.

The scripts `copyFiles.sh` will copy the files to the desired directories. For legacy reasons, a copy of the master files will be copied to the production directories
