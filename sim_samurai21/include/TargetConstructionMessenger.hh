#ifndef TargetCONSTRUCTIONMESSENGER_HH
#define TargetCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class TargetConstruction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;

class TargetConstructionMessenger : public G4UImessenger
{
public:
  TargetConstructionMessenger(TargetConstruction*);
  virtual ~TargetConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  TargetConstruction* fTargetConstruction;

  G4UIdirectory* fTargetDirectory;
  G4UIcmdWith3VectorAndUnit* fTargetPosCmd;
  G4UIcmdWithABool* fTargetFillCmd;

};

#endif

