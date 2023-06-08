
#include "SAMURAI21DetectorConstructionMessenger.hh"
#include "SAMURAI21DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

SAMURAI21DetectorConstructionMessenger::SAMURAI21DetectorConstructionMessenger(SAMURAI21DetectorConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Modification Commands for SAMURAI21");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",this);
  fUpdateGeometryCmd->SetGuidance("Update SAMURAI Geometry");

  fFillAirCmd = new G4UIcmdWithABool("/samurai/geometry/FillAir",this);
  fFillAirCmd->SetGuidance("Fill Air or Vacuum for Experimental room");
  fFillAirCmd->SetParameterName("FillAir",true);
  fFillAirCmd->SetDefaultValue(false);
}

SAMURAI21DetectorConstructionMessenger::~SAMURAI21DetectorConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
  delete fFillAirCmd;
}

void SAMURAI21DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, 
							G4String  newValue)
{
  if       ( command == fUpdateGeometryCmd ){
    fDetectorConstruction->UpdateGeometry();
  }else if ( command == fFillAirCmd ){
    fDetectorConstruction->SetFillAir(fFillAirCmd->GetNewBoolValue(newValue));
  }
}


