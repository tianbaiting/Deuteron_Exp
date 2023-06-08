#ifndef SIMDALIConstruction_H
#define SIMDALIConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class SIMDALIConstructionMessenger;
class S21DALIConstruction;

class G4VSensitiveDetector;

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "MINOSConstruction.hh"
#include "SourceHolderConstruction.hh"


class SIMDALIConstruction : public G4VUserDetectorConstruction
{
public:

  SIMDALIConstruction();
  ~SIMDALIConstruction();

  void UpdateGeometry();

  G4VPhysicalVolume* Construct();

  void PutGammaSource(bool tf){fPutGammaSource = tf;}
               
private:
    
  // Physical volumes
  G4VPhysicalVolume* fTarget_phys;

  SIMDALIConstructionMessenger *fDetectorConstructionMessenger;

  S21DALIConstruction   *fS21DALIConstruction;

  MINOSConstruction   *fMINOSConstruction;

  SourceHolderConstruction *fSourceHolderConstruction;

  G4ThreeVector fTargetPos;

  G4VSensitiveDetector* fDALISD;

  bool fPutGammaSource;

};

#endif

