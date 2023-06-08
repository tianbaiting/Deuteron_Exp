#ifndef CATANACONSTRUCTION
#define CATANACONSTRUCTION

#include <TString.h>
#include <TObject.h>

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh" // Geant4.10

class G4LogicalVolume;
class G4VSensitiveDetector;
class CATANAConstructionMessenger;
class TCATANASimParameter;

class CATANAConstruction: public G4VUserDetectorConstruction {
public:
  CATANAConstruction();
  virtual ~CATANAConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();
  virtual G4LogicalVolume* PutCATANA(G4LogicalVolume *ExpHall_log);

  G4LogicalVolume *GetLogicType1Crystal() {return fLogicCrystal[0];}
  G4LogicalVolume *GetLogicType2Crystal() {return fLogicCrystal[1];}
  G4LogicalVolume *GetLogicType3Crystal() {return fLogicCrystal[2];}
  G4LogicalVolume *GetLogicType4Crystal() {return fLogicCrystal[3];}
  G4LogicalVolume *GetLogicType5Crystal() {return fLogicCrystal[4];}
  
  G4LogicalVolume *GetLogicPMT1() {return fLogicPMT[0];}
  G4LogicalVolume *GetLogicPMT2() {return fLogicPMT[1];}
  
  G4ThreeVector GetPosition() const {return fPosition;}
  bool DoesCATANAExist() const {return fCATANAExist;}

protected:
  CATANAConstructionMessenger *fCATANAConstructionMessenger;
  TCATANASimParameter *fCATANASimParameter;
  G4ThreeVector fPosition;

  // Type 1~5
  G4LogicalVolume *fLogicCrystal[5]; 
  G4LogicalVolume *fLogicHousing[5];
  G4LogicalVolume *fLogicReflector[5];

  G4LogicalVolume* fLogicPMT[2]; // PMT1: R580, PMT2:R11265
  
  bool fCATANAExist;
  G4VSensitiveDetector *fCATANASD;

  // Define Object to hold the detector geometry data
  class DetectorGeometry : public TObject {
  public:
    DetectorGeometry(){dettype=-1;x1=0;x2=0;x3=0;x4=0;y1=0;y2=0;z=0;
      seal_dt=0;housing_dt=0;reflector_dt=0;crystalz=0;pmttype=-1;}
    ~DetectorGeometry() {;}

    DetectorGeometry& operator=(const DetectorGeometry &right);
    Int_t dettype;
    Double_t x1, x2, x3,x4; // x direction size of housing
    Double_t y1, y2; // y direction size of housing
    Double_t z; // z direction size of housing
    Double_t seal_dt; // thickness of top seal part of the housing
    Double_t housing_dt;
    Double_t reflector_dt;
    Double_t crystalz; // z direction size of the crystal
    Int_t pmttype;
    
  };

  DetectorGeometry fGeometry[5];
  Double_t fPMTlength[2];
};
#endif

