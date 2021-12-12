#include "Geometry.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
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
   G4VSolid* solid_Albox_filled = new G4Box(
      "Solid_AlBox_filld",
      30 * cm / 2,
      100 * cm / 2,
      38 * cm / 2
   );
   G4VSolid* solid_Albox_hole = new G4Box(
      "Solid_AlBox_hole",
      (30 - 4) * cm / 2,
      100 * cm / 2,
      2 * cm / 2
   );
   G4VSolid* solid_Albox = solid_Albox_filled;
   for (G4int i = -3; i <= 3; i++){
      solid_Albox = new G4SubtractionSolid(
         "Solid_AlBox",
         solid_Albox,
         solid_Albox_hole,
         new G4RotationMatrix(),
         G4ThreeVector(0, 0, (3 + 1*2) * i * cm)
      );
   }
   
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

G4LogicalVolume* Geometry::constructScinti()
{
   G4VSolid* solid_Scinti = new G4Box(
      "Solid_Scinti",
      4 * cm / 2,
      75 * cm / 2,
      1 * cm / 2
   );
   G4Material* materi_Scinti = materi_Man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   G4LogicalVolume* logVol_Scinti = new G4LogicalVolume(
      solid_Scinti,
      materi_Scinti,
      "LogVol_Scinti"
   );
   G4VisAttributes* attr_Scinti = new G4VisAttributes(true);
   attr_Scinti->SetColor(G4Color::Red());
   logVol_Scinti->SetVisAttributes(attr_Scinti);

   return logVol_Scinti;
}

G4VPhysicalVolume* Geometry::Construct()
{
   G4LogicalVolume* logVol_World = constructLogWorld();
   G4LogicalVolume* logVol_AlBox = constructLogAlminumBox();
   G4LogicalVolume* logVol_Scinti = constructScinti();

   // placement world
   G4int copyNum_World = 0;
   G4PVPlacement* physVol_World  = new G4PVPlacement(
      G4Transform3D(),
      "PhysVol_World",
      logVol_World,
      0,
      false,
      copyNum_World,
      true
      );

   // placement AlBox into world
   G4int copyNum_AlBox = 1;
   G4PVPlacement* physVol_AlBox = new G4PVPlacement(
      G4Transform3D(),
      "PhysVol_AlBox",
      logVol_AlBox,
      physVol_World,
      false,
      copyNum_AlBox,
      true
   );

   // placement scinti into AlBox
   G4int copyNum_Scinti = 100;
   for (G4int i = -3; i <= 4; i++){
      for (G4int j = 0; j < 4; j++){
         for (G4int k = 0; k < 1; k++){
            // k=0: downside, k=1: upside
            new G4PVPlacement(
               G4Transform3D(
                  G4RotationMatrix(),
                  G4ThreeVector((-6 + 4*j) * cm, 0, (5*i - 0.5 + k) * cm)
                  ),
               G4String("PhysVol_Scinti") + G4String(std::to_string(copyNum_Scinti)),
               logVol_Scinti,
               physVol_AlBox,
               false,
               copyNum_Scinti
            );
            copyNum_Scinti++;
         }

      }
   }

   return physVol_World;
}
