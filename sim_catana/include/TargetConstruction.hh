#ifndef _TARGETCONSTRUCTION_
#define _TARGETCONSTRUCTION_

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4LogicalVolume;
class G4Material;
class DetectorConstruction;
class TargetConstructionMessenger;

class TargetConstruction : public G4VUserDetectorConstruction
{
  friend class DetectorConstruction;
  
public:
  TargetConstruction();
  virtual ~TargetConstruction();

  G4VPhysicalVolume* Construct();
  virtual G4LogicalVolume* ConstructSub();
  virtual G4LogicalVolume* PutTargetChamber(G4LogicalVolume *ExpHall_log);
  virtual G4LogicalVolume* PutTargetHolder(G4LogicalVolume *ExpHall_log);
  virtual G4LogicalVolume* PutReactionTarget(G4LogicalVolume *ExpHall_log);
  virtual G4LogicalVolume* PutCalibrationSource(G4LogicalVolume *ExpHall_log);
  
  G4bool IsReactionTarget() const {return fIsReactionTarget;}
  G4bool IsCalibrationSource() const {return fIsCalibrationSource;}

  void SetTargetMaterialName(G4String name){fTargetMaterialName = name;}
  void SetTargetThickness(G4double val){fThickness = val;}
  G4String GetTargetMaterialName() const {return fTargetMaterialName;}
  G4double GetTargetThickness() const {return fThickness;}
  
  void SetTargetPosition(G4ThreeVector vec){fTargetPosition = vec;}
  G4ThreeVector GetTargetPosition() const {return fTargetPosition;}


  
protected:
  TargetConstructionMessenger *fMessenger;
  G4ThreeVector fTargetPosition;
  G4double fThickness;

  G4LogicalVolume *fLogicTarget;
  G4LogicalVolume *fLogicTargetHolder1;
  G4LogicalVolume *fLogicTargetHolder2;
  G4LogicalVolume *fLogicTargetHolderLeg;
  G4LogicalVolume *fLogicSource;
  G4LogicalVolume *fLogicTargetChamber;
  G4LogicalVolume *fLogicTargetChamberPbCover;
  G4LogicalVolume *fLogicTargetChamberSnCover;
  
  G4bool fIsReactionTarget;
  G4bool fIsCalibrationSource;

  G4String fTargetMaterialName;

};

#endif

