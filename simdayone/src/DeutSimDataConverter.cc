#include "DeutSimDataConverter.hh"

#include "SimDataManager.hh"
#include "TSimData.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include <iostream>
//____________________________________________________________________
DeutSimDataConverter::DeutSimDataConverter(TString name)
  : SimDataConverter(name)
{
  ClearBuffer();
}
//____________________________________________________________________
DeutSimDataConverter::~DeutSimDataConverter()
{;}
//____________________________________________________________________
int DeutSimDataConverter::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fDeutSimParameter = (TDeutSimParameter*)sman->FindParameter("DeutParameter");
  if (fDeutSimParameter==0){
    std::cout<<"DeutSimDataConveter : DeutParameter is not found."
	     <<std::endl;
    return 1;
  }

  fFragSimDataArray = sman->FindSimDataArray("FragSimData");
  if (fFragSimDataArray==0){
    std::cout<<"DeutSimDataConveter : FragSimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  fNEBULASimDataArray = sman->FindSimDataArray("NEBULASimData");

  return 0;
}
//____________________________________________________________________
int DeutSimDataConverter::DefineBranch(TTree *tree)
{
  if (fDataStore) {
    tree->Branch("target_x",&fTargetX,"target_x/D");
    tree->Branch("target_y",&fTargetY,"target_y/D");
    tree->Branch("target_z",&fTargetZ,"target_z/D");
    tree->Branch("target_angle",&fTargetAngle,"target_angle/D");
    tree->Branch("ok_target",&fOK_Target,"ok_target/O");

    tree->Branch("pdc1x",&fPDC1X,"pdc1x/D");
    tree->Branch("pdc1y",&fPDC1Y,"pdc1y/D");
    tree->Branch("pdc2x",&fPDC2X,"pdc2x/D");
    tree->Branch("pdc2y",&fPDC2Y,"pdc2y/D");
    tree->Branch("Energy",&fEnergy,"Energy/D");
    tree->Branch("ok_pdc1",&fOK_PDC1,"ok_pdc1/O");
    tree->Branch("ok_pdc2",&fOK_PDC2,"ok_pdc2/O");
  }
  return 0;
}
//____________________________________________________________________
int DeutSimDataConverter::ConvertSimData()
{
  Int_t ndata = fFragSimDataArray->GetEntries();

  TVector3 pos_target = fDeutSimParameter->fTargetPosition;
  Double_t ang_target = fDeutSimParameter->fTargetAngle;
  TVector3 pos_target_in{NAN, NAN, NAN}, pos_target_out{NAN, NAN, NAN};
  TLorentzVector mom_target_out{NAN, NAN, NAN, NAN};

  Double_t ang_pdc = fDeutSimParameter->fPDCAngle;
  TVector3 pos_pdc1 = fDeutSimParameter->fPDC1Position;
  TVector3 pos_pdc2 = fDeutSimParameter->fPDC2Position;
  TVector3 pos_pdc1_in{NAN, NAN, NAN}, pos_pdc1_out{NAN, NAN, NAN};
  TVector3 pos_pdc2_in{NAN, NAN, NAN}, pos_pdc2_out{NAN, NAN, NAN};
  Double_t energy_pdc1_in{NAN};

  for (int i=0; i<ndata; ++i)
  {
    auto data = (TSimData*)fFragSimDataArray->At(i);
    auto pos_pre = data->fPrePosition;
    auto pos_post = data->fPostPosition;
    
    if (data->fDetectorName=="PDC")
    {
      pos_pre.RotateY(-ang_pdc);
      pos_post.RotateY(-ang_pdc);

      if ( TMath::Abs(pos_pre.Z() - pos_pdc1.Z()) 
         < TMath::Abs(pos_pre.Z() - pos_pdc2.Z()) ) // This step is in PDC1
      {
        if (!(pos_pdc1_in.Z() < pos_pre.Z())) // The first step
        {
          pos_pdc1_in = pos_pre;
          energy_pdc1_in = data->fPreKineticEnergy;
        }
        if (!(pos_pdc1_out.Z() > pos_post.Z())) // The last step
        {
          pos_pdc1_out = pos_post;
        }
      }
      else // This step is in PDC2
      {
        if (!(pos_pdc2_in.Z() < pos_pre.Z())) // The first step
        {
          pos_pdc2_in = pos_pre;
        }
        if (!(pos_pdc2_out.Z() > pos_post.Z())) // The last step
        {
          pos_pdc2_out = pos_post;
        }
      }
    }
    else if (data->fDetectorName=="Target" && data->fA == 2 && data->fZ == 1)
    {
      if (!(pos_target_in.Z() < pos_pre.Z())) // The first step
      {
        pos_target_in = pos_pre;
      }
      if (!(pos_target_out.Z() > pos_post.Z())) // The last step
      {
        pos_target_out = pos_post;
        mom_target_out = data->fPostMomentum;
      }
    }
  }

  // To avoid kasuri event (dz(PDC)=32)
  fOK_PDC1 = (pos_pdc1_out.Z() - pos_pdc1_in.Z()) > 30;
  fOK_PDC2 = (pos_pdc2_out.Z() - pos_pdc2_in.Z()) > 30;

  auto pos_diff = pos_target_out - pos_target_in;
  pos_diff.RotateY(-ang_target);
  fOK_Target = pos_diff.Z() > fDeutSimParameter->fTargetThickness * 0.9;

  if (fOK_PDC1)
  {
    fPDC1X = 0.5*(pos_pdc1_in.X() + pos_pdc1_out.X()) - pos_pdc1.X();
    fPDC1Y = 0.5*(pos_pdc1_in.Y() + pos_pdc1_out.Y()) - pos_pdc1.Y();
    fEnergy = energy_pdc1_in;
  }
  if (fOK_PDC2)
  {
    fPDC2X = 0.5*(pos_pdc2_in.X() + pos_pdc2_out.X()) - pos_pdc2.X();
    fPDC2Y = 0.5*(pos_pdc2_in.Y() + pos_pdc2_out.Y()) - pos_pdc2.Y();
  }
  if (fOK_Target)
  {
    fTargetX = 0.5*(pos_target_in.X() + pos_target_out.X());
    fTargetY = 0.5*(pos_target_in.Y() + pos_target_out.Y());
    fTargetZ = 0.5*(pos_target_in.Z() + pos_target_out.Z());
    fTargetAngle = mom_target_out.Theta()/TMath::Pi()*180;
  }
/*
  ndata = fNEBULASimDataArray->GetEntries();
  for (int i=0; i<ndata; ++i)
  {
    auto data = (TSimData*)fFragSimDataArray->At(i);
    auto pos_pre = data->fPrePosition;
    auto pos_post = data->fPostPosition;
*/
  return 0;
}
//____________________________________________________________________
int DeutSimDataConverter::ClearBuffer()
{
  fTargetX = -999;
  fTargetY = -999;
  fTargetZ = -999;
  fTargetAngle = -1;
  fOK_Target = false;
  fPDC1X = -999;
  fPDC1Y = -999;
  fPDC2X = -999;
  fPDC2Y = -999;
  fEnergy = -1;
  fOK_PDC1 = false;
  fOK_PDC2 = false;
  return 0;
}
//____________________________________________________________________
