#ifndef SimtraceDetectorConstruction_H
#define SimtraceDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class SimtraceDetectorConstructionMessenger;
class DipoleConstruction;
class FDC1Construction;
class FDC2Construction;
class HODConstruction;
class ExitWindowC1Construction;
class ExitWindowC2Construction;
class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class SimtraceDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  SimtraceDetectorConstruction();
  ~SimtraceDetectorConstruction();

  void SetTargetPos(G4ThreeVector pos);
  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  const G4VPhysicalVolume* GetTarget()  {return fTarget_phys;};
  const G4ThreeVector GetTargetPos(){return fTargetPos;}
               
private:
    
  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  SimtraceDetectorConstructionMessenger *fDetectorConstructionMessenger;

  DipoleConstruction *fDipoleConstruction;
  FDC1Construction   *fFDC1Construction;
  FDC2Construction   *fFDC2Construction;
  HODConstruction    *fHODConstruction;
  ExitWindowC1Construction *fExitWindowC1Construction;
  ExitWindowC2Construction *fExitWindowC2Construction;

  G4double      fMagAngle;
  G4ThreeVector fTargetPos;

  G4VSensitiveDetector* fWindowHoleSD;
};

#endif

