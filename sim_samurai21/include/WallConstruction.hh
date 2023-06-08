#ifndef WallCONSTRUCTION_HH
#define WallCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class WallConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  WallConstruction();
  virtual ~WallConstruction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();

  void SetFacePosition(G4ThreeVector pos){fFacePosition = pos;}
  G4ThreeVector GetFacePosition(){return fFacePosition;}
  G4ThreeVector GetPosition(){fPosition=fFacePosition;
    fPosition.setZ(fPosition.z()+0.5*fWallSize.z());
    return fPosition;}

protected:
  G4Material* fWallMaterial;
  G4ThreeVector fFacePosition;
  G4ThreeVector fPosition;
  G4ThreeVector fWallSize;
};

#endif

