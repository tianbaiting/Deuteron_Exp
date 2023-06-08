#include "DayoneDetectorConstruction.hh"
#include "DayoneDetectorConstructionMessenger.hh"

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

#include "DipoleConstruction.hh"
#include "FDC1Construction.hh"
#include "FDC2Construction.hh"
#include "HODConstruction.hh"
#include "ExitWindowC1Construction.hh"
#include "ExitWindowNConstruction.hh"
#include "NEBULAConstruction.hh"
#include "VacuumUpstreamConstruction.hh"
#include "VacuumDownstreamConstruction.hh"

#include "MagField.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "FragmentSD.hh"
#include "NEBULASD.hh"

#include "SimDataManager.hh"
#include "TFragSimParameter.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//______________________________________________________________________________
DayoneDetectorConstruction::DayoneDetectorConstruction() 
  :
  fFillAir(false), fUseLiTarget(false),
  fTarget_phys(0), 
  fTargetPos(0,0,-3867.28*mm), fTargetSize(40,40,20.393*mm),
  fFDC1SD(0),fWindowHoleSD(0),fFDC2SD(0),fHODSD(0),fNEBULASD(0)
{
  G4cout << "Constructor of DayoneDetectorConstruction" << G4endl;
  fDetectorConstructionMessenger = new DayoneDetectorConstructionMessenger(this);

  fDipoleConstruction = new DipoleConstruction();
  fFDC1Construction = new FDC1Construction();
  fFDC2Construction = new FDC2Construction();
  fHODConstruction = new HODConstruction();
  fExitWindowC1Construction = new ExitWindowC1Construction();
  fExitWindowNConstruction = new ExitWindowNConstruction();
  fNEBULAConstruction = new NEBULAConstruction();
  fVacuumUpstreamConstruction = new VacuumUpstreamConstruction();
  fVacuumDownstreamConstruction = new VacuumDownstreamConstruction();
}
//______________________________________________________________________________
DayoneDetectorConstruction::~DayoneDetectorConstruction()
{
  delete fDetectorConstructionMessenger;
  delete fDipoleConstruction;
  delete fFDC1Construction;
  delete fFDC2Construction;
  delete fHODConstruction;
  delete fExitWindowC1Construction;
  delete fExitWindowNConstruction;
  delete fNEBULAConstruction;
  delete fVacuumUpstreamConstruction;
  delete fVacuumDownstreamConstruction;
}
//______________________________________________________________________________
G4VPhysicalVolume* DayoneDetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Material* AirMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4Material *Lithium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Li"); 
  G4Material* WorldMaterial;
  G4Material* TargetMaterial;
  if (fFillAir) WorldMaterial = AirMaterial;
  else          WorldMaterial = Vacuum;

  if (fUseLiTarget) TargetMaterial = Lithium;
  else              TargetMaterial = Vacuum;

  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  G4double expHall_x = 5.0*m;
  G4double expHall_y = 5*0.5*m;
  G4double expHall_z = 10.0*m;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);


  G4LogicalVolume *experimentalHall_log 
    = new G4LogicalVolume(experimentalHall_box,WorldMaterial,"expHall_log");

  G4VPhysicalVolume *experimentalHall_phys 
    = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);

  //------------------------------ SAMURAI Magnet ------------------------------
  //------------------------------ yoke
  fDipoleConstruction->ConstructSub();
  G4VPhysicalVolume *Dipole_phys = fDipoleConstruction->PutSAMURAIMagnet(experimentalHall_log);
  G4double magAngle = fDipoleConstruction->GetAngle();

  G4SDManager *SDMan = G4SDManager::GetSDMpointer();

  // prepare parameter for detector positions for charged fragment
  SimDataManager *simDataManager = SimDataManager::GetSimDataManager();
  TFragSimParameter *frag_prm = (TFragSimParameter*)simDataManager->FindParameter("FragParameter");
  if (frag_prm==0) {
    frag_prm = new TFragSimParameter("FragParameter");
    simDataManager->AddParameter(frag_prm);
  }

  //------------------------------ Vacuum upstream of Magnet
  G4LogicalVolume *vacuum_upstream_log = fVacuumUpstreamConstruction->ConstructSub(Dipole_phys);
  G4ThreeVector vacuum_upstream_pos = fVacuumUpstreamConstruction->GetPosition();
  new G4PVPlacement(0,vacuum_upstream_pos,
		    vacuum_upstream_log,"vacuum_upstream",
		    experimentalHall_log,false,0);

  //--------------------------------------- Beam Line ---------------------------------------------------
  //------------------------------ Target
  //G4Box* tgt_box = new G4Box("tgt_box",40*0.5*mm,40*0.5*mm,10*0.5*mm);
  G4Box* tgt_box = new G4Box("tgt_box",
			     fTargetSize.x()*0.5*mm,
			     fTargetSize.y()*0.5*mm,
			     fTargetSize.z()*0.5*mm );
  G4LogicalVolume *LogicTarget = new G4LogicalVolume(tgt_box,TargetMaterial,"target_log");

//  fTarget_phys = new G4PVPlacement(0,fTargetPos,
//				   LogicTarget,"Target",experimentalHall_log,false,0);
  fTarget_phys = new G4PVPlacement(0,fTargetPos - vacuum_upstream_pos,
				   LogicTarget,"Target",vacuum_upstream_log,false,0);

  LogicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Magenta()));

  frag_prm->fTargetPosition.SetXYZ(fTargetPos.x()/mm,
				   fTargetPos.y()/mm,
				   fTargetPos.z()/mm);

  //------------------------------ FDC1
  G4LogicalVolume *fdc1enc_log = fFDC1Construction->ConstructSub();
  G4ThreeVector FDC1Pos = fFDC1Construction->GetPosition();
  // put FDC1 in the vacuum_upstream. --> input position is relative to vacuum_upstream
  new G4PVPlacement(0,FDC1Pos - vacuum_upstream_pos,fdc1enc_log,"fdc1enc",vacuum_upstream_log,false,0);

  if (fFDC1SD==0){
    fFDC1SD = new FragmentSD("/FDC1");
    SDMan->AddNewDetector(fFDC1SD);
  }
  fFDC1Construction->GetActiveVolume()->SetSensitiveDetector(fFDC1SD);

  frag_prm->fFDC1Position.SetXYZ(FDC1Pos.x()/mm,
				 FDC1Pos.y()/mm,
				 FDC1Pos.z()/mm);
  frag_prm->fFDC1Angle = 0;

  //------------------------------ FDC2
//  fFDC2Construction->SetAngle(59.907*deg);
  fFDC2Construction->SetAngle(magAngle+29.907*deg);
  G4LogicalVolume *fdc2enc_log = fFDC2Construction->ConstructSub();  
  G4double FDC2Angle = -fFDC2Construction->GetAngle();//SAMURAI def. -> Geant def.
  G4RotationMatrix fdc2enc_rm; fdc2enc_rm.rotateY(FDC2Angle);
  G4ThreeVector FDC2Pos_lab = fFDC2Construction->GetPosition();
  FDC2Pos_lab.rotateY(FDC2Angle);
  new G4PVPlacement(G4Transform3D(fdc2enc_rm,FDC2Pos_lab),fdc2enc_log,"fdc2enc",experimentalHall_log,false,0);

  if (fFDC2SD==0){
    fFDC2SD = new FragmentSD("/FDC2");
    SDMan->AddNewDetector(fFDC2SD);
  }
  fFDC2Construction->GetActiveVolume()->SetSensitiveDetector(fFDC2SD);

  frag_prm->fFDC2Position.SetXYZ(FDC2Pos_lab.x()/mm,
				 FDC2Pos_lab.y()/mm,
				 FDC2Pos_lab.z()/mm);
  frag_prm->fFDC2Angle = FDC2Angle/deg;

  //------------------------------ exit window for charged particles
  G4LogicalVolume *window_flange_log = fExitWindowC1Construction->ConstructSub();
  G4ThreeVector pWindow_flange = fExitWindowC1Construction->GetPosition();// Get Default position (Dayone)
  pWindow_flange.rotateY(FDC2Angle);

  G4VPhysicalVolume *window_phys = new G4PVPlacement(G4Transform3D(fdc2enc_rm,pWindow_flange),
		    window_flange_log,"window_flange",experimentalHall_log,false,0);

  if (fWindowHoleSD==0){
    fWindowHoleSD = new FragmentSD("/WindowHole");
    SDMan->AddNewDetector(fWindowHoleSD);
  }
  fExitWindowC1Construction->GetWindowHoleVolume()->SetSensitiveDetector(fWindowHoleSD);

  frag_prm->fWindowHolePosition.SetXYZ(pWindow_flange.x()/mm,
				       pWindow_flange.y()/mm,
				       pWindow_flange.z()/mm);
  frag_prm->fWindowHoleAngle = FDC2Angle/deg;

  //------------------------------ Vacuum extention
  G4LogicalVolume *vacuum_downstream_log = fVacuumDownstreamConstruction->ConstructSub(Dipole_phys,
										       window_phys);
  G4ThreeVector vacuum_downstream_pos = fVacuumDownstreamConstruction->GetPosition();
  new G4PVPlacement(G4Transform3D(fdc2enc_rm,vacuum_downstream_pos),
		    vacuum_downstream_log,"vacuum_downstream",
		    experimentalHall_log,false,0);

  //------------------------------ HODF
  fHODConstruction->SetAngle(-FDC2Angle);// geant4 def. -> SAMURAI def.
  G4LogicalVolume *hodenc_log = fHODConstruction->ConstructSub();
  G4double HODAngle = -fHODConstruction->GetAngle();//SAMURAI def. -> Geant def.
  G4RotationMatrix hodenc_rm; hodenc_rm.rotateY(HODAngle);
  G4ThreeVector pHODF = fHODConstruction->GetPosition();
  pHODF.rotateY(HODAngle);
  new G4PVPlacement(G4Transform3D(hodenc_rm,pHODF),
		    hodenc_log,"hodenc",experimentalHall_log,false,0);

  if (fHODSD==0){
    fHODSD = new FragmentSD("/HODF");
    SDMan->AddNewDetector(fHODSD);
  }
  fHODConstruction->GetActiveVolume()->SetSensitiveDetector(fHODSD);

  frag_prm->fHODPosition.SetXYZ(pHODF.x()/mm,
				pHODF.y()/mm,
				pHODF.z()/mm);
  frag_prm->fHODAngle = FDC2Angle/deg;

  //------------------------------ exit window for neutrons
  fExitWindowNConstruction->ConstructSub();
  fExitWindowNConstruction->SetAngle(magAngle);
  fExitWindowNConstruction->PutExitWindow(experimentalHall_log);

  FragmentSD *NeutronWinSD = new FragmentSD("NeutronWindow");
  SDMan->AddNewDetector(NeutronWinSD);
  fExitWindowNConstruction->GetWindowVolume()->SetSensitiveDetector(NeutronWinSD);

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

  return experimentalHall_phys;
}
//______________________________________________________________________________
void DayoneDetectorConstruction::SetTargetPos(G4ThreeVector pos)
{
  fTargetPos = pos;
//  std::cout<<"DayoneDetectorConstruction : Set Target position at  "
//	   <<fTargetPos.x()/cm<<" cm  "
//	   <<fTargetPos.y()/cm<<" cm  "
//	   <<fTargetPos.z()/cm<<" cm  "
//	   <<std::endl;
}
//______________________________________________________________________________
void DayoneDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"DayoneDetectorConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//______________________________________________________________________________
