#include "MINOSConstruction.hh"
#include "MINOSConstructionMessenger.hh"

#include "DALIMaterialList.hh"

#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

#include "G4NistManager.hh"

//______________________________________________________________________________________
MINOSConstruction::MINOSConstruction()
  : fPosition(0.0*mm, 0.0*mm, 0.0*mm),
    fTargetCell_Radius(0.5*39*mm),
    fTargetCell_Length(150*mm),
    fFillTargetLH2(true),
    fPutExitCap(false)
{
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  fMINOSConstructionMessenger = new MINOSConstructionMessenger(this);
}
//______________________________________________________________________________________
MINOSConstruction::~MINOSConstruction()
{;}
//______________________________________________________________________________________
G4VPhysicalVolume* MINOSConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //---------------------------------------------------
  // World LV
  G4ThreeVector worldSize(3.0*m,3.0*m,3.0*m);
  G4Box* solidWorld = new G4Box("World",
				worldSize.x()/2,
				worldSize.y()/2,
				worldSize.z()/2);
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fWorldMaterial,"World");
  G4ThreeVector worldPos(0, 0, 0);  
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);

  //---------------------------------------------------
  PutMINOS(LogicWorld);
  G4cout <<* (G4Material::GetMaterialTable()) << G4endl;

  return world;
}
//______________________________________________________________________________________
G4LogicalVolume* MINOSConstruction::PutMINOS(G4LogicalVolume* ExpHall_log)
{
  DALIMaterialList *MaterialList = new DALIMaterialList;

  G4RotationMatrix rot3D;
  rot3D.set(0, 0, 0);
  G4Tubs* sPipeTubs= new G4Tubs("sPipeTubs",36.*mm,38*mm,(458./2)*mm,0*deg,360*deg);
  G4Tubs* sCylinderTubs= new G4Tubs("sCylinderTubs",36.*mm,(156./2)*mm,(24./2)*mm,0*deg,360*deg);
  G4UnionSolid *sBeamPipeTubs 
    = new G4UnionSolid("sBeamPipeTubs",sPipeTubs,sCylinderTubs,
		       G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0.0*mm,(-458./2+1.8)*mm)));

  G4LogicalVolume *lBeamPipe 
    = new G4LogicalVolume(sBeamPipeTubs,MaterialList->GetMaterial("Al"),"lBeamPipe",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0.0*m,.0*mm,87.4*mm)+fPosition),
		    lBeamPipe,"pBeamPipe",ExpHall_log,false, 999999);
  
  G4Tubs* schamber1 = new G4Tubs("schamber1",(250./2.)*mm,(258./2.)*mm,(260./2.)*mm,0*deg,360*deg);
  G4Tubs* schamber2 = new G4Tubs("schamber2",0*mm,(258./2.)*mm,(260./2.)*mm,0*deg,360*deg);
  G4Tubs* schamber3 = new G4Tubs("schamber3",0*mm,(110./2.)*mm,(310./2.)*mm,0*deg,360*deg);
  G4Tubs* schamber4 = new G4Tubs("schamber4",(102./2)*mm,(110./2.)*mm,(310./2.)*mm,0*deg,360*deg);
  rot3D.set(0,90*deg,0);
  G4SubtractionSolid* schamber5 
    = new G4SubtractionSolid("schamber5",schamber1,schamber3,
			     G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0.0*mm,-49.0*mm)));
  G4SubtractionSolid* schamber6 
    = new G4SubtractionSolid("schamber6",schamber4,schamber2,
			     G4Transform3D(rot3D,G4ThreeVector(0.0*mm,49.0*mm,10.0*mm)));
  G4UnionSolid* schamber7 
    = new G4UnionSolid("schamber7",schamber6,schamber5,
		       G4Transform3D(rot3D,G4ThreeVector(0.0*mm,49.0*mm,9.9*mm)));
  G4Tubs* schamber8 = new G4Tubs("schamber7",(110.1/2)*mm,(156.1/2)*mm,10.0*mm,0*deg,360*deg);
  rot3D.set(0,0,0);
  G4UnionSolid *schamber 
    = new G4UnionSolid("schamber",schamber7,schamber8,
		       G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0.0*mm,(310./2-10.0)*mm)));
  G4LogicalVolume *lchamber 
    = new G4LogicalVolume(schamber,MaterialList->GetMaterial("Al"),"lchamber",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0*mm,-300.8*mm)+fPosition),
		    lchamber,"pchamber",ExpHall_log,false,999970);

  rot3D.set(0,0,0);						       
  G4Tubs *sCuTrackerTubs
    = new G4Tubs("sCuTrackerTubs",92.4*mm,(92.4+0.03)*mm,(300.0/2)*mm,0*deg,360*deg);
  G4LogicalVolume *lCuTracker 
    = new G4LogicalVolume(sCuTrackerTubs,MaterialList->GetMaterial("Cu"),"lCuTracker",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,
				  G4ThreeVector(0.0*m,.0*mm,-75*mm+150*mm+26.12*mm)+fPosition),
		    lCuTracker,"pCuTracker",ExpHall_log,false, 999998);
 
  G4Tubs *sGlassFiberTubs
    = new G4Tubs("sGlassFiberTubs",92.4*mm,(92.4+0.2)*mm,(300.0/2)*mm,0*deg,360*deg);
  G4LogicalVolume *lGlassFiber 
    = new G4LogicalVolume(sGlassFiberTubs,MaterialList->GetMaterial("epoxy"),"lGlassFiber",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,
				  G4ThreeVector(0.0*m,.0*mm,-75*mm+150*mm+26.12*mm)+fPosition),
		    lGlassFiber,"pGlassFiber",ExpHall_log,false, 999997);

  G4Tubs                *sMM[2];
  G4LogicalVolume       *lMM[2]; 

  sMM[0]=new G4Tubs("MM[0]",37*mm,100*mm,2.5*mm,0,360*deg);
  lMM[0]=new G4LogicalVolume(sMM[0],MaterialList->GetMaterial("Al"),"lMM[0]",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,
				  G4ThreeVector(0.0*m,.0*mm,-75*mm+26.12*mm+2.5*mm)+fPosition),
		    lMM[0],"pMM[0]",ExpHall_log,false,999996);
 

  sMM[1]=new G4Tubs("MM[1]",37*mm,100*mm,1.6*mm,0,360*deg);
  lMM[1]=new G4LogicalVolume(sMM[1],MaterialList->GetMaterial("PCB"),"lMM[1]",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,
				  G4ThreeVector(0.0*m,.0*mm,-75*mm+26.12*mm+(5+1.6)*mm)+fPosition),
		    lMM[1],"pMM[1]",ExpHall_log,false,999995);

  G4Tubs *sCathode = new G4Tubs("sCathode",37*mm,100*mm,1.6*mm,0,360*deg);
  G4LogicalVolume *lCathode 
    = new G4LogicalVolume(sCathode,MaterialList->GetMaterial("epoxy"),"lCathode",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,
				  G4ThreeVector(0.0*m,.0*mm,-75*mm+26.12*mm+(300+3.2)*mm)+fPosition),
		    lCathode,"pCathode",ExpHall_log,false,999994);

  G4Box *sboard1 = new G4Box("sboard",(85./2)*mm,(5./2)*mm,(85./2)*mm);
  G4Box *sboard2 = new G4Box("sboard",(20./2)*mm,(5./2)*mm,(85./2)*mm);
  G4ThreeVector shift(0,5.*mm,0);
  G4Transform3D transform(rot3D,shift);
  G4UnionSolid *sboard = new G4UnionSolid("sboard",sboard1,sboard2,transform);
  G4LogicalVolume *lboard 
    = new G4LogicalVolume(sboard,MaterialList->GetMaterial("Scintillator"),"lboard",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0,-92.4*mm,-75*mm+(300+3.2)*mm+26.12*mm+(85./2.)*mm)+fPosition), lboard,"pboard0",ExpHall_log,false,999980);
  rot3D.set(180*deg,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0,92.4*mm,-75*mm+(300+3.2)*mm+26.12*mm+(85./2.)*mm)+fPosition),lboard,"pboard1",ExpHall_log,false,999981);
 
  G4VisAttributes* visAttBeamPipe = new G4VisAttributes(G4Colour(.5,.5,.5));
  G4VisAttributes* visAttChamber = new G4VisAttributes(G4Colour(.7,.7,.8));
  G4VisAttributes* visAttEndCap = new G4VisAttributes(G4Colour(.0,.0,1.0));
  G4VisAttributes* visAttBoard = new G4VisAttributes(G4Colour(.6,1,.6));

  lBeamPipe->SetVisAttributes(visAttBeamPipe);
  lchamber->SetVisAttributes(visAttChamber);
  lMM[0]->SetVisAttributes(visAttEndCap);
  lMM[1]->SetVisAttributes(visAttEndCap);
  lCathode->SetVisAttributes(visAttEndCap);
  lboard->SetVisAttributes(visAttBoard);

  if (fFillTargetLH2) FillTargetLH2(ExpHall_log);
  if (fPutExitCap) PutExitCap(ExpHall_log);

  return 0;
}
//______________________________________________________________________________________
G4LogicalVolume* MINOSConstruction::FillTargetLH2(G4LogicalVolume* ExpHall_log)
{ 
  DALIMaterialList *MaterialList = new DALIMaterialList;

  G4Tubs *sMTarget = new G4Tubs("MTarget",0.0*mm,fTargetCell_Radius,0.5*fTargetCell_Length,
				0*deg,360*deg);
  G4LogicalVolume *lMTarget = new G4LogicalVolume(sMTarget, MaterialList->GetMaterial("LH2"), 
						  "MTarget", 0, 0, 0);
  G4VisAttributes* visAttTarget = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  lMTarget->SetVisAttributes(visAttTarget); 
  new G4PVPlacement(0,G4ThreeVector(0.0*m,0.0*m,0.0*m)+fPosition,
		    lMTarget,"LH2Target",ExpHall_log,false,1111);
  
  return 0;
}
//______________________________________________________________________________________
G4LogicalVolume* MINOSConstruction::PutExitCap(G4LogicalVolume* ExpHall_log)
{ 
  DALIMaterialList *MaterialList = new DALIMaterialList;
  G4RotationMatrix rot3D;
  rot3D.set(0, 0, 0);
  G4Tubs *sEC0 = new G4Tubs("sEC0",37.*mm,60*mm,(10./2.)*mm,0*deg,360*deg);
  G4Tubs *sEC1 = new G4Tubs("sEC1",37.*mm,47*mm,(25./2.)*mm,0*deg,360*deg);
  G4UnionSolid *sEC = new G4UnionSolid("sEC",sEC0,sEC1,
				       G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0.0*mm,-17.5*mm)));
  G4LogicalVolume *lEC = new G4LogicalVolume(sEC,MaterialList->GetMaterial("acryl"),"lEC",0,0,0); 
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0.0*m,0.0*mm,331.4*mm)+fPosition),
		    lEC,"MINOSExitCap",ExpHall_log,false, 999991);

  G4VisAttributes* visSourceHolder = new G4VisAttributes(G4Colour(0.1,0.0,0.3));
  lEC->SetVisAttributes(visSourceHolder);

  return lEC;
}
//______________________________________________________________________________________
