#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

#include "SimDataInitializer.hh"
#include "SimDataManager.hh"
#include "CATANASimParameterReader.hh"
#include "CATANASimDataConverter_TArtCATANACsI.hh"

#include "CATANAConstruction.hh"
#include "CATANAConstructionMessenger.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANAConstructionMessenger::CATANAConstructionMessenger()
{
  std::cout << "CATANAConstructionMessenger" << std::endl;

  fCATANADirectory = new G4UIdirectory("/catana/");
  fCATANADirectory->SetGuidance("Control Commands for CATANA");

  fParameterFileNameCmd = new G4UIcmdWithAString("/catana/ParameterFileName",
						 this);
  fParameterFileNameCmd->SetGuidance("Parameter file name for CATANA");
  fParameterFileNameCmd->SetParameterName("ParaCATANA",false);

  fDetectorParameterFileNameCmd = new G4UIcmdWithAString("/catana/DetectorParameterFileName",this);
  fDetectorParameterFileNameCmd->SetGuidance("Parameter file name for CATANA-CsI");
  fDetectorParameterFileNameCmd->SetParameterName("ParaCATANACsI",false);

  fResolutionFileNameCmd = new G4UIcmdWithAString("/catana/ResolutionFileName",
						  this);
  fResolutionFileNameCmd->SetGuidance("Resolution file name for CATANA");
  fResolutionFileNameCmd->SetParameterName("ResoCATANACsI", false);

  fCATANAResolutionCmd = new G4UIcmdWithABool("/catana/Resolution",this);
  fCATANAResolutionCmd->SetGuidance("Include detectorresolution or not");
  fCATANAResolutionCmd->SetGuidance("true (default)");
  fCATANAResolutionCmd->SetDefaultValue(true);
  fCATANAResolutionCmd->SetParameterName("Resolution",true);

  
  fCATANAStoreStepsCmd = new G4UIcmdWithABool("/dali/StoreSteps",this);
  fCATANAStoreStepsCmd->SetGuidance("Store Steps in CATANA or not");
  fCATANAStoreStepsCmd->SetGuidance("false (default)");
  fCATANAStoreStepsCmd->SetDefaultValue(false);
  fCATANAStoreStepsCmd->SetParameterName("StoreSteps",true);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANAConstructionMessenger::~CATANAConstructionMessenger()  
{
  delete fCATANADirectory;
  delete fParameterFileNameCmd;
  delete fDetectorParameterFileNameCmd;
  delete fResolutionFileNameCmd;
  delete fCATANAResolutionCmd;
  delete fCATANAStoreStepsCmd;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CATANAConstructionMessenger::SetNewValue(G4UIcommand* command,
					      G4String newValue)
{
  if( command == fParameterFileNameCmd ){
    CATANASimParameterReader reader;
    reader.ReadCATANAParameters(newValue.data());
  }
  else if( command == fDetectorParameterFileNameCmd ){
    CATANASimParameterReader reader;
    reader.ReadCATANACsIParameters(newValue.data());
  }
  else if( command == fResolutionFileNameCmd ){
    CATANASimParameterReader reader;
    reader.ReadCATANACsIParameters(newValue.data());
  }
  else if( command == fCATANAResolutionCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    CATANASimDataConverter_TArtCATANACsI *converter =
      (CATANASimDataConverter_TArtCATANACsI*)sman->FindConverter("CATANASimDataConverter_TArtCATANACsI");
    if(converter != 0){
      converter->SetIncludeResolution(fCATANAResolutionCmd->
				      GetNewBoolValue(newValue));
    }
    else{
      G4cout << __FILE__
	     << ": CATANASimDataConverter_TArtCATANACsI is not defined, skip"
	     << G4endl;
    }
  }
  else if( command == fCATANAStoreStepsCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    SimDataInitializer *initializer =
      sman->FindInitializer("CATANASimDataInitializer");
    initializer->SetDataStore(fCATANAStoreStepsCmd->GetNewBoolValue(newValue));
  }
}
    
