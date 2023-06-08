#ifndef TargetCONSTRUCTION_HH
#define TargetCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class TargetConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  TargetConstruction();
  virtual ~TargetConstruction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetPosition(G4ThreeVector pos){fPosition = pos;}
  G4ThreeVector GetPosition(){return fPosition;}
  G4double GetThickness(){return fTargetThickness;}
  void SetFill(bool tf);

protected:
  G4Material* fTargetMaterial;
  G4ThreeVector fPosition;
  G4double fTargetThickness;
  bool fTargetFill;
};

#endif

