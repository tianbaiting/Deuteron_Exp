#ifndef TM1510DetectorConstruction_H
#define TM1510DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class TM1510DetectorConstructionMessenger;
class DipoleConstruction;
class TargetConstruction;
class DALIConstruction;
class FDC1Construction;
class FDC2Construction;
class HODSConstruction;
class ICFConstruction;
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

class TM1510DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  TM1510DetectorConstruction();
  ~TM1510DetectorConstruction();

  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  void SetFillAir(bool tf){fFillAir = tf;}
               
private:
  bool fFillAir;
    
  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  TM1510DetectorConstructionMessenger *fDetectorConstructionMessenger;

  DipoleConstruction *fDipoleConstruction;
  TargetConstruction *fTargetConstruction;
  DALIConstruction *fDALIConstruction;
  FDC1Construction   *fFDC1Construction;
  FDC2Construction   *fFDC2Construction;
  ICFConstruction    *fICFConstruction;
  HODSConstruction    *fHODConstruction;
  ExitWindowC2Construction *fExitWindowC2Construction;
  ExitWindowNConstruction *fExitWindowNConstruction;
  NEBULAConstruction *fNEBULAConstruction;
  NeuLANDConstruction *fNeuLANDConstruction;
  VacuumUpstreamConstruction *fVacuumUpstreamConstruction;
  VacuumDownstreamConstruction *fVacuumDownstreamConstruction;
  WallConstruction *fWallConstruction;

  G4ThreeVector fTargetPos;

  G4VSensitiveDetector* fDALISD;
  G4VSensitiveDetector* fFDC1SD;
  G4VSensitiveDetector* fWindowHoleSD;
  G4VSensitiveDetector* fFDC2SD;
  G4VSensitiveDetector* fICFSD;
  G4VSensitiveDetector* fHODSD;
  G4VSensitiveDetector* fNEBULASD;
  G4VSensitiveDetector* fNeuLANDSD;

};

#endif

