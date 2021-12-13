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

TGraph2D* getDetectorGraph2D(){
    // length unit => cm
    const double alX = 30;
    const double alY = 100;
    const double alZ = 38;

    double boxEdge[8][3];

    int counter = 0;
    for (int i = -1; i < 2; i += 2) {
        for (int j = -1; j < 2; j += 2) {
            for (int k = -1; k < 2; k += 2) {
                boxEdge[counter][0] = i * (alX / 2);
                boxEdge[counter][1] = j * (alY / 2);
                boxEdge[counter][2] = k * (alZ / 2);
                counter++;
            }
        }
    }

    TGraph2D* detectorGraph = new TGraph2D();
    for (int i = 0; i < 8; i++){
        detectorGraph->SetPoint(
            i,
            boxEdge[i][0],
            boxEdge[i][1],
            boxEdge[i][2]
        );
    }
    
    return detectorGraph;
}