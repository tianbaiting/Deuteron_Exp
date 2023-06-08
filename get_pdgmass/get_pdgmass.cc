//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN03.cc,v 1.39 2010-12-01 05:56:17 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

#include "NoConstruction.hh"

#include "G4PhysListFactorySAMURAI.hh"
#include "G4VModularPhysicsList.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"

#include <iostream>
#include <iomanip>
#include "G4SystemOfUnits.hh"//Geant4.10

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  if (argc<3){
    std::cout<<"get_pdgmass Z A"<<std::endl;
    return 0;
  }

  //----------------------------------------------
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;
  runManager->SetUserInitialization(new NoConstruction);

  //----------------------------------------------
  // set physics construction class
  G4String physName = "QGSP_INCLXX_XS"; // default
  G4PhysListFactorySAMURAI physListFactorySAMURAI;
  G4VModularPhysicsList* modularPhysicsList = physListFactorySAMURAI.GetReferencePhysList(physName);
  runManager->SetUserInitialization(modularPhysicsList);

  //----------------------------------------------
  runManager->Initialize();

  //----------------------------------------------
  // get particle
  G4int Z = atoi(argv[1]);
  G4int A = atoi(argv[2]);

  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();  
  G4ParticleDefinition *particle = 0;
  if (Z==0 && A==1) particle = particleTable->FindParticle("neutron");
  else              particle = particleTable->GetIonTable()->GetIon(Z,A,0);//Geant4.10
  //else              particle = particleTable->GetIon(Z,A,0);//Geant4.9

  if (particle!=0){
    std::cout<<"------------------------------------"<<std::endl;
    std::cout<<"particle = "<<particle->GetParticleName().data()<<std::endl;
    std::cout<<"Z = "<<Z<<"   A = "<<A<<std::endl;
    std::cout<<std::setprecision(10)<<"mass = "<<particle->GetPDGMass()/MeV<<"MeV"<<std::endl;
    std::cout<<"------------------------------------"<<std::endl;
  }else {
    std::cout<<std::endl;
    std::cout<<"Z = "<<Z<<"   A = "<<A<<std::endl;
    std::cout<<"Unknown particle"<<std::endl;
  }


  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
