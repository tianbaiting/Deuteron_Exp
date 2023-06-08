
#include "DayoneDetectorConstructionMessenger.hh"
#include "DayoneDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

DayoneDetectorConstructionMessenger::DayoneDetectorConstructionMessenger(DayoneDetectorConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Modification Commands for Dayone");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",this);
  fUpdateGeometryCmd->SetGuidance("Update SAMURAI Geometry");

  fFillAirCmd = new G4UIcmdWithABool("/samurai/geometry/FillAir",this);
  fFillAirCmd->SetGuidance("Fill Air or Vacuum for Experimental room");
  fFillAirCmd->SetParameterName("FillAir",true);
  fFillAirCmd->SetDefaultValue(false);

  fTargetDirectory = new G4UIdirectory("/samurai/geometry/Target/");
  fTargetDirectory->SetGuidance("Modification Commands for Dayone Target");

  fSetLiTargetCmd = new G4UIcmdWithABool("/samurai/geometry/Target/SetLiTarget",this);
  fSetLiTargetCmd->SetGuidance("Define Li Target");
  fSetLiTargetCmd->SetParameterName("SetLiTarget",true);
  fSetLiTargetCmd->SetDefaultValue(false);

  fTargetSizeCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/Target/Size",this);
  fTargetSizeCmd->SetGuidance("Set size of Target");
  fTargetSizeCmd->SetGuidance("  40 40 20.393*mm (default)");
  fTargetSizeCmd->SetUnitCategory("Length");
  fTargetSizeCmd->SetParameterName("x","y","z",true);
  fTargetSizeCmd->SetDefaultValue(G4ThreeVector(40,40,20.393*mm));
  fTargetSizeCmd->AvailableForStates(G4State_Idle);

}

DayoneDetectorConstructionMessenger::~DayoneDetectorConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
  delete fFillAirCmd;

  delete fTargetDirectory;
  delete fSetLiTargetCmd;
  delete fTargetSizeCmd;
}

void DayoneDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, 
							G4String  newValue)
{
  if       ( command == fUpdateGeometryCmd ){
    fDetectorConstruction->UpdateGeometry();
  }else if ( command == fFillAirCmd ){
    fDetectorConstruction->SetFillAir(fFillAirCmd->GetNewBoolValue(newValue));

  }else if ( command == fSetLiTargetCmd ){
    fDetectorConstruction->SetLiTarget(fFillAirCmd->GetNewBoolValue(newValue));

  }else if ( command == fTargetSizeCmd ){
    G4ThreeVector size = fTargetSizeCmd->GetNew3VectorValue(newValue);
    fDetectorConstruction->SetTargetSize(size);
  }
}


