// example for generating fragment, neutron gamma
// HOW TO USE
// root [0] .L macros/examples/GenerateInputTree_PhaseSpaceDecay_25o.cc+
// root [1] GenerateInputTree_PhaseSpaceDecay_25o()

#include "TBeamSimData.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>

void GenerateInputTree_fng(Double_t Erel_in=1.0, 
			   Double_t E_in=180){
  // parameteres
  const int neve = 100000;

  cout<<"Erel_in = "<<Erel_in<<" MeV"<<endl
      <<"energy = "<<E_in<<" AMeV"<<endl
      <<"Num of events = "<<neve
      <<endl;

  const Int_t Abeam = 80;
  const Int_t Afrag = 79;
  const Int_t Zfrag = 34;
  const Int_t Ndecay = Abeam - Afrag + 1;

  const Int_t    Z[Ndecay]={Zfrag,0};
  const Int_t    A[Ndecay]={Afrag,1};
  const Double_t Masses[Ndecay] = {73494.80292, 939.565};

  // Init Phase Space Decay
  Double_t Mbeam = Erel_in;
  for (Int_t i=0;i<Ndecay;++i) Mbeam+=Masses[i];
  Double_t Tbeam = Abeam*E_in;
  Double_t Ebeam = Mbeam + Tbeam;
  Double_t Pbeam_abs = sqrt(Tbeam*(Tbeam+2.0*Mbeam));
  TGenPhaseSpace Decay;

  TFile *file = new TFile("root/examples/inputtree_fng.root","recreate");
  TTree *tree = new TTree("tree_input","Input tree for simtrace");
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);

  // for check
  TH1* herel_gen = new TH1D("herel_gen","Erel generated",200,0,10);
  TH1* hxypos = new TH2D("hxypos","XY pos (mm)",100,-50,50, 100,-50,50);
  TH1* hzxpos = new TH2D("hzxpos","ZX pos (mm)",100,-5000,-4000, 100,-50,50);
  TH1* hxyang = new TH2D("hxyang","XY ang (mard)",100,-50,50, 100,-50,50);

  for (int i=0;i<neve;++i){

    if (i%1000==0) 
      cout<<"\r "<<i<<" / "<<neve
	  <<" ("<<100.*i/neve<<"%)"
	  <<flush;

    gBeamSimDataArray->clear();

    TLorentzVector Pbeam(0, 0, Pbeam_abs, Ebeam);

    // position spread: uniform, +-20mm for XY
    Double_t Zpos_target = -4139.;
    TVector3 pos_vec(gRandom->Uniform(-20,20),
                     gRandom->Uniform(-20,20),
                     Zpos_target); 

    // angular spread xy: +-50mrad
    Double_t xang = gRandom->Gaus(0,0.05);
    Double_t yang = gRandom->Gaus(0,0.05);
    Pbeam.RotateY(xang);
    Pbeam.RotateX(-yang);
    Decay.SetDecay(Pbeam,Ndecay,Masses);

    // Generate decayed events
    double weight = Decay.Generate();

    TLorentzVector P[Ndecay];
    for (int i=0;i<Ndecay;++i){
      P[i] = *(Decay.GetDecay(i));
      TBeamSimData particle(Z[i],A[i],P[i],pos_vec);
      if (Z[i]==0) particle.fParticleName="neutron";
      particle.fPrimaryParticleID = i+1;
      gBeamSimDataArray->push_back(particle);
    }

    // Generate gamma rays
    Double_t Egamma = 1.0;// MeV
    Double_t cosTheta = gRandom->Uniform(-1.0,1.0);
    Double_t sinTheta = sqrt(1.0-cosTheta*cosTheta);
    Double_t Phi = gRandom->Uniform(0,2.*TMath::Pi());
    TLorentzVector Pgamma(Egamma*sinTheta*cos(Phi), 
			  Egamma*sinTheta*sin(Phi), 
			  Egamma*cosTheta, 
			  Egamma);
    TBeamSimData gamma(0,0,Pgamma,pos_vec);
    gamma.fParticleName="gamma";
    gamma.fPrimaryParticleID = Ndecay+1;
    gBeamSimDataArray->push_back(gamma);
    
    // checking input
    TLorentzVector Ptot(0,0,0,0);
    Double_t Mtot=0;
    for (Int_t i=0;i<Ndecay;i++) {
      Ptot += P[i];
      Mtot += P[i].M();
    }
    Double_t Erel_gen = Ptot.M() - Mtot;
    herel_gen->Fill(Erel_gen);
    hxypos->Fill(pos_vec.x(), pos_vec.y());
    hzxpos->Fill(pos_vec.z(), pos_vec.x());
    hxyang->Fill(Ptot.Px()/Ptot.Pz()*1000., Ptot.Py()/Ptot.Pz()*1000.);

    // Fill tree
    tree->Fill();

  }
  file->Write();
}
