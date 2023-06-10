#include "PDCConstruction.hh"

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
PDCConstruction::PDCConstruction()
  : fPosition(400*mm, 0, 4100*mm),
    fAngle(57*deg)                 // PDC1 configure in deuteron experiment with B=1.3T
{
  const auto nist = G4NistManager::Instance();
  fVacuumMaterial = nist->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = nist->FindOrBuildMaterial("G4_Galactic");

  auto *Argon = nist->FindOrBuildMaterial("G4_Ar");
  auto *C4H10 = nist->FindOrBuildMaterial("G4_BUTANE");
  const auto frac_Ar_PDC = 0.75, frac_C4H10_PDC = 0.25;
  const auto molemass_Ar_PDC = Argon->GetMassOfMolecule() * (frac_Ar_PDC * CLHEP::Avogadro);
  const auto molemass_C4H10_PDC = C4H10->GetMassOfMolecule() * (frac_C4H10_PDC * CLHEP::Avogadro);
  const auto molemass_PDC = molemass_Ar_PDC + molemass_C4H10_PDC;

  // 75% Ar + 25% i-C₄H₁₀ (1 atm)
  fPDCMaterial = new G4Material("mat_PDC", molemass_PDC/(24.055*L/mole), 2, kStateGas);
  fPDCMaterial->AddMaterial(Argon, molemass_Ar_PDC/molemass_PDC);
  fPDCMaterial->AddMaterial(C4H10, molemass_C4H10_PDC/molemass_PDC);
}
//______________________________________________________________________________________
PDCConstruction::~PDCConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* PDCConstruction::Construct()
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
  // Construct PDC LV
  G4LogicalVolume *LogicPDC = ConstructSub();

  // position
  G4RotationMatrix PDCenc_rm; PDCenc_rm.rotateY(-fAngle);
  G4ThreeVector PDCPos_lab = fPosition;
  PDCPos_lab.rotateY(-fAngle);
  new G4PVPlacement(G4Transform3D(PDCenc_rm,PDCPos_lab),LogicPDC,"PDCenc",LogicWorld,false,0);

  //  LogicPDC->SetVisAttributes(G4VisAttributes::GetInvisible());
  //  LogicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* PDCConstruction::ConstructSub()
{
  std::cout << "Creating PDC" << std::endl;

  //------------------------------ PDC

  // PDC enclosure
  G4double PDCenc_x = 1700 * 0.5 *mm;
  G4double PDCenc_y = 800 * 0.5 *mm;
  G4double PDCenc_z = 190 * 0.5 *mm;
  G4Box* PDCenc_box = new G4Box("PDCenc_box",PDCenc_x,PDCenc_y,PDCenc_z);
  G4LogicalVolume *PDCenc_log = new G4LogicalVolume(PDCenc_box,fPDCMaterial,"PDCenc_log");

  // active area
  G4double PDC_x = 1680*0.5*mm;// =window size *0.5
  G4double PDC_y = 780*0.5*mm; // =window size *0.5
  G4double PDCc_z = 16*mm;// volume enclosed by cathodes
  G4Box* PDCc_box = new G4Box("PDCc_box",PDC_x,PDC_y,PDCc_z);
  fPDCActive_log = new G4LogicalVolume(PDCc_box,fPDCMaterial,"PDCActive_log");
  new G4PVPlacement(0,G4ThreeVector(0,0,0),fPDCActive_log,"PDC",PDCenc_log,false,0);

  fPDCActive_log->SetVisAttributes(new G4VisAttributes{false});

  return PDCenc_log;
}
//______________________________________________________________________________________
