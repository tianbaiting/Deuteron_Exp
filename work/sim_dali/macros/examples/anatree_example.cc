/*
  example for doppler correction from output of simulator
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
#include "TDALISimParameter.hh"
#include "TDALINaISimParameter.hh"

#include "TArtDALINaI.hh"

void anatree_example(TString filename ="root/Beam_example0000.root")
{
  TFile *file = new TFile(filename.Data(),"readonly");

  TRunSimParameter *RunPrm = 0;
  file->GetObject("RunParameter",RunPrm);
  RunPrm->Print();

  TDALISimParameter *DALIPrm = 0;
  file->GetObject("DALIParameter",DALIPrm);
  DALIPrm->Print();

  std::map<int,TDALINaISimParameter*> *DALIParaMap = &(DALIPrm->fDALINaISimParameterMap);

  TTree *tree = 0;
  file->GetObject(RunPrm->fTreeName.Data(),tree);

  TClonesArray *DALINaIArray = new TClonesArray("TArtDALINaI",10);

  tree->SetBranchAddress("beam",&gBeamSimDataArray);
  tree->SetBranchAddress("DALINaI",&DALINaIArray);

  TString fout(filename);
  fout.ReplaceAll(".root","_analyzed.root");
  TFile *outfile = new TFile(fout.Data(),"recreate");
  outfile->cd();

  TH1* hecal = new TH1D("hecal","Energy (keV)",100,0,2000);
  TH1* hedop = new TH1D("hedop","Edop (keV)",100,0,2000);

  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    gBeamSimDataArray->clear();
    DALINaIArray->Delete();

    tree->GetEntry(ieve);

    // Get beam data
    TBeamSimData beam = (*gBeamSimDataArray)[0];
    Double_t beta = beam.fMomentum.P()/beam.fMomentum.E();
    Double_t gamma = 1./sqrt(1.-beta*beta);

    Int_t nhit = DALINaIArray->GetEntries();
    for (Int_t ihit=0;ihit<nhit;++ihit){
      TArtDALINaI* nai = (TArtDALINaI*)DALINaIArray->At(ihit);
      Int_t ID = nai->GetID();
      TDALINaISimParameter* para = DALIParaMap->at(ID);
      TVector3 pos = para->fPosition;
      Double_t costheta = pos.z()/pos.Mag();
      Double_t edop = nai->GetEnergy() * gamma *(1.0-beta*costheta);

      hecal->Fill(nai->GetEnergy());
      hedop->Fill(edop);

    }


  }

}
