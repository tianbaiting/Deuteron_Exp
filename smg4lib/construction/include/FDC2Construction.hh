#ifndef FDC2CONSTRUCTION_HH
#define FDC2CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class FDC2Construction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  FDC2Construction();
  virtual ~FDC2Construction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetPosition(G4ThreeVector vec){fPosition=vec;G4cout<<"SetPosition"<<fPosition/mm<<"mm"<<G4endl;}
  G4ThreeVector GetPosition(){return fPosition;}

  void SetAngle(G4double val){fAngle = val;}
  G4double GetAngle(){return fAngle;}

  G4LogicalVolume *GetActiveVolume(){return fFDC2Active_log;}

protected:
  G4ThreeVector fPosition;
  G4double fAngle;

  G4Material* fWorldMaterial;
  G4Material* fVacuumMaterial;

  G4LogicalVolume* fFDC2Active_log;

};

#endif

