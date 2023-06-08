#include "SimtraceDetectorConstruction.hh"
#include "SimtraceDetectorConstructionMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "DipoleConstruction.hh"
#include "FDC1Construction.hh"
#include "FDC2Construction.hh"
#include "HODConstruction.hh"
#include "ExitWindowC1Construction.hh"
#include "ExitWindowC2Construction.hh"
#include "MagField.hh"
#include "globals.hh"
#include "FragmentSD.hh"

#include "SimDataManager.hh"
#include "TFragSimParameter.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SimtraceDetectorConstruction::SimtraceDetectorConstruction() 
  :
  fTarget_phys(0), 
  fMagAngle(0),
  fTargetPos(0,0,-400*cm),
  fWindowHoleSD(0)
{
  G4cout << "Constructor of SimtraceDetectorConstruction" << G4endl;
  fDetectorConstructionMessenger = new SimtraceDetectorConstructionMessenger(this);

  fDipoleConstruction = new DipoleConstruction();
  fFDC1Construction = new FDC1Construction();
  fFDC2Construction = new FDC2Construction();
  fHODConstruction = new HODConstruction();
  fExitWindowC1Construction = new ExitWindowC1Construction();
  fExitWindowC2Construction = new ExitWindowC2Construction();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SimtraceDetectorConstruction::~SimtraceDetectorConstruction()
{
  delete fDetectorConstructionMessenger;
  delete fDipoleConstruction;
  delete fFDC1Construction;
  delete fFDC2Construction;
  delete fHODConstruction;
  delete fExitWindowC1Construction;
  delete fExitWindowC2Construction;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* SimtraceDetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

//  //------------------------------------------------------ materials
//  G4NistManager* man = G4NistManager::Instance();
//  G4double a;  // atomic mass
//  G4double z;  // atomic number
//  G4double density;
//
//  G4Material* fHe = man->FindOrBuildMaterial("G4_He");
//  G4Material* fCH4 = man->FindOrBuildMaterial("G4_METHANE");
//  G4Material* FDCgas = new G4Material("FDCgas",0.4668*mg/cm3,2);
//  FDCgas->AddMaterial(fHe,0.4);
//  FDCgas->AddMaterial(fCH4,0.6);
//
//
//  //G4Material* fAirMaterial = man->FindOrBuildMaterial("G4_AIR");
//  //G4Material* fPlasticMaterial = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  G4Material* fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  //G4double expHall_x = 10.0*m;
  G4double expHall_x = 5.0*m;
  G4double expHall_y = 5*0.5*m;
  //G4double expHall_z = 10.0*m;
  G4double expHall_z = 5.0*m;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);

  G4LogicalVolume *experimentalHall_log 
    = new G4LogicalVolume(experimentalHall_box,fWorldMaterial,"expHall_log");

  G4VPhysicalVolume *experimentalHall_phys 
    = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);

  //------------------------------ SAMURAI Magnet ------------------------------
  //------------------------------ yoke
  fDipoleConstruction->ConstructSub();
  fDipoleConstruction->PutSAMURAIMagnet(experimentalHall_log);
  fMagAngle = fDipoleConstruction->GetAngle();

  G4SDManager *SDMan = G4SDManager::GetSDMpointer();

  SimDataManager *simDataManager = SimDataManager::GetSimDataManager();
  TFragSimParameter *frag_prm = (TFragSimParameter*)simDataManager->FindParameter("FragParameter");
  if (frag_prm==0) {
    frag_prm = new TFragSimParameter("FragmentParameter");
    simDataManager->AddParameter(frag_prm);
  }
  //--------------------------------------- Beam Line ---------------------------------------------------
  //------------------------------ Target
  G4Box* tgt_box = new G4Box("tgt_box",40*0.5*mm,40*0.5*mm,10*0.5*mm);
  G4LogicalVolume *LogicTarget = new G4LogicalVolume(tgt_box,Vacuum,"target_log");
  fTarget_phys = new G4PVPlacement(0,fTargetPos,
				   LogicTarget,"Target",experimentalHall_log,false,0);

  LogicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));

  frag_prm->fTargetPosition.SetXYZ(fTargetPos.x()/mm,
				   fTargetPos.y()/mm,
				   fTargetPos.z()/mm);

  //------------------------------ FDC1
  G4LogicalVolume *fdc1enc_log = fFDC1Construction->ConstructSub();
  G4ThreeVector FDC1Pos = fFDC1Construction->GetPosition();
  new G4PVPlacement(0,FDC1Pos,fdc1enc_log,"fdc1enc",experimentalHall_log,false,0);

  FragmentSD *FDC1SD = new FragmentSD("FDC1");
  SDMan->AddNewDetector(FDC1SD);
  fFDC1Construction->GetActiveVolume()->SetSensitiveDetector(FDC1SD);

  frag_prm->fFDC1Position.SetXYZ(FDC1Pos.x()/mm,
				 FDC1Pos.y()/mm,
				 FDC1Pos.z()/mm);
  frag_prm->fFDC1Angle = 0;
  //------------------------------ FDC2
  fFDC2Construction->SetAngle(59.907*deg);
  G4LogicalVolume *fdc2enc_log = fFDC2Construction->ConstructSub();  
  G4double FDC2Angle = -fFDC2Construction->GetAngle();//SAMURAI def. -> Geant def.
  G4RotationMatrix fdc2enc_rm; fdc2enc_rm.rotateY(FDC2Angle);
  G4ThreeVector FDC2Pos_lab = fFDC2Construction->GetPosition();
  FDC2Pos_lab.rotateY(FDC2Angle);
  new G4PVPlacement(G4Transform3D(fdc2enc_rm,FDC2Pos_lab),fdc2enc_log,"fdc2enc",experimentalHall_log,false,0);

  FragmentSD *FDC2SD = new FragmentSD("FDC2");
  SDMan->AddNewDetector(FDC2SD);
  fFDC2Construction->GetActiveVolume()->SetSensitiveDetector(FDC2SD);

  frag_prm->fFDC2Position.SetXYZ(FDC2Pos_lab.x()/mm,
				 FDC2Pos_lab.y()/mm,
				 FDC2Pos_lab.z()/mm);
  frag_prm->fFDC2Angle = FDC2Angle/deg;

  //------------------------------ exit window for charged particles
  fExitWindowC2Construction->ConstructSub();
  fExitWindowC2Construction->SetAngle(FDC2Angle);
  fExitWindowC2Construction->PutExitWindow(experimentalHall_log);

  if (fWindowHoleSD==0){
    fWindowHoleSD = new FragmentSD("WindowHole");
    SDMan->AddNewDetector(fWindowHoleSD);
  }
  fExitWindowC2Construction->GetWindowHoleVolume()->SetSensitiveDetector(fWindowHoleSD);

  G4ThreeVector pWindow_flange = fExitWindowC2Construction->GetPosition();
  pWindow_flange.rotateY(FDC2Angle);
  frag_prm->fWindowHolePosition.SetXYZ(pWindow_flange.x()/mm,
				       pWindow_flange.y()/mm,
				       pWindow_flange.z()/mm);
  frag_prm->fWindowHoleAngle = FDC2Angle/deg;

//  G4LogicalVolume *window_flange_log = fExitWindowC2Construction->ConstructSub();
//  G4ThreeVector pWindow_flange = fExitWindowC2Construction->GetPosition();// Get Default position (Dayone)
//  pWindow_flange.rotateY(FDC2Angle);
//  new G4PVPlacement(G4Transform3D(fdc2enc_rm,pWindow_flange),
//		    window_flange_log,"window_flange",experimentalHall_log,false,0);
//
//  if (fWindowHoleSD==0){
//    fWindowHoleSD = new FragmentSD("WindowHole");
//    SDMan->AddNewDetector(fWindowHoleSD);
//    fExitWindowC2Construction->GetWindowHoleVolume()->SetSensitiveDetector(fWindowHoleSD);
//  }
//
//  frag_prm->fWindowHolePosition.SetXYZ(pWindow_flange.x()/mm,
//				       pWindow_flange.y()/mm,
//				       pWindow_flange.z()/mm);
//  frag_prm->fWindowHoleAngle = FDC2Angle/deg;
//
  //------------------------------ HODF
  G4LogicalVolume *hodenc_log = fHODConstruction->ConstructSub();
  G4RotationMatrix hodenc_rm; hodenc_rm.rotateY(FDC2Angle);
  G4ThreeVector pHODF = fHODConstruction->GetPosition();
  pHODF.rotateY(FDC2Angle);
  new G4PVPlacement(G4Transform3D(hodenc_rm,pHODF),
		    hodenc_log,"hodenc",experimentalHall_log,false,0);

  FragmentSD *HODSD = new FragmentSD("HODF");
  SDMan->AddNewDetector(HODSD);
  fHODConstruction->GetActiveVolume()->SetSensitiveDetector(HODSD);

  frag_prm->fHODPosition.SetXYZ(pHODF.x()/mm,
				pHODF.y()/mm,
				pHODF.z()/mm);
  frag_prm->fHODAngle = FDC2Angle/deg;

  return experimentalHall_phys;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SimtraceDetectorConstruction::SetTargetPos(G4ThreeVector pos)
{
  fTargetPos = pos;
//  std::cout<<"SimtraceDetectorConstruction : Set Target position at  "
//	   <<fTargetPos.x()/cm<<" cm  "
//	   <<fTargetPos.y()/cm<<" cm  "
//	   <<fTargetPos.z()/cm<<" cm  "
//	   <<std::endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SimtraceDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"SimtraceDetectorConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
