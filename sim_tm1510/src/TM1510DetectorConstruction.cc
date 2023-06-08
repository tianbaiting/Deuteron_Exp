#include "TM1510DetectorConstruction.hh"
#include "TM1510DetectorConstructionMessenger.hh"

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
#include "DALIConstruction.hh"
#include "FDC1Construction.hh"
#include "FDC2Construction.hh"
#include "ICFConstruction.hh"
#include "HODSConstruction.hh"
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
#include "DALISD.hh"
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
TM1510DetectorConstruction::TM1510DetectorConstruction() 
  :
  fFillAir(false),
  fDALISD(0), fFDC1SD(0),fWindowHoleSD(0),fFDC2SD(0), fICFSD(0), fHODSD(0),
  fNEBULASD(0), fNeuLANDSD(0)
{
  G4cout << "Constructor of TM1510DetectorConstruction" << G4endl;
  fDetectorConstructionMessenger = new TM1510DetectorConstructionMessenger(this);

  fDipoleConstruction = new DipoleConstruction();
  fTargetConstruction = new TargetConstruction();
  fDALIConstruction = new DALIConstruction();
  fFDC1Construction = new FDC1Construction();
  fFDC2Construction = new FDC2Construction();
  fICFConstruction = new ICFConstruction();
  fHODConstruction = new HODSConstruction();
  fExitWindowC2Construction = new ExitWindowC2Construction();
  fExitWindowNConstruction = new ExitWindowNConstruction();
  fNEBULAConstruction = new NEBULAConstruction();
  fNeuLANDConstruction = new NeuLANDConstruction();
  fVacuumUpstreamConstruction = new VacuumUpstreamConstruction();
  fVacuumDownstreamConstruction = new VacuumDownstreamConstruction();
  fWallConstruction = new WallConstruction();
}
//______________________________________________________________________________
TM1510DetectorConstruction::~TM1510DetectorConstruction()
{
  delete fDetectorConstructionMessenger;
  delete fDipoleConstruction;
  delete fTargetConstruction;
  delete fDALIConstruction;
  delete fFDC1Construction;
  delete fFDC2Construction;
  delete fICFConstruction;
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
G4VPhysicalVolume* TM1510DetectorConstruction::Construct()
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

//  //------------------------------ Vacuum upstream of Magnet
//  G4LogicalVolume *vacuum_upstream_log = fVacuumUpstreamConstruction->ConstructSub(Dipole_phys);
//  G4ThreeVector vacuum_upstream_pos = fVacuumUpstreamConstruction->GetPosition();
//  new G4PVPlacement(0,vacuum_upstream_pos,
//		    vacuum_upstream_log,"vacuum_upstream",
//		    experimentalHall_log,false,0);

  //--------------------------------------- Beam Line ---------------------------------------------------
  //------------------------------ Target
  G4LogicalVolume *target_log = fTargetConstruction->ConstructSub();
  G4ThreeVector TargetPos = fTargetConstruction->GetPosition();

  fTarget_phys = new G4PVPlacement(0,TargetPos,
				   target_log,"Target",experimentalHall_log,false,0);

  frag_prm->fTargetPosition.SetXYZ(TargetPos.x()/mm,
				   TargetPos.y()/mm,
				   TargetPos.z()/mm);

  //------------------------------ DALI
  fDALIConstruction->ConstructSub();
  fDALIConstruction->PutDALI(experimentalHall_log);

  // Sensitive Detector
  if (fDALISD==0){
    fDALISD = new DALISD("/DALI");
    SDMan->AddNewDetector(fDALISD);
  }

  if (fDALIConstruction->DoesDALIExist()){
    G4LogicalVolume *sg_log = fDALIConstruction->GetLogicDali2CrystalSG();
    sg_log->SetSensitiveDetector(fDALISD);
    G4LogicalVolume *sc_log = fDALIConstruction->GetLogicDali2CrystalSC();
    sc_log->SetSensitiveDetector(fDALISD);
    G4LogicalVolume *dali1_log = fDALIConstruction->GetLogicDali1Crystal();
    dali1_log->SetSensitiveDetector(fDALISD);
  }

  //------------------------------ FDC1
  G4LogicalVolume *fdc1enc_log = fFDC1Construction->ConstructSub();
  G4ThreeVector FDC1Pos = fFDC1Construction->GetPosition();
  // put FDC1 in the vacuum_upstream. --> input position is relative to vacuum_upstream
  //new G4PVPlacement(0,FDC1Pos - vacuum_upstream_pos,fdc1enc_log,"fdc1enc",vacuum_upstream_log,false,0);
  new G4PVPlacement(0,FDC1Pos,fdc1enc_log,"fdc1enc",experimentalHall_log,false,0);

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

  //------------------------------ ICF
  fICFConstruction->SetAngle(-FDC2Angle);
  G4LogicalVolume *icfenc_log = fICFConstruction->ConstructSub();
  G4double ICFAngle = -fICFConstruction->GetAngle();//SAMURAI def. -> Geant def.
  G4RotationMatrix icfenc_rm; icfenc_rm.rotateY(ICFAngle);
  G4ThreeVector pICF = fICFConstruction->GetPosition();
  pICF.rotateY(ICFAngle);
  new G4PVPlacement(G4Transform3D(icfenc_rm,pICF),
		    icfenc_log,"icfenc",experimentalHall_log,false,0);

  if (fICFSD==0){
    fICFSD = new FragmentSD("/ICF");
    SDMan->AddNewDetector(fICFSD);
  }
  fICFConstruction->GetActiveVolume()->SetSensitiveDetector(fICFSD);

  frag_prm->fICFPosition.SetXYZ(pICF.x()/mm,
				pICF.y()/mm,
				pICF.z()/mm);
  frag_prm->fICFAngle = FDC2Angle/deg;

  //------------------------------ HODS
  fHODConstruction->SetAngle(-FDC2Angle);// geant4 def. -> SAMURAI def.
  G4LogicalVolume *hodenc_log = fHODConstruction->ConstructSub();
  G4double HODAngle = -fHODConstruction->GetAngle();//SAMURAI def. -> Geant def.
  G4RotationMatrix hodenc_rm; hodenc_rm.rotateY(HODAngle);
  G4ThreeVector pHODF = fHODConstruction->GetPosition();
  pHODF.rotateY(HODAngle);
  new G4PVPlacement(G4Transform3D(hodenc_rm,pHODF),
		    hodenc_log,"hodenc",experimentalHall_log,false,0);

  if (fHODSD==0){
    fHODSD = new FragmentSD("/HODS");
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

  //FragmentSD *NeutronWinSD = new FragmentSD("NeutronWindow");
  //SDMan->AddNewDetector(NeutronWinSD);
  //fExitWindowNConstruction->GetWindowVolume()->SetSensitiveDetector(NeutronWinSD);


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



  return experimentalHall_phys;
}
//______________________________________________________________________________
void TM1510DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"TM1510DetectorConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//______________________________________________________________________________
