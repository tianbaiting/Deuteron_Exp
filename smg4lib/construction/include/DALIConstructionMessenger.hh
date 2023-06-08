#ifndef DALICONSTRUCTIONMESSENGER_HH
#define DALICONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class DALIConstructionMessenger : public G4UImessenger
{
public:
  DALIConstructionMessenger();
  virtual ~DALIConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  G4UIdirectory* fDALIDirectory;
  G4UIcmdWithAString* fParameterFileNameCmd;
  G4UIcmdWithAString* fDetectorParameterFileNameCmd;
  G4UIcmdWithAString* fResolutionFileNameCmd;
  G4UIcmdWithABool* fDALIResolutionCmd;
  G4UIcmdWithABool* fDALIStoreStepsCmd;

};

#endif

