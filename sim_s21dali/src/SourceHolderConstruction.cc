#include "SourceHolderConstruction.hh"
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
SourceHolderConstruction::SourceHolderConstruction()
  : fPosition(0.0*mm, 0.0*mm, 0.0*mm)
{
  fVacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
}
//______________________________________________________________________________________
SourceHolderConstruction::~SourceHolderConstruction()
{;}
//______________________________________________________________________________________
G4LogicalVolume* SourceHolderConstruction::PutSourceHolder(G4LogicalVolume* ExpHall_log)
{
  
  DALIMaterialList *MaterialList = new DALIMaterialList;
  
  G4RotationMatrix rot3D;
  rot3D.set(0, 0, 0);
  G4Tubs *ssource1 = new G4Tubs("ssource1",0*mm,(25./2)*mm,(6./2)*mm,0*deg,360*deg);
  G4Tubs *ssource2 = new G4Tubs("ssource2",0*mm,(10./2)*mm,(1.5/2)*mm,0*deg,360*deg);
  G4SubtractionSolid *ssource 
    = new G4SubtractionSolid("ssource",ssource1,ssource2,
			     G4Transform3D(rot3D,G4ThreeVector(0.0*mm,0.0*mm,(-1.5/2)*mm)));
  G4LogicalVolume *lsource = new G4LogicalVolume(ssource,
						 MaterialList->GetMaterial("acryl"),
						 "lsource",0,0,0);
  new G4PVPlacement(G4Transform3D(rot3D,G4ThreeVector(0.0*m,.0*mm,339.*mm)+fPosition),
		    lsource,"psource",ExpHall_log,false,999993);

  G4VisAttributes* visSource = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
  
  lsource->SetVisAttributes(visSource);

  return lsource;
}
//____________________________________________________________________
