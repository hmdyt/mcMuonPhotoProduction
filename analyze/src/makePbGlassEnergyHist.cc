#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include <vector>

void makePbGlassEnergyHist(TString file_path = "/home/tomoe/mcMuonPhotoProduction/bench/test_run_05.root"){

    // open data file
    auto file = TFile::Open(file_path);
    TTree *tree = (TTree*)file->Get("tree");

    // define vector
    vector<double> *eDep = 0;
    vector<int> *preCopyNo = 0;
    vector<int> *postCopyNo = 0;

    // define TH1D
    double nBins = 1000;
    double binMin = 0;
    double binMax = 1000;
    TH1D *eDep_hist = new TH1D("eDep_hist", "Energy deposit [MeV]", 1000, 0, 1000);

    // set branch status
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("eDep", 1);
    tree->SetBranchStatus("preCopyNo", 1);
    tree->SetBranchStatus("postCopyNo", 1);

    // set branch address
    tree->SetBranchAddress("eDep", &eDep);
    tree->SetBranchAddress("preCopyNo", &preCopyNo);
    tree->SetBranchAddress("postCopyNo", &postCopyNo);

    int N = tree->GetEntries();

    for (int i = 0; i < N; i++){
        //std::cout << i << std::endl;
        tree->GetEntry(i);
        double total_eDep = 0;
        for(int j = 0; j < eDep->size(); j++){
            if((200 <= preCopyNo->at(j)) && (preCopyNo->at(j) <= 203) && (200 <= postCopyNo->at(j)) && (postCopyNo->at(j) <= 203)){
                total_eDep += eDep->at(j);
                //std::cout << total_eDep << std::endl;
            }
        }
        if(0 < total_eDep){
            eDep_hist->Fill(total_eDep);
        }
    }
    TCanvas *canvas = new TCanvas();

    // draw
    eDep_hist->Draw();
    canvas->Draw();
}