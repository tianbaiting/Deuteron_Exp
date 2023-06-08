#include "NeuLANDSimDataInitializer.hh"
#include "SimDataManager.hh"

#include <iostream>

#include "TFile.h"
#include "TTree.h"

//____________________________________________________________________
NeuLANDSimDataInitializer::NeuLANDSimDataInitializer(TString name)
  : SimDataInitializer(name)
{
  fDataStore = false;// default
}
//____________________________________________________________________
NeuLANDSimDataInitializer::~NeuLANDSimDataInitializer()
{;}
//____________________________________________________________________
int NeuLANDSimDataInitializer::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fSimDataArray = sman->FindSimDataArray("NeuLANDSimData");

  if (fSimDataArray==0){
    fSimDataArray = new TClonesArray("TSimData",256);
    fSimDataArray->SetName("NeuLANDSimData");
    fSimDataArray->SetOwner();
  }

  return 0;
}
//____________________________________________________________________
int NeuLANDSimDataInitializer::DefineBranch(TTree* tree)
{
  if (fDataStore) tree->Branch(fSimDataArray->GetName(),&fSimDataArray);
  return 0;
}
//____________________________________________________________________
int NeuLANDSimDataInitializer::AddParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int NeuLANDSimDataInitializer::RemoveParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int NeuLANDSimDataInitializer::PrintParameters(TFile* file)
{
  return 0;
}
//____________________________________________________________________
int NeuLANDSimDataInitializer::ClearBuffer()
{
  fSimDataArray->Delete();
  return 0;
}
//____________________________________________________________________
