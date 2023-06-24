#include "DeutDetectorConstruction.hh"
#include "DeutDetectorConstructionMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "DipoleConstruction.hh"
#include "PDCConstruction.hh"
#include "NEBULAConstruction.hh"

#include "MagField.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "FragmentSD.hh"
#include "NEBULASD.hh"

#include "SimDataManager.hh"
#include "TFragSimParameter.hh"

#include "G4SystemOfUnits.hh"//Geant4.10
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//______________________________________________________________________________
DeutDetectorConstruction::DeutDetectorConstruction() 
  :
  fFillAir(false), fSetTarget(false), fTargetMat("empty")
  // Otherwise they'd be initialized randomly
{
  G4cout << "Constructor of DeutDetectorConstruction" << G4endl;
  fDetectorConstructionMessenger = new DeutDetectorConstructionMessenger(this);

  fDipoleConstruction = new DipoleConstruction();
  fPDCConstruction    = new PDCConstruction();
  fNEBULAConstruction = new NEBULAConstruction();
}
//______________________________________________________________________________
DeutDetectorConstruction::~DeutDetectorConstruction()
{
  delete fDetectorConstructionMessenger;
  delete fDipoleConstruction;
  delete fPDCConstruction;
  delete fNEBULAConstruction;
}
//______________________________________________________________________________
G4VPhysicalVolume* DeutDetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Material List
  const auto nist = G4NistManager::Instance();
  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material* Sn = nist->FindOrBuildMaterial("G4_Sn");

  G4Material* WorldMaterial;
  G4Material* TargetMaterial;
  if (fFillAir) WorldMaterial = Air;
  else          WorldMaterial = Vacuum;

  if (fTargetMat == "empty")    TargetMaterial = WorldMaterial;
  else if (fTargetMat == "Sn")  TargetMaterial = Sn;
  else if (fTargetMat == "CsI") TargetMaterial = CsI;
  else {
    TargetMaterial = WorldMaterial;
    G4ExceptionDescription ed;
    ed << "The target material \"" << fTargetMat << "\" is invalid, using empty target"; 
    G4Exception("DeutDetectorConstruction::Construct()", "material", JustWarning, ed);
  }

  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  G4double expHall_x = 7.0*m;
  G4double expHall_y = 5*0.5*m;
  G4double expHall_z = 10.0*m;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);


  G4LogicalVolume *experimentalHall_log 
    = new G4LogicalVolume(experimentalHall_box,WorldMaterial,"expHall_log");

  G4VPhysicalVolume *experimentalHall_phys 
    = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);

  // Make experimental hall invisible 
  experimentalHall_log->SetVisAttributes(new G4VisAttributes{false});

  //------------------------------ SAMURAI Magnet ------------------------------
  //------------------------------ yoke
  fDipoleConstruction->ConstructSub();
  fDipoleConstruction->PutSAMURAIMagnet(experimentalHall_log);

  G4SDManager *SDMan = G4SDManager::GetSDMpointer();

  // prepare parameter for target and PDC positions
  SimDataManager *simDataManager = SimDataManager::GetSimDataManager();
  TFragSimParameter *frag_prm = (TFragSimParameter*)simDataManager->FindParameter("FragParameter");
  if (frag_prm==0) {
    frag_prm = new TFragSimParameter("FragParameter");
    simDataManager->AddParameter(frag_prm);
  }

  //--------------------------------------- Beam Line ---------------------------------------------------
  //------------------------------ Target

  if (fSetTarget) {
    G4Box* tgt_box = new G4Box {
      "tgt_box",
			fTargetSize.x()*0.5*mm,
			fTargetSize.y()*0.5*mm,
			fTargetSize.z()*0.5*mm };
    G4LogicalVolume *LogicTarget = new G4LogicalVolume {tgt_box, TargetMaterial, "target_log"};
    G4LogicalVolume *LogicTargetSD = new G4LogicalVolume {tgt_box, TargetMaterial, "target_log"};
    new G4PVPlacement {0, G4ThreeVector{0,0,0}, LogicTargetSD, "Target_SD", LogicTarget, false, 0};

    G4RotationMatrix target_rm; target_rm.rotateY(-fTargetAngle);
    new G4PVPlacement {G4Transform3D(target_rm, fTargetPos), LogicTarget, "Target", 
                       experimentalHall_log, false, 0, true};
    frag_prm->fTargetPosition.SetXYZ(
      fTargetPos.x()/mm, 
      fTargetPos.y()/mm, 
      fTargetPos.z()/mm
    );
    frag_prm->fTargetAngle = -fTargetAngle;
    frag_prm->fTargetThickness = fTargetSize.z();

    if (fTargetSD==0){
      fTargetSD = new FragmentSD("/Target");
      SDMan->AddNewDetector(fTargetSD);
    }
    LogicTargetSD->SetSensitiveDetector(fTargetSD);
  }

  //------------------------------ NEBULA
  fNEBULAConstruction->ConstructSub();
  fNEBULAConstruction->PutNEBULA(experimentalHall_log);

  // Sensitive Detector
  if (fNEBULASD==0){
    fNEBULASD = new NEBULASD("/NEBULA");
    SDMan->AddNewDetector(fNEBULASD);
  }

  if (fNEBULAConstruction->DoesNeutExist()){
    G4LogicalVolume *neut_log = fNEBULAConstruction->GetLogicNeut();
    neut_log->SetSensitiveDetector(fNEBULASD);
  }

  if (fNEBULAConstruction->DoesVetoExist()){
    G4LogicalVolume *veto_log = fNEBULAConstruction->GetLogicVeto();
    veto_log->SetSensitiveDetector(fNEBULASD);
  }

  //------------------------------ PDCs 
  auto pdc_log = fPDCConstruction->ConstructSub();
  auto pdc_vis = new G4VisAttributes(G4Colour::Magenta());
  pdc_log->SetVisAttributes(pdc_vis);

  if (fPDCSD==0){
    fPDCSD = new FragmentSD("/PDC");
    SDMan->AddNewDetector(fPDCSD);
  }
  fPDCConstruction->GetActiveVolume()->SetSensitiveDetector(fPDCSD);
  
  G4double pdc_angle = -fPDCAngle;// SAMURAI def. (clockwise)-> Geant def. (counterclockwise)
  frag_prm->fPDCAngle = pdc_angle;

  //------------------------------ PDC1
  G4RotationMatrix pdc1_rm; pdc1_rm.rotateY(pdc_angle);
  G4ThreeVector pdc1_pos_lab{fPDC1Pos}; pdc1_pos_lab.rotateY(pdc_angle);
  G4Transform3D pdc1_trans{pdc1_rm, pdc1_pos_lab};
  new G4PVPlacement{pdc1_trans, pdc_log, "PDC1", experimentalHall_log, false, 0, true};

  frag_prm->fPDC1Position.SetXYZ(
    fPDC1Pos.x()/mm, 
    fPDC1Pos.y()/mm, 
    fPDC1Pos.z()/mm
  );

  //------------------------------ PDC2
  G4RotationMatrix pdc2_rm; pdc2_rm.rotateY(pdc_angle);
  G4ThreeVector pdc2_pos_lab{fPDC2Pos}; pdc2_pos_lab.rotateY(pdc_angle);
  G4Transform3D pdc2_trans{pdc2_rm, pdc2_pos_lab};
  new G4PVPlacement{pdc2_trans, pdc_log, "PDC2", experimentalHall_log, false, 1, true};

  frag_prm->fPDC2Position.SetXYZ(
    fPDC2Pos.x()/mm, 
    fPDC2Pos.y()/mm, 
    fPDC2Pos.z()/mm
  );

  //------------------------------ Beam Dump
  auto box_sol = new G4Box{"Box", 2.5*m/2, 2.5*m/2, 3*m/2};
  auto opening_sol = new G4Box{"Opening", 34*cm/2, 38*cm/2, 120*cm/2};
  auto opening_pos = new G4Translate3D{0, 25*cm, -90*cm};
  auto dump_sol = new G4SubtractionSolid{"Dump", box_sol, opening_sol, *opening_pos};

  auto dump_log = new G4LogicalVolume{dump_sol, Water, "Dump"};
  auto dump_colour = new G4Colour{0, 1, 1, 0.7};
  auto dump_vis = new G4VisAttributes{*dump_colour};
  dump_vis->SetForceSolid(true);
  dump_log->SetVisAttributes(dump_vis);

  return experimentalHall_phys;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetTargetPos(G4ThreeVector pos)
{
  fTargetPos = pos;
  std::cout<<"DeutDetectorConstruction : Set target position at  "
	   <<fTargetPos.x()/cm<<" cm  "
	   <<fTargetPos.y()/cm<<" cm  "
	   <<fTargetPos.z()/cm<<" cm  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetTargetAngle(G4double angle)
{
  fTargetAngle = angle;
  std::cout<<"DeutDetectorConstruction : Set target angle at  "
	   <<fTargetAngle/deg<<" deg  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetPDCAngle(G4double angle)
{
  fPDCAngle = angle;
  std::cout<<"DeutDetectorConstruction : Set PDC angle at  "
	   <<fPDCAngle/deg<<" deg  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetPDC1Pos(G4ThreeVector pos)
{
  fPDC1Pos = pos;
  std::cout<<"DeutDetectorConstruction : Set PDC1 position at  "
	   <<fPDC1Pos.x()/cm<<" cm  "
	   <<fPDC1Pos.y()/cm<<" cm  "
	   <<fPDC1Pos.z()/cm<<" cm  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetPDC2Pos(G4ThreeVector pos)
{
  fPDC2Pos = pos;
  std::cout<<"DeutDetectorConstruction : Set PDC2 position at  "
	   <<fPDC2Pos.x()/cm<<" cm  "
	   <<fPDC2Pos.y()/cm<<" cm  "
	   <<fPDC2Pos.z()/cm<<" cm  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetDumpAngle(G4double angle)
{
  fDumpAngle = angle;
  std::cout<<"DeutDetectorConstruction : Set beam dump angle at  "
	   <<fDumpAngle/deg<<" deg  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::SetDumpPos(G4ThreeVector pos)
{
  fDumpPos = pos;
  std::cout<<"DeutDetectorConstruction : Set beam dump position at  "
	   <<fDumpPos.x()/cm<<" cm  "
	   <<fDumpPos.y()/cm<<" cm  "
	   <<fDumpPos.z()/cm<<" cm  "
	   <<std::endl;
}
//______________________________________________________________________________
void DeutDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"DeutDetectorConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//______________________________________________________________________________
