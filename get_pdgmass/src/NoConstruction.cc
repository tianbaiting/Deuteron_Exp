#include "NoConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"//Geant4.10
G4VPhysicalVolume* NoConstruction::Construct()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4Box* box = new G4Box("expHall_box",10*cm,10*cm,10*cm);
  G4LogicalVolume *vvol = new G4LogicalVolume(box,mat,"box");
  G4VPhysicalVolume *pvol = new G4PVPlacement(0,G4ThreeVector(),vvol,"pvol",0,false,0);
  return pvol;
}
