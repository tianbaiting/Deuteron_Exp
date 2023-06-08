
#include "TargetConstructionMessenger.hh"
#include "TargetConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

TargetConstructionMessenger::TargetConstructionMessenger(TargetConstruction* Det)
 : fTargetConstruction(Det)
{

  fTargetDirectory = new G4UIdirectory("/samurai/geometry/Target/");
  fTargetDirectory->SetGuidance("Modification Commands for Target");

  fTargetPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/Target/Position",this);
  fTargetPosCmd->SetGuidance("Set position of Target");
  fTargetPosCmd->SetGuidance("  0 0 -4500*mm (default)");
  fTargetPosCmd->SetUnitCategory("Length");
  fTargetPosCmd->SetParameterName("x","y","z",true);
  fTargetPosCmd->SetDefaultValue(G4ThreeVector(0,0,-4500*mm));
  fTargetPosCmd->AvailableForStates(G4State_Idle);

}

TargetConstructionMessenger::~TargetConstructionMessenger()
{
  delete fTargetDirectory;
  delete fTargetPosCmd;
}

void TargetConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fTargetPosCmd ){
    G4ThreeVector pos = fTargetPosCmd->GetNew3VectorValue(newValue);
    fTargetConstruction->SetPosition(pos);
  }


}


