#include "ICFConstruction.hh"
#include "ICFConstructionMessenger.hh"

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
ICFConstruction::ICFConstruction()
  : fPosition(719.36*mm, 0, 5000*mm),
    fAngle(59.907*deg)                    //dayone angle
{
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  new ICFConstructionMessenger(this);
}
//______________________________________________________________________________________
ICFConstruction::~ICFConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* ICFConstruction::Construct()
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
  // Construct ICF LV
  G4LogicalVolume *LogicICF = ConstructSub();

  // position
  G4RotationMatrix icfenc_rm; icfenc_rm.rotateY(-fAngle);
  G4ThreeVector ICFPos_lab = fPosition;
  ICFPos_lab.rotateY(-fAngle);
  new G4PVPlacement(G4Transform3D(icfenc_rm,ICFPos_lab),LogicICF,"icfenc",LogicWorld,false,0);

  //  LogicICF->SetVisAttributes(G4VisAttributes::Invisible);
  //  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* ICFConstruction::ConstructSub()
{
  std::cout << "Creating ICF" << std::endl;

  //------------------------------ ICF

  // ICF enclosure
  G4double icfenc_x = 970 * 0.5 *mm;
  G4double icfenc_y = 610 * 0.5 *mm;
  G4double icfenc_z = 640  * 0.5 *mm;
  G4Box* icfenc_box = new G4Box("icfenc_box",icfenc_x,icfenc_y,icfenc_z);
  G4LogicalVolume *icfenc_log = new G4LogicalVolume(icfenc_box,fVacuumMaterial,"icfenc_log");

  // active area
  G4double icf_x = 750*0.5*mm;
  G4double icf_y = 400*0.5*mm;
  G4double icfc_z = 480*0.5*mm;
  G4Box* icfc_box = new G4Box("icfc_box",icf_x,icf_y,icfc_z);
  fICFActive_log = new G4LogicalVolume(icfc_box,fVacuumMaterial,"ICFActive_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fICFActive_log,"ICF",icfenc_log,false,0);

  fICFActive_log->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));

  return icfenc_log;
}
//______________________________________________________________________________________
