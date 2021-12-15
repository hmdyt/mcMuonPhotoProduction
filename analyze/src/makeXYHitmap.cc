#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void makeXYHitmap(
int i_event = 0,
TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_1.root",
TString img_folder = "muonBeam1500MeV_1.root"
)
{
    TString saveFilePath = Form("../img/%s/%d.png", img_folder.Data(), i_event);

    TChain* chain = new TChain("tree");
    chain->Add(file_path);

    chain->SetBranchStatus("*", 0);
    //chain->SetBranchStatus("particleName", 1);
    chain->SetBranchStatus("preCopyNo", 1);
    chain->SetBranchStatus("postCopyNo", 1);

    std::vector<int>* preCopyNo = 0;
    std::vector<int>* postCopyNo = 0;
    chain->SetBranchAddress("preCopyNo", &preCopyNo);
    chain->SetBranchAddress("postCopyNo", &postCopyNo);

    chain->GetEntry(i_event);
    
    int pre_x, pre_y, post_x, post_y, preScintiNo, postScintiNo;

    TH2D* hist = new TH2D("hist", "Hit_map(X_Y); scintillator X Axis; scintillator Y Axis", 4, 0, 4, 1 , 0, 4);

    for (int i = 0; i < preCopyNo->size(); i++){
        preScintiNo = preCopyNo->at(i) - 100;
        postScintiNo = postCopyNo->at(i) - 100;
        if (preScintiNo > 0){
            pre_x = (preScintiNo % 8) / 2;
            pre_y = 1;            
        }
        if (postScintiNo > 0){
            post_x = (postScintiNo % 8) / 2;
            post_y = 1;
        }
        else continue;
        hist->Fill(pre_x, pre_y);
        hist->Fill(post_x, post_y);
        
    }
    TCanvas* canvas = new TCanvas();
    hist->Draw("colz");
    canvas->SaveAs(saveFilePath);
    
    delete hist;
    delete chain;
    delete canvas;

    }