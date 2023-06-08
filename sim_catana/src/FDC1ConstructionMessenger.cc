#include "FDC1ConstructionMessenger.hh"
#include "FDC1Construction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh" // Geant4.10

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
FDC1ConstructionMessenger::FDC1ConstructionMessenger(FDC1Construction* det)
  : fFDC1Construction(det)
{
  fFDC1Directory = new G4UIdirectory("/samurai/geometry/FDC1/");
  fFDC1Directory->SetGuidance("Modification Commands for FDC1");

  fFDC1PosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/FDC1/Position", this);
  fFDC1PosCmd->SetGuidance("Position of FDC1");
  fFDC1PosCmd->SetGuidance("  0 0 -2720.971*mm (defalut=DayOne)");
  fFDC1PosCmd->SetUnitCategory("Length");
  fFDC1PosCmd->SetParameterName("x","y","z",true);
  fFDC1PosCmd->SetDefaultValue(G4ThreeVector(0,0,-2720.971*mm));
  fFDC1PosCmd->AvailableForStates(G4State_Idle);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
FDC1ConstructionMessenger::~FDC1ConstructionMessenger(){
  delete fFDC1Directory;
  delete fFDC1PosCmd;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void FDC1ConstructionMessenger::SetNewValue(G4UIcommand* command,
					    G4String newValue){

  if(command == fFDC1PosCmd){
    G4ThreeVector pos = fFDC1PosCmd->GetNew3VectorValue(newValue);
    fFDC1Construction->SetPosition(pos);
  }
}


  
