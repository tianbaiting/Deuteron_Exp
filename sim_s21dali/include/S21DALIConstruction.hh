#ifndef S21DALICONSTRUCTION_HH
#define S21DALICONSTRUCTION_HH

#include "DALIConstruction.hh"

class G4LogicalVolume;
class G4Material;

class S21DALIConstruction : public DALIConstruction
{
public:
  S21DALIConstruction();
  virtual ~S21DALIConstruction();

  G4VPhysicalVolume* Construct();
  G4LogicalVolume* ConstructSub();
  G4LogicalVolume* PutDALI(G4LogicalVolume* ExpHall_log);
  G4LogicalVolume* PutFrame(G4LogicalVolume* ExpHall_log);

protected:
  G4LogicalVolume* fLogicPMTSG;
  G4LogicalVolume* fLogicPMTSC;
  G4LogicalVolume* fLogicPMTDali1;
  G4LogicalVolume* fLogicframe[5]; 

};

#endif

