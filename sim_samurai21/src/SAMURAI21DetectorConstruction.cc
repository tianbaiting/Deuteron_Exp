#include "SAMURAI21DetectorConstruction.hh"
#include "SAMURAI21DetectorConstructionMessenger.hh"

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
#include "TargetConstruction.hh"
#include "MINOSTargetChamberConstruction.hh"
#include "FDC1Construction.hh"
#include "FDC2Construction.hh"
#include "HODF24Construction.hh"
#include "ExitWindowC2Construction.hh"
#include "ExitWindowNConstruction.hh"
#include "NEBULAConstruction.hh"
#include "NeuLANDConstruction.hh"
#include "VacuumUpstreamConstruction.hh"
#include "VacuumDownstreamConstruction.hh"
#include "WallConstruction.hh"

#include "MagField.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "FragmentSD.hh"
#include "NEBULASD.hh"
#include "NeuLANDSD.hh"

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
SAMURAI21DetectorConstruction::SAMURAI21DetectorConstruction() 
  :
  fFillAir(false),
  fFDC1SD(0),fWindowHoleSD(0),fFDC2SD(0),fHODSD(0),
  fNeutronWinSD(0), fNEBULASD(0), fNeuLANDSD(0)
{
  G4cout << "Constructor of SAMURAI21DetectorConstruction" << G4endl;
  fDetectorConstructionMessenger = new SAMURAI21DetectorConstructionMessenger(this);

  fDipoleConstruction = new DipoleConstruction();
  fTargetConstruction = new TargetConstruction();
  fMINOSTargetChamberConstruction = new MINOSTargetChamberConstruction();
  fFDC1Construction = new FDC1Construction();
  fFDC2Construction = new FDC2Construction();
  fHODConstruction = new HODF24Construction();
  fExitWindowC2Construction = new ExitWindowC2Construction();
  fExitWindowNConstruction = new ExitWindowNConstruction();
  fNEBULAConstruction = new NEBULAConstruction();
  fNeuLANDConstruction = new NeuLANDConstruction();
  fVacuumUpstreamConstruction = new VacuumUpstreamConstruction();
  fVacuumDownstreamConstruction = new VacuumDownstreamConstruction();
  fWallConstruction = new WallConstruction();
}
//______________________________________________________________________________
SAMURAI21DetectorConstruction::~SAMURAI21DetectorConstruction()
{
  delete fDetectorConstructionMessenger;
  delete fDipoleConstruction;
  delete fTargetConstruction;
  delete fMINOSTargetChamberConstruction;
  delete fFDC1Construction;
  delete fFDC2Construction;
  delete fHODConstruction;
  delete fExitWindowC2Construction;
  delete fExitWindowNConstruction;
  delete fNEBULAConstruction;
  delete fNeuLANDConstruction;
  delete fVacuumUpstreamConstruction;
  delete fVacuumDownstreamConstruction;
  delete fWallConstruction;
}
//______________________________________________________________________________
G4VPhysicalVolume* SAMURAI21DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Material* AirMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4Material* WorldMaterial;
  if (fFillAir) WorldMaterial = AirMaterial;
  else          WorldMaterial = Vacuum;

  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  G4double expHall_x = 12*m;
  G4double expHall_y = 12*m;
  G4double expHall_z = 35.0*m;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x/2,expHall_y/2,expHall_z/2);


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
  G4LogicalVolume *target_log = fTargetConstruction->ConstructSub();
  G4ThreeVector TargetPos = fTargetConstruction->GetPosition();

  fTarget_phys = new G4PVPlacement(0,TargetPos,
				   target_log,"Target",experimentalHall_log,false,0);

  frag_prm->fTargetPosition.SetXYZ(TargetPos.x()/mm,
				   TargetPos.y()/mm,
				   TargetPos.z()/mm);
  frag_prm->fTargetThickness = fTargetConstruction->GetThickness()/mm;
  //------------------------------ MINOS Target Chamber
  G4LogicalVolume *minos_duct_log = fMINOSTargetChamberConstruction->ConstructSub();
  G4ThreeVector minos_duct_pos = fMINOSTargetChamberConstruction->GetPosition();
  G4cout<<minos_duct_pos.z()/mm<<G4endl;
  new G4PVPlacement(0,minos_duct_pos,
		    minos_duct_log,"MINOSTargetChamber",experimentalHall_log,false,0);

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
  fFDC2Construction->SetAngle(magAngle+29.91*deg);
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
  fExitWindowC2Construction->ConstructSub();
  fExitWindowC2Construction->SetAngle(FDC2Angle);
  fExitWindowC2Construction->PutExitWindow(experimentalHall_log);

  if (fWindowHoleSD==0){
    fWindowHoleSD = new FragmentSD("/WindowHole");
    SDMan->AddNewDetector(fWindowHoleSD);
  }
  fExitWindowC2Construction->GetWindowHoleVolume()->SetSensitiveDetector(fWindowHoleSD);

  G4ThreeVector pWindow_flange = fExitWindowC2Construction->GetPosition();
  pWindow_flange.rotateY(FDC2Angle);
  frag_prm->fWindowHolePosition.SetXYZ(pWindow_flange.x()/mm,
				       pWindow_flange.y()/mm,
				       pWindow_flange.z()/mm);
  frag_prm->fWindowHoleAngle = FDC2Angle/deg;

  //------------------------------ Vacuum extention
  G4LogicalVolume *vacuum_downstream_log = fVacuumDownstreamConstruction->ConstructSub(Dipole_phys,fExitWindowC2Construction->GetWindowHolePhys());
  G4ThreeVector vacuum_downstream_pos = fVacuumDownstreamConstruction->GetPosition();
  new G4PVPlacement(G4Transform3D(fdc2enc_rm,vacuum_downstream_pos),
		    vacuum_downstream_log,"vacuum_downstream",
		    experimentalHall_log,false,0);

  //------------------------------ HODF24
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

  if (fNeutronWinSD==0){
    fNeutronWinSD = new FragmentSD("/NeutronWindow");
    SDMan->AddNewDetector(fNeutronWinSD);
  }
  fExitWindowNConstruction->GetWindowVolume()->SetSensitiveDetector(fNeutronWinSD);


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

  //------------------------------ NeuLAND
  fNeuLANDConstruction->ConstructSub();
  fNeuLANDConstruction->PutNeuLAND(experimentalHall_log);

  // Sensitive Detector
  if (fNeuLANDSD==0){
    fNeuLANDSD = new NeuLANDSD("/NeuLAND");
    SDMan->AddNewDetector(fNeuLANDSD);
  }

  if (fNeuLANDConstruction->DoesNeutExist()){
    G4LogicalVolume *neut_log = fNeuLANDConstruction->GetLogicNeut();
    neut_log->SetSensitiveDetector(fNeuLANDSD);
  }

  if (fNeuLANDConstruction->DoesVetoExist()){
    G4LogicalVolume *veto_log = fNeuLANDConstruction->GetLogicVeto();
    veto_log->SetSensitiveDetector(fNeuLANDSD);
  }


//  //------------------------------ Concrete wall
//  G4LogicalVolume *wall_log = fWallConstruction->ConstructSub();
//  G4ThreeVector WallPos = fWallConstruction->GetPosition();
//
//  G4cout<<WallPos.x()/m<<" "
//	<<WallPos.y()/m<<" "
//	<<WallPos.z()/m<<" "
//	<<G4endl;
//
//  new G4PVPlacement(0,WallPos,
//		    wall_log,"Wall",experimentalHall_log,false,0);

  return experimentalHall_phys;
}
//______________________________________________________________________________
void SAMURAI21DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"SAMURAI21DetectorConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//______________________________________________________________________________
