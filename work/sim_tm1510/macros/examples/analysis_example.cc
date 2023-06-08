/*
  Simple analysis example
  root[0] .L macros/examples/analysis_example.cc+g
  root[1] analysis_example()
*/
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"

#include "TBeamSimData.hh"
#include "TFragSimData.hh"
#include "TNEBULASimData.hh"
#include "TArtNEBULAPla.hh"

#include "TRunSimParameter.hh"

void analysis_example(TString filename="root/examples/example_tree0000.root")
{

  TFile *file = new TFile(filename.Data(),"readonly");
  TRunSimParameter *RunPrm = 0;
  file->GetObject("RunParameter",RunPrm);

  RunPrm->Print();
  TTree *tree = 0;
  file->GetObject(RunPrm->fTreeName.Data(),tree);

  TClonesArray *NEBULAPlaArray = new TClonesArray("TArtNEBULAPla",144);

  tree->SetBranchAddress("beam",&gBeamSimDataArray);
  tree->SetBranchAddress("fragment",&gFragSimDataArray);
  tree->SetBranchAddress("NEBULAPla",&NEBULAPlaArray);
  
  TH1* hbeamposxy = new TH2D("hbeamposxy","Beam Pos XY (mm)",200,-50,50,200,-50,50);
  TH1* hmulti = new TH1I("hmulti","NEBULA Multiplicity",10,-0.5,9.5);
  TH1* hidx = new TH2D("hidx","ID X(mm)",605,0.5,605.5, 200,-2000,2000);
  TH1* hidy = new TH2D("hidy","ID Y(mm)",605,0.5,605.5, 200,-2000,2000);
  TH1* hidz = new TH2D("hidz","ID Z(mm)",605,0.5,605.5, 200,0,20000);
  TH1* hnebulatq = new TH2D("hnebulatq","NEBULA TOF(ns) Q(MeVee)",200,0,200,200,0,200);
  TH1* hnebulaxy = new TH2D("hnebulaxy","NEBULA Pos XY (mm)",200,-2000,2000,200,-2000,2000);
  TH1* hnebulazx = new TH2D("hnebulazx","NEBULA Pos ZX (mm)",200,7000,10000,200,-2000,2000);

  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    gBeamSimDataArray->clear();
    gFragSimDataArray->clear();
    NEBULAPlaArray->Delete();

    tree->GetEntry(ieve);

    // Get beam data
    Int_t nbeam = gBeamSimDataArray->size();
    for (Int_t ibeam=0;ibeam<nbeam;++ibeam){
      TBeamSimData beam = (*gBeamSimDataArray)[ibeam];
      hbeamposxy->Fill(beam.fPosition.fX, beam.fPosition.fY);
    }

    // Get fragment data
    Bool_t OK_FDC1    = false;
    Bool_t OK_WinHole = false;
    Bool_t OK_FDC2    = false;
    Bool_t OK_HOD     = false;
    Int_t nfrag = gFragSimDataArray->size();
    for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
      TFragSimData frag = (*gFragSimDataArray)[ifrag];
      if (frag.fDetectorName == "FDC1") OK_FDC1 = true;
      if (frag.fDetectorName == "WindowHole") OK_WinHole = true;
      if (frag.fDetectorName == "FDC2") OK_FDC2 = true;
      if (frag.fDetectorName == "HOD") OK_HOD = true;
    }

    // Get NEBULA data
    Int_t npla = NEBULAPlaArray->GetEntries();
    hmulti->Fill(npla);
    for (Int_t ipla=0;ipla<npla;++ipla){
      TArtNEBULAPla *pla = (TArtNEBULAPla*)NEBULAPlaArray->At(ipla);
      hidx->Fill(pla->GetID(), pla->GetPos(0));
      hidy->Fill(pla->GetID(), pla->GetPos(1));
      hidz->Fill(pla->GetID(), pla->GetPos(2));

      hnebulatq->Fill(pla->GetTAveSlw(),pla->GetQAveCal());
      hnebulaxy->Fill(pla->GetPos(0),pla->GetPos(1));
      hnebulazx->Fill(pla->GetPos(2),pla->GetPos(0));
    }
  }

}
