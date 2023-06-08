#ifndef TM1510DetectorCONSTRUCTIONNEBULAMESSENGER_HH
#define TM1510DetectorCONSTRUCTIONNEBULAMESSENGER_HH

#include "G4UImessenger.hh"

class TM1510DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class TM1510DetectorConstructionMessenger : public G4UImessenger
{
public:
  TM1510DetectorConstructionMessenger(TM1510DetectorConstruction*);
  virtual ~TM1510DetectorConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  TM1510DetectorConstruction* fDetectorConstruction;

  G4UIdirectory* fGeometryDirectory;
  G4UIcmdWithoutParameter* fUpdateGeometryCmd;
  G4UIcmdWithABool* fFillAirCmd;

};

#endif

