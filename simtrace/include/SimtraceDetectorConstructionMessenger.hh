#ifndef SimtraceDetectorCONSTRUCTIONNEBULAMESSENGER_HH
#define SimtraceDetectorCONSTRUCTIONNEBULAMESSENGER_HH

#include "G4UImessenger.hh"

class SimtraceDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;

class SimtraceDetectorConstructionMessenger : public G4UImessenger
{
public:
  SimtraceDetectorConstructionMessenger(SimtraceDetectorConstruction*);
  virtual ~SimtraceDetectorConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  SimtraceDetectorConstruction* fDetectorConstruction;

  G4UIdirectory* fGeometryDirectory;
  G4UIcmdWithoutParameter* fUpdateGeometryCmd;

};

#endif

