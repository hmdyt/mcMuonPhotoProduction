#!/bin/bash
# 1.5GeV Muon 10000 * 5000 event
# cut neutral particle
# only save in sensitive detector
# execute this file at /mcMuonPhotoProduction/bench/.

for i in {1000..5999}
do
    ../bin/Application_Main muonBeam1500MeV.mac "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_${i}.root"
done