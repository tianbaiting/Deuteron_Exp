
#include "ICFConstructionMessenger.hh"
#include "ICFConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

ICFConstructionMessenger::ICFConstructionMessenger(ICFConstruction* Det)
 : fICFConstruction(Det)
{

  fICFDirectory = new G4UIdirectory("/samurai/geometry/ICF/");
  fICFDirectory->SetGuidance("Modification Commands for ICF");

  fICFPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/ICF/Position",this);
  fICFPosCmd->SetGuidance("Set position of ICF");
  fICFPosCmd->SetGuidance("  719.36 0 5000*mm (default)");
  fICFPosCmd->SetUnitCategory("Length");
  fICFPosCmd->SetParameterName("x","y","z",true);
  fICFPosCmd->SetDefaultValue(G4ThreeVector(719.36,0,5000*mm));
  fICFPosCmd->AvailableForStates(G4State_Idle);

}

ICFConstructionMessenger::~ICFConstructionMessenger()
{
  delete fICFDirectory;
  delete fICFPosCmd;
}

void ICFConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fICFPosCmd ){
    G4ThreeVector pos = fICFPosCmd->GetNew3VectorValue(newValue);
    fICFConstruction->SetPosition(pos);
  }


}


