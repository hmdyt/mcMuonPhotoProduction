#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TFile.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>

void makeManyTracks(
    TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_*.root",
    TString img_folder = "muonBeam1500MeV_*.root"
){
    TChain* chain_search = new TChain("tree");
    chain_search->Add(file_path);
    std::vector<int> pionEventList = makePionEventList(chain_search, false);
    delete chain_search;

    for (int i = 0; i < pionEventList.size(); i++){
        drawTrack(
            pionEventList.at(i),
            file_path,
            img_folder
        );
        if (i % (pionEventList.size()/100) == 0) {
            std::cout << (100*i/pionEventList.size()) << "\% Done"<< std::endl;
        }
    }
}