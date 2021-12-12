#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

#include "TTree.h"
#include "TFile.h"
#include <vector>
#include <string>
class G4Step;

class SensitiveDetector : public G4VSensitiveDetector
{
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);

        void setOutFileName(G4String outFileName_arg);
        G4String getOutFileName();

        void saveTTreeAsRootFile();

    private:
        G4String outFileName;
        TTree* tree;
        TFile* tfile;
        std::vector<G4int> trackID;
        std::vector<G4int> parentID;
        std::vector<G4double> globalTime;
        std::vector<std::string> particleName;
        std::vector<G4double> charge;
        std::vector<G4double> eDep;
        std::vector<G4double> prePosX;
        std::vector<G4double> prePosY;
        std::vector<G4double> prePosZ;
        std::vector<G4double> postPosX;
        std::vector<G4double> postPosY;
        std::vector<G4double> postPosZ;
        std::vector<u_short> preCopyNo;
        std::vector<u_short> postCopyNo;
        G4int i_event;
};

#endif 