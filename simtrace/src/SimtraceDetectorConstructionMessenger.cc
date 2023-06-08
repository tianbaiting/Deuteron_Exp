
#include "SimtraceDetectorConstructionMessenger.hh"
#include "SimtraceDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
//#include "G4UIcmdWithAString.hh"
//#include "G4UIcmdWithABool.hh"

SimtraceDetectorConstructionMessenger::SimtraceDetectorConstructionMessenger(SimtraceDetectorConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/samurai/geometry/");
  fGeometryDirectory->SetGuidance("Modification Commands for Simtrace");

  fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/samurai/geometry/Update",this);
  fUpdateGeometryCmd->SetGuidance("Update SAMURAI Geometry");

}

SimtraceDetectorConstructionMessenger::~SimtraceDetectorConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fUpdateGeometryCmd;
}

void SimtraceDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, 
							G4String /* newValue */ )
{
  if( command == fUpdateGeometryCmd ){
    fDetectorConstruction->UpdateGeometry();
  }
}


