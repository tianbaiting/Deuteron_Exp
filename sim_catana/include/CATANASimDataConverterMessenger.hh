#ifndef CATANASIMDATACONVERTERMESSENGER_
#define CATANASIMDATACONVERTERMESSENGER_

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;

class CATANASimDataConverterMessenger : public G4UImessenger
{
public:
  CATANASimDataConverterMessenger();
  virtual ~CATANASimDataConverterMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  G4UIdirectory *fCATANADirectory;

  G4UIcmdWithABool *fCATANAResolutionCmd;
};

#endif

