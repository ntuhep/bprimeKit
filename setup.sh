#!/bin/bash

echo
echo "Welcome to the bprimekit setup script!"
echo
echo "Set up JetToolBox"
git clone https://github.com/cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_91X

echo "Set up Muon user code"
git clone https://github.com/ETHZ/sixie-Muon-MuonAnalysisTools.git UserCode/sixie/Muon/MuonAnalysisTools

echo "Fix Electron HEEP temporarily by downloading additional package of RecoEgamma/EgammaIsolationAlgos"
# HEEP ID fix
git cms-addpkg RecoEgamma/EgammaIsolationAlgos
git cms-merge-topic rgoldouz:TrkIsoFix -u

echo "Fix Photon ID temporarily by downloading additional package of RecoEgamma/PhotonIdentification"
# Photon ID fix
git cms-addpkg RecoEgamma/PhotonIdentification
cp /afs/cern.ch/user/i/ikrav/public/EGMCode/20171105/PhotonIDValueMapProducer.cc ./RecoEgamma/PhotonIdentification/plugins/.

echo
echo "Done with setup script! You still need to build!"
