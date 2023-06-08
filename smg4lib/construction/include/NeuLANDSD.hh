#ifndef NeuLANDSD_HH
#define NeuLANDSD_HH
 
#include "G4VSensitiveDetector.hh"

class NeuLANDSD : public G4VSensitiveDetector
{
public:
  NeuLANDSD(G4String name);
  virtual ~NeuLANDSD();

public:
  void Initialize(G4HCofThisEvent* HCTE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCTE);
};

#endif
