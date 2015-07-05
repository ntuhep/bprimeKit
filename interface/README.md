# `interface` directory
Here is the quick list of the classes and functions define in this directory sorted by files.

### `format.h`
The file [`format.h`](format.h) defines the branches that are stored in the bprimeKit output Ntuple files. 
All branche classes should have the same structure:
   * All data stored are public, standard C++ datatypes 
   * `RegisterTree( TTree , string )` method: for root file writing setup. 
   * `Register( TTree ,  string )` method: for root file reading setup.

The current defined branches for the bprimeKit ntuple is listed below:
   * `EvtInfoBranches`
   * `LepInfoBranches`
   * `JetInfoBranches`
   * `PairInfoBranches`
   * `PhotonInfoBranches`
   * `VertexInfoBranches` 
   * `GenInfoBranches`

For an example of using the branches, see that file: [`proj.cc`](../test/proj.cc)

For a utility to maintain the format.h see the [BprimeKit-Format-Generator](https://github.com/enochnotsocool/BprimeKit-Format-Generator) package.

### `bprimeKit.h`
The file [`bprimeKit`](bprimeKit) defines the custom `EDAnalyzer` class that performs the bprimeKit ntuplizing process.
For the documentation of the method implementations, read the [`README.md`](../plugins/README.md) in the plugins directory.


### `bprimeKit_util.h`
The file [`bprimeKit_util.h`](./bprimeKit_util.h) defines the utility functions unique to the bprimeKit as 
`extern` functions for all the method implementation file to be able to see.
For implementations, read the [`README.md`](../plugins/README.md) in the plugins directory.

The function currently defined for the bprimeKit is:

   * For particle generation information:
   Read the code in the generation information sector of the files [`bprimeKit_lepton.cc`](plugins/bprimeKit_leptons.cc) for example.
      - `int genMCTag( GenParticle* )`: Generates the MC tags.
      - `genMCTag( GenInterator& ,  Particle& )`: Generates the MC tags.


### `TriggerBooking.h`
The [`TriggerBooking.h`](./TriggerBooking.h) defines the high level triggers used in the events of interest. 
They are store as string list and `enum` pairs for simple coding interface in the plugins files. 
Maintenance of the high level trigger paths are achieve through the [`./check_HLTpath.csh`](./check_HLTpath.csh) script.  

