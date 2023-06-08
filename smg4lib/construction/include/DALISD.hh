#ifndef DALISD_HH
#define DALISD_HH
 
#include "G4VSensitiveDetector.hh"

class DALISD : public G4VSensitiveDetector
{
public:
  DALISD(G4String name);
  virtual ~DALISD();

public:
  void Initialize(G4HCofThisEvent* HCTE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCTE);
};

#endif
