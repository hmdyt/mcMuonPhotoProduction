#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TFile.h"
#include "TH3D.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>

void drawTrack(
    int i_event = 0,
    TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_*.root",
    TString img_folder = "muonBeam1500MeV_*.root" // MUST PREPARE MKDIR
)
{
    TString saveFilePath = Form("../img/%s/%d.png", img_folder.Data(), i_event);
    TString saveROOTFilePath = Form("../img/%s/%d.root", img_folder.Data(), i_event);

    TChain* chain = new TChain("tree");
    chain->Add(file_path);
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
    chain->GetEntry(i_event);

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

    std::vector<TGraph2D*> trackGraphs = {};
    TLegend* legend = new TLegend(0.75, 0.75, 1, 1);

    int i_trackID;
    int i_graph = 0;
    int graph_counter;
    for(auto itr = trackPrePosX.begin(); itr != trackPrePosX.end(); ++itr){
        i_trackID = itr->first;
        graph_counter = 0;
        trackGraphs.push_back(new TGraph2D());
        for (int i = 0; i < trackPrePosX.at(i_trackID).size(); i++){
            trackGraphs.at(i_graph)->SetPoint(
                graph_counter,
                trackPrePosX.at(i_trackID).at(i),
                trackPrePosY.at(i_trackID).at(i),
                trackPrePosZ.at(i_trackID).at(i)
            );
            graph_counter++;
        }
        // graph settins
        if (trackParticleName.at(i_trackID).at(0) == "mu+") trackGraphs.at(i_graph)->SetMarkerColor(4); // blue
        if (trackParticleName.at(i_trackID).at(0) == "mu-") trackGraphs.at(i_graph)->SetMarkerColor(4); // blue
        if (trackParticleName.at(i_trackID).at(0) == "pi-") trackGraphs.at(i_graph)->SetMarkerColor(2); // red
        if (trackParticleName.at(i_trackID).at(0) == "pi+") trackGraphs.at(i_graph)->SetMarkerColor(2); // red
        if (trackParticleName.at(i_trackID).at(0) == "e-")  trackGraphs.at(i_graph)->SetMarkerColor(6); // pink
        if (trackParticleName.at(i_trackID).at(0) == "proton") trackGraphs.at(i_graph)->SetMarkerColor(8); // green
        if (
            trackParticleName.at(i_trackID).at(0) != "e-" &&
            trackParticleName.at(i_trackID).at(0) != "proton"
        ){
            legend->AddEntry(trackGraphs.at(i_graph), trackParticleName.at(i_trackID).at(0).c_str(), "p");
        }
        i_graph++;
    }

    TCanvas* canvas = new TCanvas();
    TGraph2D* detectorGraph = getDetectorGraph2D();
    detectorGraph->SetTitle(Form("track %d;x [cm];y [cm];z [cm]", i_event));
    detectorGraph->Draw("p0");
    for (int i = 0; i < trackGraphs.size(); i++){
        trackGraphs.at(i)->SetMarkerStyle(20);
        trackGraphs.at(i)->Draw("p line same");
    }
    legend->AddEntry((TObject*)0, "Pink: e-");
    legend->AddEntry((TObject*)0, "Green: proton");
    legend->Draw();
    canvas->SaveAs(saveFilePath);
    delete canvas;

    // Tfile save
    TCanvas* canvasROOTFile =  new TCanvas("canvas", "canvas");
    canvasROOTFile->cd();
    TH3D* axis = new TH3D(
        "axisTH3D",
        Form("track %d;x [cm];y [cm];z [cm]", i_event),
        0, -50, 50,
        0, -50, 50,
        0, -50, 50
    );
    axis->SetStats(0);
    axis->Draw("AXIS");
    detectorGraph->Draw("p0 same");
    for (int i = 0; i < trackGraphs.size(); i++){
        trackGraphs.at(i)->SetMarkerStyle(20);
        trackGraphs.at(i)->Draw("p line same");
    }
    legend->Draw();
    
    TFile* outFile = new TFile(saveROOTFilePath, "recreate");
    outFile->Add(canvas);
    outFile->Write();

    // delete
    for (int i = 0; i < trackGraphs.size(); i++) delete trackGraphs[i];
    delete chain;
    delete detectorGraph;
    delete canvasROOTFile;
    delete legend;
    delete outFile;
    delete axis;
}