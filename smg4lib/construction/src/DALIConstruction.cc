#include "DALIConstruction.hh"
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

#include "G4NistManager.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "DALISimParameterReader.hh"

#include "globals.hh"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "G4SystemOfUnits.hh"//Geant4.10
//______________________________________________________________________________

DALIConstruction::DALIConstruction()
  : fDALISimParameter(0),fPosition(0,0,0),
    fLogicDali2CrystalSG(0),fLogicDali2CrystalSC(0),fLogicDali1Crystal(0),
    fLogicDali2AlHouseSG(0),fLogicDali2AlHouseSC(0),fLogicDali1AlHouse(0),
    fLogicDali2MgOCoatSG(0),fLogicDali2MgOCoatSC(0),fLogicDali1MgOCoat(0),
    fDALIExist(false),
    fDALISD(0)
{
  fDALIConstructionMessenger = new DALIConstructionMessenger();

  DALIMaterialList *mlist = new DALIMaterialList;
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fMgOMaterial = mlist->GetMaterial("MgO");
  fAlMaterial = mlist->GetMaterial("Al");
  fNaIMaterial = mlist->GetMaterial("NaI");

}

//______________________________________________________________________________
DALIConstruction::~DALIConstruction()
{;}
//______________________________________________________________________________
G4VPhysicalVolume* DALIConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // -------------------------------------------------
  // World LV
  G4ThreeVector worldSize(3.0*m,
			  3.0*m,
			  3.0*m);

  G4Box* solidWorld = new G4Box("World",
				0.5*worldSize.x(),
				0.5*worldSize.y(),
				0.5*worldSize.z());
  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,fVacuumMaterial,"World");

  // World PV
  G4ThreeVector worldPos(0,0,0);
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);
  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  //-------------------------------------------------------
  // DALI
  ConstructSub();
  PutDALI(LogicWorld);

  return world;
}
//______________________________________________________________________________
G4LogicalVolume* DALIConstruction::ConstructSub()
{
  G4cout<< "Creating Crystals of DALI"<<G4endl;

  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fDALISimParameter = (TDALISimParameter*)sman->FindParameter("DALIParameter");  
  if (fDALISimParameter==0) {
    fDALISimParameter = new TDALISimParameter("DALIParameter");
    sman->AddParameter(fDALISimParameter);
  }

  // The Saint-Gobain Housing:
  G4Box *sDali2AlHouseOutSG = new G4Box("sDali2AlHouseOutSG",17.64*cm/2.0,8.52*cm/2.0,5.014*cm/2.0);
  G4Box *sDali2AlHouseInSG  = new G4Box("sDali2AlHouseInSG",17.44*cm/2.0,8.32*cm/2.0,4.814*cm/2.0);
  G4SubtractionSolid *sDali2AlHouseSG    = new G4SubtractionSolid("sDali2AlHouseSG",sDali2AlHouseOutSG,sDali2AlHouseInSG);
  fLogicDali2AlHouseSG    = new G4LogicalVolume(sDali2AlHouseSG,fAlMaterial,"fLogicDali2AlHouseSG",0,0,0);
  // The Scionix Housing:
  G4Box *sDali2AlHouseOutSC = new G4Box("sDali2AlHouseOutSC",18.*cm/2.0,8.3*cm/2.0,4.5*cm/2.0);
  G4Box *sDali2AlHouseInSC  = new G4Box("sDali2AlHouseInSC",17.8*cm/2.0,8.1*cm/2.0,4.3*cm/2.0);
  G4SubtractionSolid *sDali2AlHouseSC    = new G4SubtractionSolid("sDali2AlHouseSC",sDali2AlHouseOutSC,sDali2AlHouseInSC);
  fLogicDali2AlHouseSC    = new G4LogicalVolume(sDali2AlHouseSC,fAlMaterial,"fLogicDali2AlHouseSC",0,0,0);
  //------------------------------
  // The Saint-Gobain Coating:
  G4Box *sDali2MgOCoatOutSG = new G4Box("sDali2MgOCoatOutSG",16.28*cm/2.0, 8.28*cm/2.0,4.78*cm/2.0);
  G4Box *sDali2MgOCoatInSG  = new G4Box("sDali2MgOCoatInSG",16.*cm/2.0, 8.*cm/2.0,4.5*cm/2.0);
  G4SubtractionSolid *sDali2MgOCoatSG    = new G4SubtractionSolid("sDali2MgOCoatSG",sDali2MgOCoatOutSG,sDali2MgOCoatInSG);
  fLogicDali2MgOCoatSG    = new G4LogicalVolume(sDali2MgOCoatSG,fMgOMaterial,"fLogicDali2MgOCoatSG",0,0,0);
  // The Scionix Housing:
  G4Box *sDali2MgOCoatOutSC = new G4Box("sDali2MgOCoatOutSC",16.4*cm/2.0,8.1*cm/2.0,4.3*cm/2.0);
  G4Box *sDali2MgOCoatInSC  = new G4Box("sDali2MgOCoatInSC",16.*cm/2.0, 8.*cm/2.0,4.0*cm/2.0);
  G4SubtractionSolid *sDali2MgOCoatSC    = new G4SubtractionSolid("sDali2MgOCoatSC",sDali2MgOCoatOutSC,sDali2MgOCoatInSC);
  fLogicDali2MgOCoatSC    = new G4LogicalVolume(sDali2MgOCoatSC,fMgOMaterial,"fLogicDali2MgOCoatSC",0,0,0);
  // -----------------------------
  // the Crystals:
  // The Saint-Gobain:
  G4Box *sDali2CrystalSG    = new G4Box("sDali2CrystalSG",16.*cm/2.0,8.*cm/2.0,4.5*cm/2.0);
  fLogicDali2CrystalSG    = new G4LogicalVolume(sDali2CrystalSG,fNaIMaterial,"fLogicDali2CrystalSG",0,0,0);
  //The Scionix:
  G4Box *sDali2CrystalSC    = new G4Box("sDali2CrystalSC",16.*cm/2.0, 8.*cm/2.0,4.*cm/2.0);
  fLogicDali2CrystalSC    = new G4LogicalVolume(sDali2CrystalSC,fNaIMaterial,"fLogicDali2CrystalSC",0,0,0);

  //_____________________________________________
  // Including also the old dali1 crystals...
  G4Box *sDali1AlHouseOut = new G4Box("sDali1AlHouseOut",13.83*cm/2.0,6.571*cm/2.0,6.571*cm/2.0);
  G4Box *sDali1AlHouseIn  = new G4Box("sDali1AlHouseIn",13.73*cm/2.0,6.414*cm/2.0,6.414*cm/2.0);
  G4SubtractionSolid *sDali1AlHouse    = new G4SubtractionSolid("sDali1AlHouse",sDali1AlHouseOut,sDali1AlHouseIn);
  fLogicDali1AlHouse    = new G4LogicalVolume(sDali1AlHouse,fAlMaterial,"fLogicDali1AlHouse",0,0,0);

  G4Box *sDali1Crystal    = new G4Box("sDali1Crystal",12.192*cm/2.0, 6.096*cm/2.0,6.096*cm/2.0);
  fLogicDali1Crystal    = new G4LogicalVolume(sDali1Crystal,fNaIMaterial,"fLogicDali1Crystal",0,0,0);

  // The Dali1 corting:
  G4Box *sDali1MgOCoatOut = new G4Box("sDali1MgOCoatOut",12.510*cm/2.0,6.414*cm/2.0,6.414*cm/2.0);
  G4Box *sDali1MgOCoatIn  = new G4Box("sDali1MgOCoatIn",12.192*cm/2.0, 6.096*cm/2.0,6.096*cm/2.0);
  G4SubtractionSolid *sDali1MgOCoat    = new G4SubtractionSolid("sDali1MgOCoat",sDali1MgOCoatOut,sDali1MgOCoatIn);
  fLogicDali1MgOCoat    = new G4LogicalVolume(sDali1MgOCoat,fMgOMaterial,"fLogicDali1MgOCoat",0,0,0);
  //_____________________________________________

  
  // Seting the vis attributes:
  //G4VisAttributes* visAttRed   = new G4VisAttributes(G4Colour(1.0,0.0,0.0));  
  G4VisAttributes* visAttGreen = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  //G4VisAttributes* visAttBlue  = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* visAttHouse = new G4VisAttributes(G4Colour(1.0,1.0,1.0));

  fLogicDali2MgOCoatSG->SetVisAttributes(visAttGreen);fLogicDali2MgOCoatSC->SetVisAttributes(visAttGreen);
  fLogicDali2AlHouseSG->SetVisAttributes(visAttHouse);fLogicDali2AlHouseSC->SetVisAttributes(visAttHouse);

  fLogicDali1AlHouse->SetVisAttributes(visAttHouse);
  fLogicDali1MgOCoat->SetVisAttributes(visAttGreen);
  
  G4cout<< "Creating Crystals of DALI -->DONE"<<G4endl;
  return 0;
}
//______________________________________________________________________________
G4LogicalVolume* DALIConstruction::PutDALI(G4LogicalVolume* ExpHall_log)
{
  // Show paraemters
  fDALISimParameter->Print();

  G4cout<<"Placing the Dali2 array"<<G4endl;
  fPosition = G4ThreeVector(fDALISimParameter->fPosition.x()*mm,
			    fDALISimParameter->fPosition.y()*mm,
			    fDALISimParameter->fPosition.z()*mm);

  //G4cout<<fPosition.x()<<" "<<G4endl;

  int pvid;
  int i = 0;
  int rotSign = 0;
  int CrystalType = 0;
  std::map<int, TDALINaISimParameter*> ParaMap 
    = fDALISimParameter->fDALINaISimParameterMap;
  std::map<int,TDALINaISimParameter*>::iterator it = ParaMap.begin();

  while (it != ParaMap.end()){
    i++;
    std::cout<<*(it->second)<<std::endl;// print parameters

    //  G4cout<<"create loop : "<<i<<G4endl;
    TDALINaISimParameter* para = it->second;

    CrystalType = para->fCrystalType;//SG or SC or DALI1(2) or empty(-1)
    rotSign = para->frotSign;//detector position in Al Housing

    G4ThreeVector detPos(para->fPosition.x()*mm + fPosition.x(),
			 para->fPosition.y()*mm + fPosition.y(), 
			 para->fPosition.z()*mm + fPosition.z());

    G4cout<<detPos.x()/cm<<"cm "<<detPos.y()<<" "<<detPos.z()<<" "<<G4endl;

    // Rotation
    G4RotationMatrix RotMat = G4RotationMatrix();
    RotMat.rotateX(para->fAngle.x()*deg);
    RotMat.rotateY(para->fAngle.y()*deg);
    RotMat.rotateZ(para->fAngle.z()*deg);

    pvid =  100000 +(i+1)*100;
    if(CrystalType==-1) {++it; continue;}

    G4RotationMatrix Rot3D;
    Rot3D.set(0, 0, 0);
    Rot3D.rotateX(para->fAngle.x()*degree);
    Rot3D.rotateY(para->fAngle.y()*degree);  
    Rot3D.rotateZ(para->fAngle.z()*degree);  

    G4ThreeVector AlHousePosition;
    G4ThreeVector Dali2CrystalPosition;
    G4ThreeVector Dali2HousePosition;

    //The detector type (SG==0,SC==1):

    if(CrystalType==0)  {
      Dali2HousePosition = detPos + Rot3D(G4ThreeVector(-rotSign*0.535*cm,0.,0)); 
      new G4PVPlacement(G4Transform3D(Rot3D,Dali2HousePosition),fLogicDali2AlHouseSG,"SG_House",ExpHall_log,false,pvid+2);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos),fLogicDali2MgOCoatSG,"SG_MgO",ExpHall_log,false,pvid+1);
      //new G4PVPlacement(G4Transform3D(Rot3D,detPos),fLogicDali2CrystalSG,"SG_Crystal",ExpHall_log,false,pvid);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos),fLogicDali2CrystalSG,"SG_Crystal",ExpHall_log,false,para->fID);
    }
    else if(CrystalType==1)  {
      Dali2HousePosition = detPos + Rot3D(G4ThreeVector(-rotSign*0.7*cm,0.,0)); 
      new G4PVPlacement(G4Transform3D(Rot3D,detPos),fLogicDali2AlHouseSC,"SC_House",ExpHall_log,false,pvid+2);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos),fLogicDali2MgOCoatSC,"SC_MgO",ExpHall_log,false,pvid+1); 
      //new G4PVPlacement(G4Transform3D(Rot3D,detPos), fLogicDali2CrystalSC,"SC_Crystal",ExpHall_log,false,pvid);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos), fLogicDali2CrystalSC,"SC_Crystal",ExpHall_log,false,para->fID);
    }
    else if(CrystalType==2)  {
      Dali2HousePosition = detPos + Rot3D(G4ThreeVector(-rotSign*0.451*cm,0.,0)); 
      new G4PVPlacement(G4Transform3D(Rot3D,Dali2HousePosition), fLogicDali1AlHouse,"DALI1_House",ExpHall_log,false,pvid+2);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos), fLogicDali1MgOCoat,"DALI1_MgO",ExpHall_log,false,pvid+1);
      //new G4PVPlacement(G4Transform3D(Rot3D,detPos), fLogicDali1Crystal,"DALI1_Crystal",ExpHall_log,false,pvid);
      new G4PVPlacement(G4Transform3D(Rot3D,detPos), fLogicDali1Crystal,"DALI1_Crystal",ExpHall_log,false,para->fID);

    }
    //    else {G4cout<<"You chose the wrong detector type"<<G4endl;return;}

    // // Assigning the positions:
    // fPosX[dali2Det] = Pos.getX();
    // fPosY[dali2Det] = Pos.getY();
    // fPosZ[dali2Det] = Pos.getZ();
    // //-----------------------------------------
    // //------------------------------------------------
    // cout << id << " " << fPosX[dali2Det]/cm 
    //            << " " << fPosY[dali2Det]/cm 
    //            << " " << fPosZ[dali2Det]/cm << " " << endl;
    // //----------------------------------------------------

    // float distance = sqrt(fPosX[dali2Det]/mm*fPosX[dali2Det]/mm  +
    //                       fPosY[dali2Det]/mm*fPosY[dali2Det]/mm	 +
    //                       fPosZ[dali2Det]/mm*fPosZ[dali2Det]/mm);

    // float thetaPlaced = acos(fPosZ[dali2Det]/mm/distance);
    // float phiPlaced   = acos(fPosX[dali2Det]/mm/distance/sin(thetaPlaced));

    // if(fPosY[dali2Det]/mm < 0.0) phiPlaced = 2.0*3.14159-phiPlaced;
    // thetaPlaced = thetaPlaced/3.14159*180.0;
    // phiPlaced   = phiPlaced/3.14159*180.0;

    // if(abs(fPosY[dali2Det]/mm)<1.0 && fPosX[dali2Det]/mm>=0.0) phiPlaced = 0.0;
    // if(abs(fPosY[dali2Det]/mm)<1.0 && fPosX[dali2Det]/mm<0.0)  phiPlaced = 180.0;

    // fprintf(fFileOut,"%i %i %f %f %f \n", dali2Det, CrystalType, thetaPlaced, phiPlaced, distance);


    fDALIExist = true;
    ++it;   
  }


  G4cout<<"Creating the Dali2 array --> DONE"<<G4endl;
  return 0;
}

//______________________________________________________________________________
