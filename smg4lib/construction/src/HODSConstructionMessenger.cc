
#include "HODSConstructionMessenger.hh"
#include "HODSConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

HODSConstructionMessenger::HODSConstructionMessenger(HODSConstruction* Det)
 : fHODConstruction(Det)
{

  fHODDirectory = new G4UIdirectory("/samurai/geometry/HODS/");
  fHODDirectory->SetGuidance("Modification Commands for HODS");

  fHODPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/HODS/Position",this);
  fHODPosCmd->SetGuidance("Set position of HODS");
  fHODPosCmd->SetGuidance("  0 0 5500*mm (default)");
  fHODPosCmd->SetUnitCategory("Length");
  fHODPosCmd->SetParameterName("x","y","z",true);
  fHODPosCmd->SetDefaultValue(G4ThreeVector(0,0,5500*mm));
  fHODPosCmd->AvailableForStates(G4State_Idle);

}

HODSConstructionMessenger::~HODSConstructionMessenger()
{
  delete fHODDirectory;
  delete fHODPosCmd;
}

void HODSConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fHODPosCmd ){
    G4ThreeVector pos = fHODPosCmd->GetNew3VectorValue(newValue);
    fHODConstruction->SetPosition(pos);
  }


}


