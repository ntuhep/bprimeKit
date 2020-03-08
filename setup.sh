#!/bin/bash

echo
echo "Welcome to the bprimekit setup script!"
echo

echo "set up fixing codes for the electron energy scale and smearing interacts with the ECAL-tracker combination"
git clone git@github.com:cms-egamma/EgammaPostRecoTools.git  EgammaUser/EgammaPostRecoTools
echo

echo "Set up JetToolBox"
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_102X_v3
echo

echo "Done with setup script! You still need to build!"
echo
