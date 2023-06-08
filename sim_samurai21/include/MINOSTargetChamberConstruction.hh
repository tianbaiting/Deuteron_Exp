#ifndef MINOSTargetChamberCONSTRUCTION_HH
#define MINOSTargetChamberCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class MINOSTargetChamberConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  MINOSTargetChamberConstruction();
  virtual ~MINOSTargetChamberConstruction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetPosition(G4ThreeVector pos){fPosition = pos;}
  G4ThreeVector GetPosition(){return fPosition;}

protected:
  G4Material* fDuctMaterial;
  G4ThreeVector fPosition;
};

#endif

