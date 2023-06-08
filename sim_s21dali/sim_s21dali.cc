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
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "SimDataManager.hh"
#include "BeamSimDataInitializer.hh"
#include "DALISimDataInitializer.hh"
#include "DALISimDataConverter_TArtDALINaI.hh"
#include "DALISimDataConverter_TArtDALINaI_S21.hh"

#include "G4PhysListFactorySAMURAI.hh"
#include "G4VModularPhysicsList.hh"

#include "SIMDALIConstruction.hh"
#include "PrimaryGeneratorActionBasic.hh"
#include "RunActionBasic.hh"
#include "EventActionBasic.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // detector construction
  SIMDALIConstruction* detector = new SIMDALIConstruction();
  runManager->SetUserInitialization(detector);

  // set physics construction class
  G4String physName = "QGSP_INCLXX_XS"; // default
  // Physics List name defined via 3rd argument
  if(argc==3){
    physName = argv[2];
  }
  G4PhysListFactorySAMURAI physListFactorySAMURAI;
  G4VModularPhysicsList* modularPhysicsList = physListFactorySAMURAI.GetReferencePhysList(physName);
  if(!modularPhysicsList) return 1; // exit failure
  runManager->SetUserInitialization(modularPhysicsList);


  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
     
  // Set user action classes
  //
  SimDataManager* simDataManager = SimDataManager::GetSimDataManager();
  simDataManager->RegistInitializer(new BeamSimDataInitializer);
  simDataManager->RegistInitializer(new DALISimDataInitializer);
  //simDataManager->RegistConverter(new DALISimDataConverter_TArtDALINaI);
  simDataManager->RegistConverter(new DALISimDataConverter_TArtDALINaI_S21);// for changing resolutions

  G4UserRunAction* userRunAction = new RunActionBasic;
  runManager->SetUserAction(userRunAction);

  G4VUserPrimaryGeneratorAction* userPrimaryGeneratorAction =  new PrimaryGeneratorActionBasic;
  runManager->SetUserAction(userPrimaryGeneratorAction);

  G4UserEventAction* userEventAction = new EventActionBasic;
  runManager->SetUserAction(userEventAction);

  // Initialize G4 kernel
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  //gen_action->SetKinematics();
  if (argc!=1) {  // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    //G4String fileName = "vis.mac";
    UImanager->ApplyCommand(command+fileName);
  }else{  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac"); 
#endif
//      if (ui->IsGUI())
//	UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
  }


  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
