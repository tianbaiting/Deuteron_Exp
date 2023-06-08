#ifndef NeuLANDCONSTRUCTIONMESSENGER_HH
#define NeuLANDCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class NeuLANDConstructionMessenger : public G4UImessenger
{
public:
  NeuLANDConstructionMessenger();
  virtual ~NeuLANDConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  G4UIdirectory* fNeuLANDDirectory;
  G4UIcmdWithAString* fParameterFileNameCmd;
  G4UIcmdWithAString* fDetectorParameterFileNameCmd;
  G4UIcmdWithABool* fNeuLANDStoreStepsCmd;

};

#endif

