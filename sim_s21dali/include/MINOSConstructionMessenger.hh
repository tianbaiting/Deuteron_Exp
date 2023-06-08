#ifndef MINOSCONSTRUCTIONMESSENGER_HH
#define MINOSCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class MINOSConstruction;
class G4UIdirectory;
class G4UIcmdWithABool;

class MINOSConstructionMessenger : public G4UImessenger
{
public:
  MINOSConstructionMessenger(MINOSConstruction*);
  virtual ~MINOSConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  MINOSConstruction* fDetectorConstruction;

  G4UIdirectory* fGeometryDirectory;
  G4UIcmdWithABool* fFillLH2Cmd;
  G4UIcmdWithABool* fPutExitCapCmd;

};

#endif

