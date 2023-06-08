
#include "FDC2ConstructionMessenger.hh"
#include "FDC2Construction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

FDC2ConstructionMessenger::FDC2ConstructionMessenger(FDC2Construction* Det)
 : fFDC2Construction(Det)
{

  fFDC2Directory = new G4UIdirectory("/samurai/geometry/FDC2/");
  fFDC2Directory->SetGuidance("Modification Commands for FDC2");

  fFDC2PosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/FDC2/Position",this);
  fFDC2PosCmd->SetGuidance("Set position of FDC2");
  fFDC2PosCmd->SetGuidance("  719.36 0 4126.915*mm (default)");
  fFDC2PosCmd->SetUnitCategory("Length");
  fFDC2PosCmd->SetParameterName("x","y","z",true);
  fFDC2PosCmd->SetDefaultValue(G4ThreeVector(719.36,0,4126.915*mm));
  fFDC2PosCmd->AvailableForStates(G4State_Idle);

}

FDC2ConstructionMessenger::~FDC2ConstructionMessenger()
{
  delete fFDC2Directory;
  delete fFDC2PosCmd;
}

void FDC2ConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fFDC2PosCmd ){
    G4ThreeVector pos = fFDC2PosCmd->GetNew3VectorValue(newValue);
    fFDC2Construction->SetPosition(pos);
  }


}


