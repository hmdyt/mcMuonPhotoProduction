#include "SensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"

SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector(){}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    G4cout << "start" << G4endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4cout << "aStep" << G4endl;
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    G4cout << "end" << G4endl;
}