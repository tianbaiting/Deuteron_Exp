#include "HODConstruction.hh"
#include "HODConstructionMessenger.hh"

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
HODConstruction::HODConstruction()
  : fPosition(586.515*mm, 0, 5116.35*mm), // dayone position
    fAngle(59.907*deg),                   //dayone angle
    fHODScinti_log(0)
{
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fPlasticMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  new HODConstructionMessenger(this);
}
//______________________________________________________________________________________
HODConstruction::~HODConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* HODConstruction::Construct()
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
  // Construct HOD LV
  G4LogicalVolume *LogicHOD = ConstructSub();

  // position
  G4ThreeVector pos(0,0,0);
  G4double phi = 0*deg;
  G4RotationMatrix rm; rm.rotateY(phi);
  new G4PVPlacement(G4Transform3D(rm,pos),
		    LogicHOD,"HOD",LogicWorld,false,0);

  //  LogicHOD->SetVisAttributes(G4VisAttributes::Invisible);
  //  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* HODConstruction::ConstructSub()
{
  std::cout << "Creating HOD" << std::endl;

  //------------------------------ HOD
  G4int    numHODPla = 16;
  G4double hodenc_x = 100*mm * numHODPla * 0.5;
  G4double hod_x = 100*0.5*mm;
  G4double hod_y = 1200*0.5*mm;
  G4double hod_z = 10*0.5*mm;

  G4Box* hodenc_box = new G4Box("hod2c_box",hodenc_x,hod_y,hod_z);
  G4LogicalVolume *hodenc_log = new G4LogicalVolume(hodenc_box,fWorldMaterial,"hodenc_log");

  G4Box* hod_box = new G4Box("hod_box",hod_x,hod_y,hod_z);
  //hod_log = new G4LogicalVolume(hod_box,fPlasticMaterial,"hod_log");
  fHODScinti_log = new G4LogicalVolume(hod_box,fVacuumMaterial,"HODScinti_log");// for acceptance
  fHODScinti_log->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));
  new G4PVReplica("HOD",
		  fHODScinti_log,
		  hodenc_log,
		  kXAxis,
		  numHODPla,
		  2*hod_x);

  return hodenc_log;
}
//______________________________________________________________________________________
