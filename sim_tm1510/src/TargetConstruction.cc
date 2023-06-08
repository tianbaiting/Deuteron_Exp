#include "TargetConstruction.hh"
#include "TargetConstructionMessenger.hh"

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

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"//Geant4.10
//______________________________________________________________________________________
TargetConstruction::TargetConstruction()
  : fPosition(0,0,-4139*mm)// for TM1510
{
  fTargetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  new TargetConstructionMessenger(this);
}
//______________________________________________________________________________________
TargetConstruction::~TargetConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* TargetConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //---------------------------------------------------
  // World LV
  G4ThreeVector worldSize(10*m,10*m,10*m);
  G4Box* solidWorld = new G4Box("World",
				worldSize.x()/2,
				worldSize.y()/2,
				worldSize.z()/2);
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fTargetMaterial,"World");
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
G4LogicalVolume* TargetConstruction::ConstructSub()
{
  std::cout << "Creating Target" << std::endl;

  G4double target_dx = 40*0.5*mm;
  G4double target_dy = 40*0.5*mm;
  G4double target_dz = 10*0.5*mm;
  G4Box* target_box = new G4Box("target_box",target_dx,target_dy,target_dz);
  G4LogicalVolume *target_log = new G4LogicalVolume(target_box,fTargetMaterial,"target_log");
  target_log->SetVisAttributes(new G4VisAttributes(G4Colour::Cyan()));

  return target_log;
}
//______________________________________________________________________________________
