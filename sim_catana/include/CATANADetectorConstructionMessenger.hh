#ifndef _CATANADETECTORCONSTRUCTIONMESSENGER_
#define _CATANADETECTORCONSTRUCTIONMESSENGER_

#include "G4UImessenger.hh"

class CATANADetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class CATANADetectorConstructionMessenger : public G4UImessenger
{
public:
  CATANADetectorConstructionMessenger(CATANADetectorConstruction*);
  virtual ~CATANADetectorConstructionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

protected:
  CATANADetectorConstruction* fDetectorConstruction;

  G4UIdirectory *fGeometryDirectory;
  G4UIcmdWithoutParameter* fUpdateGeometryCmd;
  G4UIcmdWithABool* fPutSourceCmd;
  G4UIcmdWithABool* fCATANAResolutionCmd;
  G4UIcmdWithABool* fHECTORResolutionCmd;
  
};

#endif
