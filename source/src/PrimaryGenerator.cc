#include "PrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

#include "CRYSetup.h"

PrimaryGenerator::PrimaryGenerator(){}

PrimaryGenerator::~PrimaryGenerator(){}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
  // particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  // create primary vertex
  G4ThreeVector vertexPos = G4ThreeVector(0, 0, 50*cm);
  G4PrimaryVertex* vertex = new G4PrimaryVertex(vertexPos, 0.0*ns);

  // create muon
  G4double E_mu = 1.5 * GeV;
  G4PrimaryParticle* particleMuPlus = new G4PrimaryParticle(
    particleTable->FindParticle("mu+")
  );
  particleMuPlus->SetTotalEnergy(E_mu);
  particleMuPlus->SetMomentumDirection(G4ThreeVector(0., 0., -1.
  ));

  // set
  vertex->SetPrimary(particleMuPlus);
  anEvent->AddPrimaryVertex(vertex);
}
