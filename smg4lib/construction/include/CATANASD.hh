#ifndef _CATANASD_
#define _CATANASD_

#include "G4VSensitiveDetector.hh"

class CATANASD : public G4VSensitiveDetector
{
public:
  CATANASD(G4String name);
  virtual ~CATANASD();

public:
  void Initialize(G4HCofThisEvent* HCTE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCTE);

};

#endif

