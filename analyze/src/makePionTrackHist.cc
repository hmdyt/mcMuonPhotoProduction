#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include <vector>
#include <string>
#include <fstream>

std::vector<int> makePionEventList(TChain* chain){
    std::cout << "pion event search start ..." << std::endl;
    if (chain == nullptr) {return {-1};}

    chain->SetBranchStatus("*", 0);
    chain->SetBranchStatus("particleName", 1);
    std::vector<std::string>* particleName = 0;
    chain->SetBranchAddress("particleName", &particleName);
    int nEntries = chain->GetEntries();

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


    return pionEventList;
}

void makePionTrackHist(TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_*.root"){
    TChain* chain = new TChain("tree");
    chain->Add(file_path);

    std::vector<int>  pionEventList = makePionEventList(chain);

    cout << pionEventList.size() << endl;
}