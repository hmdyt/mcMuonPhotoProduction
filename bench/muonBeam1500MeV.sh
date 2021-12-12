#!/bin/bash
# 1.5GeV Muon 10000 * 100 event
# cut neutral particle
# only save in sensitive detector
# execute this file at /mcMuonPhotoProduction/bench/.

for i in {0..99}
do
    ../bin/Application_Main muonBeam1500MeV.mac "/data/hamada/geant4_data/mcMuonPhotoProduction/muomBeam1500MeV_${i}.root"
done