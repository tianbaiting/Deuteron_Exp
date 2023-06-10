#ifndef DayoneDetectorConstruction_H
#define DayoneDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class DayoneDetectorConstructionMessenger;
class DipoleConstruction;
class PDCConstruction;
class NEBULAConstruction;

class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"

class DayoneDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DayoneDetectorConstruction();
  ~DayoneDetectorConstruction();

  void SetPDCAngle(G4double angle);
  void SetPDC1Pos(G4ThreeVector pos);
  void SetPDC2Pos(G4ThreeVector pos);
  void SetDumpAngle(G4double angle);
  void SetDumpPos(G4ThreeVector pos);
  void SetTargetAngle(G4double angle);
  void SetTargetPos(G4ThreeVector pos);
  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  const G4VPhysicalVolume* GetTarget()  {return fTarget_phys;};
  const G4ThreeVector GetTargetPos(){return fTargetPos;}

  void SetFillAir(G4bool tf){fFillAir = tf;}

  void SetTarget(G4bool tf){fSetTarget = tf;}
  void SetTargetMat(G4String mat){fTargetMat = mat;}
  void SetTargetSize(G4ThreeVector size){fTargetSize = size;}
               
private:
  G4bool fFillAir;
  G4bool fSetTarget;

  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  DayoneDetectorConstructionMessenger *fDetectorConstructionMessenger;

  DipoleConstruction *fDipoleConstruction;
  PDCConstruction    *fPDCConstruction;
  NEBULAConstruction *fNEBULAConstruction;

  G4String      fTargetMat;
  G4ThreeVector fTargetPos;   // position at laboratory coordinate in mm
  G4ThreeVector fTargetSize;
  G4double      fTargetAngle; // angle (counterclockwise) in rad

  G4double      fPDCAngle;    // angle (clockwise) in rad
  G4ThreeVector fPDC1Pos;     // position at rotated coordinate in mm
  G4ThreeVector fPDC2Pos;     // position at rotated coordinate in mm

  G4double      fDumpAngle;   // angle (clockwise) in rad
  G4ThreeVector fDumpPos;     // position at rotated coordinate in mm

  G4VSensitiveDetector* fTargetSD = 0; // reserved for track reconstruction
  G4VSensitiveDetector* fPDCSD;
  G4VSensitiveDetector* fNEBULASD;

};

#endif

