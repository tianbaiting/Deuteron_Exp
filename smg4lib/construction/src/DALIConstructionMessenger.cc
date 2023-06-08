#include "DALIConstructionMessenger.hh"
#include "DALIConstruction.hh"

#include "DALISimParameterReader.hh"
#include "SimDataManager.hh"
#include "SimDataInitializer.hh"
#include "DALISimDataConverter_TArtDALINaI.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
//____________________________________________________________________
DALIConstructionMessenger::DALIConstructionMessenger()
{
  std::cout<<"DALIConstructionMessenger"<<std::endl;

  fDALIDirectory = new G4UIdirectory("/dali/");
  fDALIDirectory->SetGuidance("Modification Commands for DALI");

  fParameterFileNameCmd = new G4UIcmdWithAString("/dali/ParameterFileName",this);
  fParameterFileNameCmd->SetGuidance("Parameter file name for DALI.");
  fParameterFileNameCmd->SetParameterName("ParaDALI",false);

  fDetectorParameterFileNameCmd = new G4UIcmdWithAString("/dali/DetectorParameterFileName",this);
  fDetectorParameterFileNameCmd->SetGuidance("DetectorParameter file name for DALI.");
  fDetectorParameterFileNameCmd->SetParameterName("ParaDALIDetector",false);

  fResolutionFileNameCmd = new G4UIcmdWithAString("/dali/ResolutionFileName",this);
  fResolutionFileNameCmd->SetGuidance("Resolution file name for DALI.");
  fResolutionFileNameCmd->SetParameterName("ResolutionDALI",false);

  fDALIResolutionCmd = new G4UIcmdWithABool("/dali/Resolution",this);
  fDALIResolutionCmd->SetGuidance("Convolute DALI data by resolution or not");
  fDALIResolutionCmd->SetGuidance("true (default)");
  fDALIResolutionCmd->SetDefaultValue(true);
  fDALIResolutionCmd->SetParameterName("Resolution",true);

  fDALIStoreStepsCmd = new G4UIcmdWithABool("/dali/StoreSteps",this);
  fDALIStoreStepsCmd->SetGuidance("Store Steps in DALI or not");
  fDALIStoreStepsCmd->SetGuidance("false (default)");
  fDALIStoreStepsCmd->SetDefaultValue(false);
  fDALIStoreStepsCmd->SetParameterName("StoreSteps",true);

}
//____________________________________________________________________
DALIConstructionMessenger::~DALIConstructionMessenger()
{
  delete fDALIDirectory;
  delete fParameterFileNameCmd;
  delete fDetectorParameterFileNameCmd;
  delete fResolutionFileNameCmd;
  delete fDALIResolutionCmd;
  delete fDALIStoreStepsCmd;
}
//____________________________________________________________________
void DALIConstructionMessenger::SetNewValue(G4UIcommand* command, G4String  newValue )
{
  if( command == fParameterFileNameCmd ){
    DALISimParameterReader reader;
    reader.ReadDALIParameters(newValue.data());

  }else  if( command == fDetectorParameterFileNameCmd ){
    DALISimParameterReader reader;
    reader.ReadDALINaIParameters(newValue.data());

  }else  if( command == fResolutionFileNameCmd ){
    DALISimParameterReader reader;
    reader.ReadDALINaIParameters(newValue.data());

  }else  if( command == fResolutionFileNameCmd ){
    // read resolution file

  }else  if( command == fDALIResolutionCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    DALISimDataConverter_TArtDALINaI *converter 
      = (DALISimDataConverter_TArtDALINaI*)sman->FindConverter("DALISimDataConverter_TArtDALINaI");
    if (converter!=0){
      converter->SetIncludeResolution(fDALIResolutionCmd->GetNewBoolValue(newValue));
    }else{
      G4cout<<__FILE__
	    <<": DALISimDataConverter_TArtDALINaI is not defined, skip"<<G4endl;
    }

  }else if( command == fDALIStoreStepsCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    SimDataInitializer *initializer = sman->FindInitializer("DALISimDataInitializer");
    initializer->SetDataStore(fDALIStoreStepsCmd->GetNewBoolValue(newValue));

  }
}
//____________________________________________________________________

