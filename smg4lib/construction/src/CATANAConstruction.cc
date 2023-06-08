#include "CATANAConstruction.hh"
#include "CATANAConstructionMessenger.hh"

#include "SimDataManager.hh"
#include "TCATANASimParameter.hh"
#include "TCATANACsISimParameter.hh"
#include "CATANASimParameterReader.hh"

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4NistManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4Transform3D.hh"

#include "G4SystemOfUnits.hh" // Geant4.10

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include <iostream>

//#define DEBUG

#ifdef DEBUG
G4bool CHK_OVERLAP = true;
#else
G4bool CHK_OVERLAP = false;
#endif

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANAConstruction::CATANAConstruction()
  : fCATANASimParameter(0), fPosition(-1000,-1000,-1000),
    fCATANAExist(false), fCATANASD(0)
{
  for(Int_t i=0;i<5;++i){
    Double_t x1,x2,x3,x4,y1,y2,z,crystalz;
    Int_t pmttype; // 1: H7195, 2:H11934
    Double_t seal_dt = 12*mm, housing_dt = 0.5*mm, reflector_dt = 0.5*mm;

    if(i == 0){ // crystal type 1
      x1 = 62.3*mm; x2 = 62.3*mm; x3 = 95.7*mm; y1 = 36.6*mm; y2 = 56.3*mm;
      z = 107*mm; crystalz = 93.*mm; pmttype = 1;
    }
    if(i == 1){ // crystal type 2
      x1 = 57.1*mm; x2 = 63.6*mm; x3 = 84.5*mm; y1 = 34.9*mm; y2 = 55.4*mm;
      z = 117*mm; crystalz = 103.*mm; pmttype = 1;
    }
    if(i == 2){ // crystal type 3
      x1 = 49.7*mm; x2 = 58.5*mm; x3 = 74.9*mm; y1 = 38.3*mm; y2 = 64.7*mm;
      z = 137*mm; crystalz = 123.*mm; pmttype = 1;
    }
    if(i == 3){ // crystal type 4
      x1 = 40.0*mm; x2 = 50.2*mm; x3 = 60.3*mm; y1 = 38.3*mm; y2 = 66.4*mm;
      z = 152*mm; crystalz = 138.5*mm; pmttype = 1;
    }
    if(i == 4){ // crystal type 5
      x1 = 28.4*mm; x2 = 39.7*mm; x3 = 41.5*mm; y1 = 38.3*mm; y2 = 69.9*mm;
      z = 162*mm; crystalz = 148.5*mm; pmttype = 2;
    }
    x4 = x3 + (y2/y1)*(x2-x1); // planarity condition
    Double_t beta1 = 90*deg + std::atan((x2-x1)/(y1*2));// should be
    Double_t beta2 = 90*deg + std::atan((x4-x3)/(y2*2));// beta1 = beta2
    if(std::abs(beta1-beta2)>1e-4){
      std::cout << "Housing type " <<i << " is not planar" << std::endl;
    }
    std::cout << "Type " << i+1 << ": x4 = " << x4 << std::endl;
    
    fGeometry[i].dettype = i+1;
    fGeometry[i].x1 = x1;
    fGeometry[i].x2 = x2;
    fGeometry[i].x3 = x3;
    fGeometry[i].y1 = y1;
    fGeometry[i].y2 = y2;
    fGeometry[i].x4 = x4;
    fGeometry[i].z = z;
    fGeometry[i].seal_dt = seal_dt;
    fGeometry[i].housing_dt = housing_dt;
    fGeometry[i].reflector_dt = reflector_dt;
    fGeometry[i].crystalz = crystalz;
    fGeometry[i].pmttype = pmttype;

  }

  // from H7195 (Hamamatsu) technical drawing:
  // -6mm corresponds to the length inserted to the CATANA seal part
  fPMTlength[0] = (162. - 6.) *mm;

  // from H11934 (Hamamatsu) technical drawing
  fPMTlength[1] = 32.5*mm;
  
  for(G4int itype=0;itype<5;++itype){
    fLogicCrystal[itype] = 0;
    fLogicHousing[itype] = 0;
    fLogicReflector[itype] = 0;
  }
  for(G4int itype=0;itype<2;++itype) fLogicPMT[itype] = 0;

  fCATANAConstructionMessenger = new CATANAConstructionMessenger();
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANAConstruction::~CATANAConstruction(){;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4VPhysicalVolume* CATANAConstruction::Construct(){

  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // World LV
  G4ThreeVector worldSize(3.0*m, 3.0*m, 3.0*m);
  G4Box* solidWorld = new G4Box("World",
				0.5*worldSize.x(),
				0.5*worldSize.y(),
				0.5*worldSize.z());

  G4NistManager *nistmanager = G4NistManager::Instance();
  G4Material *Vacuum = nistmanager->FindOrBuildMaterial("G4_Galactic");

  G4LogicalVolume *LogicWorld = new G4LogicalVolume(solidWorld,
						    Vacuum,
						    "World");

  G4ThreeVector worldPos(0,0,0);
  G4VPhysicalVolume* world = new G4PVPlacement(0,
					       worldPos,
					       LogicWorld,
					       "World",
					       0,
					       false,
					       0);
  LogicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  ConstructSub();
  PutCATANA(LogicWorld);

  return world;
  
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* CATANAConstruction::ConstructSub(){

  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fCATANASimParameter =
    (TCATANASimParameter*)sman->FindParameter("CATANAParameter");

  if(fCATANASimParameter == 0){
    fCATANASimParameter = new TCATANASimParameter("CATANAParameter");
    sman->AddParameter(fCATANASimParameter);
  }

  // Define Materials
  G4NistManager *nistmanager = G4NistManager::Instance();
  G4Material *CsI = nistmanager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material *Teflon = nistmanager->FindOrBuildMaterial("G4_TEFLON");
  G4Material *Al = nistmanager->FindOrBuildMaterial("G4_Al");

  // PMT1: PMT for type 1~4: H7195 from Hamamatsu
  G4double rmin = 0, rmax = 47./2*mm;
  G4double sphi = 0., ephi = 360*deg;
  G4Tubs *solidPMT1 = new G4Tubs("solidPMT1", rmin, rmax, fPMTlength[0]*0.5,
				 sphi, ephi);

  fLogicPMT[0] = new G4LogicalVolume(solidPMT1, Al, "fLogicPMT1");
  fLogicPMT[0]->SetVisAttributes(new G4VisAttributes(G4Colour::Black()));

  // PMT2: PMT for type 5: H11934 from Hamamatsu
  G4double dx = 30./2., dy = 30./2.;
  G4Box *solidPMT2 = new G4Box("solidPMT2",dx,dy,fPMTlength[1]*0.5);

  fLogicPMT[1] = new G4LogicalVolume(solidPMT2, Al, "fLogicPMT2");
  fLogicPMT[1]->SetVisAttributes(new G4VisAttributes(G4Colour::Black()));

  for(Int_t itype=0;itype<5;++itype){
    // Parameters for G4Trap
    G4double pDz, pDy1, pDy2, pDx1, pDx2, pDx3, pDx4;
    G4double pTheta=0.*deg, pPhi=0.*deg, pAlp1=0.*deg, pAlp2=0*deg;

    // housing outside
    pDz  = fGeometry[itype].z*0.5; 
    pDy1 = fGeometry[itype].y1*0.5;
    pDy2 = fGeometry[itype].y2*0.5;
    pDx1 = fGeometry[itype].x1*0.5;
    pDx2 = fGeometry[itype].x2*0.5;
    pDx3 = fGeometry[itype].x3*0.5;
    pDx4 = fGeometry[itype].x4*0.5;
    
    char name[50];
    sprintf(name,"solidHousingOUT%d",itype+1);
    G4Trap *solidHousingOUT = new G4Trap(name, pDz, pTheta, pPhi,
					 pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3,
					 pDx4, pAlp2);

    // -- Al housing inside
    G4double length = fGeometry[itype].z;
    G4double housing_dt = fGeometry[itype].housing_dt;
    G4double seal_dt = fGeometry[itype].seal_dt;
    G4double ax1 = pDx1;
    G4double bx1 = pDx3;
    G4double ax2 = pDx2;
    G4double bx2 = pDx4;
    G4double ay1 = pDy1;
    G4double by1 = pDy2;

    pDz = (length - seal_dt - housing_dt)/2.;
    pDy1 = (by1-ay1)/length * housing_dt + ay1 - housing_dt;
    pDx1 = (bx1-ax1)/length * housing_dt + ax1 - housing_dt;
    pDx2 = (bx2-ax2)/length * housing_dt + ax2 - housing_dt;
    pDy2 = (by1-ay1)/length * (length - seal_dt) + ay1 - housing_dt;
    pDx3 = (bx1-ax1)/length * (length - seal_dt) + ax1 - housing_dt;
    //pDx4 = (bx2-ax2)/length * (length - seal_dt) + ax2 - housing_dt;
    pDx4 = pDx3 + (pDy2 / pDy1)*(pDx2 - pDx1); // planarity condition
    
    sprintf(name,"solidHousingIN%d",itype+1);
    G4Trap* solidHousingIN = new G4Trap(name, pDz, pTheta, pPhi,
					pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3,
					pDx4, pAlp2);
  
    G4RotationMatrix rotHousing;
    rotHousing.set(0,0,0);
    G4double offset = -(length*0.5 - pDz - housing_dt);
    sprintf(name,"solidHousing%d",itype+1);
    G4SubtractionSolid* solidHousing = 
      new G4SubtractionSolid(name, solidHousingOUT,// mother
			     solidHousingIN, &rotHousing,
			     G4ThreeVector(0.,0.,offset));

    sprintf(name,"logicHousing%d",itype+1);
    fLogicHousing[itype] = new G4LogicalVolume(solidHousing, Al,
					       name,0,0,0);

    // -- Crystal --
    G4double reflector_dt = fGeometry[itype].reflector_dt;
    G4double space = 2.*mm; // space btw. crystal and housing
    G4double crystalz = fGeometry[itype].crystalz;
    length = pDz * 2.; // housing inner z length
    ax1 = pDx1;
    bx1 = pDx3;
    ax2 = pDx2;
    bx2 = pDx4;
    ay1 = pDy1;
    by1 = pDy2;

    pDz = crystalz*0.5;    
    pDy1 = (by1-ay1)/length * reflector_dt + ay1 - space;
    pDx1 = (bx1-ax1)/length * reflector_dt + ax1 - space;
    pDx2 = (bx2-ax2)/length * reflector_dt + ax2 - space;
    pDy2 = (by1-ay1)/length * (reflector_dt + crystalz) + ay1 - space;
    pDx3 = (bx1-ax1)/length * (reflector_dt + crystalz) + ax1 - space;
    //pDx4 = (bx2-ax2)/length * (reflector_dt + crystalz) + ax2 - space;
    pDx4 = pDx3 + (pDy2 / pDy1)*(pDx2 - pDx1); // planarity condition
    
    sprintf(name,"solidCrystal%d",itype+1);
    G4Trap* solidCrystal = new G4Trap(name, pDz, pTheta, pPhi,
				      pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3,
				      pDx4, pAlp2);

    sprintf(name,"logicCrystal%d",itype+1);
    fLogicCrystal[itype] = new G4LogicalVolume(solidCrystal,// solid
					       CsI, // Material
					       name, // name
					       0,0,0);

    // -- Teflon reflector
    length = crystalz;
    ax1 = pDx1;
    bx1 = pDx3;
    ax2 = pDx2;
    bx2 = pDx4;
    ay1 = pDy1;
    by1 = pDy2;

    pDz = crystalz*0.5 + reflector_dt;
    pDy1 = (by1-ay1)/length * -reflector_dt + ay1 + reflector_dt;
    pDx1 = (bx1-ax1)/length * -reflector_dt + ax1 + reflector_dt;
    pDx2 = (bx2-ax2)/length * -reflector_dt + ax2 + reflector_dt;
    pDy2 = (by1-ay1)/length * (reflector_dt + crystalz) + ay1 + reflector_dt;
    pDx3 = (bx1-ax1)/length * (reflector_dt + crystalz) + ax1 + reflector_dt;
    //pDx4 = (bx2-ax2)/length * (reflector_dt + crystalz) + ax2 + reflector_dt;
    pDx4 = pDx3 + (pDy2 / pDy1)*(pDx2 - pDx1); // planarity condition

    sprintf(name,"solidReflectorOUT%d",itype+1);
    G4Trap* solidReflectorOUT = new G4Trap(name,pDz, pTheta,
					   pPhi, pDy1, pDx1, pDx2, pAlp1,
					   pDy2, pDx3, pDx4, pAlp2);
    
    G4RotationMatrix rotmat(0,0,0);
    sprintf(name,"solidReflector%d",itype+1);
    G4SubtractionSolid* solidReflector =
      new G4SubtractionSolid(name, solidReflectorOUT,// mother
			     solidCrystal, &rotmat,
			     G4ThreeVector(0.,0.,0.));

    sprintf(name,"logicReflector%d",itype+1);
    fLogicReflector[itype] = new G4LogicalVolume(solidReflector, Teflon,
						 name,
						 0,0,0);
  }

  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
G4LogicalVolume* CATANAConstruction::PutCATANA(G4LogicalVolume *ExpHall_log){

  fCATANASimParameter->Print();

  G4cout << "Placing the CATANA Array" << G4endl;
  fPosition = G4ThreeVector(fCATANASimParameter->fPosition.X()*mm,
			    fCATANASimParameter->fPosition.Y()*mm,
			    fCATANASimParameter->fPosition.Z()*mm);

  // Configure each types of detectors as assemblies
  G4RotationMatrix rotmat(0,0,0);// no rotation
  G4double housingZOffset = 0.;
  G4ThreeVector pos(0,0,0);
  G4Transform3D t3d(rotmat,pos);
  G4double crystalz = 0.;
  G4double housingz = 0.;
  G4double reflector_dt = 0.5*mm;
  G4double housing_dt = 0.5*mm;

  G4AssemblyVolume *type[5]; // type 1~5

  for(Int_t itype=0;itype<5;++itype){
    pos.set(0,0,0);
    t3d = G4Transform3D(rotmat, pos);
    type[itype] = new G4AssemblyVolume();
    // Place Crystal and Reflector at (0,0,0)
    type[itype]->AddPlacedVolume(fLogicCrystal[itype], t3d);
    type[itype]->AddPlacedVolume(fLogicReflector[itype], t3d);

    // Put housing with Z offset
    crystalz   = fGeometry[itype].crystalz;
    housingz   = fGeometry[itype].z;
    housing_dt = fGeometry[itype].housing_dt;
    housingZOffset = (housingz - crystalz)*0.5 - housing_dt - reflector_dt;
    pos = G4ThreeVector(0,0,housingZOffset);
    t3d = G4Transform3D(rotmat,pos);
    type[itype]->AddPlacedVolume(fLogicHousing[itype], t3d);

    // Put PMT with Z offset
    Int_t pmttype = fGeometry[itype].pmttype-1;
    Double_t pmtlength = fPMTlength[pmttype];
    pos = G4ThreeVector(0,0,housingZOffset + (pmtlength + housingz)*0.5);
    t3d = G4Transform3D(rotmat, pos);
    type[itype]->AddPlacedVolume(fLogicPMT[pmttype], t3d);
  }
  
  std::map<int, TCATANACsISimParameter*> ParaMap =
    fCATANASimParameter->fCATANACsISimParameterMap;
  std::map<int, TCATANACsISimParameter*>::iterator it = ParaMap.begin();

  while(it != ParaMap.end()){
    std::cout << *(it->second) << std::endl; // check parameters
    TCATANACsISimParameter *para = it->second;
    Int_t id = para->fID;
    Int_t detectortype = para->fDetectorType;
    G4ThreeVector detpos(para->fPosition.X()*mm + fPosition.x(),
			 para->fPosition.Y()*mm + fPosition.y(),
			 para->fPosition.Z()*mm + fPosition.z());
    G4RotationMatrix Rm(0,0,0);
    Rm.rotateX((-para->fTheta)*degree);
    Rm.rotateZ(para->fPhi*degree);
    G4Transform3D t3d2(Rm,detpos);

    if(detectortype > 0 && detectortype <= 5) {
      type[detectortype-1]->MakeImprint(ExpHall_log, t3d2, 20000+id-1, CHK_OVERLAP);
    }
    else {
      std::cout << "Strange detector type! Check csv file " << detectortype
		<< std::endl;
    }
	    
    ++it;
  }

  fCATANAExist = true;
  return 0;
}

    


