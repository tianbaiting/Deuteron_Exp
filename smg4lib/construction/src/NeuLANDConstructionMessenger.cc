#include "NeuLANDConstructionMessenger.hh"
#include "NeuLANDConstruction.hh"

#include "NeuLANDSimParameterReader.hh"

#include "SimDataManager.hh"
#include "SimDataInitializer.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
//____________________________________________________________________
NeuLANDConstructionMessenger::NeuLANDConstructionMessenger()
{
  std::cout<<"NeuLANDConstructionMessenger"<<std::endl;

  fNeuLANDDirectory = new G4UIdirectory("/samurai/geometry/NeuLAND/");
  fNeuLANDDirectory->SetGuidance("Modification Commands for NeuLAND");

  fParameterFileNameCmd = new G4UIcmdWithAString("/samurai/geometry/NeuLAND/ParameterFileName",this);
  fParameterFileNameCmd->SetGuidance("Parameter file name for NeuLAND.");
  fParameterFileNameCmd->SetParameterName("ParaNeuLAND",false);

  fDetectorParameterFileNameCmd = new G4UIcmdWithAString("/samurai/geometry/NeuLAND/DetectorParameterFileName",this);
  fDetectorParameterFileNameCmd->SetGuidance("Parameter file name for Neutron Detectors.");
  fDetectorParameterFileNameCmd->SetParameterName("ParaNeuLAND",false);

  fNeuLANDStoreStepsCmd = new G4UIcmdWithABool("/action/data/NeuLAND/StoreSteps",this);
  fNeuLANDStoreStepsCmd->SetGuidance("Store Steps in NeuLAND or not");
  fNeuLANDStoreStepsCmd->SetGuidance("false (default)");
  fNeuLANDStoreStepsCmd->SetDefaultValue(false);
  fNeuLANDStoreStepsCmd->SetParameterName("StoreSteps",true);

}
//____________________________________________________________________
NeuLANDConstructionMessenger::~NeuLANDConstructionMessenger()
{
  delete fNeuLANDDirectory;
  delete fParameterFileNameCmd;
  delete fDetectorParameterFileNameCmd;
  delete fNeuLANDStoreStepsCmd;
}
//____________________________________________________________________
void NeuLANDConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fParameterFileNameCmd ){
    NeuLANDSimParameterReader reader;
    reader.ReadNeuLANDParameters(newValue.data());

  }else  if( command == fDetectorParameterFileNameCmd ){
    NeuLANDSimParameterReader reader;
    reader.ReadNeuLANDDetectorParameters(newValue.data());

  }else if( command == fNeuLANDStoreStepsCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    SimDataInitializer *initializer = sman->FindInitializer("NeuLANDSimDataInitializer");
    initializer->SetDataStore(fNeuLANDStoreStepsCmd->GetNewBoolValue(newValue));

  }
}
//____________________________________________________________________

