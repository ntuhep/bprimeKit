#!/bin/bash

echo
echo "Welcome to the bprimekit setup script!"
echo

#echo "set up fixing codes for the electron energy scale and smearing interacts with the ECAL-tracker combination"
#git cms-merge-topic cms-egamma:EgammaPostRecoTools
#echo

echo "Set up JetToolBox"
git clone https://github.com/cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_120X
echo

echo "Done with setup script! You still need to build!"
echo
