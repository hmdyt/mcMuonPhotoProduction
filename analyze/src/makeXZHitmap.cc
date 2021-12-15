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

void makeXZHitmap(TString file_path = "/data/hamada/geant4_data/mcMuonPhotoProduction/muonBeam1500MeV_0.root"){
    TChain* chain = new TChain("tree");
    chain->Add(file_path);

    chain->SetBranchStatus("*", 0);
    chain->SetBranchStatus("particleName", 1);
    chain->SetBranchStatus("preCopyNo", 1);
    chain->SetBranchStatus("postCopyNo", 1);

    std::vector<std::string>* particleName = 0;
    std::vector<int>* preCopyNo = 0;
    std::vector<int>* postCopyNo = 0;
    chain->SetBranchAddress("particleName", &particleName);
    chain->SetBranchAddress("preCopyNo", &preCopyNo);
    chain->SetBranchAddress("postCopyNo", &postCopyNo);
    
    chain->GetEntry(1);

    int pre_x, pre_z, post_x, post_z, preScintiNo, postScintiNo;

    TH2D* h = new TH2D("h", "Hit map; scintillator X Axis; scintillator Z Axis", 4, 0 - 0.5, 4 - 0.5, 30, 0 - 0.5, 30 - 0.5);

    for (int i = 0;i < preCopyNo->size(); i++){
       std::cout << preCopyNo->at(i) <<  "\t" << postCopyNo->at(i) << std::endl;
       preScintiNo = preCopyNo->at(i) - 100;
       postScintiNo = postCopyNo->at(i) - 100;
       if(preScintiNo > 0){
            pre_x = (preScintiNo % 8) / 2;
            pre_z = ((2 * (preScintiNo / 8)) + (preScintiNo % 2)) + 2 * ((preScintiNo / 8));
       }
        if(postScintiNo > 0){
            post_x = (postScintiNo % 8) / 2;
            post_z = (2 * (postScintiNo / 8)) + (postScintiNo % 2) + 2 * ((preScintiNo / 8));
       }
       else{
           continue;
       }
       h->Fill(pre_x, pre_z);
       h->Fill(post_x, post_z);
       
    } 
    h->Draw("colz");
    
}