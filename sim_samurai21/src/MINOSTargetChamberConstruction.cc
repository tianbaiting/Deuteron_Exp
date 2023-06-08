#include "MINOSTargetChamberConstruction.hh"
//#include "MINOSTargetChamberConstructionMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
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
MINOSTargetChamberConstruction::MINOSTargetChamberConstruction()
  : fPosition(0,0,-4208.5*mm)
{
  fDuctMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  //new MINOSTargetChamberConstructionMessenger(this);
}
//______________________________________________________________________________________
MINOSTargetChamberConstruction::~MINOSTargetChamberConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* MINOSTargetChamberConstruction::Construct()
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
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fDuctMaterial,"World");
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
G4LogicalVolume* MINOSTargetChamberConstruction::ConstructSub()
{
  std::cout << "Creating MINOS Target Chamber" << std::endl;

  G4double duct0_inner_r = 36.0*mm;
  G4double duct0_outer_r = 38.0*mm;
  G4double duct0_dz      = 403*0.5*mm;
  G4Tubs* duct0_tube = new G4Tubs("MINOSduct0",duct0_inner_r,duct0_outer_r,duct0_dz,
				  0.0*deg,360.0*deg);

  G4double duct1_inner_r = 29.75*mm;
  G4double duct1_outer_r = 37.0*mm;
  G4double duct1_dz      = 33.*0.5*mm;
  G4Tubs* duct1_tube = new G4Tubs("MINOSduct1",duct1_inner_r,duct1_outer_r,duct1_dz,
				  0.0*deg,360.0*deg);

  G4UnionSolid* duct_tube = new G4UnionSolid("MINOSduct",
					     duct0_tube,duct1_tube,0,
					     G4ThreeVector(0,0,0.5*(403.+33.)*mm));

  G4LogicalVolume *duct_log = new G4LogicalVolume(duct_tube,fDuctMaterial,
						  "target_chamber_log");

  duct_log->SetVisAttributes(new G4VisAttributes(G4Colour::Gray()));

  G4cout << duct_log << G4endl;

  return duct_log;
}
//______________________________________________________________________________________
