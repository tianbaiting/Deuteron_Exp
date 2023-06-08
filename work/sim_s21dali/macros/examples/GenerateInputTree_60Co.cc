// simple example for 60co (generating 2 gammas)
// HOW TO USE
// root [0] .L macros/examples/GenerateInputTree_60Co.cc+
// root [1] GenerateInputTree_60Co()

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

void GenerateInputTree_60Co(Int_t neve=1000){
  // parameteres

  TFile *file = new TFile("root/examples/inputtree_60co_example.root","recreate");
  TTree *tree = new TTree("tree_input","Input tree for 1n simulation");
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);

  // for check
  TH1* he_gen = new TH1D("he_gen","Egamma (MeV) generated",200,0,1.5);
  TH1* hphicostht = new TH2D("hphicostht","Phi cos(Theta) generated",100,0,360,100,-1,1);
  TH1* hxypos = new TH2D("hxypos","XY pos (mm)",100,-50,50, 100,-50,50);
  TH1* hzxpos = new TH2D("hzxpos","ZX pos (mm)",100,-50,-50, 100,-50,50);

  Double_t pi = TMath::Pi();
  Double_t Egamma, cosTheta, sinTheta, Phi;
  TLorentzVector P;
  for (int i=0;i<neve;++i){
    if (neve%1000==0){
      cout<<"\r event = "<<neve;
    }

    gBeamSimDataArray->clear();

    //----------------------------------
    // target position without spread
    TVector3 pos_vec(0,0,0); 
    hxypos->Fill(pos_vec.x(), pos_vec.y());
    hzxpos->Fill(pos_vec.z(), pos_vec.x());
    
    //----------------------------------
    //1.173MeV
    Egamma = 1.173228;
    cosTheta = gRandom->Uniform(-1.0,1.0);
    sinTheta = sqrt(1.0-cosTheta*cosTheta);
    Phi = gRandom->Uniform(0,2.*pi);
    P.SetPxPyPzE(Egamma*sinTheta*cos(Phi), 
		 Egamma*sinTheta*sin(Phi), 
		 Egamma*cosTheta, 
		 Egamma);
    TBeamSimData gamma1(0,0,P,pos_vec);
    gamma1.fParticleName="gamma";
    gamma1.fPrimaryParticleID = 1;
    gBeamSimDataArray->push_back(gamma1);

    he_gen->Fill(P.E());
    hphicostht->Fill(Phi/pi*180., cosTheta);

    //----------------------------------
    //1.332MeV
    Egamma = 1.332492;
    cosTheta = gRandom->Uniform(-1.0,1.0);
    sinTheta = sqrt(1.0-cosTheta*cosTheta);
    Phi = gRandom->Uniform(0,2.*pi);
    P.SetPxPyPzE(Egamma*sinTheta*cos(Phi), 
		 Egamma*sinTheta*sin(Phi), 
		 Egamma*cosTheta, 
		 Egamma);
    TBeamSimData gamma2(0,0,P,pos_vec);
    gamma2.fParticleName="gamma";
    gamma2.fPrimaryParticleID = 2;
    gBeamSimDataArray->push_back(gamma2);

    he_gen->Fill(P.E());
    hphicostht->Fill(Phi/pi*180., cosTheta);


    //----------------------------------
    // Fill tree
    tree->Fill();

  }
  file->Write();
}
