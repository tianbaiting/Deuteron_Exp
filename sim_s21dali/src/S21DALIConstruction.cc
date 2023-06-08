#include "S21DALIConstruction.hh"
#include "DALIConstructionMessenger.hh"

#include "DALIMaterialList.hh"

#include "SimDataManager.hh"
#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"//Geant4.10

#include "G4NistManager.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "DALISimParameterReader.hh"

#include "globals.hh"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
//______________________________________________________________________________

S21DALIConstruction::S21DALIConstruction()
  : fLogicPMTSG(0),fLogicPMTSC(0),fLogicPMTDali1(0)
{;}
//______________________________________________________________________________
S21DALIConstruction::~S21DALIConstruction()
{;}
//______________________________________________________________________________
G4VPhysicalVolume* S21DALIConstruction::Construct()
{
  G4VPhysicalVolume *world = DALIConstruction::Construct();
  //-------------------------------------------------------
  // DALI
  ConstructSub();
  //PutFrame(world->GetLogicalVolume());

  return world;
}
//______________________________________________________________________________
G4LogicalVolume* S21DALIConstruction::ConstructSub()
{
  DALIConstruction::ConstructSub();

  //_____________________________________________
  //Frame
  G4RotationMatrix Rot3D;
  Rot3D.set(0,0,0);
  G4Box *splate = new G4Box("splate",1240.*mm/2.,1240.*mm/2.,5.0*mm/2.);
  G4Box *splate_h = new G4Box("splate_h",1240.*mm/4.,1241.*mm/2.,5.2*mm/2.);

  G4Tubs *shole[5];
  shole[0]= new G4Tubs("shole[0]",0.*mm,380*mm/2.,5.1*mm/2.,0*deg,360*deg);
  shole[1]= new G4Tubs("shole[1]",0.*mm,400*mm/2.,5.1*mm/2.,0*deg,360*deg);
  shole[2]= new G4Tubs("shole[2]",0.*mm,580*mm/2.,5.1*mm/2.,0*deg,360*deg);
  shole[3]= new G4Tubs("shole[3]",0.*mm,680*mm/2.,5.1*mm/2.,0*deg,360*deg);
  shole[4]= new G4Tubs("shole[4]",0.*mm,720*mm/2.,5.1*mm/2.,0*deg,360*deg);

  G4SubtractionSolid* sframe00 = new G4SubtractionSolid("sframe00",splate,shole[0]);
  G4SubtractionSolid *sframe[5];

  sframe[0] = new G4SubtractionSolid("sframe[0]",sframe00,splate_h,G4Transform3D(Rot3D,G4ThreeVector(1240.*mm/4,0,0)));
  fLogicframe[0] = new G4LogicalVolume(sframe[0],fAlMaterial,"Logicframe[0]",0,0,0);
  sframe[1] = new G4SubtractionSolid("sframe[1]",splate,shole[1]);  
  fLogicframe[1] = new G4LogicalVolume(sframe[1],fAlMaterial,"Logicframe[1]",0,0,0);
  sframe[2] = new G4SubtractionSolid("sframe[2]",splate,shole[2]);
  fLogicframe[2] = new G4LogicalVolume(sframe[2],fAlMaterial,"Logicframe[2]",0,0,0);
  sframe[3] = new G4SubtractionSolid("sframe[3]",splate,shole[3]);
  fLogicframe[3] = new G4LogicalVolume(sframe[3],fAlMaterial,"Logicframe[3]",0,0,0);
  sframe[4] = new G4SubtractionSolid("sframe[4]",splate,shole[4]);
  fLogicframe[4] = new G4LogicalVolume(sframe[4],fAlMaterial,"Logicframe[4]",0,0,0);
  
  // Seting the vis attributes:
  G4VisAttributes* visAttFrame   = new G4VisAttributes(G4Colour(0.4,0.4,0.5));  
  for(int ifl = 0;ifl<5;ifl++){
    fLogicframe[ifl]->SetVisAttributes(visAttFrame);
  }
  G4cout<< "Creating Parts of DALI -->DONE"<<G4endl;
  return 0;
}
//______________________________________________________________________________
G4LogicalVolume* S21DALIConstruction::PutDALI(G4LogicalVolume* ExpHall_log)
{
  DALIConstruction::PutDALI(ExpHall_log);
  PutFrame(ExpHall_log);
  return 0;
}

//______________________________________________________________________________
G4LogicalVolume* S21DALIConstruction::PutFrame(G4LogicalVolume* ExpHall_log)
{
  G4ThreeVector DALIPosition = G4ThreeVector(fDALISimParameter->fPosition.x()*mm,
					     fDALISimParameter->fPosition.y()*mm,
					     fDALISimParameter->fPosition.z()*mm);

  G4RotationMatrix Rot3D;
  Rot3D.set(0,0,0);
  //1
  Rot3D.set(0,0,0);
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(-200*mm,0.0*mm,-116.6*mm-5.0*mm)+DALIPosition),fLogicframe[0],"pframe[0]",ExpHall_log,false,999980);
  Rot3D.set(0,0,180*deg);
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(200.*mm ,0.0*mm,-116.6*mm-5.0*mm)+DALIPosition),fLogicframe[0],"pframe[1]",ExpHall_log,false,999981);
  //2  
  Rot3D.set(0,0,0);
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(-200.*mm,0.0*mm,-16.6*mm-5.0*mm)+DALIPosition),fLogicframe[0],"pframe[2]",ExpHall_log,false,999982);
  Rot3D.set(0,0,180*deg); 
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(200.*mm,0.0*mm,-16.6*mm-5.0*mm)+DALIPosition),fLogicframe[0],"pframe[3]",ExpHall_log,false,999983);
  //3
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(0.0*mm,0.0*mm,83.2*mm-5.0*mm)+DALIPosition),fLogicframe[2],"pframe[4]",ExpHall_log,false,999984);
  //4  
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(0.0*mm,0.0*mm,183.2*mm-5.0*mm)+DALIPosition),fLogicframe[3],"pframe[5]",ExpHall_log,false,999985);
  //5 
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(0.0*mm,0.0*mm,288.36*mm-5.0*mm)+DALIPosition),fLogicframe[4],"pframe[6]",ExpHall_log,false,999986);
  //6 
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(0.0*mm,0.0*mm,398.56*mm-5.0*mm)+DALIPosition),fLogicframe[3],"pframe[7]",ExpHall_log,false,999987);
  //7 
  new G4PVPlacement(G4Transform3D(Rot3D,G4ThreeVector(0.0*mm,0.0*mm,525.6*mm-5.0*mm)+DALIPosition),fLogicframe[1],"pframe[8]",ExpHall_log,false,999988);

  G4cout<<"DALI Frame -->DONE"<<G4endl;
  return 0;
}
