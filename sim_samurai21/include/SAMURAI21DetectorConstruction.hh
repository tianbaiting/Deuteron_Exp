#ifndef SAMURAI21DetectorConstruction_H
#define SAMURAI21DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class SAMURAI21DetectorConstructionMessenger;
class DipoleConstruction;
class TargetConstruction;
class MINOSTargetChamberConstruction;
class FDC1Construction;
class FDC2Construction;
class HODF24Construction;
class ExitWindowC2Construction;
class ExitWindowNConstruction;
class NEBULAConstruction;
class NeuLANDConstruction;
class VacuumUpstreamConstruction;
class VacuumDownstreamConstruction;
class WallConstruction;

class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class SAMURAI21DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  SAMURAI21DetectorConstruction();
  ~SAMURAI21DetectorConstruction();

  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  void SetFillAir(bool tf){fFillAir = tf;}
               
private:
  bool fFillAir;
    
  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  SAMURAI21DetectorConstructionMessenger *fDetectorConstructionMessenger;

  DipoleConstruction *fDipoleConstruction;
  TargetConstruction *fTargetConstruction;
  MINOSTargetChamberConstruction *fMINOSTargetChamberConstruction;
  FDC1Construction   *fFDC1Construction;
  FDC2Construction   *fFDC2Construction;
  HODF24Construction    *fHODConstruction;
  ExitWindowC2Construction *fExitWindowC2Construction;
  ExitWindowNConstruction *fExitWindowNConstruction;
  NEBULAConstruction *fNEBULAConstruction;
  NeuLANDConstruction *fNeuLANDConstruction;
  VacuumUpstreamConstruction *fVacuumUpstreamConstruction;
  VacuumDownstreamConstruction *fVacuumDownstreamConstruction;
  WallConstruction *fWallConstruction;

  G4ThreeVector fTargetPos;

  G4VSensitiveDetector* fFDC1SD;
  G4VSensitiveDetector* fWindowHoleSD;
  G4VSensitiveDetector* fFDC2SD;
  G4VSensitiveDetector* fHODSD;
  G4VSensitiveDetector* fNeutronWinSD;
  G4VSensitiveDetector* fNEBULASD;
  G4VSensitiveDetector* fNeuLANDSD;

};

#endif

