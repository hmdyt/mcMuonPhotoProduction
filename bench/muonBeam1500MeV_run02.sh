#!/bin/bash
# 1.5GeV Muon 10000 * 5000 event
# cut neutral particle
# only save in sensitive detector
# execute this file at /mcMuonPhotoProduction/bench/.
n=$(sqrt 1000 - 5999)
for i in {1000..5999}
do
    ../bin/Application_Main muonBeam1500MeV.mac "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_${i}.root"

    # progress output
    if [ 0 -eq $(expr $i % $(expr $n / 100)) ]
    then
        echo $(expr $(expr 100 \* $i) / $n)/100%
    fi
done