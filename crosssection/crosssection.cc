
// first argument: physics list name
// ex.) ./bin/Linux-g++/crosssection QGSP_INCLXX_XS
//
// result will be output to ./txt
// ex.)
// cd txt
// gnuplot
// gnuplot> load "hogehoge.plt"

#include "G4RunManager.hh"

#include "DetectorConstruction.hh"

#include "G4PhysListFactorySAMURAI.hh"
#include "G4VModularPhysicsList.hh"

#include "PrimaryGeneratorActionBasic.hh"

#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4NistManager.hh"
#include "G4HadronicProcessStore.hh"

#include "G4NucleiProperties.hh"
#include "G4NistManager.hh"
#include "G4StableIsotopes.hh"

#include "G4SystemOfUnits.hh"//Geant4.10

#include <fstream>
#include <sstream>
#include <string>

#include "QGSP_BERT.hh"
#include "QGSP_BERT_XS.hh"
#include "QGSP_BERT_XSHP.hh"
#include "QGSP_INCLXX_XS.hh"
#include "QGSP_MENATE_R.hh"

// see DirectAccess.cc

static G4String dir = "txt";
static G4String physName = "QGSP_INCLXX_XS"; // default
static G4double emin = 1.*MeV;
static G4double emax = 1000.*MeV;
static G4double estep = 1.01;
static G4String energyunit = "MeV";
static G4String crosssectionunit = "barn";

void CrossSection(std::ofstream& ofs_plt, G4String particleName, G4String elementName);

int main(int argc,char** argv)
{
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // set detector construction class
  runManager->SetUserInitialization(new DetectorConstruction);

  // set physics construction class
  // Physics List name defined via 2nd argument
  if(argc==2){
    physName = argv[1];
  }
  G4PhysListFactorySAMURAI physListFactorySAMURAI;
  G4VModularPhysicsList* modularPhysicsList = physListFactorySAMURAI.GetReferencePhysList(physName);
  if(!modularPhysicsList) return 1; // exit failure
  runManager->SetUserInitialization(modularPhysicsList);

  // initialize
  runManager->Initialize();

  PrimaryGeneratorActionBasic* primaryGeneratorActionBasic = new PrimaryGeneratorActionBasic;
  primaryGeneratorActionBasic->SetBeamType("Geantino");
  runManager->SetUserAction(primaryGeneratorActionBasic);

  runManager->BeamOn(1);

  G4String particleName = "neutron";
  G4String filename_plt = dir+"/"+physName+"_"+particleName+".plt";
  std::ofstream ofs_plt(filename_plt.data());
  ofs_plt << "set xrange [1:1000]" << std::endl;
  ofs_plt << "set yrange [1:10000]" << std::endl;
  ofs_plt << "set title \"" << physName << "\"" << std::endl;
  ofs_plt << "set xlabel \"energy (MeV)\"" << std::endl;
  ofs_plt << "set ylabel \"cross section (mb)\"" << std::endl;
  ofs_plt << "set logscale" << std::endl;
  ofs_plt << "plot \\" << std::endl;
  
  CrossSection(ofs_plt, particleName, "C");
  ofs_plt << ", \\" << std::endl;
  CrossSection(ofs_plt, particleName, "H");
  ofs_plt << ", \\" << std::endl;
  CrossSection(ofs_plt, particleName, "Li");
  ofs_plt << ", \\" << std::endl;
  CrossSection(ofs_plt, particleName, "Fe");

  delete runManager;

  return 0;
}

void CrossSection(std::ofstream& ofs_plt, G4String particleName, G4String elementName)
{
  G4String filename_txt = physName+"_"+particleName+"_"+elementName+".txt";
  std::ofstream ofs_txt((dir+"/"+filename_txt).data());

  ofs_plt << "\"" << filename_txt << "\" using 1:($2*1e3) title \"" << particleName << "+" << elementName << " elastic\" with lines, \\" << std::endl;
  ofs_plt << "\"" << filename_txt << "\" using 1:($3*1e3) title \"" << particleName << "+" << elementName << " inelastic\" with lines";

  const G4Element* elm = G4NistManager::Instance()->FindOrBuildElement(elementName);
  const G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particleName);

  if(!particle || !elm) {
    G4cout << "HistoManager WARNING Particle or element undefined" << G4endl;
    return;
  }

  G4cout << "### Fill Cross Sections for " << particleName << " of " << elementName << G4endl;

  G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();

  G4double eu = G4UnitDefinition::GetValueOf(energyunit);
  G4double cu = G4UnitDefinition::GetValueOf(crosssectionunit);

  ofs_txt << "# Physics List : " << physName << G4endl;
  ofs_txt << "# Particle : " << particleName << G4endl;
  ofs_txt << "# Element : " << elementName << G4endl;
  ofs_txt << "# Energy(" << energyunit << ") Elastic(" << crosssectionunit << ") Inelastic(" << crosssectionunit << ")" << G4endl;

  for(G4double e = emin; e < emax; e = e*estep){
    G4Material*  fTargetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4double elastic = store->GetElasticCrossSectionPerAtom(particle,e,elm,fTargetMaterial);
    G4double inelastic = store->GetInelasticCrossSectionPerAtom(particle,e,elm,fTargetMaterial);

    ofs_txt << e/eu << " " << elastic/cu << " " << inelastic/cu << G4endl;
  }
}
