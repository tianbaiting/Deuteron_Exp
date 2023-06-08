#ifndef _CATANACONSTRUCTIONMESSENGER_
#define _CATANACONSTRUCTIONMESSENGER_

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class CATANAConstructionMessenger : public G4UImessenger
{
public:
  CATANAConstructionMessenger();
  virtual ~CATANAConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  G4UIdirectory *fCATANADirectory;

  G4UIcmdWithAString *fParameterFileNameCmd;
  G4UIcmdWithAString *fDetectorParameterFileNameCmd;
  G4UIcmdWithAString *fResolutionFileNameCmd;
  G4UIcmdWithABool *fCATANAResolutionCmd;
  G4UIcmdWithABool *fCATANAStoreStepsCmd;

};

#endif


