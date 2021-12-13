#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>

std::map<int, std::vector<double>> drawTrack(TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_*.root"){
    TChain* chain = new TChain("tree");
    TChain* chainSearch = new TChain("tree");
    chain->Add(file_path);
    chainSearch->Add(file_path);
    std::vector<int>  pionEventList = makePionEventList(chainSearch, true);
    delete chainSearch;
    chain->SetBranchStatus("*", 0);

    std::map<int, std::vector<std::string>> trackParticleName;
    std::map<int, std::vector<double>> trackPrePosX;
    std::map<int, std::vector<double>> trackPrePosY;
    std::map<int, std::vector<double>> trackPrePosZ;
    std::map<int, std::vector<double>> trackPostPosX;
    std::map<int, std::vector<double>> trackPostPosY;
    std::map<int, std::vector<double>> trackPostPosZ;
    std::map<int, std::vector<unsigned short>> trackPreCopyNo;
    std::map<int, std::vector<unsigned short>> trackPostCopyNo;

    std::vector<int>* trackID = 0;
    std::vector<std::string>* particleName = 0;
    std::vector<double>* prePosX = 0;
    std::vector<double>* prePosY = 0;
    std::vector<double>* prePosZ = 0;
    std::vector<double>* postPosX = 0;
    std::vector<double>* postPosY = 0;
    std::vector<double>* postPosZ = 0;
    std::vector<unsigned short>* preCopyNo = 0;
    std::vector<unsigned short>* postCopyNo = 0;

    chain->SetBranchAddress("trackID", &trackID);
    chain->SetBranchAddress("particleName", &particleName);
    chain->SetBranchAddress("prePosX", &prePosX);
    chain->SetBranchAddress("prePosY", &prePosY);
    chain->SetBranchAddress("prePosZ", &prePosZ);
    chain->SetBranchAddress("postPosX", &postPosX);
    chain->SetBranchAddress("postPosY", &postPosY);
    chain->SetBranchAddress("postPosZ", &postPosZ);
    chain->SetBranchAddress("preCopyNo", &preCopyNo);
    chain->SetBranchAddress("postCopyNo", &postCopyNo);

    // temporary
    chain->GetEntry(pionEventList.at(0));

    // init vector in map
    for (int i = 0; i < trackID->size(); i++){
        trackParticleName[trackID->at(i)] = {};
        trackPrePosX[trackID->at(i)] = {};
        trackPrePosY[trackID->at(i)] = {};
        trackPrePosZ[trackID->at(i)] = {};
        trackPostPosX[trackID->at(i)] = {};
        trackPostPosY[trackID->at(i)] = {};
        trackPostPosZ[trackID->at(i)] = {};
        trackPreCopyNo[trackID->at(i)] = {};
        trackPostCopyNo[trackID->at(i)] = {};
    }

    // fill into map
    for (int i = 0; i < trackID->size(); i++){
        trackParticleName.at(trackID->at(i)).push_back(particleName->at(i));
        trackPrePosX.at(trackID->at(i)).push_back(prePosX->at(i));
        trackPrePosY.at(trackID->at(i)).push_back(prePosY->at(i));
        trackPrePosZ.at(trackID->at(i)).push_back(prePosZ->at(i));
        trackPostPosX.at(trackID->at(i)).push_back(postPosX->at(i));
        trackPostPosY.at(trackID->at(i)).push_back(postPosY->at(i));
        trackPostPosZ.at(trackID->at(i)).push_back(postPosZ->at(i));
        trackPreCopyNo.at(trackID->at(i)).push_back(preCopyNo->at(i));
        trackPostCopyNo.at(trackID->at(i)).push_back(postCopyNo->at(i));
    }

    TGraph2D* trackGraph = new TGraph2D();

    int i_trackID;
    int i_graph = 0;
    for(auto itr = trackPrePosX.begin(); itr != trackPrePosX.end(); ++itr){
        i_trackID = itr->first;
        for (int i = 0; i < trackPrePosX.at(i_trackID).size(); i++){
            trackGraph->SetPoint(
                i_graph,
                trackPrePosX.at(i_trackID).at(i),
                trackPrePosY.at(i_trackID).at(i),
                trackPrePosZ.at(i_trackID).at(i)
            );
            i_graph++;
        }
    }

    trackGraph->SetMarkerStyle(20);
    getDetectorGraph2D()->Draw("p0");
    trackGraph->Draw("p same");
    return trackPrePosX;
}