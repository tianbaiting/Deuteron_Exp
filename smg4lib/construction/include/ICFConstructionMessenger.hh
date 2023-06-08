#ifndef ICFCONSTRUCTIONMESSENGER_HH
#define ICFCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class ICFConstruction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class ICFConstructionMessenger : public G4UImessenger
{
public:
  ICFConstructionMessenger(ICFConstruction*);
  virtual ~ICFConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  ICFConstruction* fICFConstruction;

  G4UIdirectory* fICFDirectory;
  G4UIcmdWith3VectorAndUnit* fICFPosCmd;

};

#endif

