#ifndef DALICONSTRUCTION_HH
#define DALICONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include <TString.h>
#include <vector>
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class G4VSensitiveDetector;
class DetectorConstruction;
class DALIConstructionMessenger;
class TDALISimParameter;


class DALIConstruction : public G4VUserDetectorConstruction
{
public:
  DALIConstruction();
  virtual ~DALIConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();
  virtual G4LogicalVolume* PutDALI(G4LogicalVolume* ExpHall_log);
  G4LogicalVolume* GetLogicDali2CrystalSG(){return fLogicDali2CrystalSG;}
  G4LogicalVolume* GetLogicDali2CrystalSC(){return fLogicDali2CrystalSC;}
  G4LogicalVolume* GetLogicDali1Crystal(){return fLogicDali1Crystal;}

  G4ThreeVector GetPosition(){return fPosition;}

  bool DoesDALIExist(){return fDALIExist;}

protected:
  DALIConstructionMessenger* fDALIConstructionMessenger;

  TDALISimParameter *fDALISimParameter;

  G4ThreeVector fPosition;

  G4LogicalVolume* fLogicDali2CrystalSG;
  G4LogicalVolume* fLogicDali2CrystalSC;
  G4LogicalVolume* fLogicDali1Crystal; 
  G4LogicalVolume* fLogicDali2AlHouseSG;
  G4LogicalVolume* fLogicDali2AlHouseSC;
  G4LogicalVolume* fLogicDali1AlHouse;
  G4LogicalVolume* fLogicDali2MgOCoatSG;
  G4LogicalVolume* fLogicDali2MgOCoatSC;
  G4LogicalVolume* fLogicDali1MgOCoat;

  G4Material* fVacuumMaterial;
  G4Material* fMgOMaterial;
  G4Material* fAlMaterial;
  G4Material* fNaIMaterial;

  bool fDALIExist;
  G4VSensitiveDetector* fDALISD;


};

#endif

