#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include <vector>
#include <string>
#include <fstream>

std::vector<int> makePionEventList(TChain* chain, bool use_chache = true){
    std::cout << "pion event search start ..." << std::endl;
    if (chain == nullptr) {return {-1};}

    // chache return
    ifstream ifs("makePionEventList.chache");
    int buff;
    std::vector<int> retChache = {};
    if (use_chache && ifs){
        while (ifs >> buff){
            retChache.push_back(buff);
        }
        std::cout << "used chache" << std::endl;
        return retChache;
    }
    
    // setbranch address
    chain->SetBranchStatus("*", 0);
    chain->SetBranchStatus("particleName", 1);
    std::vector<std::string>* particleName = 0;
    chain->SetBranchAddress("particleName", &particleName);
    int nEntries = chain->GetEntries();

    // search pion event
    std::vector<int>  pionEventList = {};
    bool isExistPion;
    for (int i = 0; i < nEntries; i++){
        chain->GetEntry(i);
        if (particleName->size() == 0) continue;

        isExistPion = false;
        for (int i = 0; i < particleName->size(); i++) {
            if (particleName->at(i) == "pi+" || particleName->at(i) == "pi-" )
                { isExistPion = true; }
        }

        if (isExistPion) {
            pionEventList.push_back(i);
        }

        if (i % (nEntries/100) == 0) {
            std::cout << (100*i/nEntries) << "\% Done"<< std::endl;
        }
    }

    // make chache
    std::ofstream ofs("makePionEventList.chache", std::ios::trunc);
    for (int i = 0; i < pionEventList.size(); i++){
        ofs << pionEventList.at(i) << " ";
    }
    std::cout << "chache saved" << std::endl;

    return pionEventList;
}

double calcDist3D(
    double x1, double y1, double z1,
    double x2, double y2, double z2
){
    return TMath::Sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
}

void makePionTrackHist(TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_*.root"){
    TChain* chain = new TChain("tree");
    TChain* chainSearch = new TChain("tree");
    chain->Add(file_path);
    chainSearch->Add(file_path);
    std::vector<int>  pionEventList = makePionEventList(chainSearch, true);
    chain->SetBranchStatus("*", 1);
    
    vector<int>* trackID = 0;
    vector<int>* parentID = 0;
    vector<double>* globalTime = 0;
    vector<std::string>* particleName = 0;
    vector<double>* charge = 0;
    vector<double>* eDep = 0;
    vector<double>* prePosX = 0;
    vector<double>* prePosY = 0;
    vector<double>* prePosZ = 0;
    vector<double>* postPosX = 0;
    vector<double>* postPosY = 0;
    vector<double>* postPosZ = 0;
    vector<unsigned short>* preCopyNo = 0;
    vector<unsigned short>* postCopyNo = 0;

    chain->SetBranchAddress("trackID", &trackID);
    chain->SetBranchAddress("parentID", &parentID);
    chain->SetBranchAddress("globalTime", &globalTime);
    chain->SetBranchAddress("particleName", &particleName);
    chain->SetBranchAddress("charge", &charge);
    chain->SetBranchAddress("eDep", &eDep);
    chain->SetBranchAddress("prePosX", &prePosX);
    chain->SetBranchAddress("prePosY", &prePosY);
    chain->SetBranchAddress("prePosZ", &prePosZ);
    chain->SetBranchAddress("postPosX", &postPosX);
    chain->SetBranchAddress("postPosY", &postPosY);
    chain->SetBranchAddress("postPosZ", &postPosZ);
    chain->SetBranchAddress("preCopyNo", &preCopyNo);
    chain->SetBranchAddress("postCopyNo", &postCopyNo);

    double nBins = 50;
    double binMin = 0;
    double binMax = 150;
    TH1D* histPiPlus = new TH1D("histPiPlus", "pi+;Flight Distance [cm];Events", nBins, binMin, binMax);
    TH1D* histPiMinus = new TH1D("histPiMinus", "pi-;Flight Distance [cm];Events", nBins, binMin, binMax);
    TH1D* histPions = new TH1D("histPions", "pi +/-;Flight Distance [cm];Events", nBins, binMin, binMax);
    TH1D* histCompare = new TH1D("histCompare", "compare pions;Flight Distance [cm];Events", nBins, binMin, binMax);
    double flightDistPiPlus;
    double flightDistPiMinus;
    double distPreToPost;

    for (int i = 0; i < pionEventList.size(); i++){
        chain->GetEntry(pionEventList.at(i));
        distPreToPost = 0;
        flightDistPiPlus = 0;
        flightDistPiMinus = 0;
        for (int j = 0; j < trackID->size(); j++){
            if (!(particleName->at(j) == "pi+" || particleName->at(j) == "pi-")) continue;
            distPreToPost += calcDist3D(
                prePosX->at(j), prePosY->at(j), prePosZ->at(j),
                postPosX->at(j), postPosY->at(j), postPosZ->at(j)
                );
            if (particleName->at(j) == "pi+") flightDistPiPlus += distPreToPost;
            if (particleName->at(j) == "pi-") flightDistPiMinus += distPreToPost;
        }
        if (flightDistPiPlus != 0) {
            histPions->Fill(flightDistPiPlus);
            histPiPlus->Fill(flightDistPiPlus);
        }
        if (flightDistPiMinus != 0) {
            histPions->Fill(flightDistPiMinus);
            histPiMinus->Fill(flightDistPiMinus);
        }
    }

    histPiPlus->SetLineColor(4);  // blue
    histPiMinus->SetLineColor(2); // red
    histPions->SetLineColor(1);   // black

    TCanvas* canvas = new TCanvas("canvas name", "canvas title", 1200, 1200);
    canvas->Divide(2, 2);

    canvas->cd(1);
    histPiMinus->Draw();

    canvas->cd(2);
    histPiPlus->Draw();

    canvas->cd(3);
    histPions->Draw();

    canvas->cd(4);
    histCompare->GetYaxis()->SetRangeUser(0, 20);
    histCompare->Draw();
    histPiPlus->Draw("same");
    histPiMinus->Draw("same");

    canvas->Draw();
}