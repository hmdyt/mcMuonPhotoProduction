#ifndef Geometry_h
#define Geometry_h 1

#include "G4VUserDetectorConstruction.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4NistManager;


class Geometry : public G4VUserDetectorConstruction
{
  public:
    Geometry();
    ~Geometry();

    G4VPhysicalVolume* Construct();

  private:
    G4NistManager* materi_Man;

    G4LogicalVolume* constructLogWorld();
    G4LogicalVolume* constructLogAlminumBox();
    G4LogicalVolume* constructScinti();
    G4LogicalVolume* constructTriggerScinti();

};
#endif
