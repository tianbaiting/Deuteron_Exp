#include "FragSimDataConverter_Basic.hh"

#include "SimDataManager.hh"
#include "TSimData.hh"

#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include <iostream>

//____________________________________________________________________
FragSimDataConverter_Basic::FragSimDataConverter_Basic(TString name)
  : SimDataConverter(name)
{
  ClearBuffer();
}
//____________________________________________________________________
FragSimDataConverter_Basic::~FragSimDataConverter_Basic()
{;}
//____________________________________________________________________
int FragSimDataConverter_Basic::Initialize()
{
  SimDataManager *sman = SimDataManager::GetSimDataManager();
  fFragSimParameter = (TFragSimParameter*)sman->FindParameter("FragParameter");
  if (fFragSimParameter==0){
    std::cout<<"FragSimDataConveter_Basic : FragmentParameter is not found."
	     <<std::endl;
    return 1;
  }

  fFragSimDataArray = sman->FindSimDataArray("FragSimData");
  if (fFragSimDataArray==0){
    std::cout<<"FragSimDataConveter_Basic : FragSimDataArray is not found."
	     <<std::endl;
    return 1;
  }

  return 0;
}
//____________________________________________________________________
int FragSimDataConverter_Basic::DefineBranch(TTree *tree)
{
  if (fDataStore) {
    tree->Branch("target_x",&fTargetX,"target_x/D");
    tree->Branch("target_y",&fTargetY,"target_y/D");
    tree->Branch("target_theta",&fTargetTheta,"target_y/D");
    tree->Branch("target_phi",&fTargetPhi,"target_y/D");
    tree->Branch("target_energy",&fTargetEnergy,"target_energy/D");
    tree->Branch("ok_target",&fOK_Target,"ok_target/O");

    tree->Branch("pdc1x",&fPDC1X,"pdc1x/D");
    tree->Branch("pdc1y",&fPDC1Y,"pdc1y/D");
    tree->Branch("pdc2x",&fPDC2X,"pdc2x/D");
    tree->Branch("pdc2y",&fPDC2Y,"pdc2y/D");
    tree->Branch("ok_pdc1",&fOK_PDC1,"ok_pdc1/O");
    tree->Branch("ok_pdc2",&fOK_PDC2,"ok_pdc2/O");
  }
  return 0;
}
//____________________________________________________________________
int FragSimDataConverter_Basic::ConvertSimData()
{
  Int_t ndata = fFragSimDataArray->GetEntries();

  TVector3 pos_target = fFragSimParameter->fTargetPosition;
  Double_t ang_target = fFragSimParameter->fTargetAngle;

  TVector3 pos_target_in{NAN, NAN, NAN};
  Int_t stepno_target_in{-1};
  TLorentzVector mom_target_in{NAN, NAN, NAN, NAN};

  Double_t ang_pdc = fFragSimParameter->fPDCAngle;
  TVector3 pos_pdc1 = fFragSimParameter->fPDC1Position;
  TVector3 pos_pdc2 = fFragSimParameter->fPDC2Position;
  pos_pdc1.RotateY(ang_pdc); pos_pdc2.RotateY(ang_pdc); // Lab frame

  Int_t stepno_pdc1_in{-1}, stepno_pdc1_out{-1};
  Int_t stepno_pdc2_in{-1}, stepno_pdc2_out{-1};
  TVector3 pos_pdc1_in{NAN, NAN, NAN}, pos_pdc1_out{NAN, NAN, NAN};
  TVector3 pos_pdc2_in{NAN, NAN, NAN}, pos_pdc2_out{NAN, NAN, NAN};

  for (int i=0;i<ndata;++i){
    auto data = (TSimData*)fFragSimDataArray->At(i);

    if (data->fDetectorName=="Target"){

      if (stepno_target_in == -1 || data->fStepNo < stepno_target_in){
        stepno_target_in = data->fStepNo;
        pos_target_in = data->fPrePosition - pos_target;
        mom_target_in = data->fPreMomentum;
        pos_target_in.RotateY(-ang_target); // Change to target frame
        mom_target_in.RotateY(-ang_target);
      }

    }else if (data->fDetectorName=="PDC1"){

      if (stepno_pdc1_in == -1 || data->fStepNo < stepno_pdc1_in){
        stepno_pdc1_in = data->fStepNo;
        pos_pdc1_in = data->fPrePosition - pos_pdc1;
        pos_pdc1_in.RotateY(-ang_pdc); // Change to PDC frame
      }
      if (stepno_pdc1_out == -1 || data->fStepNo > stepno_pdc1_out){
        stepno_pdc1_out = data->fStepNo;
        pos_pdc1_out = data->fPostPosition - pos_pdc1;
        pos_pdc1_out.RotateY(-ang_pdc);
      }

    }else if (data->fDetectorName=="PDC2"){

      if (stepno_pdc2_in == -1 || data->fStepNo < stepno_pdc2_in){
        stepno_pdc2_in = data->fStepNo;
        pos_pdc2_in = data->fPrePosition - pos_pdc2;
        pos_pdc2_in.RotateY(-ang_pdc); // Change to PDC frame
      }
      if (stepno_pdc2_out == -1 || data->fStepNo > stepno_pdc2_out){
        stepno_pdc2_out = data->fStepNo;
        pos_pdc2_out = data->fPostPosition - pos_pdc2;
        pos_pdc2_out.RotateY(-ang_pdc);
      }
      
    }
  }

  // To avoid kasuri event (dz(PDC)=32)
  fOK_PDC1 = (pos_pdc1_out.Z() - pos_pdc1_in.Z()) > 31;
  fOK_PDC2 = (pos_pdc2_out.Z() - pos_pdc2_in.Z()) > 31;
  fOK_Target = stepno_target_in > 0; // As long as it hit the target

  if (fOK_PDC1)
  {
    fPDC1X = 0.5*(pos_pdc1_in.X() + pos_pdc1_out.X()) - pos_pdc1.X();
    fPDC1Y = 0.5*(pos_pdc1_in.Y() + pos_pdc1_out.Y()) - pos_pdc1.Y();
  }
  if (fOK_PDC2)
  {
    fPDC2X = 0.5*(pos_pdc2_in.X() + pos_pdc2_out.X()) - pos_pdc2.X();
    fPDC2Y = 0.5*(pos_pdc2_in.Y() + pos_pdc2_out.Y()) - pos_pdc2.Y();
  }
  if (fOK_Target)
  {
    fTargetX = pos_target_in.X(); // Neglect the target thickness
    fTargetY = pos_target_in.Y();
    fTargetTheta = mom_target_in.Theta()/TMath::Pi()*180;
    fTargetPhi = mom_target_in.Phi()/TMath::Pi()*180;
    fTargetEnergy = mom_target_in.Energy() - mom_target_in.M();
  }

  return 0;
}
//____________________________________________________________________
int FragSimDataConverter_Basic::ClearBuffer()
{
  fTargetX = -99;
  fTargetY = -99;
  fTargetTheta = -99;
  fTargetPhi = -999;
  fTargetEnergy = -99;
  fOK_Target = false;
  fPDC1X = -999;
  fPDC1Y = -999;
  fPDC2X = -999;
  fPDC2Y = -999;
  fOK_PDC1 = false;
  fOK_PDC2 = false;
  return 0;
}
//____________________________________________________________________
