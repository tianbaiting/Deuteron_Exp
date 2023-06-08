#ifndef FDC1CONSTRUCTION_HH
#define FDC1CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class FDC1Construction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  FDC1Construction();
  virtual ~FDC1Construction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetPosition(G4ThreeVector vec){fPosition=vec;G4cout<<"SetPosition"<<fPosition/mm<<"mm"<<G4endl;}
  G4ThreeVector GetPosition(){return fPosition;}

  G4LogicalVolume *GetActiveVolume(){return fFDC1Active_log;}

protected:
  G4ThreeVector fPosition;

  G4Material* fWorldMaterial;
  G4Material* fVacuumMaterial;

  G4LogicalVolume* fFDC1Active_log;
};

#endif

