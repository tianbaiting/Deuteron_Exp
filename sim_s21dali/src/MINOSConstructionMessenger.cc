
#include "MINOSConstructionMessenger.hh"
#include "MINOSConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

//____________________________________________________________________
MINOSConstructionMessenger::MINOSConstructionMessenger(MINOSConstruction* Det)
 : fDetectorConstruction(Det)
{

  fGeometryDirectory = new G4UIdirectory("/minos");
  fGeometryDirectory->SetGuidance("Modification Commands for MINOS");

  fFillLH2Cmd = new G4UIcmdWithABool("/minos/FillTargetLH2",this);
  fFillLH2Cmd->SetGuidance("Fill MINOS Target LH2 or not");
  fFillLH2Cmd->SetGuidance("true (default)");
  fFillLH2Cmd->SetDefaultValue(true);
  fFillLH2Cmd->SetParameterName("FillTargetLH2",true);

  fPutExitCapCmd = new G4UIcmdWithABool("/minos/PutExitCap",this);
  fPutExitCapCmd->SetGuidance("Put MINOS Exit Cap or not");
  fPutExitCapCmd->SetGuidance("false (default)");
  fPutExitCapCmd->SetDefaultValue(false);
  fPutExitCapCmd->SetParameterName("PutExitCap",true);
}
//____________________________________________________________________
MINOSConstructionMessenger::~MINOSConstructionMessenger()
{
  delete fGeometryDirectory;
  delete fFillLH2Cmd;
  delete fPutExitCapCmd;
}
//____________________________________________________________________
void MINOSConstructionMessenger::SetNewValue(G4UIcommand* command, 
							G4String  newValue)
{
  if       ( command == fFillLH2Cmd ){
    fDetectorConstruction->FillTargetLH2(fFillLH2Cmd->GetNewBoolValue(newValue));
  }
  else if  ( command == fPutExitCapCmd ){
    fDetectorConstruction->PutExitCap(fPutExitCapCmd->GetNewBoolValue(newValue));
  }
}
//____________________________________________________________________


