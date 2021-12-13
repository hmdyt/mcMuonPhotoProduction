#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"

void makePionTrackHist(TString file_path = ){
    TChain* chain = new TChain("tree");
    chain->Add(file_path);
}