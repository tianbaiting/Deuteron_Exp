#ifndef SAMURAI21DetectorCONSTRUCTIONNEBULAMESSENGER_HH
#define SAMURAI21DetectorCONSTRUCTIONNEBULAMESSENGER_HH

#include "G4UImessenger.hh"

class SAMURAI21DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class SAMURAI21DetectorConstructionMessenger : public G4UImessenger
{
public:
  SAMURAI21DetectorConstructionMessenger(SAMURAI21DetectorConstruction*);
  virtual ~SAMURAI21DetectorConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  SAMURAI21DetectorConstruction* fDetectorConstruction;

  G4UIdirectory* fGeometryDirectory;
  G4UIcmdWithoutParameter* fUpdateGeometryCmd;
  G4UIcmdWithABool* fFillAirCmd;

};

#endif

