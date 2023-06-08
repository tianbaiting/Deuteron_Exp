#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

#include "SimDataInitializer.hh"
#include "SimDataManager.hh"
#include "CATANASimDataConverter_TArtCATANACsI.hh"

#include "CATANASimDataConverterMessenger.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANASimDataConverterMessenger::CATANASimDataConverterMessenger()
{

  std::cout << "CATANASimDataConverterMessenger" << std::endl;

  fCATANADirectory = new G4UIdirectory("/mycatana/");
  fCATANADirectory->SetGuidance("Control Commands for myCATANA class");

  fCATANAResolutionCmd = new G4UIcmdWithABool("/mycatana/Resolution",this);
  fCATANAResolutionCmd->SetGuidance("Include detectorresolution or not");
  fCATANAResolutionCmd->SetGuidance("true (default)");
  fCATANAResolutionCmd->SetDefaultValue(true);
  fCATANAResolutionCmd->SetParameterName("Resolution",true);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
CATANASimDataConverterMessenger::~CATANASimDataConverterMessenger()
{
  delete fCATANAResolutionCmd;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CATANASimDataConverterMessenger::SetNewValue(G4UIcommand *command,
						  G4String newValue)
{
  if( command == fCATANAResolutionCmd ){
    SimDataManager *sman = SimDataManager::GetSimDataManager();
    CATANASimDataConverter_TArtCATANACsI *converter =
      (CATANASimDataConverter_TArtCATANACsI*)sman
      ->FindConverter("CATANASimDataConverter_TArtCATANACsI");
    if( converter != 0){
      converter->SetIncludeResolution(fCATANAResolutionCmd->
				      GetNewBoolValue(newValue));
    }
    else{
      G4cout << __FILE__
	     << ": CATANASimDataConverter_TArtCATANACsI is not defined. skip"
	     << G4endl;
    }
  }
}
  
