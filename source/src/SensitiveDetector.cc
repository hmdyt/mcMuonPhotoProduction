#include "SensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "TTree.h"
#include "TFile.h"
#include <vector>

SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{   
    i_event = 0;
    outFileName = "tmp.root";
    tree = new TTree("tree", "mcMuonPhotoProduction Output");
    tree->Branch("trackID", &trackID);
    tree->Branch("globalTime", &globalTime);
    tree->Branch("particleName", &particleName);
    tree->Branch("prePosX", &prePosX);
    tree->Branch("prePosY", &prePosY);
    tree->Branch("prePosZ", &prePosZ);
    tree->Branch("postPosX", &postPosX);
    tree->Branch("postPosY", &postPosY);
    tree->Branch("postPosZ", &postPosZ);
}

SensitiveDetector::~SensitiveDetector(){}

G4String SensitiveDetector::getOutFileName(){ return outFileName; }
void SensitiveDetector::setOutFileName(G4String outFileName_arg)
{
    outFileName = outFileName_arg;
}

void SensitiveDetector::saveTTreeAsRootFile()
{
    tfile = new TFile(outFileName, "recreate");
    tree->Write();
    tfile->Close();
    delete tree;
}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    trackID = {};
    globalTime = {};
    particleName = {};
    prePosX = {};
    prePosY = {};
    prePosZ = {};
    postPosX = {};
    postPosY = {};
    postPosZ = {};
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4Track* aTrack = aStep->GetTrack();
    G4ThreeVector preStepPos = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector poststepPos = aStep->GetPostStepPoint()->GetPosition();

    trackID.push_back(aTrack->GetTrackID());
    globalTime.push_back(aTrack->GetGlobalTime());
    particleName.push_back(aTrack->GetDynamicParticle()->GetParticleDefinition()->GetParticleName());
    prePosX.push_back(preStepPos.x()/cm);
    prePosY.push_back(preStepPos.y()/cm);
    prePosZ.push_back(preStepPos.z()/cm);
    postPosX.push_back(poststepPos.x()/cm);
    postPosY.push_back(poststepPos.y()/cm);
    postPosZ.push_back(poststepPos.z()/cm);

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    tree->Fill();
    if (i_event % 1000 == 0){ G4cout << i_event << G4endl; }
    i_event++;
}