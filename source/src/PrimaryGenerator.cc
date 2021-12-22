#include "PrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "CRYGenerator.h"
#include "G4Event.hh"
#include "G4ios.hh"

#include "CRYSetup.h"

PrimaryGenerator::PrimaryGenerator(){
  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

}

PrimaryGenerator::~PrimaryGenerator(){}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  G4String particleName;

  // CRY setup
  CRYSetup *setup = new CRYSetup("", "../build/_deps/cry-src/data");

  // Setup the CRY event generator
  gen = new CRYGenerator(setup);

  // Generate the events
  std::vector<CRYParticle*> *ev = new std::vector<CRYParticle*>;
  ev->clear();
  gen->genEvent(ev);

  //....debug output
  G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size()
         << G4endl;

  for ( unsigned j=0; j<vect->size(); j++) {
    particleName=CRYUtils::partName((*vect)[j]->id());

    //....debug output  
    G4cout << "  "          << particleName << " "
         << "charge="      << (*vect)[j]->charge() << " "
         << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
         << "pos (m)"
         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
         << " " << "direction cosines "
         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
         << " " << G4endl;

    particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
    particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*m, (*vect)[j]->y()*m, (*vect)[j]->z()*m));
    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
    particleGun->SetParticleTime((*vect)[j]->t());
    particleGun->GeneratePrimaryVertex(anEvent);

  }
  delete setup;
  delete particleGun;
}
