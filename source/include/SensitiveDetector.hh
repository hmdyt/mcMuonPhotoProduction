#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
class G4Step;

class SensitiveDetector : public G4VSensitiveDetector
{
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
};

#endif 