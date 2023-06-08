#include "FDC2Construction.hh"
#include "FDC2ConstructionMessenger.hh"

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
//______________________________________________________________________________________
FDC2Construction::FDC2Construction()
  : fPosition(719.36*mm, 0, 4126.915*mm), //dayone position
    fAngle(59.907*deg)                    //dayone angle
{
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  new FDC2ConstructionMessenger(this);
}
//______________________________________________________________________________________
FDC2Construction::~FDC2Construction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* FDC2Construction::Construct()
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
  // Construct FDC2 LV
  G4LogicalVolume *LogicFDC2 = ConstructSub();

  // position
  G4RotationMatrix fdc2enc_rm; fdc2enc_rm.rotateY(-fAngle);
  G4ThreeVector FDC2Pos_lab = fPosition;
  FDC2Pos_lab.rotateY(-fAngle);
  new G4PVPlacement(G4Transform3D(fdc2enc_rm,FDC2Pos_lab),LogicFDC2,"fdc2enc",LogicWorld,false,0);

  //  LogicFDC2->SetVisAttributes(G4VisAttributes::Invisible);
  //  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* FDC2Construction::ConstructSub()
{
  std::cout << "Creating FDC2" << std::endl;

  //------------------------------ FDC2

  // FDC2 enclosure
  G4double fdc2enc_x = 2616 * 0.5 *mm;
  G4double fdc2enc_y = 1156 * 0.5 *mm;
  G4double fdc2enc_z = 876  * 0.5 *mm;
  G4Box* fdc2enc_box = new G4Box("fdc2enc_box",fdc2enc_x,fdc2enc_y,fdc2enc_z);
  G4LogicalVolume *fdc2enc_log = new G4LogicalVolume(fdc2enc_box,fVacuumMaterial,"fdc2enc_log");

  // active area
  G4double fdc2_x = 2296*0.5*mm;// =window size *0.5
  G4double fdc2_y = 836*0.5*mm; // =window size *0.5
  G4double fdc2c_z = (308.66+8.66)*mm;//=|wirepos(X1)+max_dl| (=active area *0.5)
  G4Box* fdc2c_box = new G4Box("fdc2c_box",fdc2_x,fdc2_y,fdc2c_z);
  fFDC2Active_log = new G4LogicalVolume(fdc2c_box,fVacuumMaterial,"FDC2Active_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fFDC2Active_log,"FDC2",fdc2enc_log,false,0);

  fFDC2Active_log->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));

  return fdc2enc_log;
}
//______________________________________________________________________________________
