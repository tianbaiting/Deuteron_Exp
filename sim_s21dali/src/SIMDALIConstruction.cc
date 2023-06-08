#include "SIMDALIConstruction.hh"
#include "SIMDALIConstructionMessenger.hh"
#include "MINOSConstruction.hh"
#include "SourceHolderConstruction.hh"

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
#include "HODF24Construction.hh"
#include "DALIConstruction.hh"
#include "S21DALIConstruction.hh"
#include "ExitWindowC2Construction.hh"
#include "ExitWindowNConstruction.hh"

#include "MagField.hh"
#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "DALISD.hh"

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
SIMDALIConstruction::SIMDALIConstruction() 
  :
  fDALISD(0), fPutGammaSource(false)
{
  G4cout << "Constructor of SIMDALIConstruction" << G4endl;
  fDetectorConstructionMessenger = new SIMDALIConstructionMessenger(this);

  fS21DALIConstruction = new S21DALIConstruction();
  fMINOSConstruction = new MINOSConstruction();
  fSourceHolderConstruction = new SourceHolderConstruction();

}
//______________________________________________________________________________
SIMDALIConstruction::~SIMDALIConstruction()
{
  delete fS21DALIConstruction;
  delete fMINOSConstruction;
  delete fSourceHolderConstruction;
  delete fDetectorConstructionMessenger;
}
//______________________________________________________________________________
G4VPhysicalVolume* SIMDALIConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  G4Material* WorldMaterial = Vacuum;

  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  G4double expHall_x = 3*m;
  G4double expHall_y = 3*m;
  G4double expHall_z = 3*m;

  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x/2,expHall_y/2,expHall_z/2);


  G4LogicalVolume *experimentalHall_log 
    = new G4LogicalVolume(experimentalHall_box,WorldMaterial,"expHall_log");

  G4VPhysicalVolume *experimentalHall_phys 
    = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);

  //--------------------------------
  // DALI
  fS21DALIConstruction->ConstructSub();
  fS21DALIConstruction->PutDALI(experimentalHall_log);
  G4SDManager *SDMan = G4SDManager::GetSDMpointer();
  //MINOS
  fMINOSConstruction->PutMINOS(experimentalHall_log);
  G4cout<<"OK_DALI_MINOS"<<G4endl;
  //SourceHolder
  if (fPutGammaSource)
    fSourceHolderConstruction->PutSourceHolder(experimentalHall_log);

  
  // Sensitive Detector
  if (fDALISD==0){
    fDALISD = new DALISD("/DALI");
    SDMan->AddNewDetector(fDALISD);
  }

  if (fS21DALIConstruction->DoesDALIExist()){
    G4LogicalVolume *sg_log = fS21DALIConstruction->GetLogicDali2CrystalSG();
    sg_log->SetSensitiveDetector(fDALISD);
    G4LogicalVolume *sc_log = fS21DALIConstruction->GetLogicDali2CrystalSC();
    sc_log->SetSensitiveDetector(fDALISD);
    G4LogicalVolume *dali1_log = fS21DALIConstruction->GetLogicDali1Crystal();
    dali1_log->SetSensitiveDetector(fDALISD);
  }
  
  return experimentalHall_phys;
}
//______________________________________________________________________________
void SIMDALIConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  std::cout<<"SIMDALIConstruction : SAMURAI Geometry is updated"<<std::endl;
}
//______________________________________________________________________________
