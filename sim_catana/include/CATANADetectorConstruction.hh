#ifndef _CATANADETECTORCONSTRUCTION_
#define _CATANADETECTORCONSTRUCTION_

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSensitiveDetector;
class CATANADetectorConstructionMessenger;
class TargetConstruction;
class CATANAConstruction;
class HECTORConstruction;
class FDC1Construction;
class FDC1ConstructionMessenger;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class CATANADetectorConstruction : public G4VUserDetectorConstruction
{
public:
  CATANADetectorConstruction();
  ~CATANADetectorConstruction();

  void UpdateGeometry();

  G4VPhysicalVolume *Construct();

  void SetSourceRun(G4bool val){fIsSourceRun = val;}
  G4bool IsSourceRun() const {return fIsSourceRun;}
  
private:
  
  CATANADetectorConstructionMessenger *fMessenger;
  TargetConstruction *fTargetConstruction;
  CATANAConstruction *fCATANAConstruction;
  HECTORConstruction *fHECTORConstruction;
  FDC1Construction *fFDC1Construction;
  FDC1ConstructionMessenger *fFDC1ConstructionMessenger;
  
  G4VSensitiveDetector *fCATANASD;
  G4VSensitiveDetector *fHECTORSD;
  G4VSensitiveDetector *fFDC1SD;
  
  G4bool fIsSourceRun;
};

#endif

