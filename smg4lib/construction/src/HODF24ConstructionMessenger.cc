
#include "HODF24ConstructionMessenger.hh"
#include "HODF24Construction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

HODF24ConstructionMessenger::HODF24ConstructionMessenger(HODF24Construction* Det)
 : fHODConstruction(Det)
{

  fHODDirectory = new G4UIdirectory("/samurai/geometry/HODF24/");
  fHODDirectory->SetGuidance("Modification Commands for HODF24");

  fHODPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/HODF24/Position",this);
  fHODPosCmd->SetGuidance("Set position of HODF24");
  fHODPosCmd->SetGuidance("  0 0 5116.35*mm (default)");
  fHODPosCmd->SetUnitCategory("Length");
  fHODPosCmd->SetParameterName("x","y","z",true);
  fHODPosCmd->SetDefaultValue(G4ThreeVector(0,0,5116.35*mm));
  fHODPosCmd->AvailableForStates(G4State_Idle);

}

HODF24ConstructionMessenger::~HODF24ConstructionMessenger()
{
  delete fHODDirectory;
  delete fHODPosCmd;
}

void HODF24ConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fHODPosCmd ){
    G4ThreeVector pos = fHODPosCmd->GetNew3VectorValue(newValue);
    fHODConstruction->SetPosition(pos);
  }


}


