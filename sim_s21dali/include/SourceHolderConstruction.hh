#ifndef SourceHolderCONSTRUCTION_HH
#define SourceHolderCONSTRUCTION_HH
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;

class SourceHolderConstruction
{
  friend class DetectorConstruction;
  
public:
  SourceHolderConstruction();
  ~SourceHolderConstruction();
  G4LogicalVolume* PutSourceHolder(G4LogicalVolume* ExpHall_log);


  void SetPosition(G4ThreeVector vec){fPosition=vec;G4cout<<"SetPosition"<<fPosition/mm<<"mm"<<G4endl;}
  G4ThreeVector GetPosition(){return fPosition;}

protected:
  G4ThreeVector fPosition;
  G4Material* fWorldMaterial;
  G4Material* fVacuumMaterial;
};

#endif

