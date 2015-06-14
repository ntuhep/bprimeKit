# `plugins` Directory
The plugins directory is the main backbone of the bprimeKit packages, 
defining how the various data is taken from the standard CMSSW format and store into the Ntuple branches defined in [`format.h`](../interface/format.h).
The bprimeKit is self is a inherited `EDAnalyzer` class with the various functionalities implemented in various `bprimeKit_*.cc` files for clarity.

### `bprimeKit.cc`
In [`bprimeKit.cc`](bprimeKit.cc), all the `EDAnalyzer` virtual functions, and class requirements are defined in this file which includes:
   * Constructor/Destructor: 
      Setting up the CMSSW standard input parameters into associated data members.
   * `beginJob()`/`endJob()` methods:
      Setting up the files and branches for file writting
   * `beginRun()`/`endRun()` methods:
      Initializing some datamembers for job running.
   * `analyze( event , setup )` methods:
      High level control flow for the ntuplizing process.

### `bprimeKit_utils*.cc`
In these files, the package unique functions defined in [`bprimeKit_util.h`](../interface/bprimeKit_util.h) are implemented.


### `bprimeKit*.cc` 
In these files, the information of the physical objects are extracted. A list of references are found below:



