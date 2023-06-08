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
    tree->Branch("fdc1x",&fFDC1X,"fdc1x/D");
    tree->Branch("fdc1a",&fFDC1A,"fdc1a/D");
    tree->Branch("fdc1y",&fFDC1Y,"fdc1y/D");
    tree->Branch("fdc1b",&fFDC1B,"fdc1b/D");
    tree->Branch("fdc2x",&fFDC2X,"fdc2x/D");
    tree->Branch("fdc2a",&fFDC2A,"fdc2a/D");
    tree->Branch("fdc2y",&fFDC2Y,"fdc2y/D");
    tree->Branch("fdc2b",&fFDC2B,"fdc2b/D");
    tree->Branch("ok_fdc1",&fOK_FDC1,"ok_fdc1/O");
    tree->Branch("ok_window",&fOK_Window,"ok_window/O");
    tree->Branch("ok_fdc2",&fOK_FDC2,"ok_fdc2/O");
    tree->Branch("ok_icf",&fOK_ICF,"ok_icf/O");
    tree->Branch("ok_hod",&fOK_HOD,"ok_hod/O");
  }
  return 0;
}
//____________________________________________________________________
int FragSimDataConverter_Basic::ConvertSimData()
{
  Int_t ndata = fFragSimDataArray->GetEntries();

  for (int i=0;i<ndata;++i){
    TSimData* data = (TSimData*)fFragSimDataArray->At(i);
    if (data->fDetectorName=="FDC1"){

      TVector3 pos_fdc1 = fFragSimParameter->fFDC1Position;
      TVector3 pos_pre = data->fPrePosition - pos_fdc1;
      TVector3 pos_post = data->fPostPosition - pos_fdc1;
      fFDC1X = 0.5*(pos_pre.x() + pos_post.x());
      fFDC1Y = 0.5*(pos_pre.y() + pos_post.y()); 
      fFDC1A = (pos_post.x() - pos_pre.x())/(pos_post.z()-pos_pre.z());
      fFDC1B = (pos_post.y() - pos_pre.y())/(pos_post.z()-pos_pre.z());

      Double_t dz = pos_post.z() - pos_pre.z();
      if (dz>139) fOK_FDC1 = true; // to avoid aksuri event (dz(FDC1)=140)
//      std::cout<<"fdc1 "<<pos_post.z() - pos_pre.z()<<" "
//	       <<pos_pre.z()<<" "
//	       <<pos_post.z()<<" "
//	       <<fOK_FDC1<<" "
//	       <<std::endl;


    }else if (data->fDetectorName=="WinC1_Hole" ||
	      data->fDetectorName=="WinC2_Hole"    ){
      fOK_Window = true;

    }else if (data->fDetectorName=="FDC2"){

      TVector3 pos_fdc2 = fFragSimParameter->fFDC2Position;
      Double_t ang_fdc2 = fFragSimParameter->fFDC2Angle/180.*TMath::Pi();//deg

      TVector3 pos_pre = data->fPrePosition;
      TVector3 pos_post = data->fPostPosition;
      pos_fdc2.RotateY(-ang_fdc2);
      pos_pre.RotateY(-ang_fdc2);
      pos_post.RotateY(-ang_fdc2);
      pos_pre -= pos_fdc2;
      pos_post -= pos_fdc2;
      fFDC2X = 0.5*(pos_pre.x() + pos_post.x());
      fFDC2Y = 0.5*(pos_pre.y() + pos_post.y()); 
      fFDC2A = (pos_post.x() - pos_pre.x())/(pos_post.z()-pos_pre.z());
      fFDC2B = (pos_post.y() - pos_pre.y())/(pos_post.z()-pos_pre.z());

      Double_t dz = pos_post.z() - pos_pre.z();
      if (dz>634) fOK_FDC2 = true;// to avoid kasuri event (dz(FDC2)=634.64)

//      std::cout<<"fdc2 "<<pos_post.z() - pos_pre.z()<<" "
//	       <<pos_pre.z()<<" "
//	       <<pos_post.z()<<" "
//	       <<fOK_FDC2<<" "
//	       <<std::endl;

    }else if (data->fDetectorName=="ICF"){
      TVector3 pos_icf = fFragSimParameter->fICFPosition;
      Double_t ang_icf = fFragSimParameter->fICFAngle/180.*TMath::Pi();//deg

      TVector3 pos_pre  = data->fPrePosition;
      TVector3 pos_post = data->fPostPosition;
      pos_icf.RotateY(-ang_icf);
      pos_pre.RotateY(-ang_icf);
      pos_post.RotateY(-ang_icf);
      pos_pre -= pos_icf;
      pos_post -= pos_icf;

      Double_t dz = pos_post.z() - pos_pre.z();
      if (dz>479) fOK_ICF = true;// to avoid kasuri event (dz(ICF)=480)

//      std::cout<<"icf "<<pos_post.z() - pos_pre.z()<<" "
//	       <<pos_pre.z()<<" "
//	       <<pos_post.z()<<" "
//	       <<fOK_ICF<<" "
//	       <<std::endl;


    }else if (data->fDetectorName=="HOD"){
      fOK_HOD = true;
    }
  }

  return 0;
}
//____________________________________________________________________
int FragSimDataConverter_Basic::ClearBuffer()
{
  fFDC1X = -9999;
  fFDC1A = -9999;
  fFDC1Y = -9999;
  fFDC1B = -9999;
  fFDC2X = -9999;
  fFDC2A = -9999;
  fFDC2Y = -9999;
  fFDC2B = -9999;
  fOK_FDC1   = false;
  fOK_Window = false;
  fOK_FDC2   = false;
  fOK_ICF    = false;
  fOK_HOD    = false;
  return 0;
}
//____________________________________________________________________
