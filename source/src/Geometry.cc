#include "Geometry.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"


Geometry::Geometry(){
   materi_Man = G4NistManager::Instance();
}

Geometry::~Geometry() {}

G4LogicalVolume* Geometry::constructLogWorld()
{
   G4double leng_X_World = 5.0 * m;
   G4double leng_Y_World = 5.0 * m;
   G4double leng_Z_World = 5.0 * m;
   G4Box* solid_World = new G4Box(
      "Solid_World",
      leng_X_World / 2.0,
      leng_Y_World / 2.0,
      leng_Z_World / 2.0
      );
   G4Material* materi_World = materi_Man->FindOrBuildMaterial("G4_AIR");
   G4LogicalVolume* logVol_World = new G4LogicalVolume(
      solid_World,
      materi_World,
      "LogVol_World");
   logVol_World->SetVisAttributes(G4VisAttributes::Invisible);

   return logVol_World;
}

G4LogicalVolume* Geometry::constructLogAlminumBox()
{
   G4double leng_X_AlBox = 30 * cm;
   G4double leng_Y_AlBox = 100 * cm;
   G4double leng_Z_AlBox = 30 * cm;
   G4Box* solid_Albox = new G4Box(
      "Solid_AlBox",
      leng_X_AlBox / 2,
      leng_Y_AlBox / 2,
      leng_Z_AlBox / 2
   );
   G4Material* materi_AlBox = materi_Man->FindOrBuildMaterial("G4_Al");
   G4LogicalVolume* logVol_AlBox = new G4LogicalVolume(
      solid_Albox,
      materi_AlBox,
      "LogVol_AlBox"
   );
   
   G4VisAttributes* attr_Albox = new G4VisAttributes(true);
   attr_Albox->SetColour(G4Color::Cyan());
   logVol_AlBox->SetVisAttributes(attr_Albox);
   
   return logVol_AlBox;
}

G4VPhysicalVolume* Geometry::Construct()
{
   G4LogicalVolume* logVol_World = constructLogWorld();
   G4LogicalVolume* logVol_AlBox = constructLogAlminumBox();

   // placement world
   G4int copyNum_World = 0;
   G4PVPlacement* physVol_World  = new G4PVPlacement(
      G4Transform3D(),
      "PhysVol_World",
      logVol_World,
      0,
      false,
      copyNum_World
      );

   // placement AlBox into world
   G4int copyNum_AlBox = 1;
   new G4PVPlacement(
      G4Transform3D(),
      "PhysVol_AlBox",
      logVol_AlBox,
      physVol_World,
      false,
      copyNum_AlBox,
      true
   );

   return physVol_World;
}
