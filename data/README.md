# The Process Setting Files

For various different version of `CMSSW` or different production tags,
the input tags for obtaining the desired physical objects maybe changed.

The `Config File` method is a less painful way of maintaining the same recipe
in the bprimeKit while switching the input tags for python configuration files. 
Note that this is by no means a completely backwards compatible settings file,
just so much so that any changes that doesn't involve changing the C++ code 
of the `bprimeKit` main code should be relatively easy.

The parsing of the configuration files could be found in the 
[`python/OptionParser.py`](../python/OptionParser.py) file
