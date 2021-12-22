#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "CRYGenerator.h"
#include "G4ParticleTable.hh"
#include <vector>

class G4Event;
class G4ParticleGun;

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGenerator();
    ~PrimaryGenerator();

  public:
    void GeneratePrimaries(G4Event*);
  
  private:
    G4ParticleTable* particleTable;
    std::vector<CRYParticle*> *vect;
    G4ParticleGun* particleGun;
    CRYGenerator* gen;
};
#endif
