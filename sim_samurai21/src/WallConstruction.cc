#include "WallConstruction.hh"

#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"//Geant4.10
#include "G4NistManager.hh"
//______________________________________________________________________________________
WallConstruction::WallConstruction()
  : fFacePosition(0,0,13*m), fWallSize(10*m, 10*m, 4*m)
{
  //fWallMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWallMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");
}
//______________________________________________________________________________________
WallConstruction::~WallConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* WallConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //---------------------------------------------------
  // World LV
  G4ThreeVector worldSize(10*m,10*m,20*m);
  G4Box* solidWorld = new G4Box("World",
				worldSize.x()/2,
				worldSize.y()/2,
				worldSize.z()/2);
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fWallMaterial,"World");
  G4ThreeVector worldPos(0, 0, 0);  
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* WallConstruction::ConstructSub()
{
  std::cout << "Creating Wall" << std::endl;

  G4Box* wall_box = new G4Box("wall_box",fWallSize.x()/2,fWallSize.y()/2,fWallSize.z()/2);
  G4LogicalVolume *wall_log = new G4LogicalVolume(wall_box,fWallMaterial,"wall_log");
  wall_log->SetVisAttributes(new G4VisAttributes(G4Colour::Cyan()));

  return wall_log;
}
//______________________________________________________________________________________
