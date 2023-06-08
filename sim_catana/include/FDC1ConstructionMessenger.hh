#ifndef _FDC1CONSTRUCTIONMESSENGER_
#define _FDC1CONSTRUCTIONMESSENGER_

#include "G4UImessenger.hh"

class FDC1Construction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class FDC1ConstructionMessenger: public G4UImessenger {
public:
  FDC1ConstructionMessenger(FDC1Construction*);
  virtual ~FDC1ConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  FDC1Construction* fFDC1Construction;

  G4UIdirectory* fFDC1Directory;
  G4UIcmdWith3VectorAndUnit* fFDC1PosCmd;
};

#endif

