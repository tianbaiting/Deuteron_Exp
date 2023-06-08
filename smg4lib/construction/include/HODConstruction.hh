#ifndef HODCONSTRUCTION_HH
#define HODCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class HODConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  HODConstruction();
  virtual ~HODConstruction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetPosition(G4ThreeVector vec){fPosition=vec;G4cout<<"SetHODPos"<<fPosition/mm<<"mm"<<G4endl;}
  G4ThreeVector GetPosition(){return fPosition;}

  void SetAngle(G4double val){fAngle = val;}
  G4double GetAngle(){return fAngle;}

  G4LogicalVolume *GetActiveVolume(){return fHODScinti_log;}

protected:
  G4ThreeVector fPosition;
  G4double fAngle;

  G4Material* fPlasticMaterial;
  G4Material* fWorldMaterial;
  G4Material* fVacuumMaterial;

  G4LogicalVolume *fHODScinti_log;

};

#endif

