
#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "G4NistManager.hh"

DetectorConstruction::DetectorConstruction()
{
  fLogicWorld = 0;
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //  G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
  G4NistManager::Instance()->FindOrBuildMaterial("G4_H");
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Li");
  G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
}

DetectorConstruction::~DetectorConstruction()
{;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // World LV
  G4ThreeVector world_pos(0,0,0) ;  
  G4ThreeVector world_size(1,1,1);
  G4Box* solidW = new G4Box("World",
			    world_size.x()/2.,
			    world_size.y()/2.,
			    world_size.z()/2.) ;
  fLogicWorld = new G4LogicalVolume(solidW,fWorldMaterial,"World");
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       world_pos,
					       fLogicWorld,
					       "World",
					       0,
					       false,
					       0);

  // Print status
  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}

