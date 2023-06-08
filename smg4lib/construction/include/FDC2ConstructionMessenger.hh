#ifndef FDC2CONSTRUCTIONMESSENGER_HH
#define FDC2CONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"

class FDC2Construction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

class FDC2ConstructionMessenger : public G4UImessenger
{
public:
  FDC2ConstructionMessenger(FDC2Construction*);
  virtual ~FDC2ConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  FDC2Construction* fFDC2Construction;

  G4UIdirectory* fFDC2Directory;
  G4UIcmdWith3VectorAndUnit* fFDC2PosCmd;

};

#endif

