
#include "HODConstructionMessenger.hh"
#include "HODConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

HODConstructionMessenger::HODConstructionMessenger(HODConstruction* Det)
 : fHODConstruction(Det)
{

  fHODDirectory = new G4UIdirectory("/samurai/geometry/HOD/");
  fHODDirectory->SetGuidance("Modification Commands for HOD");

  fHODPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/HOD/Position",this);
  fHODPosCmd->SetGuidance("Set position of HOD");
  fHODPosCmd->SetGuidance("  0 0 5116.35*mm (default)");
  fHODPosCmd->SetUnitCategory("Length");
  fHODPosCmd->SetParameterName("x","y","z",true);
  fHODPosCmd->SetDefaultValue(G4ThreeVector(0,0,5116.35*mm));
  fHODPosCmd->AvailableForStates(G4State_Idle);

}

HODConstructionMessenger::~HODConstructionMessenger()
{
  delete fHODDirectory;
  delete fHODPosCmd;
}

void HODConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fHODPosCmd ){
    G4ThreeVector pos = fHODPosCmd->GetNew3VectorValue(newValue);
    fHODConstruction->SetPosition(pos);
  }


}


