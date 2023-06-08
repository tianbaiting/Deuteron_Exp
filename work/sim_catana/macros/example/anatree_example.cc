/*
  example to analyze the simulator output
 */
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"

#include "TBeamSimData.hh"
#include "TRunSimParameter.hh"
#include "TCATANASimParameter.hh"
#include "TCATANACsISimParameter.hh"

#include "TArtCATANACsI.hh"

void anatree_example(TString filename ="rootfiles/GammaSource/Source0.662MeV_0000.root")
{
  TFile *file = new TFile(filename.Data(),"readonly");

  TRunSimParameter *RunPrm = 0;
  file->GetObject("RunParameter",RunPrm);
  RunPrm->Print();

  TCATANASimParameter *CATANAPrm = 0;
  file->GetObject("CATANAParameter",CATANAPrm);
  CATANAPrm->Print();

  std::map<int,TCATANACsISimParameter*> *CATANAParaMap = &(CATANAPrm->fCATANACsISimParameterMap);

  TTree *tree = 0;
  file->GetObject(RunPrm->fTreeName.Data(),tree);

  TClonesArray *CATANACsIArray = new TClonesArray("TArtCATANACsI",10);

  tree->SetBranchAddress("CATANACsI",&CATANACsIArray);

  TString fout(filename);
  fout.ReplaceAll(".root","_analyzed.root");
  TFile *outfile = new TFile(fout.Data(),"recreate");
  outfile->cd();

  TH1* hecal = new TH1D("hecal","Energy (MeV)",100,0,2000);

  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    CATANACsIArray->Delete();

    tree->GetEntry(ieve);

    Int_t nhit = CATANACsIArray->GetEntries();
    for (Int_t ihit=0;ihit<nhit;++ihit){
      TArtCATANACsI* csi = (TArtCATANACsI*)CATANACsIArray->At(ihit);
      Int_t ID = csi->GetID();
      TCATANACsISimParameter* para = CATANAParaMap->at(ID);
      TVector3 pos = para->fPosition;

      hecal->Fill(csi->GetEnergy());

    }


  }

}
