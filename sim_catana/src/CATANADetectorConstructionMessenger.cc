#include "CATANADetectorConstructionMessenger.hh"
#include "CATANADetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANADetectorConstructionMessenger::CATANADetectorConstructionMessenger(CATANADetectorConstruction* det)
  : fDetectorConstruction(det)
{
  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Geometry commands for  CATANA");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",
						   this);
  fUpdateGeometryCmd->SetGuidance("Update CATANA geometry");

  fPutSourceCmd = new G4UIcmdWithABool("/samurai/geometry/PutSource",this);
  fPutSourceCmd->SetGuidance("Put Gamma Source or not at target position");
  fPutSourceCmd->SetParameterName("PutSource",true);
  fPutSourceCmd->SetDefaultValue(false);

  fCATANAResolutionCmd =
    new G4UIcmdWithABool("/samurai/geometry/CATANAResolution", this);
  fCATANAResolutionCmd->SetGuidance("Include E/T resolution or not with TArtCATANACsI");
  fCATANAResolutionCmd->SetGuidance("true (default)");
  fCATANAResolutionCmd->SetParameterName("Resolution",true);
  fCATANAResolutionCmd->SetDefaultValue(true);

  /*
  fHECTORResolutionCmd =
    new G4UIcmdWithABool("/samurai/geometry/HECTORResolution", this);
  fHECTORResolutionCmd->SetGuidance("Include E/T resolution or not with TArtHECTORLaBr");
  fHECTORResolutionCmd->SetGuidance("true (default)");
  fHECTORResolutionCmd->SetParameterName("Resolution",true);
  fHECTORResolutionCmd->SetDefaultValue(true);
  */
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANADetectorConstructionMessenger::~CATANADetectorConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
  delete fPutSourceCmd;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CATANADetectorConstructionMessenger::SetNewValue(G4UIcommand* command,
							 G4String newValue)
{
  if(command == fUpdateGeometryCmd){
    fDetectorConstruction->UpdateGeometry();
  }
  else if(command == fPutSourceCmd){
    fDetectorConstruction->
      SetSourceRun(fPutSourceCmd->GetNewBoolValue(newValue));
  }
}

