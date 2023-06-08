#ifndef MINOSCONSTRUCTION_HH
#define MINOSCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "DALIMaterialList.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;
class MINOSConstructionMessenger;

class MINOSConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  MINOSConstruction();
  virtual ~MINOSConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume*   PutMINOS(G4LogicalVolume* ExpHall_log);
  virtual G4LogicalVolume*   FillTargetLH2(G4LogicalVolume* ExpHall_log);
  virtual G4LogicalVolume*   PutExitCap(G4LogicalVolume* ExpHall_log);

  virtual void FillTargetLH2(bool tf){fFillTargetLH2 = tf;}
  virtual void PutExitCap(bool tf){fPutExitCap = tf;}

  void SetPosition(G4ThreeVector vec){fPosition=vec;G4cout<<"SetPosition"<<fPosition/mm<<"mm"<<G4endl;}
  G4ThreeVector GetPosition(){return fPosition;}

protected:
  MINOSConstructionMessenger* fMINOSConstructionMessenger;
  G4ThreeVector fPosition;
  G4double    fTargetCell_Radius;
  G4double    fTargetCell_Length;
  bool        fFillTargetLH2;
  bool        fPutExitCap;

  G4Material* fWorldMaterial;
  G4Material* fVacuumMaterial;
};

#endif

