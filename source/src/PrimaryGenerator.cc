#include "PrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "CRYGenerator.h"
#include "G4Event.hh"
#include "G4ios.hh"

#include <fstream>

#include "CRYSetup.h"

#define IS_DEBUG 0

void genSpecificParticle(CRYGenerator* gen, std::vector<CRYParticle*> *particles){
  G4double triggerScintiLengthX = 0.21 * m;
  G4double triggerScintiLengthY = 1.26 * m;
  while (true){
    particles->clear();
    gen->genEvent(particles);
    G4bool is_inner = false;
    for (int i = 0; i < particles->size(); i++){
      if (
        abs(particles->at(i)->x() * m) < (triggerScintiLengthX / 2) &&
        abs(particles->at(i)->y() * m) < (triggerScintiLengthY / 2)
      ){
        is_inner = true;
      }
    }
    if (is_inner){ break; }
  }
}

PrimaryGenerator::PrimaryGenerator(){
  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // read the setup file
  G4String filename = "../source/src/setupFile.txt";
  std::ifstream inputFile(filename);
  //inputFile.open(filename, std::ios::in);
  if(!inputFile){
    G4cout << "Failed to open file." << G4endl;
  }
  
  G4String buffer;
  //G4int i = 0;  

  while (inputFile >> buffer){    
    setupString += buffer;
    setupString += " ";
  }
  // CRY setup
  CRYSetup *setup = new CRYSetup(setupString, "../build/_deps/cry-src/data");

  // Setup the CRY event generator
  gen = new CRYGenerator(setup);


}

PrimaryGenerator::~PrimaryGenerator(){}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  G4String particleName;

  // Generate the events
  std::vector<CRYParticle*> *particles = new std::vector<CRYParticle*>;
  particles->clear();
  genSpecificParticle(gen, particles);

  //....debug output
  if (IS_DEBUG){
    G4cout << "\nEvent=" << anEvent->GetEventID() << " "
    << "CRY generated nparticles=" << particles->size()
    << G4endl;
  }
  for ( unsigned j=0; j<particles->size(); j++) {
    particleName=CRYUtils::partName(particles->at(j)->id());

    //....debug output  
    if (IS_DEBUG){
      G4cout << "  "          << particleName << " "
      << "charge="      << particles->at(j)->charge() << " "
      << "energy (MeV)=" << particles->at(j)->ke()*MeV << " "
      << "pos (m)"
      << G4ThreeVector(particles->at(j)->x(), particles->at(j)->y(), particles->at(j)->z())
      << " " << "direction cosines "
      << G4ThreeVector(particles->at(j)->u(), particles->at(j)->v(), particles->at(j)->w())
      << " " << G4endl;
    }
    

    particleGun->SetParticleDefinition(particleTable->FindParticle(particles->at(j)->PDGid()));
    particleGun->SetParticleEnergy(particles->at(j)->ke()*MeV);
    particleGun->SetParticlePosition(G4ThreeVector(particles->at(j)->x()*m, particles->at(j)->y()*m, ( particles->at(j)->z() + 0.19 )*m));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(particles->at(j)->u(), particles->at(j)->v(), particles->at(j)->w()));
    particleGun->SetParticleTime(particles->at(j)->t());
    particleGun->GeneratePrimaryVertex(anEvent);
    delete particles->at(j);
  }
  delete particles;
  delete particleGun;
}
