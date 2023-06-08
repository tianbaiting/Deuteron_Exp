#include "TargetConstruction.hh"
#include "TargetConstructionMessenger.hh"

#include <TVector3.h>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh" // Geant4.10
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "G4NistManager.hh"
//______________________________________________________________________________________
TargetConstruction::TargetConstruction()
  : fTargetPosition(0,0,0), // Fujino value
    fThickness(1.5*mm), fIsReactionTarget(false), fIsCalibrationSource(false),
    fTargetMaterialName("G4_Pb")
{

  fMessenger = new TargetConstructionMessenger(this);
}
//______________________________________________________________________________________
TargetConstruction::~TargetConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* TargetConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //---------------------------------------------------
  // World LV
  G4ThreeVector worldSize(5*m,5*m,5*m);
  G4Box* solidWorld = new G4Box("World",
				worldSize.x()/2,
				worldSize.y()/2,
				worldSize.z()/2);

  G4Material *Vacuum = G4NistManager::Instance()->
    FindOrBuildMaterial("G4_Galactic");
  
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,
						    Vacuum,
						    "World");
  G4ThreeVector worldPos(0, 0, 0);  
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* TargetConstruction::ConstructSub()
{
  std::cout << "Creating Target" << std::endl;

  /* Target -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  G4Material *targetMaterial = G4NistManager::Instance()->
    FindOrBuildMaterial(fTargetMaterialName);

  G4cout << "Target Material: " << targetMaterial->GetName() << G4endl;
  
  G4double rmin = 0.;
  G4double rmax = 85./2.*mm;
  G4double dz = fThickness * 0.5;
  G4double sphi = 0.; // starting phi
  G4double ephi = 360.*deg;
  G4Tubs *target_tube = new G4Tubs("target_tube", rmin, rmax, dz, sphi, ephi);

  fLogicTarget = new G4LogicalVolume(target_tube, targetMaterial,
				     "fLogicTarget");
  fLogicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Cyan()));

  /* Calibration source -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  /* Type 401 from JRIAS  */
  /* https://www.jrias.or.jp/products/pdf/RI-catalog-03-20190401.pdf */

  G4double A = 1.00794 *g/mole;
  G4Element *elH = new G4Element("Hydrogen", "H", 1, A);
  A = 12.011 *g/mole;
  G4Element *elC = new G4Element("Carbon", "C",6, A);
  A = 15.9994 *g/mole;
  G4Element *elO = new G4Element("Oxygen", "O",8, A);

  G4double density = 1.19*g/cm3;
  G4Material *Acrylic = new G4Material("Acrylic", density, 3);
  Acrylic->AddElement(elC,5);
  Acrylic->AddElement(elH,8);
  Acrylic->AddElement(elO,2);

  rmin = 0.;
  rmax = 25./2. *mm;
  dz = 6/2 *mm;
  sphi = 0.; // starting phi
  ephi = 360.*deg;
  G4Tubs *source_tube = new G4Tubs("source_tube", rmin, rmax, dz, sphi, ephi);
  fLogicSource = new G4LogicalVolume(source_tube, Acrylic, "fLogicSource");

  /* Target Chamber -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  /* Made from Al, thickness = 3.2 mm */
  G4Material *Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  rmin = 158.6/2. *mm;
  rmax = 165/2. *mm;
  dz = 720/2. *mm;
  sphi = 0.; // starting phi
  ephi = 360.*deg;
  G4Tubs *chamber_tube = new G4Tubs("chamber_tube",rmin,rmax,dz,sphi,ephi);
  fLogicTargetChamber = new G4LogicalVolume(chamber_tube, Al, "fLogicTargetChamber");

  /* Pb cover on the Al target : thickness is 1mm */
  G4Material *Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
  rmin = 167./2.; // 
  rmax = 169./2.; // thickness is 1mm
  G4Tubs *pbcover_tube = new G4Tubs("pbcover_tube",rmin,rmax,dz,sphi,ephi);
  fLogicTargetChamberPbCover = new G4LogicalVolume(pbcover_tube, Pb, "fLogicTargetChamberPbCover");

  /* Sn cover on the Pb cover : thickness is 1mm */
  G4Material *Sn = G4NistManager::Instance()->FindOrBuildMaterial("G4_Sn");
  rmin = 171./2.; // 
  rmax = 173./2.; // thickness is 1mm
  G4Tubs *sncover_tube = new G4Tubs("sncover_tube",rmin,rmax,dz,sphi,ephi);
  fLogicTargetChamberSnCover = new G4LogicalVolume(sncover_tube, Sn, "fLogicTargetChamberSnCover");
  
  /* Target Holder -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--= */
  /* Made from Acrylic */
  /* Holder1: 3mm thick holder */
  rmin = 80./2. *mm;
  rmax = 100./2. *mm;
  dz = 3./2. *mm;
  sphi = 0.; // starting phi
  ephi = 360.*deg;
  G4Tubs *holder1center = new G4Tubs("holder1center",rmin,rmax,dz,sphi,ephi);

  rmin = 100./2. *mm;
  rmax = 74.3 *mm;
  dz = 3./2. *mm;
  sphi = 220.*deg; // starting phi
  ephi = 100.*deg;
  G4Tubs *holder1lower = new G4Tubs("holder1lower",rmin,rmax,dz,sphi,ephi);

  G4UnionSolid *holder1union = new G4UnionSolid("union1",
						holder1center,
						holder1lower);

  // Leg
  G4ThreeVector legSize(3*mm, 25*mm, 100*mm);
  G4Box *solidLeg = new G4Box("Leg",
			      legSize.x()/2.,
			      legSize.y()/2.,
			      legSize.z()/2.);

    fLogicTargetHolderLeg =
      new G4LogicalVolume(solidLeg, Acrylic, "fLogicTargetHolderLeg");


    TVector3 legpos(0,-(79.3-25./2),0);
    legpos.RotateZ(-40 * M_PI/180.);
    G4ThreeVector shift1(legpos.X(), legpos.Y(), legpos.Z());

    G4RotationMatrix *rotmat = new G4RotationMatrix;
    rotmat->rotateZ(40*degree);

    G4SubtractionSolid *holder1sub1 = new G4SubtractionSolid("h1sub1",
							     holder1union,
							     solidLeg,
							     rotmat,
							     shift1);
    legpos.RotateZ(80 * M_PI/180.);
    G4ThreeVector shift2(legpos.X(), legpos.Y(), legpos.Z());
    rotmat->rotateZ(-80*degree);
    G4SubtractionSolid *holder1 = new G4SubtractionSolid("holder1",
							 holder1sub1,
							 solidLeg,
							 rotmat,
							 shift2);

    fLogicTargetHolder1 = new G4LogicalVolume(holder1, Acrylic, "fLogicTargetHolder1");

    /* Holder2: 2mm thick holder */
    rmin = 80./2. *mm;
    rmax = 100./2. *mm;
    dz = 2./2. *mm;
    sphi = 0.; // starting phi
    ephi = 360.*deg;
    G4Tubs *holder2center = new G4Tubs("holder2center",rmin,rmax,dz,sphi,ephi);

    rmin = 100./2. *mm;
    rmax = 74.3 *mm;
    dz = 2./2. *mm;
    sphi = 220.*deg; // starting phi
    ephi = 100.*deg;
    G4Tubs *holder2lower = new G4Tubs("holder2lower",rmin,rmax,dz,sphi,ephi);

    G4UnionSolid *holder2union = new G4UnionSolid("union2",
						  holder2center,
						  holder2lower);

    rotmat->set(0,0,0);
    rotmat->rotateZ(40*degree);
    G4SubtractionSolid *holder2sub = new G4SubtractionSolid("h2sub",
							    holder2union,
							    solidLeg,
							    rotmat,
							    shift1);
    rotmat->rotateZ(-80*degree);
    G4SubtractionSolid *holder2 = new G4SubtractionSolid("holder2",
							 holder2sub,
							 solidLeg,
							 rotmat,
							 shift2);
    fLogicTargetHolder2 = new G4LogicalVolume(holder2, Acrylic, "fLogicTargetHolder2");

    return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* TargetConstruction::PutTargetChamber(G4LogicalVolume *ExpHall_log)
{
  G4ThreeVector pos(0,0,300*mm);
  pos += fTargetPosition;
  new G4PVPlacement(0, pos, fLogicTargetChamber,
		    "PhysTargetChamber",ExpHall_log,false, 50001, false);
  new G4PVPlacement(0, pos, fLogicTargetChamberPbCover,
		    "PhysTargetChamberPbCover",ExpHall_log,false,50002,false);
  new G4PVPlacement(0, pos, fLogicTargetChamberSnCover,
		    "PhysTargetChamberSnCover",ExpHall_log,false,50003,false);

  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* TargetConstruction::PutTargetHolder(G4LogicalVolume *ExpHall_log)
{
  // Place holder1
  G4ThreeVector holder1pos(fTargetPosition.x(),
			   fTargetPosition.y(),
			   fTargetPosition.z() - fThickness/2. - 1.5*mm);

  new G4PVPlacement(0, holder1pos, fLogicTargetHolder1, "PhysHolder1",
		    ExpHall_log, false, 50004, false);

  // Place holder2
  G4ThreeVector holder2pos(fTargetPosition.x(),
			   fTargetPosition.y(),
			   fTargetPosition.z() + fThickness/2. + 1.*mm);

  new G4PVPlacement(0, holder2pos, fLogicTargetHolder2, "PhysHolder2",
		    ExpHall_log, false, 50005, false);

  // Place holder legs
  TVector3 leg1pos(fTargetPosition.x(),
		   fTargetPosition.y() - (79.3-25./2)*mm,
		   fTargetPosition.z() -6.5*mm);
  leg1pos.RotateZ(-40*M_PI/180.);
  G4ThreeVector holderLeg1pos(leg1pos.X(), leg1pos.Y(), leg1pos.Z());
  G4cout << "holderLeg1: " << holderLeg1pos << G4endl;
  
  G4RotationMatrix *rotmat1 = new G4RotationMatrix;
  rotmat1->set(0,0,0);
  rotmat1->rotateZ(40*degree);
  new G4PVPlacement(rotmat1, holderLeg1pos, fLogicTargetHolderLeg, "PhysHolderLeg1",
		    ExpHall_log, false, 50006, false);

  leg1pos.RotateZ(80*M_PI/180.);
  G4ThreeVector holderLeg2pos(leg1pos.X(), leg1pos.Y(), leg1pos.Z());
  G4cout << "holderLeg2: " << holderLeg2pos << G4endl;
  G4RotationMatrix *rotmat2 = new G4RotationMatrix;
  rotmat2->set(0,0,0);
  rotmat2->rotateZ(-40*degree);
  new G4PVPlacement(rotmat2, holderLeg2pos, fLogicTargetHolderLeg, "PhysHolderLeg2",
		    ExpHall_log, false, 50007, false);

  return 0;

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* TargetConstruction::PutReactionTarget(G4LogicalVolume *ExpHall_log)
{
  // Place target
  new G4PVPlacement(0, fTargetPosition, fLogicTarget, "PhysTarget",
		    ExpHall_log, false, 50008, false);

  fIsReactionTarget = true;
  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* TargetConstruction::PutCalibrationSource(G4LogicalVolume *ExpHall_log)
{
  // Place Calibrationsource
  G4ThreeVector pos = fTargetPosition + G4ThreeVector(0,0,6./2*mm);
  new G4PVPlacement(0, pos, fLogicSource, "PhysSource", ExpHall_log, false, 50009, false);

  fIsCalibrationSource = true;
  return 0;
}
