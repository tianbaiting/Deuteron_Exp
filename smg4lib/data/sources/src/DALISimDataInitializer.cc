#include "DALISimDataInitializer.hh"
#include "SimDataManager.hh"

#include <iostream>

#include "TFile.h"
#include "TTree.h"

//____________________________________________________________________
DALISimDataInitializer::DALISimDataInitializer(TString name)
  : SimDataInitializer(name)
{
  fDataStore = false;// default
}
//____________________________________________________________________
DALISimDataInitializer::~DALISimDataInitializer()
{;}
//____________________________________________________________________
int DALISimDataInitializer::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fSimDataArray = sman->FindSimDataArray("DALISimData");

  if (fSimDataArray==0){
    fSimDataArray = new TClonesArray("TSimData",256);
    fSimDataArray->SetName("DALISimData");
    fSimDataArray->SetOwner();
  }

  return 0;
}
//____________________________________________________________________
int DALISimDataInitializer::DefineBranch(TTree* tree)
{
  if (fDataStore) tree->Branch(fSimDataArray->GetName(),&fSimDataArray);
  return 0;
}
//____________________________________________________________________
int DALISimDataInitializer::AddParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int DALISimDataInitializer::RemoveParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int DALISimDataInitializer::PrintParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int DALISimDataInitializer::ClearBuffer()
{
  fSimDataArray->Delete();
  return 0;
}
//____________________________________________________________________
