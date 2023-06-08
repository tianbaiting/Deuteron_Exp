#ifndef SIMDALICONSTRUCTIONNEBULAMESSENGER_HH
#define SIMDALICONSTRUCTIONNEBULAMESSENGER_HH

#include "G4UImessenger.hh"

class SIMDALIConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class SIMDALIConstructionMessenger : public G4UImessenger
{
public:
  SIMDALIConstructionMessenger(SIMDALIConstruction*);
  virtual ~SIMDALIConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  SIMDALIConstruction* fDetectorConstruction;

  G4UIdirectory* fGeometryDirectory;
  G4UIcmdWithoutParameter* fUpdateGeometryCmd;
  G4UIcmdWithABool* fFillAirCmd;

};

#endif

