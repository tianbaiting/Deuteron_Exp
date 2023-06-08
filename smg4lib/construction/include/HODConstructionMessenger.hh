#ifndef HODCONSTRUCTIONMESSENGER_HH
#define HODCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class HODConstruction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class HODConstructionMessenger : public G4UImessenger
{
public:
  HODConstructionMessenger(HODConstruction*);
  virtual ~HODConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  HODConstruction* fHODConstruction;

  G4UIdirectory* fHODDirectory;
  G4UIcmdWith3VectorAndUnit* fHODPosCmd;

};

#endif

