
#include "TargetConstructionMessenger.hh"
#include "TargetConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh" // Geant4.10

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TargetConstructionMessenger::TargetConstructionMessenger(TargetConstruction* Det)
 : fTargetConstruction(Det)
{

  fTargetDirectory = new G4UIdirectory("/samurai/geometry/Target/");
  fTargetDirectory->SetGuidance("Modification Commands for Target");

  fTargetPosCmd = new G4UIcmdWith3VectorAndUnit("/samurai/geometry/Target/Position",this);
  fTargetPosCmd->SetGuidance("Set position of Target");
  fTargetPosCmd->SetGuidance("  0 0 0*mm (default)");
  fTargetPosCmd->SetUnitCategory("Length");
  fTargetPosCmd->SetParameterName("x","y","z",true);
  fTargetPosCmd->SetDefaultValue(G4ThreeVector(0,0,0));
  fTargetPosCmd->AvailableForStates(G4State_Idle);

  fTargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/samurai/geometry/Target/Thickness",this);
  fTargetThicknessCmd->SetGuidance("Set Target thickness");
  fTargetThicknessCmd->SetParameterName("thickness",false);
  fTargetThicknessCmd->SetRange("thickness>=0");
  fTargetThicknessCmd->SetUnitCategory("Length");
  fTargetThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fTargetMaterialNameCmd = new G4UIcmdWithAString("/samurai/geometry/Target/MaterialName",this);
  fTargetMaterialNameCmd->SetGuidance("Set Target Material Name, Pb or C");
  fTargetMaterialNameCmd->SetParameterName("choise",false);
  fTargetMaterialNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
TargetConstructionMessenger::~TargetConstructionMessenger()
{
  delete fTargetDirectory;
  delete fTargetPosCmd;
  delete fTargetThicknessCmd;
  delete fTargetMaterialNameCmd;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void TargetConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fTargetPosCmd ){
    G4ThreeVector pos = fTargetPosCmd->GetNew3VectorValue(newValue);
    fTargetConstruction->SetTargetPosition(pos);
  }

  if( command == fTargetThicknessCmd ){
    G4double dz = fTargetThicknessCmd->GetNewDoubleValue(newValue);
    fTargetConstruction->SetTargetThickness(dz);
  }

  if( command == fTargetMaterialNameCmd ){
    G4String name = newValue;
    G4String name2 = "G4_Galactic";
    if(name == "Pb") name2 = "G4_Pb";
    if(name == "C") name2 = "G4_C";
    fTargetConstruction->SetTargetMaterialName(name2);
  }


  
}


