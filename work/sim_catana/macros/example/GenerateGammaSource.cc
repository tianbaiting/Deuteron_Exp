/*
Root macro to generate the tree input file for the gamma source run.
How to use: 
# .L macros/example/GenerateGammaSource.cc+g
# GenerateGammaSource(0.662)
 */

#include <iostream>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TClonesArray.h>

#include "TBeamSimData.hh"

Int_t GenerateGammaSource(Double_t Egamma = 0.662 /* MeV */){

  // parameters
  //const Int_t neve = 5000000;
  const Int_t neve = 10000;

  std::cout << "Gamma energy = " << Egamma << " MeV " << std::endl;

  const Int_t Abeam = 0;
  const Int_t Zbeam = 0;

  TString beamname = "gamma";

  TString foutname = "rootfiles/input/GammaSource";
  foutname += Form("%5.3f",Egamma);
  foutname += "MeV.root";
  TFile *fout = new TFile (foutname.Data(), "recreate");
  TTree *tree = new TTree("tree_input","Input");

  tree->Branch("TBeamSimData", &gBeamSimDataArray);

  TH1 *htgt = new TH2D("htgt", "Target", 200,-20,20,200,-20,20);
  TH1 *hEgamma = new TH1D("hEgamma", "Egamma",300,0,3);
  
  TLorentzVector Pgamma;
  
  Double_t tgtx, tgty, tgtz;
  for(Int_t ievent=0;ievent<neve;++ievent){
    gBeamSimDataArray->clear();
    
    if(ievent%100 == 0){
      std::cout << "\r event: " << ievent << " (" << 100.*ievent/neve
		<< "%) " << std::flush;
    }

    Double_t rnd = gRandom->Uniform(0,1); 
    Double_t rrr = TMath::Sqrt(rnd)*5.26;// 5.26mm = radius of source part
    Double_t theta = gRandom->Uniform(0,2*TMath::Pi());

    tgtx = rrr * TMath::Cos(theta);
    tgty = rrr * TMath::Sin(theta);
    htgt->Fill(tgtx,tgty);
    
    tgtz = gRandom->Uniform(-0.9,0.9); //source width = 1.8mm
    tgtz += (3. + 3.); // thickness of target frame(3mm) + half of source (3mm)
    
    TVector3 pos(tgtx,tgty,tgtz);
    Double_t gpx,gpy,gpz;
    gRandom->Sphere(gpx,gpy,gpz,Egamma); // Random direction
    Pgamma.SetPxPyPzE(gpx,gpy,gpz,Egamma);
    hEgamma->Fill(Pgamma.E());

    TBeamSimData particle(Zbeam, Abeam, Pgamma, pos);
    if(Zbeam == 0 && Abeam == 0) particle.fParticleName = "gamma";
    particle.fPrimaryParticleID = 0;
    gBeamSimDataArray->push_back(particle);

    tree->Fill();
  }

  fout->Write();
  fout->Close();
  
  return neve;
}
