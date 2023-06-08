/*
  Simple analysis example
  root[0] .L macros/examples/analysis_crosstalk_example.cc+g
  root[1] analysis_crosstalk_example()
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
#include "TNeuLANDSimData.hh"
#include "TArtNEBULAPla.hh"

#include "TRunSimParameter.hh"
#include "TFragSimParameter.hh"


Double_t CalcErel(TLorentzVector P24o, TArtNEBULAPla *pla);

void analysis_25o_example(TString filename="root/examples/example_25o_0000.root")
{

  Double_t Qthre = 6 ;// threshold for neutron detectors

  TFile *file = new TFile(filename.Data(),"readonly");

  TRunSimParameter *RunPrm = 0;
  file->GetObject("RunParameter",RunPrm);
  RunPrm->Print();

  TFragSimParameter *FragPrm = 0;
  file->GetObject("FragParameter",FragPrm);
  FragPrm->Print();

  TTree *tree = 0;
  file->GetObject(RunPrm->fTreeName.Data(),tree);

  TClonesArray *NEBULAPlaArray = new TClonesArray("TArtNEBULAPla",605);

  tree->SetBranchAddress("beam",&gBeamSimDataArray);
  tree->SetBranchAddress("fragment",&gFragSimDataArray);
  tree->SetBranchAddress("NEBULAPla",&NEBULAPlaArray);

  TString fileoutname = filename;
  fileoutname.ReplaceAll(".root","_analyzed.root");
  TFile *fileout = new TFile(fileoutname.Data(),"recreate");
  fileout->cd();

  //-----------------------------------
  TH1* hbeamposxy = new TH2D("hbeamposxy","Beam Pos XY (mm)",200,-50,50,200,-50,50);
  TH1* hnyacc = new TH1I("hnyacc","Yacc for neutrons",2,-0.5,1.5);
  TH1* hmulti     = new TH1I("hmulti","NEBULA Multiplicity",15,-0.5,14.5);
  TH1* hmulti_cta = new TH1I("hmulti_cta","NEBULA Multiplicity cta",15,-0.5,14.5);
  TH1* hidq = new TH2D("hidq","ID Q",605,0.5,605.5, 200,0,200);
  TH1* hidx = new TH2D("hidx","ID X(mm)",605,0.5,605.5, 200,-2000,2000);
  TH1* hidy = new TH2D("hidy","ID Y(mm)",605,0.5,605.5, 200,-2000,2000);
  TH1* hidz = new TH2D("hidz","ID Z(mm)",605,0.5,605.5, 200,10000,15000);
  TH1* hnebulatq = new TH2D("hnebulatq","NEBULA TOF(ns) Q(MeVee)",200,0,200,200,0,200);
  TH1* hnebulaxy = new TH2D("hnebulaxy","NEBULA Pos XY (mm)",200,-2000,2000,200,-2000,2000);
  TH1* hnebulazx = new TH2D("hnebulazx","NEBULA Pos ZX (mm)",200,10000,18000,200,-2000,2000);
  TH1* hnebulazy = new TH2D("hnebulazy","NEBULA Pos ZY (mm)",200,10000,18000,200,-2000,2000);

  TH1* hidq_s = new TH2D("hidq_s","ID Q selected",605,0.5,605.5, 200,0,200);
  TH1* herel     = new TH1D("herel","Erel (MeV)",200,0,15);


  //-----------------------------------
  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){

    if (ieve%1000==0){
      cout<<"\r events: "<<ieve<<" / "<<neve
	  <<" ("<<100.*ieve/neve<<"%)"
	  <<flush;
    }

    gBeamSimDataArray->clear();
    gFragSimDataArray->clear();
    NEBULAPlaArray->Delete();

    tree->GetEntry(ieve);
    //-----------------------------------
    // Get beam data
    Int_t nbeam = gBeamSimDataArray->size();
    for (Int_t ibeam=0;ibeam<nbeam;++ibeam){
      TBeamSimData beam = (*gBeamSimDataArray)[ibeam];
      hbeamposxy->Fill(beam.fPosition.fX, beam.fPosition.fY);
    }

    // check y acceptance of neutrons before interaction 
    Int_t OK_nyacc=1;
    for (Int_t ibeam=0;ibeam<nbeam;++ibeam){
      TBeamSimData beam = (*gBeamSimDataArray)[ibeam];
      if (beam.fZ!=0) continue;
      Double_t Theta_y = beam.fMomentum.Py()/beam.fMomentum.Pz()*1000.;// mrad
      if (fabs(Theta_y)>60.) OK_nyacc = 0;
    }
    hnyacc->Fill(OK_nyacc);

    //-----------------------------------
    // get fragment data
    Bool_t OK_FDC1    = false;
    Bool_t OK_WinHole = false;
    Bool_t OK_FDC2    = false;
    Bool_t OK_HOD     = false;
    Int_t nfrag = gFragSimDataArray->size();
    for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
      TFragSimData frag = (*gFragSimDataArray)[ifrag];
      //cout<<frag.fDetectorName<<endl;
      if (frag.fDetectorName == "FDC1")            OK_FDC1 = true;
      if (frag.fDetectorName == "WinC2_Hole_phys") OK_WinHole = true;
      if (frag.fDetectorName == "FDC2")            OK_FDC2 = true;
      if (frag.fDetectorName == "HOD")             OK_HOD = true;
    }

    Bool_t OK_fragment = OK_FDC1 && OK_WinHole && OK_FDC2 && OK_HOD;
    //-----------------------------------
    // momentum of 24O
    TLorentzVector P24o;
    for (Int_t ibeam=0;ibeam<nbeam;++ibeam){
      TBeamSimData beam = (*gBeamSimDataArray)[ibeam];
      if (beam.fZ==8) P24o = beam.fMomentum;
    }

    //-----------------------------------
    // Get NEBULA data
    Int_t npla = NEBULAPlaArray->GetEntries();
    hmulti->Fill(npla);

    Bool_t HitVETO[3] = {false, false, false};
    for (Int_t ipla=0;ipla<npla;++ipla){
      TArtNEBULAPla *pla = (TArtNEBULAPla*)NEBULAPlaArray->At(ipla);

      //add dist. between TGT-Magnet
      Double_t z_new = pla->GetPos(2) - FragPrm->fTargetPosition.z();
      pla->SetPos(z_new,2);

      hidq->Fill(pla->GetID(), pla->GetQAveCal());
      hidx->Fill(pla->GetID(), pla->GetPos(0));
      hidy->Fill(pla->GetID(), pla->GetPos(1));
      hidz->Fill(pla->GetID(), pla->GetPos(2));

      hnebulatq->Fill(pla->GetTAveSlw(),pla->GetQAveCal());
      hnebulaxy->Fill(pla->GetPos(0),pla->GetPos(1));
      hnebulazx->Fill(pla->GetPos(2),pla->GetPos(0));
      hnebulazy->Fill(pla->GetPos(2),pla->GetPos(1));

      // check VETO
      if (pla->GetID() >= 520 && pla->GetQAveCal()>0)
	HitVETO[pla->GetLayer()-1] = true;

    }

    // find earliest hit
    Double_t tof_min = 9999;
    TArtNEBULAPla *pla0 = 0;
    for (Int_t ipla=0;ipla<npla;++ipla){
      TArtNEBULAPla *pla = (TArtNEBULAPla*)NEBULAPlaArray->At(ipla);

      Int_t ID = pla->GetID();
      Int_t Layer = pla->GetLayer();
      if (HitVETO[Layer-1]) continue;// skip VETO

      if (pla->GetQAveCal()> Qthre){
	if (pla->GetTAveSlw() < tof_min){
	  tof_min = pla->GetTAveSlw();
	  pla0 = pla;
	}
      }

    }//for (Int_t ipla=0;ipla<npla;++ipla){

    if (OK_fragment && pla0 != 0){
      hidq_s->Fill(pla0->GetID(), pla0->GetQAveCal());

      Double_t Erel = CalcErel(P24o,pla0);
      herel->Fill(Erel);
    }

    
    
    //-----------------------------------

  }//for (Int_t ieve=0;ieve<neve;++ieve){
  fileout->Write();

}
//________________________________________________________
Double_t CalcErel(TLorentzVector P24o, TArtNEBULAPla *pla){
  // momentum of 24O
  TLorentzVector P[2];
  P[0] = P24o;
  
  // momentum of neutrons
  Double_t t = pla->GetTAveSlw();
  Double_t x = pla->GetPos(0);
  Double_t y = pla->GetPos(1);
  Double_t z = pla->GetPos(2);
  Double_t FL = sqrt( x*x + y*y + z*z );
  Double_t c_light = 299.792458; // mm/ns
  Double_t beta = FL/t/c_light;
  Double_t gamma = 1./sqrt((1.0+beta)*(1.0-beta));
  Double_t Mn = 939.565;      // MeV
  Double_t Pabs = Mn*beta*gamma;

  
  P[1].SetPxPyPzE(Pabs*x/FL,
		  Pabs*y/FL,
		  Pabs*z/FL,
		  Mn*gamma);

  Double_t Mtot=0;
  TLorentzVector Ptot(0,0,0,0);
  for (int i=0;i<2;++i){
    Mtot += P[i].M();
    Ptot += P[i];
  }
      
  Double_t Erel = Ptot.M() - Mtot;

  return Erel;
}
//________________________________________________________
