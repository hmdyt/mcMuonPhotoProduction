#include "Geometry.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
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
   G4double leng_X_World = 10.0 * m;
   G4double leng_Y_World = 10.0 * m;
   G4double leng_Z_World = 10.0 * m;
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
   logVol_World->SetVisAttributes(G4VisAttributes::GetInvisible());

   return logVol_World;
}

G4LogicalVolume* Geometry::constructLogAlminumBox()
{  
   G4VSolid* solid_Albox_filled = new G4Box(
      "Solid_AlBox_filld",
      30 * cm / 2,
      100 * cm / 2,
      30 * cm / 2
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
         G4ThreeVector( 0, 0, 4 * i * cm)
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

G4LogicalVolume* Geometry::constructTriggerScinti()
{
   G4VSolid* solid_TriggerScinti = new G4Box(
      "Solid_TriggerScinti",
      7 * cm / 2,
      126 * cm / 2,
      1 * cm / 2
   );
   G4Material* materi_TriggerScinti = materi_Man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   G4LogicalVolume* logVol_TriggerScinti = new G4LogicalVolume(
      solid_TriggerScinti,
      materi_TriggerScinti,
      "LogVol_TriggerScinti"
   );
   G4VisAttributes* attr_TriggerScinti = new G4VisAttributes(true);
   attr_TriggerScinti->SetColor(G4Color::Blue());
   logVol_TriggerScinti->SetVisAttributes(attr_TriggerScinti);

   return logVol_TriggerScinti;
}

G4LogicalVolume* Geometry::constructPbGlassScinti(){
   G4VSolid* solid_PbGlassScinti = new G4Box(
      "solid_PbGlassScinti",
      31.5 / 2 * cm,
      11.5 / 2 * cm,
      11.5 / 2 * cm
   );
   G4Material* materi_PbGlassScinti = materi_Man->FindOrBuildMaterial("G4_GLASS_LEAD");
   G4LogicalVolume* logVol_PbGlassScinti = new G4LogicalVolume(
      solid_PbGlassScinti,
      materi_PbGlassScinti,
      "LogVol_PbGlassScinti"
   );
   G4VisAttributes* attr_PbGlassScinti = new G4VisAttributes(true);
   attr_PbGlassScinti->SetColor(G4Color::Magenta());
   logVol_PbGlassScinti->SetVisAttributes(attr_PbGlassScinti);

   return logVol_PbGlassScinti;
}

G4VPhysicalVolume* Geometry::Construct()
{
   G4LogicalVolume* logVol_World = constructLogWorld();
   G4LogicalVolume* logVol_AlBox = constructLogAlminumBox();
   G4LogicalVolume* logVol_Scinti = constructScinti();
   G4LogicalVolume* logVol_TriggerScinti = constructTriggerScinti();
   G4LogicalVolume* logVol_PbGlassScinti = constructPbGlassScinti();

   // sensitive detector set
   SensitiveDetector* sensitiveDetector = new SensitiveDetector("SensitiveDetector");
   logVol_Scinti->SetSensitiveDetector(sensitiveDetector);
   logVol_TriggerScinti->SetSensitiveDetector(sensitiveDetector);
   logVol_PbGlassScinti->SetSensitiveDetector(sensitiveDetector);
   G4SDManager* SDManager = G4SDManager::GetSDMpointer();
   SDManager->AddNewDetector(sensitiveDetector);

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
   new G4PVPlacement(
      G4Transform3D(),
      "PhysVol_AlBox",
      logVol_AlBox,
      physVol_World,
      false,
      copyNum_AlBox,
      true
   );

   //placement trigger scinti on Albox
   G4double trigerScintiArriance = 2 * cm;
   G4int copyNum_TriggerScinti = 10;
   for (int i = 0; i < 3; i++){
      new G4PVPlacement(
         G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(7 * (i - 1) * cm, 0, 18 * cm + trigerScintiArriance)
         ),
         G4String("PhysVol_TriggerScinti") + G4String(std::to_string(copyNum_TriggerScinti)),
         logVol_TriggerScinti,
         physVol_World,
         false,
         copyNum_TriggerScinti,
         true
      );
      copyNum_TriggerScinti++;
   }

   // placement scinti into AlBox
   G4int copyNum_Scinti = 100;
   G4double deg_MeshScinti = 7 * deg;
   G4RotationMatrix* rotMat_Scinti;
   G4RotationMatrix* rotMat_Scinti_Upside = new G4RotationMatrix();
   G4RotationMatrix* rotMat_Scinti_Downside = new G4RotationMatrix();
   rotMat_Scinti_Upside->rotateZ(+ deg_MeshScinti);
   rotMat_Scinti_Downside->rotateZ(- deg_MeshScinti);
   for (G4int i = -3; i <= 4; i++){
      for (G4int j = 0; j < 4; j++){
         for (G4int k = 0; k < 2; k++){
            // k=0: downside, k=1: upside
            if (k == 0) { rotMat_Scinti = rotMat_Scinti_Downside; }
            else { rotMat_Scinti = rotMat_Scinti_Upside; }
            new G4PVPlacement(
               G4Transform3D(
                  *rotMat_Scinti,
                  G4ThreeVector((-6 + 4*j) * cm, 0, (4*i - 0.5 + k) * cm)
                  ),
               G4String("PhysVol_Scinti") + G4String(std::to_string(copyNum_Scinti)),
               logVol_Scinti,
               physVol_World,
               false,
               copyNum_Scinti,
               true
            );
            copyNum_Scinti++;
         }
      }
   }

   // placement PbGlass into world
   G4int copyNum_PbGlassScinti = 200;
   G4double PbGlassScintiArriance = 30 * cm;
   for (G4int i = 0; i < 3; i++){
      new G4PVPlacement(
         G4Transform3D(
            G4RotationMatrix(),
            G4ThreeVector(0, 11.5 * (i - 1) * cm, - (35 * cm + PbGlassScintiArriance))
         ),
         G4String("PhysVol_PbGlassScinti") + G4String(std::to_string(copyNum_PbGlassScinti)),
         logVol_PbGlassScinti,
         physVol_World,
         false,
         copyNum_PbGlassScinti,
         true
      );
      copyNum_PbGlassScinti++;
   }

   return physVol_World;
}
