#include "FDC1Construction.hh"

#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

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
FDC1Construction::FDC1Construction()
  : fPosition(0,0,-2720.971*mm)// Dayone, Minakata value
{
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
}
//______________________________________________________________________________________
FDC1Construction::~FDC1Construction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* FDC1Construction::Construct()
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
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fWorldMaterial,"World");
  G4ThreeVector worldPos(0, 0, 0);  
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);

  //---------------------------------------------------
  // Construct FDC1 LV
  G4LogicalVolume *LogicFDC1 = ConstructSub();

  // placement
  new G4PVPlacement(0,fPosition,LogicFDC1,"fdc2enc",LogicWorld,false,0);

  //  LogicFDC1->SetVisAttributes(G4VisAttributes::Invisible);
  //  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* FDC1Construction::ConstructSub()
{
  std::cout << "Creating FDC1" << std::endl;

  //------------------------------ FDC1

  // FDC1 enclosure
  G4double fdc1enc_x = 1000 * 0.5 *mm;
  G4double fdc1enc_y = 669  * 0.5 *mm;
  G4double fdc1enc_z = 336  * 0.5 *mm;
  G4Box* fdc1enc_box = new G4Box("fdc1enc_box",fdc1enc_x,fdc1enc_y,fdc1enc_z);
  G4LogicalVolume *fdc1enc_log = new G4LogicalVolume(fdc1enc_box,fVacuumMaterial,"fdc1enc_log");

  // active area
  G4double fdc1_x = 310*0.5*mm;// =window size *0.5
  G4double fdc1_y = 170*0.5*mm; // =window size *0.5
  G4double fdc1c_z = 70*mm;//
  G4Box* fdc1c_box = new G4Box("fdc1c_box",fdc1_x,fdc1_y,fdc1c_z);
  fFDC1Active_log = new G4LogicalVolume(fdc1c_box,fVacuumMaterial,"FDC1Active_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fFDC1Active_log,"FDC1",fdc1enc_log,false,0);

  fFDC1Active_log->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));

  return fdc1enc_log;
}
//______________________________________________________________________________________
