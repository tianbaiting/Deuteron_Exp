#ifndef DayoneDetectorConstruction_H
#define DayoneDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class DayoneDetectorConstructionMessenger;
class DipoleConstruction;
class FDC1Construction;
class FDC2Construction;
class HODConstruction;
class ExitWindowC1Construction;
class ExitWindowNConstruction;
class NEBULAConstruction;
class VacuumUpstreamConstruction;
class VacuumDownstreamConstruction;

class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class DayoneDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DayoneDetectorConstruction();
  ~DayoneDetectorConstruction();

  void SetTargetPos(G4ThreeVector pos);
  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  const G4VPhysicalVolume* GetTarget()  {return fTarget_phys;};
  const G4ThreeVector GetTargetPos(){return fTargetPos;}

  void SetFillAir(bool tf){fFillAir = tf;}

  void SetLiTarget(bool tf){fUseLiTarget = tf;}
  void SetTargetSize(G4ThreeVector size){fTargetSize = size;}
               
private:
  bool fFillAir;
  bool fUseLiTarget;

  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  DayoneDetectorConstructionMessenger *fDetectorConstructionMessenger;

  DipoleConstruction *fDipoleConstruction;
  FDC1Construction   *fFDC1Construction;
  FDC2Construction   *fFDC2Construction;
  HODConstruction    *fHODConstruction;
  ExitWindowC1Construction *fExitWindowC1Construction;
  ExitWindowNConstruction *fExitWindowNConstruction;
  NEBULAConstruction *fNEBULAConstruction;
  VacuumUpstreamConstruction *fVacuumUpstreamConstruction;
  VacuumDownstreamConstruction *fVacuumDownstreamConstruction;

  G4ThreeVector fTargetPos;
  G4ThreeVector fTargetSize;

  G4VSensitiveDetector* fFDC1SD;
  G4VSensitiveDetector* fWindowHoleSD;
  G4VSensitiveDetector* fFDC2SD;
  G4VSensitiveDetector* fHODSD;
  G4VSensitiveDetector* fNEBULASD;

};

#endif

