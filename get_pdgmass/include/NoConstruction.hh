#ifndef NOCONSTRUCTION_H
#define NOCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"

class NoConstruction : public G4VUserDetectorConstruction
{
public:
  NoConstruction(){;}
  ~NoConstruction(){;}
  G4VPhysicalVolume* Construct();

};
#endif
