#ifndef HODSCONSTRUCTIONMESSENGER_HH
#define HODSCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class HODSConstruction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class HODSConstructionMessenger : public G4UImessenger
{
public:
  HODSConstructionMessenger(HODSConstruction*);
  virtual ~HODSConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  HODSConstruction* fHODConstruction;

  G4UIdirectory* fHODDirectory;
  G4UIcmdWith3VectorAndUnit* fHODPosCmd;

};

#endif

