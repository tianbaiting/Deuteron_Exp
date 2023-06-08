#include <iostream>

#include <TFile.h>
#include <TTree.h>

#include "SimDataManager.hh"
#include "CATANASimDataInitializer.hh"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANASimDataInitializer::CATANASimDataInitializer(TString name)
  : SimDataInitializer(name)
{
  fDataStore = false; // default
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
CATANASimDataInitializer::~CATANASimDataInitializer()
{;}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int CATANASimDataInitializer::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fSimDataArray = sman->FindSimDataArray("CATANASimData");

  if (fSimDataArray==0){
    fSimDataArray = new TClonesArray("TSimData",256);
    fSimDataArray->SetName("CATANASimData");
    fSimDataArray->SetOwner();
  }

  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int CATANASimDataInitializer::DefineBranch(TTree *tree)
{
  if(fDataStore) tree->Branch(fSimDataArray->GetName(), &fSimDataArray);
  return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int CATANASimDataInitializer::ClearBuffer()
{
  fSimDataArray->Delete();
  return 0;
}


