
#include "SIMDALIConstructionMessenger.hh"
#include "SIMDALIConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

//____________________________________________________________________
SIMDALIConstructionMessenger::SIMDALIConstructionMessenger(SIMDALIConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Modification Commands for SIMDALI");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",this);
  fUpdateGeometryCmd->SetGuidance("Update SAMURAI Geometry");

  fPutGammaSourceCmd = new G4UIcmdWithABool("/samurai/geometry/PutGammaSource",this);
  fPutGammaSourceCmd->SetGuidance("Put Gamma Source");
  fPutGammaSourceCmd->SetGuidance("false (default)");
  fPutGammaSourceCmd->SetDefaultValue(false);
  fPutGammaSourceCmd->SetParameterName("PutGammaSource",true);
}
//____________________________________________________________________
SIMDALIConstructionMessenger::~SIMDALIConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
}
//____________________________________________________________________
void SIMDALIConstructionMessenger::SetNewValue(G4UIcommand* command, 
					       G4String  newValue)
{
  if       ( command == fUpdateGeometryCmd ){
    fDetectorConstruction->UpdateGeometry();
  }
  else if   ( command == fPutGammaSourceCmd ){
    fDetectorConstruction->PutGammaSource(fPutGammaSourceCmd->GetNewBoolValue(newValue));
  }
}
//____________________________________________________________________


