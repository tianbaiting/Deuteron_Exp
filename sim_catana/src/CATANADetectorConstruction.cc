#include "CATANADetectorConstruction.hh"
#include "CATANADetectorConstructionMessenger.hh"

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
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "SimDataManager.hh"

#include "TargetConstruction.hh"
#include "TargetConstructionMessenger.hh"
#include "CATANAConstruction.hh"
#include "FDC1Construction.hh"
#include "FDC1ConstructionMessenger.hh"
#include "CATANASD.hh"
#include "FragmentSD.hh"
#include "TFragSimParameter.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANADetectorConstruction::CATANADetectorConstruction()
  : fCATANASD(0), fFDC1SD(0), fIsSourceRun(false)
{
  G4cout << __FILE__ << "Constructor" << G4endl;

  fMessenger = new CATANADetectorConstructionMessenger(this);

  fTargetConstruction = new TargetConstruction();
  fCATANAConstruction = new CATANAConstruction();
  fFDC1Construction = new FDC1Construction();

  fFDC1ConstructionMessenger =
    new FDC1ConstructionMessenger(fFDC1Construction);
  
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANADetectorConstruction::~CATANADetectorConstruction()
{
  delete fMessenger;
  delete fTargetConstruction;
  delete fCATANAConstruction;
  delete fFDC1Construction;
  delete fFDC1ConstructionMessenger;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4VPhysicalVolume* CATANADetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Define world
  G4Material *Vacuum = G4NistManager::Instance()->
    FindOrBuildMaterial("G4_Galactic");

  G4ThreeVector expHall_size(4.*m, 4.*m, 12.*m);
  
  G4Box* expHall_box = new G4Box("expHall_box",
				 expHall_size.x()/2,
				 expHall_size.y()/2,
				 expHall_size.z()/2);
  
  G4LogicalVolume *expHall_log
    = new G4LogicalVolume(expHall_box,Vacuum,"expHall_log");
  
  G4VPhysicalVolume *expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(0,0,0),expHall_log,
			"expHall",0,false,0);

  G4SDManager *SDMan = G4SDManager::GetSDMpointer();
  // CATANA --------
  fCATANAConstruction->ConstructSub();
  fCATANAConstruction->PutCATANA(expHall_log);

  std::cout << "-=-=-=-=-=-=-=-=- END OF PUTCATANA -=-=-=-=-=-=-=-=-=-" << std::endl;
  
  if(fCATANASD == 0){
    fCATANASD = new CATANASD("/CATANA");
    SDMan->AddNewDetector(fCATANASD);
    std::cout << "fCATANASD==0 kita-!" << std::endl;
  }

  std::cout << "DoesCATANAExist: " << fCATANAConstruction->DoesCATANAExist()
	    << std::endl;
  
  if(fCATANAConstruction->DoesCATANAExist()){
    G4LogicalVolume *logType1 = fCATANAConstruction->GetLogicType1Crystal();
    G4LogicalVolume *logType2 = fCATANAConstruction->GetLogicType2Crystal();
    G4LogicalVolume *logType3 = fCATANAConstruction->GetLogicType3Crystal();
    logType1->SetSensitiveDetector(fCATANASD);
    logType2->SetSensitiveDetector(fCATANASD);
    logType3->SetSensitiveDetector(fCATANASD);
    std::cout << "DoesCATANAExist kita-!" << std::endl;
  }
  
  // FDC1 ----------
  SimDataManager *simman = SimDataManager::GetSimDataManager();
  TFragSimParameter *frag_prm = (TFragSimParameter*)simman->
    FindParameter("FragParameter");
  if(frag_prm == 0){
    frag_prm = new TFragSimParameter("FragParameter");
    simman->AddParameter(frag_prm);
  }

  G4LogicalVolume* fdc1log = fFDC1Construction->ConstructSub();
  G4ThreeVector FDC1Pos = fFDC1Construction->GetPosition();
  new G4PVPlacement(0,FDC1Pos,fdc1log,"fdc1enc",expHall_log,false,0);
  
  frag_prm->fFDC1Position.SetXYZ(FDC1Pos.x()/mm,
  				 FDC1Pos.y()/mm,
  				 FDC1Pos.z()/mm);
  frag_prm->fFDC1Angle = 0;

  if(fFDC1SD == 0){
    fFDC1SD = new FragmentSD("/FDC1");
    SDMan->AddNewDetector(fFDC1SD);
  }
  
  fFDC1Construction->GetActiveVolume()->SetSensitiveDetector(fFDC1SD);
  
  // Target ----------
  fTargetConstruction->ConstructSub();
  fTargetConstruction->PutTargetChamber(expHall_log);
  fTargetConstruction->PutTargetHolder(expHall_log);
  if(fIsSourceRun) fTargetConstruction->PutCalibrationSource(expHall_log);
  else fTargetConstruction->PutReactionTarget(expHall_log);

  return expHall_phys;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CATANADetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}
