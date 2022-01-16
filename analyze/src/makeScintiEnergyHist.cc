#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include <vector>

void makeScintiEnergyHist(
    TString file_path = "/home/tomoe/mcMuonPhotoProduction/bench/test_run_05.root",
    TString save_file_name = "test_run_05",
    int targetCopyNo = 100
    ){

    // open data file
    auto file = TFile::Open(file_path);
    TTree *tree = (TTree*)file->Get("tree");

    // define vector
    std::vector<double> *eDep = 0;
    std::vector<int> *preCopyNo = 0;
    std::vector<int> *postCopyNo = 0;

    // define TH1D
    double nBins = 1000;
    double binMin = 0;
    double binMax = 1000;
    TH1D *eDep_hist = new TH1D("eDep_hist", Form("Energy deposit (copyNo %d) [MeV]", targetCopyNo), 1000, 0, 10);

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
            bool is_particle_in_detector = preCopyNo->at(j) == targetCopyNo;
            if(is_particle_in_detector){
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
    canvas->SaveAs(Form("../img/%s_%d.png", save_file_name.Data(), targetCopyNo));
}