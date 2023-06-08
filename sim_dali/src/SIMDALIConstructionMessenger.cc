
#include "SIMDALIConstructionMessenger.hh"
#include "SIMDALIConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

SIMDALIConstructionMessenger::SIMDALIConstructionMessenger(SIMDALIConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Modification Commands for SIMDALI");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",this);
  fUpdateGeometryCmd->SetGuidance("Update SAMURAI Geometry");

}

SIMDALIConstructionMessenger::~SIMDALIConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
}

void SIMDALIConstructionMessenger::SetNewValue(G4UIcommand* command, 
							G4String  newValue)
{
  if       ( command == fUpdateGeometryCmd ){
    fDetectorConstruction->UpdateGeometry();
  }
}


