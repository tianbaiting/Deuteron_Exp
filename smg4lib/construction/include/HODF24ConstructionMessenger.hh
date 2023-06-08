#ifndef HODF24CONSTRUCTIONMESSENGER_HH
#define HODF24CONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class HODF24Construction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class HODF24ConstructionMessenger : public G4UImessenger
{
public:
  HODF24ConstructionMessenger(HODF24Construction*);
  virtual ~HODF24ConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  HODF24Construction* fHODConstruction;

  G4UIdirectory* fHODDirectory;
  G4UIcmdWith3VectorAndUnit* fHODPosCmd;

};

#endif

