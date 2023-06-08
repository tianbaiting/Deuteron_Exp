#ifndef SIMDALIConstruction_H
#define SIMDALIConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class SIMDALIConstructionMessenger;
class DALIConstruction;

class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class SIMDALIConstruction : public G4VUserDetectorConstruction
{
public:

  SIMDALIConstruction();
  ~SIMDALIConstruction();

  void UpdateGeometry();

  G4VPhysicalVolume* Construct();
               
private:
    
  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  SIMDALIConstructionMessenger *fDetectorConstructionMessenger;

  DALIConstruction   *fDALIConstruction;

  G4ThreeVector fTargetPos;

  G4VSensitiveDetector* fDALISD;

};

#endif

